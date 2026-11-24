#pragma once

#include "btb/full_game_front_end.hpp"
#include "btb/full_game_pregame.hpp"
#include "btb/full_game_ui_bitmaps.hpp"

#include <cstddef>
#include <optional>
#include <utility>
#include <vector>

namespace btb::full_game {

// Geometry shared by the original twelve generic UI screens. Unlike the
// Activity-Select rectangle-specialized helper, this handles arbitrary
// polygons in uiHotArea.txt and excludes click-on-edge ambiguity.
[[nodiscard]] bool point_in_original_polygon(
    const front_end::HotArea& area, int x, int y) noexcept;
[[nodiscard]] std::optional<std::size_t> generic_ui_hit(
    const front_end::HotAreaScreen& screen, int x, int y) noexcept;

// Recovered hover / deferred-sound / click / route policy for ANY of the
// original table-backed UI screen indices 0..11. The original state-machine
// owner must select the index; this class does not infer e.g. which
// instruction-screen variant to use for an unknown pregame route.
class GenericUiScreenDriver final : public FrontEndDriver {
public:
    GenericUiScreenDriver(
        GenericUiCatalog catalog,
        front_end::Screen screen,
        std::vector<int> initial_random_draws)
        : catalog_(std::move(catalog)),
          screen_(screen),
          initial_random_draws_(std::move(initial_random_draws)) {}

    void configure_menu_state(const RetailMenuState& menu) override {
        if(screen_==front_end::Screen::InstructionWithDifficulty)
            difficulty_=menu.source_variant;
        if (!pregame_setup_ || !pregame_bitmaps_) {
            return;
        }
        const auto slot = retail_pregame_backdrop_index(
            *pregame_setup_,menu.selected_subgame);
        backdrop_ = slot ? pregame_bitmaps_->backdrop(*slot)
                         : std::nullopt;
    }

    // The source table is retained so re-entering a Dino/Spud/Adventure
    // instruction screen after another subgame selection uses the NEW
    // BMP rather than the original driver's first backdrop.
    [[nodiscard]] bool bind_retail_pregame_backdrops(
        OriginalUiBitmapCatalog bitmaps, game_flow::State setup) {
        const auto* pair = retail_pregame_for_state(setup);
        if (!pair || pair->setup != setup || pair->screen != screen_) {
            return false;
        }
        pregame_setup_ = setup;
        pregame_bitmaps_ = std::move(bitmaps);
        return true;
    }

    [[nodiscard]] bool initialize(
        const progress::Record& current_profile,
        progress::FinaleGate& finale_gate,
        std::string& error) override;

    void synchronize_finale_gate(progress::FinaleGate gate) noexcept override;

    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;

    [[nodiscard]] front_end::Screen screen() const noexcept {
        return screen_;
    }
    [[nodiscard]] const front_end::GenericUiRuntimeState& runtime() const
        noexcept { return ui_state_; }
    [[nodiscard]] const std::vector<front_end::ReplacementRuntimeState>&
    records() const noexcept { return replacements_; }

    // The outer state owns the uiBitmapName.txt index. Generic screen index
    // is NOT a valid replacement for that separate 23-entry asset table.
    [[nodiscard]] bool configure_backdrop(
        const OriginalUiBitmapCatalog& bitmaps,
        std::size_t original_source_slot) {
        backdrop_ = bitmaps.backdrop(original_source_slot);
        return backdrop_.has_value();
    }

    // PE32 0042A2C0 states 3c/3e/42 use literal color-keyed art,
    // NOT uiBitmapName.txt slots 19/20/21 (obsolete retail filenames).
    [[nodiscard]] bool configure_retail_replay_backdrop(
        game_flow::State setup) {
        using game_flow::State;
        const char* path=nullptr;
        switch(setup) {
        case State::PlayAgainYesNoSetup:
            path="Data/ui/playagain/playagainnodiff.bmp";break;
        case State::PlayAgainDifficultySetup:
            path="Data/ui/playagain/playagain3.bmp";break;
        case State::FireworksReplayChoiceSetup:
            path="Data/ui/editplay/Fireworkplayagain.bmp";break;
        default:return false;
        }
        const auto* pair=generic_front_end_pair_for_state(setup);
        if(!pair || pair->screen!=screen_)return false;
        backdrop_=Draw{path,0,0,std::nullopt,true};
        return true;
    }

private:
    GenericUiCatalog catalog_{};
    front_end::Screen screen_{};
    std::vector<int> initial_random_draws_{};
    std::vector<front_end::ReplacementRuntimeState> replacements_{};
    front_end::GenericUiRuntimeState ui_state_{};
    bool initialized_{};
    int secondary_delay_{};
    int difficulty_{};
    std::optional<std::size_t> captured_area_{};
    std::optional<Draw> backdrop_{};
    std::optional<game_flow::State> pregame_setup_{};
    std::optional<OriginalUiBitmapCatalog> pregame_bitmaps_{};
};

} // namespace btb::full_game
