#pragma once

#include "btb/bobs_band_activity.hpp"
#include "btb/bobs_band_persistence.hpp"
#include "btb/full_game_runtime.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace btb::full_game {

// Adapts the recovered 0x0041DB20/0x00420030/0x0041D920 Band lifecycle
// to the original 68-state root. Retains game logic in bobs_band::Activity,
// rather than reimplementing it in a platform-specific window/controller.
//
// Directory contracts:
//   original_data_dir = installed Data/SubGameOpen containing machinedata.txt;
//   save_dir = writable native musicbob*/musicwendy*/musicfarmer* namespace.
// No resources are embedded; the host owns DirectDraw/DirectSound/Bink bridges.
class BobsBandDriver final : public ActivityDriver {
public:
    BobsBandDriver(
        std::filesystem::path original_data_dir,
        std::filesystem::path save_dir,
        bobs_band::Conductor conductor = bobs_band::Conductor::Bob)
        : original_data_dir_(std::move(original_data_dir)),
          save_dir_(std::move(save_dir)),
          conductor_(conductor) {}

    void configure_menu_state(const RetailMenuState& menu) noexcept override {
        if (menu.subgame_selection_origin ==
                game_flow::State::MusicChooserUpdate &&
            menu.selected_subgame >= 0 &&
            menu.selected_subgame < bobs_band::kConductorCount) {
            conductor_ = static_cast<bobs_band::Conductor>(
                menu.selected_subgame);
        }
    }

    [[nodiscard]] bool initialize(
        int player_index, std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    [[nodiscard]] const bobs_band::Activity* activity() const noexcept {
        return activity_.get();
    }

private:
    [[nodiscard]] Draw convert_draw(
        const bobs_band::DrawCommand& command) const;
    [[nodiscard]] static Audio convert_audio(
        const bobs_band::AudioRequest& request);

    std::filesystem::path original_data_dir_{};
    std::filesystem::path save_dir_{};
    bobs_band::Conductor conductor_{bobs_band::Conductor::Bob};
    std::unique_ptr<bobs_band::Activity> activity_{};
    int player_index_{-1};
};

} // namespace btb::full_game
