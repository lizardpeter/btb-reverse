#pragma once

#include "btb/application_state.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

namespace btb::game_flow {

enum class State : std::int32_t {
    StartupMoviesThenProfileSetup = 0x00,
    PlayerProfileAndNameEntry = 0x01,
    ProfileFlowCompleteToActivitySelect = 0x02,
    MainFrontEndUpdate = 0x03,
    ActivitySelectSetup = 0x04,
    ActivitySelectUpdate = 0x05,
    DormantGenericUiCleanup = 0x06,
    RetailNoOp07 = 0x07,
    QuitTransitionSetup = 0x08,
    OpenWholeGameQuitConfirmation = 0x09,
    RetailNoOp0A = 0x0A,
    RetailNoOp0B = 0x0B,
    HerdingPregameSetup = 0x0C,
    HerdingPregameUpdate = 0x0D,
    HerdingInit = 0x0E,
    HerdingRun = 0x0F,
    DinoChooserSetup = 0x10,
    DinoChooserUpdate = 0x11,
    DinoPregameSetup = 0x12,
    DinoPregameUpdate = 0x13,
    DinoInit = 0x14,
    DinoRun = 0x15,
    SpudChooserSetup = 0x16,
    SpudChooserUpdate = 0x17,
    SpudSkatePregameSetup = 0x18,
    SpudSkatePregameUpdate = 0x19,
    SpudSkateInit = 0x1A,
    SpudSkateRun = 0x1B,
    AdventureChooserSetup = 0x1C,
    AdventureChooserUpdate = 0x1D,
    MazePregameSetup = 0x1E,
    MazePregameUpdate = 0x1F,
    MazeInit = 0x20,
    MazeRun = 0x21,
    FireworksPregameSetup = 0x22,
    FireworksPregameUpdate = 0x23,
    FireworksInit = 0x24,
    FireworksRun = 0x25,
    SquirrelPregameSetup = 0x26,
    SquirrelPregameUpdate = 0x27,
    SquirrelInit = 0x28,
    SquirrelRun = 0x29,
    MusicChooserSetup = 0x2A,
    MusicChooserUpdate = 0x2B,
    BobsBandPregameSetup = 0x2C,
    BobsBandPregameUpdate = 0x2D,
    BobsBandInit = 0x2E,
    BobsBandRun = 0x2F,
    ParkDesignerPregameSetup = 0x30,
    ParkDesignerPregameUpdate = 0x31,
    ParkDesignerInit = 0x32,
    ParkDesignerRun = 0x33,
    GolfPregameSetup = 0x34,
    GolfPregameUpdate = 0x35,
    GolfInit = 0x36,
    GolfRun = 0x37,
    SpudMazePregameSetup = 0x38,
    SpudMazePregameUpdate = 0x39,
    SpudMazeInit = 0x3A,
    SpudMazeRun = 0x3B,
    PlayAgainYesNoSetup = 0x3C,
    PlayAgainYesNoUpdate = 0x3D,
    PlayAgainDifficultySetup = 0x3E,
    PlayAgainDifficultyUpdate = 0x3F,
    SharedMovieTransition = 0x40,
    SceneSetMovieTransition = 0x41,
    FireworksReplayChoiceSetup = 0x42,
    FireworksReplayChoiceUpdate = 0x43,
};

inline constexpr std::size_t kStateCount = 68;
inline constexpr std::int32_t kHighestState = 0x43;

enum class StateKind {
    FrontEnd,
    PregameSetup,
    PregameUpdate,
    ChooserSetup,
    ChooserUpdate,
    ActivityInit,
    ActivityRun,
    ReplaySetup,
    ReplayUpdate,
    MovieTransition,
    NoOp,
};

struct StateDescriptor {
    State state{};
    std::uint32_t handler_address{};
    StateKind kind{StateKind::NoOp};
    std::string_view module{};
    std::string_view role{};
};

inline constexpr std::array<StateDescriptor,kStateCount> kStates{{
    {State::StartupMoviesThenProfileSetup,0x0042A4A8,StateKind::FrontEnd,"front_end","startup_movies_then_profile_setup"},
    {State::PlayerProfileAndNameEntry,0x0042A53D,StateKind::FrontEnd,"front_end","player_profile_and_name_entry"},
    {State::ProfileFlowCompleteToActivitySelect,0x0042A72F,StateKind::FrontEnd,"front_end","profile_flow_complete_to_activity_select"},
    {State::MainFrontEndUpdate,0x0042A745,StateKind::FrontEnd,"front_end","main_front_end_update"},
    {State::ActivitySelectSetup,0x0042A7E7,StateKind::FrontEnd,"front_end","activity_select_setup"},
    {State::ActivitySelectUpdate,0x0042A8B0,StateKind::FrontEnd,"front_end","activity_select_update"},
    {State::DormantGenericUiCleanup,0x0042A994,StateKind::FrontEnd,"front_end","dormant_generic_ui_cleanup"},
    {State::RetailNoOp07,0x0042CD65,StateKind::NoOp,"dispatcher","retail_no_op"},
    {State::QuitTransitionSetup,0x0042A9A5,StateKind::FrontEnd,"front_end","quit_transition_setup"},
    {State::OpenWholeGameQuitConfirmation,0x0042A9F3,StateKind::FrontEnd,"front_end","open_whole_game_quit_confirmation"},
    {State::RetailNoOp0A,0x0042CD65,StateKind::NoOp,"dispatcher","no_op"},
    {State::RetailNoOp0B,0x0042CD65,StateKind::NoOp,"dispatcher","no_op"},
    {State::HerdingPregameSetup,0x0042AA12,StateKind::PregameSetup,"herding","herding_pregame_setup"},
    {State::HerdingPregameUpdate,0x0042AAD4,StateKind::PregameUpdate,"herding","herding_pregame_update"},
    {State::HerdingInit,0x0042ABC9,StateKind::ActivityInit,"herding","activity_init"},
    {State::HerdingRun,0x0042AC3B,StateKind::ActivityRun,"herding","activity_run"},
    {State::DinoChooserSetup,0x0042ACC1,StateKind::ChooserSetup,"dino","dino_chooser_setup"},
    {State::DinoChooserUpdate,0x0042AD97,StateKind::ChooserUpdate,"dino","dino_chooser_update"},
    {State::DinoPregameSetup,0x0042AE87,StateKind::PregameSetup,"dino","dino_pregame_setup"},
    {State::DinoPregameUpdate,0x0042AF19,StateKind::PregameUpdate,"dino","dino_pregame_update"},
    {State::DinoInit,0x0042B03A,StateKind::ActivityInit,"dino","activity_init"},
    {State::DinoRun,0x0042B0B8,StateKind::ActivityRun,"dino","activity_run"},
    {State::SpudChooserSetup,0x0042B15F,StateKind::ChooserSetup,"spud","spud_chooser_setup"},
    {State::SpudChooserUpdate,0x0042B239,StateKind::ChooserUpdate,"spud","spud_chooser_update"},
    {State::SpudSkatePregameSetup,0x0042B583,StateKind::PregameSetup,"spud_skate","spud_skate_pregame_setup"},
    {State::SpudSkatePregameUpdate,0x0042B624,StateKind::PregameUpdate,"spud_skate","spud_skate_pregame_update"},
    {State::SpudSkateInit,0x0042B70F,StateKind::ActivityInit,"spud_skate","activity_init"},
    {State::SpudSkateRun,0x0042B77A,StateKind::ActivityRun,"spud_skate","activity_run"},
    {State::AdventureChooserSetup,0x0042B7A6,StateKind::ChooserSetup,"adventure_playground","adventure_playground_chooser_setup"},
    {State::AdventureChooserUpdate,0x0042B880,StateKind::ChooserUpdate,"adventure_playground","adventure_playground_chooser_update"},
    {State::MazePregameSetup,0x0042BBCB,StateKind::PregameSetup,"maze","maze_pregame_setup"},
    {State::MazePregameUpdate,0x0042BC89,StateKind::PregameUpdate,"maze","maze_pregame_update"},
    {State::MazeInit,0x0042BD95,StateKind::ActivityInit,"maze","activity_init"},
    {State::MazeRun,0x0042BDE7,StateKind::ActivityRun,"maze","activity_run"},
    {State::FireworksPregameSetup,0x0042BE62,StateKind::PregameSetup,"fireworks","fireworks_pregame_setup"},
    {State::FireworksPregameUpdate,0x0042BF0C,StateKind::PregameUpdate,"fireworks","fireworks_pregame_update"},
    {State::FireworksInit,0x0042C039,StateKind::ActivityInit,"fireworks","activity_init"},
    {State::FireworksRun,0x0042C08B,StateKind::ActivityRun,"fireworks","activity_run"},
    {State::SquirrelPregameSetup,0x0042C128,StateKind::PregameSetup,"squirrel","squirrel_pregame_setup"},
    {State::SquirrelPregameUpdate,0x0042C1C0,StateKind::PregameUpdate,"squirrel","squirrel_pregame_update"},
    {State::SquirrelInit,0x0042C2C1,StateKind::ActivityInit,"squirrel","activity_init"},
    {State::SquirrelRun,0x0042C313,StateKind::ActivityRun,"squirrel","activity_run"},
    {State::MusicChooserSetup,0x0042C384,StateKind::ChooserSetup,"bobs_band","music_chooser_setup"},
    {State::MusicChooserUpdate,0x0042C45E,StateKind::ChooserUpdate,"bobs_band","music_chooser_update"},
    {State::BobsBandPregameSetup,0x0042C534,StateKind::PregameSetup,"bobs_band","bobs_band_pregame_setup"},
    {State::BobsBandPregameUpdate,0x0042C5B9,StateKind::PregameUpdate,"bobs_band","bobs_band_pregame_update"},
    {State::BobsBandInit,0x0042C6AE,StateKind::ActivityInit,"bobs_band","activity_init"},
    {State::BobsBandRun,0x0042C70C,StateKind::ActivityRun,"bobs_band","activity_run"},
    {State::ParkDesignerPregameSetup,0x0042C78D,StateKind::PregameSetup,"park_designer","park_designer_pregame_setup"},
    {State::ParkDesignerPregameUpdate,0x0042C83A,StateKind::PregameUpdate,"park_designer","park_designer_pregame_update"},
    {State::ParkDesignerInit,0x0042C929,StateKind::ActivityInit,"park_designer","activity_init"},
    {State::ParkDesignerRun,0x0042C9A3,StateKind::ActivityRun,"park_designer","activity_run"},
    {State::GolfPregameSetup,0x0042B93C,StateKind::PregameSetup,"golf","golf_pregame_setup"},
    {State::GolfPregameUpdate,0x0042B9FA,StateKind::PregameUpdate,"golf","golf_pregame_update"},
    {State::GolfInit,0x0042BAEA,StateKind::ActivityInit,"golf","activity_init"},
    {State::GolfRun,0x0042BB55,StateKind::ActivityRun,"golf","activity_run"},
    {State::SpudMazePregameSetup,0x0042B2F5,StateKind::PregameSetup,"spud_maze","spud_maze_pregame_setup"},
    {State::SpudMazePregameUpdate,0x0042B3B3,StateKind::PregameUpdate,"spud_maze","spud_maze_pregame_update"},
    {State::SpudMazeInit,0x0042B4BF,StateKind::ActivityInit,"spud_maze","activity_init"},
    {State::SpudMazeRun,0x0042B52A,StateKind::ActivityRun,"spud_maze","activity_run"},
    {State::PlayAgainYesNoSetup,0x0042CAF4,StateKind::ReplaySetup,"front_end","play_again_yes_no_setup"},
    {State::PlayAgainYesNoUpdate,0x0042CB5C,StateKind::ReplayUpdate,"front_end","play_again_yes_no_update"},
    {State::PlayAgainDifficultySetup,0x0042CC28,StateKind::ReplaySetup,"front_end","play_again_difficulty_setup"},
    {State::PlayAgainDifficultyUpdate,0x0042CC73,StateKind::ReplayUpdate,"front_end","play_again_difficulty_update"},
    {State::SharedMovieTransition,0x0042CCEA,StateKind::MovieTransition,"front_end","shared_movie_transition"},
    {State::SceneSetMovieTransition,0x0042A708,StateKind::MovieTransition,"front_end","scene_set_movie_transition"},
    {State::FireworksReplayChoiceSetup,0x0042C9D4,StateKind::ReplaySetup,"fireworks","fireworks_replay_choice_setup"},
    {State::FireworksReplayChoiceUpdate,0x0042CA34,StateKind::ReplayUpdate,"fireworks","fireworks_replay_choice_update"},
}};

[[nodiscard]] constexpr bool valid_state_value(std::int32_t value) noexcept {
    return value >= 0 && value <= kHighestState;
}

[[nodiscard]] constexpr const StateDescriptor* descriptor(
    std::int32_t value) noexcept {
    if (!valid_state_value(value)) {
        return nullptr;
    }
    return &kStates[static_cast<std::size_t>(value)];
}

enum class Intercept {
    StateTable,
    ShutdownOrCredits,
    Options,
    ProgressScreen,
    GenericYesNo,
    WholeGameQuit,
    LeaveCurrentActivity,
    PlayAgainOverlay,
    LegacyNoOpArgument0,
    LegacyNoOpArgument1,
    GlobalMovieMode13,
    GlobalMovieMode14,
    InvalidState,
};

struct DispatcherInput {
    application::ShutdownPhase shutdown_phase{application::ShutdownPhase::Running};

