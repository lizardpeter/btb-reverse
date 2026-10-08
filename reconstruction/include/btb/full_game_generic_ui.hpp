#pragma once

#include "btb/full_game_front_end.hpp"
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

private:
    GenericUiCatalog catalog_{};
    front_end::Screen screen_{};
    std::vector<int> initial_random_draws_{};
    std::vector<front_end::ReplacementRuntimeState> replacements_{};
    front_end::GenericUiRuntimeState ui_state_{};
    bool initialized_{};
    std::optional<Draw> backdrop_{};
};

} // namespace btb::full_game
