#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/original_motion4.hpp"
#include "btb/spud_maze_data.hpp"
#include "btb/spud_maze_runtime.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace btb::full_game {

// Original 0x004207E0 + 0x00423F40 source-backed native activity.
// Integrates five retail screen graphs, source damaged-repair selection,
// Bob's reconstructed movement/hammer interaction, 83px repair sprite,
// countdown and activity save. The separate Pilchard motion/collision and
// original Mr Bentley narrator sequence still require their native host.
class SpudMazeDriver final : public ActivityDriver {
public:
    explicit SpudMazeDriver(std::filesystem::path root,int difficulty=0)
        :root_(std::move(root)),difficulty_(difficulty) {}

    void configure_menu_state(const RetailMenuState& menu) noexcept override;
    [[nodiscard]] bool initialize(int player,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    [[nodiscard]] const original_motion4::Player* bob() const noexcept {
        return data_ ? &bob_ : nullptr;
    }
    [[nodiscard]] int remaining_repairs() const noexcept {
        return repairs_remaining_;
    }
    [[nodiscard]] int timer() const noexcept { return timer_; }

private:
    [[nodiscard]] ActivityFrameOutput draw() const;

    std::filesystem::path root_;
    int difficulty_{};
    int player_{-1};
    std::unique_ptr<spud_maze::Data> data_{};
    original_motion4::Player bob_{};
    int total_repairs_{};
    int repairs_remaining_{};
    int repair_animation_frames_{}; // 0x005109F0: four-tick frame gate
    int repair_cycles_remaining_{}; // 0x00514514: five repair passes
    int timer_{};
    int timer_origin_{};
    int spawned_pilchard_delay_{};
    int exit_phase_{};
    bool won_{};
    bool first_audio_{};
    bool warning_played_{};
};

} // namespace btb::full_game
