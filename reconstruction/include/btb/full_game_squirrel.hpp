#pragma once
#include "btb/full_game_runtime.hpp"
#include "btb/squirrel_data.hpp"
#include "btb/squirrel_presentation.hpp"
#include "btb/squirrel_runtime.hpp"
#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace btb::full_game {

// Native adapter for 0x00425160 / 0x004279F0 Lofty's Squirrel Run.
// Uses the retail connector chain, 36-piece encoding, conveyor correctness
// flags, and recovered 95-tick run-assembly motion. Lofty's exact fetch/
// decoy-return choreography, sound IDs and transition scroll still need
// differential verification; they must not be described as retail parity.
class SquirrelDriver final : public ActivityDriver {
public:
    struct Choice {
        int piece_id{-1};
        bool correct{};
        bool active{};
    };
    explicit SquirrelDriver(std::filesystem::path root,int difficulty=0)
        :root_(std::move(root)),difficulty_(difficulty) {}
    void configure_menu_state(const RetailMenuState& menu) noexcept override;
    [[nodiscard]] bool initialize(int player,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;
    [[nodiscard]] const std::array<Choice,4>& choices() const noexcept {
        return choices_;
    }
    [[nodiscard]] int level() const noexcept {return level_;}
    [[nodiscard]] int placed_count() const noexcept {return placed_count_;}
    [[nodiscard]] bool animating() const noexcept {return animating_;}
private:
    [[nodiscard]] bool generate_choices(std::string& error);
    [[nodiscard]] ActivityFrameOutput draw() const;
    std::filesystem::path root_;
    int difficulty_{};
    int player_{-1};
    std::unique_ptr<squirrel::Data> data_{};
    squirrel::RunPlan connectors_{};
    squirrel::PlacedRunPieces placed_piece_ids_{};
    squirrel::RunAssemblyAnimationRuntime assembly_{};
    std::array<Choice,4> choices_{};
    int level_{};
    int placed_count_{};
    int selected_choice_{-1};
    bool animating_{};
    bool complete_{};
    int completion_stage_{};
};
} // namespace btb::full_game
