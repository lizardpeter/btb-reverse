#pragma once
#include "btb/full_game_runtime.hpp"
#include "btb/retail_crt_random.hpp"
#include "btb/retail_activity_closure.hpp"
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
        // Raw first comparison word written by 00426F30; decoys store
        // their encoded pair, not the decoded first connector.
        int feedback_comparison{};
    };
    explicit SquirrelDriver(std::filesystem::path root,int difficulty=0,
                            retail::OriginalRetailRandom* shared_random=nullptr)
        :root_(std::move(root)),difficulty_(difficulty),shared_random_(shared_random) {}
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
    [[nodiscard]] int conveyor_phase() const noexcept {return conveyor_.phase;}
    [[nodiscard]] int conveyor_offset() const noexcept {return conveyor_.position;}
    [[nodiscard]] const squirrel::PlacementMotion& placement() const noexcept {return placement_;}
    [[nodiscard]] bool complete() const noexcept {return complete_;}
    [[nodiscard]] int run_section() const noexcept {return assembly_.run_section;}
    [[nodiscard]] squirrel::Vec2i squirrel_displacement() const noexcept {return {assembly_.x,assembly_.y};}
    bool shared_back_available() const noexcept override {
        return placement_.state==squirrel::PlacementState::IdleSelect;
    }
private:
    [[nodiscard]] bool generate_choices(std::string& error);
    [[nodiscard]] bool start_run_section(std::string& error);
    [[nodiscard]] ActivityFrameOutput draw() const;
    [[nodiscard]] ActivityFrameOutput advance_gameplay(const ActivityFrameInput&);
    std::filesystem::path root_;
    int difficulty_{};
    retail::OriginalRetailRandom standalone_random_{};
    retail::OriginalRetailRandom* shared_random_{};
    [[nodiscard]] retail::OriginalRetailRandom& random_stream() noexcept {
        return shared_random_?*shared_random_:standalone_random_;
    }
    int player_{-1};
    std::unique_ptr<squirrel::Data> data_{};
    squirrel::RunPlan connectors_{};
    squirrel::PlacedRunPieces placed_piece_ids_{};
    squirrel::RunAssemblyAnimationRuntime assembly_{};
    std::array<Choice,4> choices_{};
    retail_activity::SquirrelConveyor conveyor_{};
    int level_{};
    int placed_count_{};
    squirrel::PlacementMotion placement_{};
    int placement_slot_{};
    int temporary_decoy_{-1};
    int carried_piece_{-1};
    bool animating_{};
    bool complete_{};
    int completion_stage_{};
    int intro_stage_{};
};
} // namespace btb::full_game
