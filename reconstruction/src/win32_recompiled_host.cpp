#include "btb/win32_recompiled_host.hpp"
#if defined(_WIN32)
#include "btb/win32_original_game_device.hpp"
#include "btb/win32_retail_bink_provider.hpp"
#include "btb/full_game_front_end.hpp"
#include "btb/full_game_golf.hpp"
#include "btb/full_game_maze.hpp"
#include "btb/full_game_spud_maze.hpp"
#include "btb/full_game_squirrel.hpp"
#include "btb/full_game_herding.hpp"
#include "btb/full_game_spud_skate.hpp"
#include "btb/win32_retail_spud_skate_movies.hpp"
#include "btb/win32_retail_fireworks_movie_bank.hpp"
#include "btb/fireworks_show_native.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"
#include "btb/retail_crt_random.hpp"
#include "btb/full_game_dino.hpp"
#include "btb/full_game_profile_screen_native.hpp"
#include "btb/full_game_fireworks.hpp"
#include "btb/full_game_bobs_band.hpp"
#include "btb/full_game_park_designer.hpp"
#include "btb/full_game_generic_ui.hpp"
#include "btb/full_game_pregame_resources.hpp"
#include "btb/full_game_startup_movies.hpp"
#include "btb/full_game_walkthrough_catalog.hpp"
#include "btb/full_game_ui_bitmaps.hpp"
#include "btb/original_asset_resolver.hpp"
#include "btb/native_input_pulse.hpp"
#include "btb/native_host_input.hpp"
#include "btb/original_active_frame.hpp"
#include "btb/win32_original_display_lifecycle.hpp"
#include "btb/win32_original_main_window_proc.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mmsystem.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace btb::full_game {
namespace {
namespace fs=std::filesystem;

// Profile persistence is independent of the original Bink decoder.
// Keep source-format progress away from installed retail save files.
std::optional<fs::path> native_profile_env_path(const wchar_t* key) {
    const DWORD length=GetEnvironmentVariableW(key,nullptr,0);
    if(length==0||length>32768)return std::nullopt;
    std::wstring chars(static_cast<std::size_t>(length),L'\0');
    const DWORD got=GetEnvironmentVariableW(key,chars.data(),length);
    if(got==0||got>=length)return std::nullopt;
    chars.resize(got);
    return fs::path(chars);
}
fs::path native_profile_directory(const fs::path& retail_root) {
    if(auto path=native_profile_env_path(L"BTB_PROFILE_DIR"))
        return *path;
    if(auto local=native_profile_env_path(L"LOCALAPPDATA"))
        return *local/"BTB-Recompiled"/"profiles";
    return retail_root/"BTB-Recompiled-Saves";
}

struct Host {
    HWND window{};
    fs::path root{};
    Win32OriginalDirectDraw graphics{};
    Win32OriginalDirectSound audio{};
    // The original 2002 binkw32.dll is directly callable from
    // the native x86 build; the x64 build rejects that ABI safely.
    Win32RetailBinkProvider movies;
    // Borrowed pointer; the actual decoder is owned by SpudSkateDriver.
    // GameDevice only asks it to blit the currently decoded surface.
    Win32RetailSpudSkateMovies* skate_movies{};
    std::unique_ptr<Win32RetailFireworksMovieBank> fireworks_bank{};
    FireworksDriver* fireworks_driver{};
    // Native Herding consumes the original 0x42FFC4 MSVC rand stream.
    // This state is owned by the host, not independently reseeded by
    // any animal, level, or minigame. Other activity providers still
    // need to migrate their host rand consumers onto this same stream.
    retail::OriginalRetailRandom original_game_random{};
    std::unique_ptr<Win32OriginalGameDevice> device{};
    GameRoot game{};
    std::unique_ptr<NativeGameSession> session{};
    fs::path profile_directory{};
    std::string error{};
    int pointer_x{320},pointer_y{240};
    NativeHostInput native_input{};
    bool running{};
    // PrintDlgA runs a nested modal Win32 message loop on this thread.
    // WM_TIMER must never reenter GameRoot/NativeGameSession while a
    // certificate print is capturing its already-submitted frame.
    bool native_frame_active{};
    bool skip_video{};
    int selected_profile{};
    original_win32::RetailWindowState retail_window{};
    original_active_frame::State original_frame{};

