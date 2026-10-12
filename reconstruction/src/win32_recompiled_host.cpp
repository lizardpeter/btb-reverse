#include "btb/win32_recompiled_host.hpp"
#if defined(_WIN32)
#include "btb/win32_original_game_device.hpp"
#include "btb/full_game_front_end.hpp"
#include "btb/full_game_golf.hpp"
#include "btb/full_game_fireworks.hpp"
#include "btb/full_game_bobs_band.hpp"
#include "btb/full_game_park_designer.hpp"
#include "btb/full_game_generic_ui.hpp"
#include "btb/full_game_startup_movies.hpp"
#include "btb/full_game_walkthrough_catalog.hpp"
#include "btb/full_game_ui_bitmaps.hpp"
#include "btb/original_asset_resolver.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace btb::full_game {
namespace {
namespace fs=std::filesystem;

// No original movie handle is opened. Nonmovie activity frames can run,
// but a Bink request is a hard failure. Never signal a movie as "finished"
// without decoding its actual frames.
class MissingOriginalBinkDecoder final : public OriginalBinkProvider {
public:
    bool service(bool& finished,std::string& error) override {
        finished=false;error.clear();return true; // no movie is active
    }
    bool open_global(const fs::path&,std::string& error) override {
        error="original 32-bit Bink movie decoder is not connected";return false;
    }
    bool open_walkthrough(const fs::path&,const fs::path&,
                          std::string& error) override {
        error="original Bink walkthrough decoder is not connected";return false;
    }
    bool close_walkthrough(std::string& error) override {
        error="no original Bink walkthrough decoder was opened";return false;
    }
    bool render_walkthrough_overlay(std::string& error) override {
        // All attempted walkthrough opens fail above, so no overlay exists.
        error.clear();return true;
    }
};

struct Host {
    HWND window{};
    fs::path root{};
    Win32OriginalDirectDraw graphics{};
    Win32OriginalDirectSound audio{};
    MissingOriginalBinkDecoder movies{};
    std::unique_ptr<Win32OriginalGameDevice> device{};
    GameRoot game{};
    std::unique_ptr<NativeGameSession> session{};
    std::string error{};
    int pointer_x{320},pointer_y{240};
    bool click_pulse{};
    bool running{};

    explicit Host(fs::path original_root):root(std::move(original_root)) {}

    [[nodiscard]] static bool parse_bitmap_names(
        const fs::path& loaddata,OriginalUiBitmapCatalog& bitmaps,
        std::string& error) {
        std::ifstream input(loaddata/"uiBitmapName.txt");
        if (!input) {
            error="original loaddata/uiBitmapName.txt missing";return false;
        }
        return bitmaps.read(input,error);
    }
    [[nodiscard]] static bool parse_movies(
        const fs::path& loaddata,OriginalStartupMovieCatalog& starts,
        OriginalWalkthroughCatalog& walk,std::string& error) {
        std::ifstream startup(loaddata/"startupmovie.txt");
        std::ifstream walkthrough(loaddata/"binkwalk.txt");
        if(!startup||!walkthrough) {
            error="original startupmovie.txt or binkwalk.txt missing";
            return false;
        }
        return starts.read(startup,error)&&walk.read(walkthrough,error);
    }
    [[nodiscard]] bool start(std::string& out_error) {
        std::error_code ec;
        if(!fs::is_directory(root,ec)||ec) {
            out_error="original game installation directory does not exist";
            return false;
        }
        if(!graphics.initialize(
            window,display::DisplayMode::Windowed,out_error))return false;
        if(!audio.initialize(window,out_error))return false;
        const auto loaddata=root/"loaddata";
        GenericUiCatalog catalog;
        try {catalog=GenericUiCatalog::load(loaddata);}
        catch(const std::exception& e){out_error=e.what();return false;}
        OriginalUiBitmapCatalog bitmaps;
        if(!parse_bitmap_names(loaddata,bitmaps,out_error))return false;
        OriginalStartupMovieCatalog startups;
        OriginalWalkthroughCatalog walkthroughs;
        if(!parse_movies(loaddata,startups,walkthroughs,out_error))
            return false;

        // Original address 0042A2C0 state 04: screen 1 is the activity
        // selection hit-area table, and bitmap source slot 2 is the authored
        // activity-select background (separate 23-entry bitmap namespace).
        auto chooser=std::make_unique<ActivitySelectDriver>(
            catalog,std::vector<int>(10,0));
        if(!chooser->configure_backdrop(bitmaps,2)){
            out_error="original activity-select bitmap source slot 2 absent";
            return false;
        }
        game.install_activity_select(std::move(chooser));

        // One set of original activity drivers, not cloned effect models.
        // Other activities are deliberately not registered until their
        // complete motion/runtime providers are linked.
        game.install(ActivityId::Golf,std::make_unique<GolfDriver>(
            root/"Data"/"SubGameGolf",0));
        game.install(ActivityId::Fireworks,std::make_unique<FireworksDriver>(
            root/"Data"/"SubGameFirework",root/"profiles"));
        game.install(ActivityId::BobsBand,std::make_unique<BobsBandDriver>(
            root/"Data"/"SubGameOpen",root/"profiles"));
        game.install(ActivityId::ParkDesigner,
            std::make_unique<ParkDesignerDriver>(
                root/"Data"/"SubGameDYP",root/"profiles"));

        // Start at the original Activity Select rather than inventing an
        // incomplete five-sign profile-entry workflow. Profile 0 is
        // explicitly provisional in this opt-in development host.
        if(!game.select_profile(0)) {
            out_error="could not select temporary original profile slot";
            return false;
        }
        game.set_outer_state(game_flow::State::ActivitySelectSetup);
        device=std::make_unique<Win32OriginalGameDevice>(
            graphics,audio,movies);
        session=std::make_unique<NativeGameSession>(
            game,*device,OriginalAssetResolver{root},
            std::move(startups),std::move(walkthroughs));

        std::ifstream sounds(root/"Data"/"sound"/"binklist.txt");
        if(!sounds) sounds.open(root/"loaddata"/"binklist.txt");
        if(!sounds) {
            out_error="original binklist.txt sound table missing";
            return false;
        }
        auto loaded=session->load_sound_catalog(sounds);
        if(!loaded.success){
            out_error="original managed sound table invalid: "+loaded.error;
            return false;
        }
        running=true;
        out_error.clear();
        return true;
    }

