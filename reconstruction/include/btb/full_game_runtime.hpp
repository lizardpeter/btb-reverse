#pragma once

#include "btb/game_flow.hpp"
#include "btb/player_progress.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace btb::full_game {

// Source-level whole-game activity registry. Each tuple matches a distinct
// pregame/init/run group in retail 0x0042A2C0's 68-state jump table.
enum class ActivityId : std::size_t {
    Herding, Dinosaur, SpudSkate, Maze, Fireworks,
    Squirrel, BobsBand, ParkDesigner, Golf, SpudMaze
};
inline constexpr std::size_t kActivityCount = 10;

struct ActivityEntry {
    ActivityId id{};
    game_flow::State pregame_setup{};
    game_flow::State pregame_update{};
    game_flow::State init{};
    game_flow::State run{};
    const char* module{};
};

inline constexpr std::array<ActivityEntry, kActivityCount> kActivities{{
    {ActivityId::Herding, game_flow::State::HerdingPregameSetup,
        game_flow::State::HerdingPregameUpdate, game_flow::State::HerdingInit,
        game_flow::State::HerdingRun, "herding"},
    {ActivityId::Dinosaur, game_flow::State::DinoPregameSetup,
        game_flow::State::DinoPregameUpdate, game_flow::State::DinoInit,
        game_flow::State::DinoRun, "dino"},
    {ActivityId::SpudSkate, game_flow::State::SpudSkatePregameSetup,
        game_flow::State::SpudSkatePregameUpdate, game_flow::State::SpudSkateInit,
        game_flow::State::SpudSkateRun, "spud_skate"},
    {ActivityId::Maze, game_flow::State::MazePregameSetup,
        game_flow::State::MazePregameUpdate, game_flow::State::MazeInit,
        game_flow::State::MazeRun, "maze"},
    {ActivityId::Fireworks, game_flow::State::FireworksPregameSetup,
        game_flow::State::FireworksPregameUpdate, game_flow::State::FireworksInit,
        game_flow::State::FireworksRun, "fireworks"},
    {ActivityId::Squirrel, game_flow::State::SquirrelPregameSetup,
        game_flow::State::SquirrelPregameUpdate, game_flow::State::SquirrelInit,
        game_flow::State::SquirrelRun, "squirrel"},
    {ActivityId::BobsBand, game_flow::State::BobsBandPregameSetup,
        game_flow::State::BobsBandPregameUpdate, game_flow::State::BobsBandInit,
        game_flow::State::BobsBandRun, "bobs_band"},
    {ActivityId::ParkDesigner, game_flow::State::ParkDesignerPregameSetup,
        game_flow::State::ParkDesignerPregameUpdate, game_flow::State::ParkDesignerInit,
        game_flow::State::ParkDesignerRun, "park_designer"},
    {ActivityId::Golf, game_flow::State::GolfPregameSetup,
        game_flow::State::GolfPregameUpdate, game_flow::State::GolfInit,
        game_flow::State::GolfRun, "golf"},
    {ActivityId::SpudMaze, game_flow::State::SpudMazePregameSetup,
        game_flow::State::SpudMazePregameUpdate, game_flow::State::SpudMazeInit,
        game_flow::State::SpudMazeRun, "spud_maze"},
}};

[[nodiscard]] constexpr const ActivityEntry*
activity_for_state(game_flow::State state) noexcept {
    for (const auto& activity : kActivities) {
        if (state == activity.init || state == activity.run ||
            state == activity.pregame_setup ||
            state == activity.pregame_update) {
            return &activity;
        }
    }
    return nullptr;
}

// Draws and sounds are source-neutral commands for the original shared
// bitmap/DirectSound systems. The host still needs to connect their real
// implementations; this is not an invented alternative renderer.
struct Rect {
    int left{};
    int top{};
    int right{};
    int bottom{};
};
struct Draw {
    std::string source_asset{};
    int x{};
    int y{};
    std::optional<Rect> source_rectangle{};
    bool color_keyed{};
};
enum class AudioOperation {
    PlayFile,
    StopBackingTrack,
    StopManagedSounds,
    ManagedSoundId,
};
struct Audio {
    AudioOperation operation{};
    std::string source_asset{};
    int sound_id{-1};
    int priority{};
    int arbitration_class{};
};
struct ProgressMutation {
    progress::Slot slot{};
    std::int32_t value{1};
};

struct ActivityFrameInput {
    int pointer_x{};
    int pointer_y{};
    bool click_pulse{};
    bool managed_sound_playing{};
    bool backing_track_playing{true};
    bool confirmation_resolved{};
    bool confirmation_yes{};
    int elapsed_centiseconds{};
    int frame_delta{1};
    int random_value{};
};

struct ActivityFrameOutput {
    std::vector<Draw> draws{};
    std::vector<Audio> audio{};
    std::vector<ProgressMutation> progress_writes{};
    std::optional<int> shared_completion_code{};
    std::optional<std::int32_t> next_outer_state{};
    std::optional<std::int32_t> next_ui_context{};
    bool save_and_unload{};
    bool open_yes_no_confirmation{};
    std::optional<int> yes_no_context{};
};

// Each activity translates its own original native effects into shared
// commands. The root must not execute an activity without a registered driver.
class ActivityDriver {
public:
    virtual ~ActivityDriver() = default;
    [[nodiscard]] virtual bool initialize(
        int player_index, std::string& error) = 0;
    [[nodiscard]] virtual ActivityFrameOutput advance(
        const ActivityFrameInput& input) = 0;
    [[nodiscard]] virtual bool unload(std::string& error) = 0;
};

struct GameGlobals {
    game_flow::DispatcherInput dispatcher{};
    std::array<progress::Record, progress::kPlayerCount> player_progress{};
    std::optional<int> active_profile{};
    progress::FinaleGate finale_gate{};
    int shared_completion_code{-1};
    int ui_context{};
    bool input_pulse{};
};

enum class FrameKind {
    InactiveProfile,
    Intercept,
    FrontEndRequiresAdapter,
    ActivityRequiresAdapter,
    ActivityInitialized,
    ActivityUpdated,
    ActivityFailed,
    StateNoOp,
    InvalidState,
};

struct GameFrame {
    FrameKind kind{FrameKind::StateNoOp};
    game_flow::DispatcherStep dispatch{};
    std::optional<ActivityId> activity{};
    ActivityFrameOutput effects{};
    std::string error{};
    bool clear_input_pulse{};
    bool state_changed{};
};

// This is the shared source-level coordinator for ten native activities.
// It does not synthesize their pregame/chooser UI or invent transitions for
// unavailable modules. The dispatch result remains observable on every frame.
class GameRoot {
public:
    [[nodiscard]] const GameGlobals& globals() const noexcept { return globals_; }
    [[nodiscard]] GameGlobals& globals() noexcept { return globals_; }

    void install(ActivityId id, std::unique_ptr<ActivityDriver> driver);
    [[nodiscard]] bool select_profile(int index) noexcept;
    void set_outer_state(game_flow::State state) noexcept;
    [[nodiscard]] GameFrame advance(const ActivityFrameInput& input);
    [[nodiscard]] bool unload_current(std::string& error);

private:
    void apply_effects(const ActivityFrameOutput& output) noexcept;
    std::array<std::unique_ptr<ActivityDriver>, kActivityCount> drivers_{};
    std::optional<ActivityId> initialized_activity_{};
    GameGlobals globals_{};
};

} // namespace btb::full_game
