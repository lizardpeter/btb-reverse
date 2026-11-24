#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/retail_crt_random.hpp"
#include "btb/original_motion4.hpp"
#include "btb/maze_input.hpp"
#include "btb/spud_maze_data.hpp"
#include "btb/spud_maze_runtime.hpp"
#include "btb/spud_maze_pilchard_native.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace btb::full_game {

// Original 0x004207E0 + 0x00423F40 source-backed native activity.
// Integrates five retail screen graphs, source damaged-repair selection,
// Bob's reconstructed movement/hammer interaction, 83px repair sprite,
// countdown, live Pilchard, scrolling maps, outcome narration and save.
// Physical retail parity still requires an interactive comparison.
class SpudMazeDriver final : public ActivityDriver {
public:
    explicit SpudMazeDriver(std::filesystem::path root,int difficulty=0,
                            retail::OriginalRetailRandom* shared_random=nullptr)
        :root_(std::move(root)),difficulty_(difficulty),shared_random_(shared_random) {}

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
    [[nodiscard]] const spud_maze::PilchardNative& pilchard() const noexcept {
        return pilchard_;
    }
    [[nodiscard]] const maze_spud_batch::CollisionState& hammer_state() const noexcept {
        return hammer_;
    }

private:
    [[nodiscard]] ActivityFrameOutput draw(const spud_maze::SlideFrame* slide=nullptr) const;
    spud_maze::SlideState slide_{};
    spud_maze::BentleyState bentley_{};
    void append_repairs(ActivityFrameOutput&,int screen,int offset) const;

    std::filesystem::path root_;
    int difficulty_{};
    retail::OriginalRetailRandom standalone_random_{};
    retail::OriginalRetailRandom* shared_random_{};
    [[nodiscard]] retail::OriginalRetailRandom& random_stream() noexcept {
        return shared_random_?*shared_random_:standalone_random_;
    }
    int player_{-1};
    std::unique_ptr<spud_maze::Data> data_{};
    original_motion4::Player bob_{};
    maze::InputModeState input_mode_{};
    bool pointer_sampled_{};
    spud_maze::PilchardNative pilchard_{};
    maze_spud_batch::CollisionState hammer_{};
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
