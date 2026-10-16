#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/maze_data.hpp"
#include "btb/maze_box_layout.hpp"
#include "btb/original_motion4.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace btb::full_game {

// Incremental native binding of retail Maze 0x0041A730 / 0x0041D4D0.
// Original three-screen map, cursor-independent keyboard graph movement,
// actor sprite and countdown are source-backed. Spud NPC/package collision,
// package pickup, mouse arbitration and success logic are
// intentionally NOT modeled as if already reconstructed.
class MazeDriver final : public ActivityDriver {
public:
    MazeDriver(std::filesystem::path install_root,int difficulty=0)
        :install_root_(std::move(install_root)),difficulty_(difficulty) {}

    void configure_menu_state(const RetailMenuState& menu) noexcept override;
    [[nodiscard]] bool initialize(int player_index,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;
    [[nodiscard]] const original_motion4::Player* player() const noexcept {
        return data_ ? &player_ : nullptr;
    }
    [[nodiscard]] int remaining_seconds() const noexcept {
        return remaining_seconds_;
    }
    [[nodiscard]] const maze::BoxAllocation& boxes() const noexcept {
        return boxes_;
    }

private:
    [[nodiscard]] ActivityFrameOutput draw() const;

    std::filesystem::path install_root_{};
    int difficulty_{};
    int player_index_{-1};
    std::unique_ptr<maze::Data> data_{};
    original_motion4::Player player_{};
    maze::BoxAllocation boxes_{};
    int remaining_seconds_{};
    int started_centiseconds_{};
    bool intro_voice_queued_{};
    bool intro_done_{};
    bool timer_started_{};
    bool lowtime_voice_queued_{};
    // 0=live;1=timeout feedback;2=source final return after voice drain.
    int exit_phase_{};
};

} // namespace btb::full_game
