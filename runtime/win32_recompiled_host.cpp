#include "btb/win32_recompiled_host.hpp"
#if defined(_WIN32)
#include "btb/win32_original_game_device.hpp"
#include "btb/win32_hybrid_bink_provider.hpp"
#include "btb/full_game_front_end.hpp"
#include "btb/full_game_golf.hpp"
#include "btb/full_game_maze.hpp"
#include "btb/full_game_spud_maze.hpp"
#include "btb/full_game_squirrel.hpp"
#include "btb/full_game_herding.hpp"
#include "btb/full_game_spud_skate.hpp"
#include "btb/win32_native_spud_skate_movies.hpp"
#include "btb/win32_native_fireworks_movie_bank.hpp"
#include "btb/fireworks_show_native.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"
#include "btb/retail_crt_random.hpp"
#include "btb/full_game_dino.hpp"
#include "btb/full_game_profile_screen_native.hpp"
#include "btb/full_game_fireworks.hpp"
#include "btb/full_game_bobs_band.hpp"
#include "btb/full_game_park_designer.hpp"
#include "btb/full_game_park_designer_native.hpp"
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

// These flags select experimental native playback on x86 only.
// x64 provider constructors always select the reconstructed decoder.
[[nodiscard]] bool native_bink_opt_in(const wchar_t* key) noexcept {
    wchar_t value[3]{};
    return GetEnvironmentVariableW(key,value,3)==1 &&
           value[0]==L'1';
}


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

struct HiddenCompositionWindow {
    HWND handle{};
    ~HiddenCompositionWindow(){if(handle)DestroyWindow(handle);}
};
struct Host {
    HWND window{};
    HiddenCompositionWindow composition{};
    Win32FrameOutput* output{};
    fs::path root{};
    Win32OriginalDirectDraw graphics{};
    Win32OriginalDirectSound audio{};
    // The x86 build can use the original DLL as a reference.
    // The x64 build uses the reconstructed Bink1 backends exclusively.
    Win32HybridBinkProvider movies;
    // Borrowed pointer; the actual decoder is owned by SpudSkateDriver.
    // GameDevice only asks it to blit the currently decoded surface.
    Win32HybridSpudSkateMovies* skate_movies{};
    std::unique_ptr<Win32HybridFireworksMovieBank> fireworks_bank{};
    FireworksDriver* fireworks_driver{};
    // One process-owned 0x42FFC4 MSVC rand stream for source-backed
    // activity initialization, movement and voice branches. Never
    // independently reseed when switching levels or minigames.
    retail::OriginalRetailRandom original_game_random{};
    std::unique_ptr<Win32OriginalGameDevice> device{};
    GameRoot game{};
    std::unique_ptr<NativeGameSession> session{};
    fs::path profile_directory{};
    std::string error{};
    int pointer_x{320},pointer_y{240};
    NativeHostInput native_input{};
    bool running{};
    // WM_TIMER is re-entrant while MessageBoxA runs its modal message loop.
    // Latch fatal presentation errors so queued timer messages cannot open
    // an unbounded stack of identical modal dialogs.
    bool fatal_dialog_shown{};
    bool frame_in_progress{};
    bool close_requested{},startup_completed{};
    int exit_code{};
    bool skip_video{};
    int selected_profile{};
    original_win32::RetailWindowState retail_window{};
    original_active_frame::State original_frame{};

