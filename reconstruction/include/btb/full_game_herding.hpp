#pragma once

#include "btb/full_game_herding_presentation.hpp"
#include "btb/herding_data.hpp"

#include <filesystem>
#include <memory>
#include <string>

namespace btb::full_game {

// The original entity steering/collisions/route-order decisions must be
// recovered from 0x4182B0 / 0x416B70, not synthesized by the renderer.
// An explicit provider can now be installed by the complete AI source
// recovery. It owns all 0x64-byte records, follower lists and game AI;
// the common compositor owns only original DrawHerdingActivity semantics.
class HerdingSimulationProvider {
public:
    virtual ~HerdingSimulationProvider() = default;
    [[nodiscard]] virtual bool initialize(
        const herding::Data& original_data,
        int difficulty,
        std::string& error) = 0;
    [[nodiscard]] virtual bool advance(
        const ActivityFrameInput& input,
        HerdingScene& out,
        int& undelivered_animals,
        std::string& error) = 0;
    [[nodiscard]] virtual bool unload(std::string& error) = 0;
};

class HerdingDriver final : public ActivityDriver {
public:
    HerdingDriver(
        std::filesystem::path original_subgame1_directory,
        std::unique_ptr<HerdingSimulationProvider> simulation,
        int default_difficulty = 0)
        : directory_(std::move(original_subgame1_directory)),
          simulation_(std::move(simulation)),
          difficulty_(default_difficulty) {}

    void configure_menu_state(
        const RetailMenuState& menu) noexcept override {
        if (menu.variant_selection_origin ==
                game_flow::State::HerdingPregameUpdate &&
            menu.source_variant >= 0 && menu.source_variant <= 2) {
            difficulty_ = menu.source_variant;
        }
    }

    [[nodiscard]] bool initialize(
        int player_index,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    [[nodiscard]] bool initialized() const noexcept { return initialized_; }
    [[nodiscard]] int difficulty() const noexcept { return difficulty_; }
    [[nodiscard]] int completion_stage() const noexcept {
        return completion_stage_;
    }
    [[nodiscard]] const HerdingScene& scene() const noexcept {
        return scene_;
    }

private:
    std::filesystem::path directory_{};
    std::unique_ptr<HerdingSimulationProvider> simulation_{};
    herding::Data data_{};
    HerdingScene scene_{};
    int difficulty_{};
    int player_index_{-1};
    int completion_stage_{};
    bool initialized_{};
    bool startup_audio_emitted_{};
};

} // namespace btb::full_game
