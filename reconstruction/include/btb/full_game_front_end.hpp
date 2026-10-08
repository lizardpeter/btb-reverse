#pragma once

#include "btb/front_end_ui.hpp"
#include "btb/front_end_ui_runtime.hpp"
#include "btb/full_game_runtime.hpp"

#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace btb::full_game {

// The real original UI source consists of three independent 12-screen tables:
// NumUiHotArea.txt, uiHotArea.txt and uiHotAreaReplace.txt. We do not invent
// a hard-coded ten-button main menu; the installed retail records own routes.
struct GenericUiCatalog {
    std::vector<front_end::HotAreaScreen> hot_areas{};
    std::vector<front_end::ReplacementScreen> replacements{};
    std::vector<std::int32_t> counts{};

    [[nodiscard]] static GenericUiCatalog load(
        const std::filesystem::path& loaddata_directory);
    [[nodiscard]] bool validate(std::string& error) const;
};

[[nodiscard]] std::optional<std::size_t> activity_select_hit(
    const front_end::HotAreaScreen& screen,
    int mouse_x, int mouse_y) noexcept;

// The native 0x04/0x05 Activity Select pair, with deferred audio click
// resolution and the progress-gated Firework Finale action 0x22.
// Rendering/hover surface loads are still handled by the common UI compositor;
// this driver returns the real sounds and outer-state changes only.
class ActivitySelectDriver final : public FrontEndDriver {
public:
    explicit ActivitySelectDriver(
        GenericUiCatalog catalog,
        std::vector<int> per_record_initial_random_draws)
        : catalog_(std::move(catalog)),
          initial_random_draws_(
              std::move(per_record_initial_random_draws)) {}

    [[nodiscard]] bool initialize(
        const progress::Record& record,
        progress::FinaleGate& finale_gate,
        std::string& error) override;

    void synchronize_finale_gate(
        progress::FinaleGate gate) noexcept override;

    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;

    [[nodiscard]] const front_end::GenericUiRuntimeState& ui_state()
        const noexcept { return ui_state_; }
    [[nodiscard]] const std::vector<front_end::ReplacementRuntimeState>&
    replacement_state() const noexcept { return replacement_runtime_; }

private:
    [[nodiscard]] const front_end::ReplacementScreen& screen() const {
        return catalog_.replacements[1];
    }

    GenericUiCatalog catalog_{};
    std::vector<int> initial_random_draws_{};
    std::vector<front_end::ReplacementRuntimeState> replacement_runtime_{};
    front_end::GenericUiRuntimeState ui_state_{};
    bool initialized_{};
};

} // namespace btb::full_game