    explicit Host(fs::path original_root,bool skip,int profile,Win32FrameOutput* sink)
        :output(sink),root(std::move(original_root)),
         movies(graphics,audio,root,
                native_bink_opt_in(L"BTB_EXPERIMENTAL_NATIVE_BINK_AUDIO"),
                native_bink_opt_in(L"BTB_EXPERIMENTAL_NATIVE_BINK_VIDEO")),
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
    [[nodiscard]] static bool parse_video_sequence(
        const fs::path& loaddata,OriginalVideoSequenceCatalog& sequence,
        std::string& error) {
        std::ifstream source(loaddata/"videoseq.txt");
        if(!source) {
            error="original loaddata/videoseq.txt missing";
            return false;
        }
        return sequence.read(source,error);
    }
    [[nodiscard]] bool start(std::string& out_error) {
        std::error_code ec;
        if(!fs::is_directory(root,ec)||ec) {
            out_error="original game installation directory does not exist";
            return false;
        }
        // Use the supplied original game directory directly. The legacy
        // walkthrough WAV names are not needed by the retail movie opener.
        // Source 0x004027FB seeds CRT srand with GetTickCount. All
        // herding initialization and per-frame RNG draws subsequently
        // share the same value across the life of this Win32 host.
        original_game_random.apply_original_startup_tick_count(
            static_cast<std::uint32_t>(GetTickCount()));
        // PE32 00401F80 -> 00401FA0 now owns the actual DD7 creation,
        // windowed native style/menu setup and initial backbuffer clear.
        if(output){
            // Never associate the DirectDraw primary/clipper with the Vulkan
            // window. The original composition surfaces live on a hidden HWND.
            composition.handle=CreateWindowExW(0,L"STATIC",L"BTB offscreen composition",
                WS_POPUP,0,0,640,480,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
            if(!composition.handle){out_error="cannot create offscreen composition window";return false;}
            if(!graphics.initialize(composition.handle,display::DisplayMode::Windowed,out_error))return false;
            if(FAILED(graphics.clear_retail_backbuffer(0))){out_error="cannot clear offscreen composition";return false;}
            graphics.set_dialog_owner(window);graphics.set_frame_output(output);
            if(!output->initialize(window,out_error))return false;
        }else if(FAILED(original_win32::initialize_display_for_game_window(
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
        OriginalVideoSequenceCatalog video_sequence;
        if(!parse_video_sequence(loaddata,video_sequence,out_error) ||
           !game.configure_startup_video_sequence(
               video_sequence.entries(),out_error))
            return false;

        std::ifstream completion_movies(loaddata/"completedmovie.txt");
        if(!completion_movies) {
            out_error="original loaddata/completedmovie.txt missing";
            return false;
        }
        if(!game.configure_completion_movies(completion_movies,out_error))
            return false;
        std::ifstream options(loaddata/"options.txt");
        if(!game.configure_options(options,out_error))return false;
        std::ifstream credits(loaddata/"credits.txt");
        if(!game.configure_credits(credits,out_error))return false;
        std::ifstream help_info(loaddata/"helpinfo.txt");
        if(!help_info){out_error="original loaddata/helpinfo.txt missing";return false;}
        if(!game.configure_contextual_help(help_info,out_error))return false;

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
        // The global movie service is a hybrid native/retail provider; a
        // missing compatible decoder remains a visible runtime failure.
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

        // Four subgame chooser states load bitmap-table offsets; three
        // replay setup states instead load literal, keyed art at 0042A2C0.
        // The obsolete table entries 19/20/21 do not exist on the disc.
        struct MenuBackdrop {
            game_flow::State setup;
            std::optional<std::size_t> background;
        };
        constexpr std::array<MenuBackdrop,7> menu_backdrops{{
            {game_flow::State::DinoChooserSetup,8},
            {game_flow::State::MusicChooserSetup,22},
            {game_flow::State::SpudChooserSetup,9},
            {game_flow::State::AdventureChooserSetup,12},
            {game_flow::State::PlayAgainYesNoSetup,std::nullopt},
            {game_flow::State::PlayAgainDifficultySetup,std::nullopt},
            {game_flow::State::FireworksReplayChoiceSetup,std::nullopt},
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
            const bool backdrop_ok=mapping.background
                ? driver->configure_backdrop(bitmaps,*mapping.background)
                : driver->configure_retail_replay_backdrop(mapping.setup);
            if(!backdrop_ok ||
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

        game.install(ActivityId::Dinosaur,std::make_unique<DinoDriver>(root,&original_game_random));
        game.install(ActivityId::Golf,std::make_unique<GolfDriver>(
            root/"Data"/"SubGameGolf",0,&original_game_random));
        game.install(ActivityId::Maze,std::make_unique<MazeDriver>(root,0,&original_game_random));
        game.install(ActivityId::SpudMaze,
            std::make_unique<SpudMazeDriver>(root,0,&original_game_random));
        game.install(ActivityId::Squirrel,
            std::make_unique<SquirrelDriver>(root,0,&original_game_random));
        // The x64 host uses five native reconstructed Bink streams;
        // x86 retains the original DLL for comparison. Both require
        // actual decoded DD7 buffers (not fabricated frame numbers).
        auto skate_backend=
            std::make_unique<Win32HybridSpudSkateMovies>(graphics,root,
                 native_bink_opt_in(L"BTB_EXPERIMENTAL_NATIVE_BINK_SKATE"));
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
        fireworks_bank=std::make_unique<Win32HybridFireworksMovieBank>(
            graphics,root,
            native_bink_opt_in(L"BTB_EXPERIMENTAL_NATIVE_BINK_FIREWORKS"));
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
            root/"Data"/"SubGameOpen",profile_dir,bobs_band::Conductor::Bob,&original_game_random));
        game.install(ActivityId::ParkDesigner,
            std::make_unique<ParkDesignerDriver>(
                root/"Data"/"SubGameDYP",profile_dir,
                std::make_shared<RetailParkDesignerNativeService>(original_game_random,
                    [this,original_profile_font](profiles::ProfileInfo& info,
                        std::span<const Rect>& glyphs,std::string& error)->bool{
                        const auto player=game.globals().active_profile;
                        if(!player||*player<0||static_cast<std::size_t>(*player)>=game.globals().profile_metadata.size()){
                            error="retail Park Designer print profile is invalid";return false;
                        }
                        info=game.globals().profile_metadata[static_cast<std::size_t>(*player)];
                        glyphs=original_profile_font->brown_font_rectangles();
                        if(glyphs.empty()){error="original fontdatabrown.txt not loaded";return false;}
                        error.clear();return true;
                    }),
                &original_game_random));

        // An explicitly selected profile is a diagnostic shortcut. Normal
        // launches use retail state 0: the real videoseq.txt movies, followed
        // by enter-name music and the five original profile signs.
        if(selected_profile>=0) {
            if(!game.select_profile(selected_profile)) {
                out_error="requested original profile slot outside 1..5";
                return false;
            }
            game.set_outer_state(game_flow::State::ActivitySelectSetup);
        } else {
            game.set_outer_state(game_flow::State::StartupMoviesThenProfileSetup);
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
                   (fireworks_driver->editor().internal_state==
                       fireworks::InternalState::ShowPlayback||
                    fireworks_driver->editor().internal_state==
                       fireworks::InternalState::Certificate))
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
        startup_completed=true;
        out_error.clear();
        return true;
    }

    [[nodiscard]] bool step() {
        if(!running||!session)return false;
        // The original CD volume label is not extracted/validated in this
        // source-only opt-in development host. The frame driver explicitly
        // marks this compatibility waiver instead of claiming disc parity.
        original_frame.media_check_waived=true;
        original_active_frame::Callbacks callbacks{};
        callbacks.context=this;
        callbacks.run_main_game_flow=+[](void* context)->bool{
            auto& h=*static_cast<Host*>(context);
            try{
            ActivityFrameInput input=h.native_input.sample(
                h.pointer_x,h.pointer_y,timeGetTime(),h.game.accepts_native_name_input());
            input.pointer_position_valid=true;
            POINT screen{};RECT client{};
            const bool inside=GetCursorPos(&screen)&&WindowFromPoint(screen)==h.window&&
                ScreenToClient(h.window,&screen)&&GetClientRect(h.window,&client)&&
                PtInRect(&client,screen);
            input.native_cursor_enabled=(inside||!h.retail_window.windowed)&&
                (!h.output||(h.pointer_x>=0&&h.pointer_x<640&&h.pointer_y>=0&&h.pointer_y<480));
            input.original_windowed_display=h.retail_window.windowed;
            input.original_random=&h.original_game_random;
            const auto click=consume_native_click(h.native_input.click_pulse);
            input.click_pulse=click.activity_click;
            const auto entry_state=h.game.globals().dispatcher.current_state;
            const auto frame=h.session->tick(
                input,click.primary_input_click,false,true);
            if(frame.failure){
                h.error="Native game state "+std::to_string(entry_state)+
                    ", presentation stage "+
                    std::to_string(static_cast<int>(frame.failure->stage))+
                    ":\n"+frame.failure->message;
                return false;
            }
            return frame.ready_for_present;
            }catch(const std::exception& e){h.error=std::string("Native frame exception: ")+e.what();return false;}
             catch(...){h.error="Unknown native frame exception";return false;}
        };
        callbacks.prepare_native_display=+[](void* context)->original_active_frame::U32 {
            auto& h=*static_cast<Host*>(context);
            try{return static_cast<original_active_frame::U32>(h.graphics.prepare_native_frame(h.error));}
            catch(...){h.error="Native display preflight exception";return 0x80004005U;}
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
            try{return static_cast<original_active_frame::U32>(
                original_win32::create_or_reset_display_manager(
                    h.output?h.composition.handle:h.window,mode,h.graphics,h.error));}
            catch(...){h.error="Native display reset exception";return 0x80004005U;}
        };
        callbacks.present_original_game_frame=+[](void* context)
            ->original_active_frame::U32 {
            auto& h=*static_cast<Host*>(context);
            try{return h.session->present_prepared(h.error)?0U:0x80004005U;}
            catch(const std::exception& e){h.error=std::string("Native present exception: ")+e.what();return 0x80004005U;}
            catch(...){h.error="Unknown native present exception";return 0x80004005U;}
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
        if(output)output->state_changed(game.globals().dispatcher.current_state);
        if(game.globals().dispatcher.shutdown_phase==application::ShutdownPhase::FinalTeardown)
            PostMessageW(window,WM_CLOSE,0,0);
        return true; // idle-ms and recoverable DD7 frames must not quit
    }

    void set_pointer(LPARAM lparam) {
        RECT rect{};
        if(!GetClientRect(window,&rect))return;
        const int width=std::max(1L,rect.right-rect.left);
        const int height=std::max(1L,rect.bottom-rect.top);
        const int x=static_cast<short>(LOWORD(lparam));
        const int y=static_cast<short>(HIWORD(lparam));
        if(output){pointer_x=x;pointer_y=y;output->map_pointer(pointer_x,pointer_y,width,height);}
        else {pointer_x=std::clamp(x*640/width,0,639);pointer_y=std::clamp(y*480/height,0,479);}
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
    // Original 004020D0 is now on the genuine Win32 message path, before
    // application-specific game activity and mouse input handling.
    // Native modal dialogs may pump messages during the game tick.
    // Never reset/destroy the physical device while that tick owns it.
    if(msg==WM_CLOSE&&host->frame_in_progress){
        host->close_requested=true;return 0;
    }
    if(msg==WM_COMMAND&&host->frame_in_progress)return 0;
    if(host->output){
        if(msg==WM_GETMINMAXINFO)return DefWindowProcW(hwnd,msg,wp,lp);
        if((msg==WM_KEYDOWN&&wp==VK_F11)||(msg==WM_COMMAND&&LOWORD(wp)==0x3eaU)){
            if(host->running&&!host->frame_in_progress&&(msg!=WM_KEYDOWN||!(lp&0x40000000)))host->output->toggle_borderless();
            return 0;
        }
    }
    if(msg==WM_SETCURSOR&&host->running&&LOWORD(lp)==HTCLIENT){
        if(host->output){POINT p{};RECT rect{};
            if(GetCursorPos(&p)&&ScreenToClient(hwnd,&p)&&GetClientRect(hwnd,&rect)){
                int x=p.x,y=p.y;host->output->map_pointer(x,y,rect.right,rect.bottom);
                if(x<0||y<0||x>=640||y>=480)return DefWindowProcW(hwnd,msg,wp,lp);
            }
        }
        SetCursor(nullptr);return TRUE;
    }
    if(msg==WM_COMMAND&&LOWORD(wp)==0x9c41U){
        if(host->running)(void)host->game.request_window_options();
        return 0;
    }
    const auto original_result=original_win32::main_window_proc_recompiled(
        hwnd,msg,wp,lp,host->retail_window,host->graphics,host->error);
    if(msg==0x212 || msg==0x232)
        host->original_frame.last_time_ms=
            host->retail_window.last_input_ms;
    if(msg==WM_COMMAND&&LOWORD(wp)==0x3e9U&&host->running)
        host->game.request_window_credits();
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
        if(!SetTimer(hwnd,kNativeTimer,16,nullptr)){
            host->running=false;
            host->startup_completed=false;
            host->exit_code=3;
            MessageBoxA(hwnd,"Cannot start the native game frame timer",
                "Native game initialization failed",MB_OK|MB_ICONERROR);
            return -1;
        }
        return 0;
    }
    case WM_SIZE:
        if(!host->retail_window.accepting_input)host->native_input.lose_focus();
        return DefWindowProcW(hwnd,msg,wp,lp);
    case WM_MOUSEMOVE:
        host->set_pointer(lp);return 0;
    case WM_LBUTTONDOWN:
        host->set_pointer(lp);
        if(host->output&&(host->pointer_x<0||host->pointer_x>=640||host->pointer_y<0||host->pointer_y>=480))return 0;
        if(!host->native_input.pointer_held)SetCapture(hwnd);
        host->native_input.press_mouse(host->pointer_x,host->pointer_y);
        return 0;
    case WM_LBUTTONUP:
        host->set_pointer(lp);
        if(!host->output||host->native_input.pointer_held)host->native_input.release_mouse();
        if(GetCapture()==hwnd)ReleaseCapture();
        return 0;
    case WM_CAPTURECHANGED:
        if(host->native_input.pointer_held)host->native_input.cancel_mouse();
        return 0;
    case WM_CANCELMODE:
        host->native_input.cancel_mouse();
        if(GetCapture()==hwnd)ReleaseCapture();
        return 0;
    case WM_KEYDOWN:
        host->native_input.key_down(
            static_cast<unsigned>(wp),(lp&0x40000000)!=0,
            (static_cast<unsigned>(lp)>>16U)&0xffU,
            (static_cast<unsigned>(lp)&0x01000000U)!=0);
        // Original MainWindowProc (PE32 0x004020D0) never destroys
        // the game window on WM_KEYDOWN/VK_ESCAPE; it dispatches
        // unhandled keyboard messages through DefWindowProc.
        // Do not forcibly terminate an ongoing minigame or abandon
        // saved progress because the player pressed Escape.
        // Escape is translated to Options by retail accelerator 103
        // before this WM_KEYDOWN handler. The normal OS close remains available.
        return 0;
    case WM_KEYUP:
        host->native_input.key_up(static_cast<unsigned>(wp));return 0;
    case WM_CHAR:
        if(host->game.accepts_native_name_input()){
            const auto bits=static_cast<unsigned>(lp);
            (void)host->native_input.queue_name_key(static_cast<unsigned>(wp),
                ((bits>>16U)&0xffU)|((bits&0x01000000U)?0x80U:0U),
                (bits&0x40000000U)!=0);
        }else host->native_input.character(static_cast<unsigned>(wp));
        return 0;
    case WM_KILLFOCUS:
        host->native_input.lose_focus();
        if(GetCapture()==hwnd)ReleaseCapture();
        return 0;
    case WM_TIMER:
        if(wp!=kNativeTimer||host->fatal_dialog_shown||host->frame_in_progress||!host->running)return 0;
        if(!host->retail_window.accepting_input&&
           host->game.globals().dispatcher.shutdown_phase==application::ShutdownPhase::Running)return 0;
        host->frame_in_progress=true;
        {
        const bool stepped=host->step();
        host->frame_in_progress=false;
        if(!stepped){
            // Stop the 16 ms frame source BEFORE entering MessageBoxA.
            // MessageBoxA pumps a nested Win32 message loop; without this
            // latch, queued WM_TIMER messages recursively open more dialogs.
            host->fatal_dialog_shown=true;
            host->running=false;
            host->exit_code=4;
            KillTimer(hwnd,kNativeTimer);
            const auto message=host->error.empty()
              ? "Native game could not submit a complete source frame"
              : host->error;
            MessageBoxA(hwnd,message.c_str(),
              "Reconstructed game stopped at unsupported native dependency",
              MB_OK|MB_ICONERROR);
            if(IsWindow(hwnd))DestroyWindow(hwnd);
        }else if(host->close_requested&&IsWindow(hwnd)){
            host->close_requested=false;
            DestroyWindow(hwnd);
        }
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
        if(host->output)host->output->shutdown();
        if(!host->startup_completed){PostQuitMessage(host->exit_code?host->exit_code:2);return 0;}
        // The original activity owns its per-player save file (Fireworks
        // firedataN, Park Designer dypdataN, Band compositions, etc).
        // Save/unload it BEFORE writing global profile records. Previously,
        // closing the window skipped all activity-specific persistence.
        {
            std::string unload_error;
            if(!host->game.unload_current(unload_error)) {
                if(!host->exit_code)host->exit_code=5;
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
                if(!host->exit_code)host->exit_code=5;
                MessageBoxA(hwnd,save_error.c_str(),
                    "Native profile save failed",MB_OK|MB_ICONERROR);
            }
        }
        PostQuitMessage(host->exit_code);
        return 0;
    default:
        return DefWindowProcW(hwnd,msg,wp,lp);
    }
}
} // namespace

int run_recompiled_native_game(
    HINSTANCE instance,const fs::path& original_root,int show,
    bool skip_video,int selected_profile,Win32FrameOutput* output) {
    Host host(original_root,skip_video,selected_profile,output);
    WNDCLASSEXW cls{};
    cls.cbSize=sizeof(cls);
    cls.hInstance=instance;
    cls.lpfnWndProc=native_window_proc;
    cls.hCursor=LoadCursorW(nullptr,MAKEINTRESOURCEW(32512));
    cls.lpszClassName=L"BTBRetailNativeCxx";
    if(!RegisterClassExW(&cls))return 1;
    RECT size{0,0,output?1280:640,output?960:480};
    if(output){RECT work{};if(SystemParametersInfoW(SPI_GETWORKAREA,0,&work,0)){
        size.right=std::min<LONG>(size.right,work.right-work.left-40);
        size.bottom=std::min<LONG>(size.bottom,work.bottom-work.top-80);
    }}
    AdjustWindowRect(&size,WS_OVERLAPPEDWINDOW,FALSE);
    const auto hwnd=CreateWindowExW(
        0,cls.lpszClassName,
        output?L"Bob Builds a Park - Enhanced Vulkan":L"Bob Builds a Park - Original Source Native DirectDraw7",
        WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
        size.right-size.left,size.bottom-size.top,
        nullptr,nullptr,instance,&host);
    if(!hwnd)return 2;
    // PE32 resource 103 contains Escape/Options and Alt+X/credits.
    // Create the exact two-entry accelerator table without embedding the
    // retail executable/resource section. Translate before dispatch as
    // in 00402700 (including WM_SYSKEYDOWN for Alt+X).
    std::array<ACCEL,kRetailAccelerators.size()> entries{};
    for(std::size_t i=0;i<entries.size();++i){
        entries[i].fVirt=kRetailAccelerators[i].flags;
        entries[i].key=kRetailAccelerators[i].key;
        entries[i].cmd=kRetailAccelerators[i].command;
    }
    const auto accelerators=CreateAcceleratorTableW(entries.data(),static_cast<int>(entries.size()));
    if(!accelerators){
        host.startup_completed=false;
        DestroyWindow(hwnd);return 3;
    }
    ShowWindow(hwnd,show);
    UpdateWindow(hwnd);
    MSG msg{};
    BOOL received{};
    if(output&&output->wants_idle_present()){
        // Run presentation independently of the source game timer. Never
        // rerun a game tick just to redraw the most recent complete frame.
        received=1;
        // High-resolution one-shot waits avoid the legacy ~15.6 ms
        // scheduler granularity without changing the system timer period.
        HANDLE idle_timer=CreateWaitableTimerExW(nullptr,nullptr,0x00000002U,TIMER_MODIFY_STATE|SYNCHRONIZE);
        if(!idle_timer)idle_timer=CreateWaitableTimerW(nullptr,FALSE,nullptr);
        for(;;){
            unsigned drained=0;
            while(drained++<128&&PeekMessageW(&msg,nullptr,0,0,PM_REMOVE)){
                if(msg.message==WM_QUIT){received=0;break;}
                if(!TranslateAcceleratorW(hwnd,accelerators,&msg)){
                    TranslateMessage(&msg);DispatchMessageW(&msg);
                }
            }
            if(!received)break;
            if(host.running&&!host.frame_in_progress&&!IsIconic(hwnd)){
                try{if(!output->idle_present(host.error))throw std::runtime_error(host.error);}
                catch(const std::exception& ex){
                    host.running=false;host.exit_code=4;
                    MessageBoxA(hwnd,ex.what(),"Enhanced presentation failed",MB_OK|MB_ICONERROR);
                    if(IsWindow(hwnd))DestroyWindow(hwnd);
                }
            }
            const unsigned wait=host.running&&!IsIconic(hwnd)?output->idle_wait_us():50000U;
            LARGE_INTEGER due{};due.QuadPart=-static_cast<LONGLONG>(std::max(1U,wait))*10;
            if(idle_timer&&SetWaitableTimer(idle_timer,&due,0,nullptr,nullptr,FALSE))
                MsgWaitForMultipleObjectsEx(1,&idle_timer,INFINITE,QS_ALLINPUT,MWMO_INPUTAVAILABLE);
            else MsgWaitForMultipleObjectsEx(0,nullptr,std::max(1U,(wait+999)/1000),QS_ALLINPUT,MWMO_INPUTAVAILABLE);
        }
        if(idle_timer)CloseHandle(idle_timer);
    }else while((received=GetMessageW(&msg,nullptr,0,0))>0){
        if(!TranslateAcceleratorW(hwnd,accelerators,&msg)){
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    DestroyAcceleratorTable(accelerators);
    if(received<0)return 6;
    return static_cast<int>(msg.wParam);
}
} // namespace btb::full_game
#endif
