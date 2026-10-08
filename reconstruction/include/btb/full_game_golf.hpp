#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/golf_activity.hpp"

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace btb::full_game {

// Wires retail Golf states 0x36 / 0x37 to the common 68-state game root.
// The file path points to the installed Data/SubGameGolf/golfdata.txt,
// not a recreated level. The native input/sprite host owns the adapter
// that sends aim, shoot, swing-finished, and voice-finished commands.
class GolfDriver final : public ActivityDriver {
public:
    GolfDriver(std::filesystem::path golf_data_dir, int difficulty)
        : golf_data_dir_(std::move(golf_data_dir)),
          difficulty_(difficulty) {}

    void configure_menu_state(const RetailMenuState& menu) noexcept override {
        // The 0x3F replay chooser sets original global 0x51C284 to 0/1/2.
        // Initial Golf difficulty is selected by an earlier pregame state,
        // not by the Adventure chooser's separate subgame selector.
        if (menu.variant_selection_origin ==
                game_flow::State::PlayAgainDifficultyUpdate &&
            menu.source_variant >= 0 && menu.source_variant <= 2) {
            difficulty_ = menu.source_variant;
        }
    }

    [[nodiscard]] bool initialize(
        int player_index, std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    void queue_control(golf::RoundInput input) noexcept {
        queued_input_ = input;
    }
    [[nodiscard]] const golf::Round* round() const noexcept {
        return round_.get();
    }

private:
    [[nodiscard]] ActivityFrameOutput compose_frame(
        const golf::RoundFrame& state) const;

    std::filesystem::path golf_data_dir_{};
    int difficulty_{};
    int player_index_{-1};
    std::unique_ptr<golf::Round> round_{};
    std::optional<golf::RoundInput> queued_input_{};
};

} // namespace btb::full_game