    explicit Host(fs::path original_root,bool skip,int profile)
        :root(std::move(original_root)),movies(graphics,root),
         skip_video(skip),selected_profile(profile) {}

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
        // Source 0x004027FB seeds CRT srand with GetTickCount. All
        // herding initialization and per-frame RNG draws subsequently
        // share the same value across the life of this Win32 host.
        original_game_random.apply_original_startup_tick_count(
            static_cast<std::uint32_t>(GetTickCount()));
        // PE32 00401F80 -> 00401FA0 now owns the actual DD7 creation,
        // windowed native style/menu setup and initial backbuffer clear.
        if(FAILED(original_win32::initialize_display_for_game_window(
            window,display::DisplayMode::Windowed,graphics,out_error)))
            return false;
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

        // All ten original pregame states have independently recovered
        // bitmap names, walkthrough indexes, and back/start action maps.
        // The separate source-backed Bink service currently rejects movie
        // requests, but game logic is no longer blocked by missing UI drivers.
        for(const auto& entry:kRetailPregamePairs){
            const auto index=static_cast<std::size_t>(entry.screen);
            auto driver=make_original_pregame_screen_driver(
                catalog,bitmaps,entry.setup,0,
                std::vector<int>(catalog.counts[index],0));
            if(!driver || !game.install_pregame_front_end_pair(
                   entry.setup,std::move(driver))){
                out_error="retail pregame screen could not be bound";
                return false;
            }
        }

        // Four source-confirmed subgame choice screens and three replay
        // screens use different bitmap-table offsets from their UI enum IDs.
        // Preserve the original names and never substitute a generic menu.
        struct MenuBackdrop {
            game_flow::State setup;
            std::size_t background;
        };
        constexpr std::array<MenuBackdrop,7> menu_backdrops{{
            {game_flow::State::DinoChooserSetup,8},
            {game_flow::State::MusicChooserSetup,22},
            {game_flow::State::SpudChooserSetup,9},
            {game_flow::State::AdventureChooserSetup,12},
            {game_flow::State::PlayAgainYesNoSetup,19},
            {game_flow::State::PlayAgainDifficultySetup,20},
            {game_flow::State::FireworksReplayChoiceSetup,21},
        }};
        for(const auto& mapping:menu_backdrops){
            const auto* spec=generic_front_end_pair_for_state(
                mapping.setup);
            if(!spec){
                out_error="original chooser state not in recovered registry";
                return false;
            }
            const auto index=static_cast<std::size_t>(spec->screen);
            auto driver=std::make_unique<GenericUiScreenDriver>(
                catalog,spec->screen,std::vector<int>(catalog.counts[index],0));
            if(!driver->configure_backdrop(bitmaps,mapping.background) ||
               !game.install_generic_front_end_pair(
                   mapping.setup,std::move(driver))){
                out_error="original chooser bitmap mapping failed";
                return false;
            }
        }

        // One set of original activity drivers, not cloned effect models.
        // Dinosaur now uses the nine genuine level folders and original
        // shared DirectDraw/DirectSound command pipeline. Remaining absent
        // activities still require source-backed adapters.
        const auto profile_dir=native_profile_directory(root);
        std::error_code profile_ec;
        fs::create_directories(profile_dir,profile_ec);
        if(profile_ec) {
            out_error="cannot create native profile directory: "+
                      profile_ec.message();
            return false;
        }
        if(!game.load_profile_files(profile_dir,out_error))return false;
        profile_directory=profile_dir;
        auto native_profiles=std::make_unique<NativeProfileScreenAdapter>(
            root,profile_dir);
        if(!native_profiles->initialize(out_error))return false;
        auto* original_profile_font=native_profiles.get();
        game.install_profile_screen(std::move(native_profiles));