    bool options_active{};
    bool progress_screen_active{};
    bool generic_yes_no_active{};
    bool whole_game_quit_active{};
    bool leave_activity_active{};
    bool play_again_overlay_active{};

    std::int32_t legacy_intercept_value{}; // global 0x0051C2D8

    std::int32_t generic_screen_mode{}; // global 0x0051C27C
    bool global_movie_finished{};

    std::int32_t current_state{};
    std::int32_t saved_state{}; // global 0x0051B418
};

struct DispatcherStep {
    Intercept intercept{Intercept::StateTable};
    std::int32_t current_state{};
    std::int32_t generic_screen_mode{};
    bool pause_global_movie{};
    bool update_global_movie{};
    bool clear_input_pulse{};
    bool unload_generic_ui{};
    bool release_transition_surface{};
    bool call_shutdown_callback{};
    std::optional<std::int32_t> legacy_noop_argument{};
    const StateDescriptor* dispatch{};
};

// Source-level reconstruction of the pre-jump-table control flow in
// 0x0042A2C0 RunMainGameFlow.
//
// This intentionally models control-flow/side-effect selection, not DirectDraw
// or modal rendering themselves.
[[nodiscard]] constexpr DispatcherStep dispatch_step(
    const DispatcherInput& input,
    bool shutdown_callback_present = true) noexcept {

    DispatcherStep out;
    out.current_state = input.current_state;
    out.generic_screen_mode = input.generic_screen_mode;

    if (input.shutdown_phase != application::ShutdownPhase::Running) {
        out.intercept = Intercept::ShutdownOrCredits;
        out.unload_generic_ui = true;
        out.release_transition_surface = true;
        out.call_shutdown_callback = shutdown_callback_present;
        return out;
    }

    if (input.options_active) {
        out.intercept = Intercept::Options;
        out.pause_global_movie = true;
        return out;
    }
    if (input.progress_screen_active) {
        out.intercept = Intercept::ProgressScreen;
        return out;
    }
    if (input.generic_yes_no_active) {
        out.intercept = Intercept::GenericYesNo;
        out.pause_global_movie = true;
        return out;
    }
    if (input.whole_game_quit_active) {
        out.intercept = Intercept::WholeGameQuit;
        out.pause_global_movie = true;
        return out;
    }
    if (input.leave_activity_active) {
        out.intercept = Intercept::LeaveCurrentActivity;
        out.pause_global_movie = true;
        return out;
    }
    if (input.play_again_overlay_active) {
        out.intercept = Intercept::PlayAgainOverlay;
        out.pause_global_movie = true;
        return out;
    }

    if (input.legacy_intercept_value > 0) {
        out.legacy_noop_argument =
            input.legacy_intercept_value < 10 ? 0 : 1;
        out.intercept =
            input.legacy_intercept_value < 10
                ? Intercept::LegacyNoOpArgument0
                : Intercept::LegacyNoOpArgument1;
        return out;
    }

    if (input.generic_screen_mode == 13) {
        out.intercept = Intercept::GlobalMovieMode13;
        out.update_global_movie = true;
        if (!input.global_movie_finished) {
            return out;
        }

        out.current_state = input.saved_state;
        out.generic_screen_mode = 2;
        out.clear_input_pulse = true;
        // Retail continues into the state-table dispatch on this same frame.
    }

    if (out.generic_screen_mode == 14) {
        out.intercept = Intercept::GlobalMovieMode14;
        out.update_global_movie = true;
        if (!input.global_movie_finished) {
            return out;
        }

        ++out.current_state;
        out.generic_screen_mode =
            out.current_state == 0x23 ? 7 : 2;
        out.clear_input_pulse = true;
        // Retail likewise falls through to the state jump table immediately.
    }

    if (!valid_state_value(out.current_state)) {
        out.intercept = Intercept::InvalidState;
        return out;
    }

    out.intercept = Intercept::StateTable;
    out.dispatch = descriptor(out.current_state);
    return out;
}

} // namespace btb::game_flow