    [[nodiscard]] bool step() {
        if(!running||!session)return false;
        ActivityFrameInput input{};
        input.pointer_x=pointer_x;input.pointer_y=pointer_y;
        input.click_pulse=click_pulse;
        click_pulse=false;
        const auto frame=session->tick(input,input.click_pulse,false);
        if(frame.failure){
            error=frame.failure->message;
            running=false;
            return false;
        }
        return frame.submitted;
    }

    void set_pointer(LPARAM lparam) {
        RECT rect{};
        if(!GetClientRect(window,&rect))return;
        const int width=std::max(1L,rect.right-rect.left);
        const int height=std::max(1L,rect.bottom-rect.top);
        const int x=static_cast<short>(LOWORD(lparam));
        const int y=static_cast<short>(HIWORD(lparam));
        pointer_x=std::clamp(x*640/width,0,639);
        pointer_y=std::clamp(y*480/height,0,479);
    }
};

constexpr UINT_PTR kNativeTimer=7;

LRESULT CALLBACK native_window_proc(
    HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) {
    if(msg==WM_NCCREATE){
        auto* creating=reinterpret_cast<CREATESTRUCTW*>(lp);
        auto* host=static_cast<Host*>(creating->lpCreateParams);
        host->window=hwnd;
        SetWindowLongPtrW(hwnd,GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(host));
    }
    auto* host=reinterpret_cast<Host*>(
        GetWindowLongPtrW(hwnd,GWLP_USERDATA));
    if(!host)return DefWindowProcW(hwnd,msg,wp,lp);
    switch(msg){
    case WM_CREATE:{
        std::string error;
        if(!host->start(error)){
            MessageBoxA(hwnd,error.c_str(),
                        "Original game native initialization failed",
                        MB_OK|MB_ICONERROR);
            return -1;
        }
        SetTimer(hwnd,kNativeTimer,16,nullptr);
        return 0;
    }
    case WM_MOUSEMOVE:
        host->set_pointer(lp);return 0;
    case WM_LBUTTONDOWN:
        host->set_pointer(lp);host->click_pulse=true;return 0;
    case WM_KEYDOWN:
        if(wp==VK_ESCAPE)DestroyWindow(hwnd);
        return 0;
    case WM_TIMER:
        if(wp==kNativeTimer&&!host->step()){
            const auto message=host->error.empty()
              ? "Native game could not submit a complete source frame"
              : host->error;
            MessageBoxA(hwnd,message.c_str(),
              "Reconstructed game stopped at unsupported native dependency",
              MB_OK|MB_ICONERROR);
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_PAINT:{
        // The original display is owned and presented by DirectDraw7.
        // GDI must not paint over the game surface.
        PAINTSTRUCT ps{};
        BeginPaint(hwnd,&ps);EndPaint(hwnd,&ps);
        return 0;
    }
    case WM_DESTROY:
        host->running=false;
        KillTimer(hwnd,kNativeTimer);
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd,msg,wp,lp);
    }
}
} // namespace

int run_recompiled_native_game(
    HINSTANCE instance,const fs::path& original_root,int show) {
    Host host(original_root);
    WNDCLASSEXW cls{};
    cls.cbSize=sizeof(cls);
    cls.hInstance=instance;
    cls.lpfnWndProc=native_window_proc;
    cls.hCursor=LoadCursorW(nullptr,IDC_ARROW);
    cls.lpszClassName=L"BTBRetailNativeCxx";
    if(!RegisterClassExW(&cls))return 1;
    RECT size{0,0,640,480};
    AdjustWindowRect(&size,WS_OVERLAPPEDWINDOW,FALSE);
    const auto hwnd=CreateWindowExW(
        0,cls.lpszClassName,
        L"Bob Builds a Park - Original Source Native DirectDraw7",
        WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
        size.right-size.left,size.bottom-size.top,
        nullptr,nullptr,instance,&host);
    if(!hwnd)return 2;
    ShowWindow(hwnd,show);
    UpdateWindow(hwnd);
    MSG msg{};
    while(GetMessageW(&msg,nullptr,0,0)>0){
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
} // namespace btb::full_game
#endif
