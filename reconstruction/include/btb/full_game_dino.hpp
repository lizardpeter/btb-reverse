#pragma once

#include "btb/dino_presentation.hpp"
#include "btb/full_game_runtime.hpp"
#include "btb/original_asset_resolver.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace btb::full_game {

// Source-backed native Dinosaur activity for original states 0x0E/0x0F.
// This is NOT the old isolated GDI demonstration: it emits original bitmap
// and managed-sound commands into the shared GameRoot/DirectDraw pipeline.
class DinoDriver final : public ActivityDriver {
public:
    explicit DinoDriver(std::filesystem::path retail_root)
        : assets_(std::move(retail_root)) {}

    void configure_menu_state(const RetailMenuState& menu) noexcept override;
    [[nodiscard]] bool initialize(int player_index,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    [[nodiscard]] bool initialized() const noexcept { return !!runtime_; }
    [[nodiscard]] const dino::Runtime* runtime() const noexcept {
        return runtime_.get();
    }
    [[nodiscard]] dino::Species species() const noexcept { return species_; }
    [[nodiscard]] dino::Difficulty difficulty() const noexcept {
        return difficulty_;
    }

private:
    OriginalAssetResolver assets_;
    std::unique_ptr<dino::Runtime> runtime_{};
    dino::CharacterAnimations animations_{};
    dino::PrintButtonState print_state_{};
    dino::Vec2i special_anchor_{};
    dino::Vec2i pickup_offset_{};
    dino::Species species_{dino::Species::Raptor};
    dino::Difficulty difficulty_{dino::Difficulty::Easy};
    std::string level_folder_{};
    std::string background_{};
    std::vector<std::string> pieces_{};
    int player_index_{-1};
    bool was_pointer_held_{};
    bool startup_pending_{};
    bool completion_started_{};
};

} // namespace btb::full_game