        game.install(ActivityId::Dinosaur,std::make_unique<DinoDriver>(root));
        game.install(ActivityId::Golf,std::make_unique<GolfDriver>(
            root/"Data"/"SubGameGolf",0));
        game.install(ActivityId::Maze,std::make_unique<MazeDriver>(root,0));
        game.install(ActivityId::SpudMaze,
            std::make_unique<SpudMazeDriver>(root,0));
        game.install(ActivityId::Squirrel,
            std::make_unique<SquirrelDriver>(root,0));
        // Use the original x86 Bink DLL for all FOUR synchronized
        // stunt-quality tracks and the fifth end.bik track, with
        // actual decoded DD7 buffers (not fabricated frame numbers).
        // x64 fails explicitly until a compatible native decoder.
        auto skate_backend=
            std::make_unique<Win32RetailSpudSkateMovies>(graphics,root);
        skate_movies=skate_backend.get();
        game.install(ActivityId::SpudSkate,
            std::make_unique<SpudSkateDriver>(
                root,std::move(skate_backend),&original_game_random,
                [this](int profile)->bool {
                    const auto& records=game.globals().player_progress;
                    return profile>=0 &&
                           static_cast<std::size_t>(profile)<records.size() &&
                           records[static_cast<std::size_t>(profile)].get(
                               progress::Slot::SpudMaze)==1;
                }));
        // Source-backed initial Pets Corner scene and Pickles navigation.
        // Autonomous animal delivery remains fail-closed (no fake wins).
        game.install(ActivityId::Herding,
            std::make_unique<HerdingDriver>(
                root/"Data"/"SubGame1",
                std::make_unique<HerdingEventSimulation>(
                    std::make_unique<NativePicklesMotionSource>(
                        original_game_random)),0,
                &original_game_random));
        fireworks_bank=std::make_unique<Win32RetailFireworksMovieBank>(
            graphics,root);
        auto fireworks_activity=std::make_unique<FireworksDriver>(
            root/"Data"/"SubGameFirework",profile_dir,
            [this](const fs::path& movie,std::string& error) {
                return movies.open_global(movie,error);
            },
            [this]() { movies.cancel_global_movie(); },
            [this](const fs::path& source,std::string& error) {
                return fireworks_bank->open(source,error);
            },
            [this]() {
                std::string ignored;
                (void)fireworks_bank->close(ignored);
            },
            [this](const fireworks::Sequence& authored,
                   fireworks::RetailShowRuntime& runtime,
                   fireworks::ShowFrameInput frame,
                   fireworks::ShowFrameOutput& out,
                   std::string& error)->bool {
                fireworks_bank->begin_show_frame();
                return fireworks::advance_native_fireworks_show(
                    authored,runtime,frame,
                    [this](std::size_t index,int x,int y,
                           bool& finished,std::string& error) {
                        return fireworks_bank->decode_and_queue(
                            index,x,y,finished,error);
                    },
                    [this](std::size_t index,std::string& error) {
                        return fireworks_bank->restart(index,error);
                    },out,error);
            },
            &original_game_random,
            [this,original_profile_font](
                int player,profiles::ProfileInfo& info,
                std::span<const Rect>& glyphs,std::string& error) {
                if(player<0 ||
                   static_cast<std::size_t>(player)>=
                       game.globals().profile_metadata.size()) {
                    error="retail certificate selected profile is invalid";
                    return false;
                }
                info=game.globals().profile_metadata[
                    static_cast<std::size_t>(player)];
                glyphs=original_profile_font->brown_font_rectangles();
                if(glyphs.empty()) {
                    error="original fontdatabrown.txt not loaded";
                    return false;
                }
                error.clear();return true;
            });
        fireworks_driver=fireworks_activity.get();
        game.install(ActivityId::Fireworks,std::move(fireworks_activity));
        game.install(ActivityId::BobsBand,std::make_unique<BobsBandDriver>(
            root/"Data"/"SubGameOpen",profile_dir));
        game.install(ActivityId::ParkDesigner,
            std::make_unique<ParkDesignerDriver>(
                root/"Data"/"SubGameDYP",profile_dir));

