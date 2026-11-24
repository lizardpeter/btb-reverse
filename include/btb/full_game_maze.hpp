#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/retail_crt_random.hpp"
#include "btb/maze_data.hpp"
#include "btb/maze_input.hpp"
#include "btb/maze_box_layout.hpp"
#include "btb/maze_package_pickup.hpp"
#include "btb/maze_transition.hpp"
#include "btb/original_motion4.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace btb::full_game {

// Incremental native binding of retail Maze 0x0041A730 / 0x0041D4D0.
// Original three-screen map, source input, package capture and progress,
// actor sprite, countdown, live Spud NPC and screen transition are
// source-backed. Physical retail parity still requires comparison.
class MazeDriver final : public ActivityDriver {
public:
    MazeDriver(std::filesystem::path install_root,int difficulty=0,
               retail::OriginalRetailRandom* shared_random=nullptr)
        :install_root_(std::move(install_root)),difficulty_(difficulty),shared_random_(shared_random) {}

    void configure_menu_state(const RetailMenuState& menu) noexcept override;
    [[nodiscard]] bool initialize(int player_index,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;
    [[nodiscard]] const original_motion4::Player* player() const noexcept {
        return data_ ? &player_ : nullptr;
    }
    [[nodiscard]] const original_motion4::Npc* npc() const noexcept {
        return data_ ? &npc_ : nullptr;
    }
    [[nodiscard]] int remaining_seconds() const noexcept {
        return remaining_seconds_;
    }
    [[nodiscard]] const maze::BoxAllocation& boxes() const noexcept {
        return boxes_;
    }

private:
    [[nodiscard]] ActivityFrameOutput draw() const;
    [[nodiscard]] ActivityFrameOutput draw_transition(const maze::ScreenTransitionFrame&) const;
    void append_boxes(ActivityFrameOutput&,int screen,int offset) const;
    void append_hud(ActivityFrameOutput&) const;
    void append_spud(ActivityFrameOutput&,int screen,int offset) const;
    maze::ScreenTransitionState transition_{};
    int drive_animation_frame_{};
    int drive_animation_wait_{};

    std::filesystem::path install_root_{};
    int difficulty_{};
    retail::OriginalRetailRandom standalone_random_{};
    retail::OriginalRetailRandom* shared_random_{};
    [[nodiscard]] retail::OriginalRetailRandom& random_stream() noexcept {
        return shared_random_?*shared_random_:standalone_random_;
    }
    int player_index_{-1};
    std::unique_ptr<maze::Data> data_{};
    original_motion4::Player player_{};
    original_motion4::Npc npc_{};
    maze::BoxAllocation boxes_{};
    maze::InputModeState input_mode_{};
    maze::DirectionalDebounce mouse_debounce_{};
    bool pointer_sampled_{};
    int pickup_animation_frame_{};
    int pickup_animation_wait_{};
    int pickup_voice_sequence_{};
    bool won_{};
    int remaining_seconds_{};
    int started_centiseconds_{};
    bool intro_voice_queued_{};
    bool intro_done_{};
    bool timer_started_{};
    bool lowtime_voice_queued_{};
    // 0=live;1=outcome voice;2=source final return after voice drain.
    int exit_phase_{};
};

} // namespace btb::full_game