        // Start at the original Activity Select rather than inventing an
        // incomplete five-sign profile-entry workflow. Profile 0 is
        // explicitly provisional in this opt-in development host.
        if(selected_profile>=0) {
            if(!game.select_profile(selected_profile)) {
                out_error="requested original profile slot outside 1..5";
                return false;
            }
            game.set_outer_state(game_flow::State::ActivitySelectSetup);
        } else {
            // Real source-backed five-sign profile screen, with original
            // bitmap/font tables and the existing C++26 name-entry caller.
            game.set_outer_state(game_flow::State::PlayerProfileAndNameEntry);
        }
        device=std::make_unique<Win32OriginalGameDevice>(
            graphics,audio,movies,
            [this](std::string& error)->bool {
                if(skate_movies &&
                   !skate_movies->blit_selected(error))return false;
                // A source show that just advanced to certificate no longer
                // owns the DD7 video overlay. Never paint its previous frame
                // over the certificate's own original artwork.
                if(fireworks_bank && fireworks_driver &&
                   fireworks_driver->initialized() &&
                   fireworks_driver->editor().internal_state==
                       fireworks::InternalState::ShowPlayback)
                    return fireworks_bank->blit_queued_show(error);
                error.clear();return true;
            });
        session=std::make_unique<NativeGameSession>(
            game,*device,OriginalAssetResolver{root},
            std::move(startups),std::move(walkthroughs),skip_video);

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
        if(native_frame_active)return true;
        struct FrameScope {
            bool& flag;
            explicit FrameScope(bool& value):flag(value) {flag=true;}
            ~FrameScope(){flag=false;}
        } frame_scope{native_frame_active};
        // The original CD volume label is not extracted/validated in this
        // source-only opt-in development host. The frame driver explicitly
        // marks this compatibility waiver instead of claiming disc parity.
        original_frame.media_check_waived=true;
        original_active_frame::Callbacks callbacks{};
        callbacks.context=this;
        callbacks.run_main_game_flow=+[](void* context)->bool{
            auto& h=*static_cast<Host*>(context);
            ActivityFrameInput input=h.native_input.sample(
                h.pointer_x,h.pointer_y,timeGetTime());
            const auto click=consume_native_click(h.native_input.click_pulse);
            input.click_pulse=click.activity_click;
            const auto frame=h.session->tick(
                input,click.primary_input_click,false,true);
            if(frame.failure){
                h.error=frame.failure->message;
                return false;
            }
            return frame.ready_for_present;
        };
        callbacks.check_directdraw_cooperative=+[](void* context)
            ->original_active_frame::U32 {
            const auto& h=*static_cast<Host*>(context);
            return static_cast<original_active_frame::U32>(
                h.graphics.cooperative_status());
        };
        callbacks.reset_display_manager=+[](void* context)
            ->original_active_frame::U32 {
            auto& h=*static_cast<Host*>(context);
            const auto mode=h.retail_window.windowed
                ? display::DisplayMode::Windowed
                : display::DisplayMode::Fullscreen;
            return static_cast<original_active_frame::U32>(
                original_win32::create_or_reset_display_manager(
                    h.window,mode,h.graphics,h.error));
        };
        callbacks.present_original_game_frame=+[](void* context)
            ->original_active_frame::U32 {
            auto& h=*static_cast<Host*>(context);
            return h.session->present_prepared(h.error)?0U:0x80004005U;
        };
        callbacks.cancel_pending_frame=+[](void* context) {
            static_cast<Host*>(context)->session->discard_prepared();
        };
        callbacks.restore_all_surfaces=+[](void* context) {
            auto& h=*static_cast<Host*>(context);
            (void)h.graphics.restore_for_frame(h.error);
        };
        callbacks.sleep_ms=+[](original_active_frame::U32 ms,void*) {
            Sleep(ms);
        };
        // DirectSound ReapFinishedSounds is invoked by the device's real
        // observe() before game advancement. Win32 messages provide event-
        // driven input; no unverified DirectInput poll is fabricated.
        const auto frame=original_active_frame::run_active_game_frame(
            original_frame,timeGetTime(),callbacks);
        if(frame.outcome==original_active_frame::Outcome::Failed){
            if(error.empty())
                error="00402440 native frame failed (status "+
                      std::to_string(frame.original_status)+")";
            running=false;
            return false;
        }
        return true; // idle-ms and recoverable DD7 frames must not quit
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
    // Windows' modal PrintDlgA pumps the owner's WM_TIMER events while
    // an original frame is already mid-draw/printing. Ignore nested ticks
    // before even reaching the original window-proc shim; resume when the
    // physical dialog closes and the existing frame can finish presenting.
    if(msg==WM_TIMER && wp==kNativeTimer && host->native_frame_active)
        return 0;
    // Original 004020D0 is now on the genuine Win32 message path, before
    // application-specific game activity and mouse input handling.
    const auto original_result=original_win32::main_window_proc_recompiled(
        hwnd,msg,wp,lp,host->retail_window,host->graphics,host->error);
    if(msg==0x212 || msg==0x232)
        host->original_frame.last_time_ms=
            host->retail_window.last_input_ms;
    if(original_result)return *original_result;
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
        host->set_pointer(lp);host->native_input.press_mouse();return 0;
    case WM_LBUTTONUP:
        host->set_pointer(lp);host->native_input.release_mouse();return 0;
    case WM_KEYDOWN:
        host->native_input.key_down(
            static_cast<unsigned>(wp),(lp&0x40000000)!=0,
            (static_cast<unsigned>(lp)>>16U)&0xffU);
        // Original MainWindowProc (PE32 0x004020D0) never destroys
        // the game window on WM_KEYDOWN/VK_ESCAPE; it dispatches
        // unhandled keyboard messages through DefWindowProc.
        // Do not forcibly terminate an ongoing minigame or abandon
        // saved progress because the player pressed Escape.
        // The normal WM_CLOSE/WM_DESTROY path remains available.
        return 0;
    case WM_KEYUP:
        host->native_input.key_up(static_cast<unsigned>(wp));return 0;
    case WM_CHAR:
        host->native_input.character(static_cast<unsigned>(wp));return 0;
    case WM_KILLFOCUS:
        host->native_input.lose_focus();return 0;
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
        // The original activity owns its per-player save file (Fireworks
        // firedataN, Park Designer dypdataN, Band compositions, etc).
        // Save/unload it BEFORE writing global profile records. Previously,
        // closing the window skipped all activity-specific persistence.
        {
            std::string unload_error;
            if(!host->game.unload_current(unload_error)) {
                const auto msg=unload_error.empty()
                  ? "Native activity could not save its original data"
                  : unload_error;
                MessageBoxA(hwnd,msg.c_str(),
                    "Native activity save/unload failed",
                    MB_OK|MB_ICONERROR);
            }
        }
        if(!host->profile_directory.empty()) {
            std::string save_error;
            if(!host->game.save_profile_files(
                   host->profile_directory,save_error)) {
                MessageBoxA(hwnd,save_error.c_str(),
                    "Native profile save failed",MB_OK|MB_ICONERROR);
            }
        }
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd,msg,wp,lp);
    }
}
} // namespace

int run_recompiled_native_game(
    HINSTANCE instance,const fs::path& original_root,int show,
    bool skip_video,int selected_profile) {
    Host host(original_root,skip_video,selected_profile);
    WNDCLASSEXW cls{};
    cls.cbSize=sizeof(cls);
    cls.hInstance=instance;
    cls.lpfnWndProc=native_window_proc;
    cls.hCursor=LoadCursorW(nullptr,MAKEINTRESOURCEW(32512));
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
