#pragma once

#include "btb/dino_presentation.hpp"
#include "btb/full_game_runtime.hpp"
#include "btb/original_asset_resolver.hpp"
#include "btb/retail_crt_random.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace btb::full_game {

// Source-backed native Dinosaur activity for original states 0x14/0x15.
// This is NOT the old isolated GDI demonstration: it emits original bitmap
// and managed-sound commands into the shared GameRoot/DirectDraw pipeline.
class DinoDriver final : public ActivityDriver {
public:
    explicit DinoDriver(std::filesystem::path retail_root,
                        retail::OriginalRetailRandom* shared_random=nullptr)
        : assets_(std::move(retail_root)),shared_random_(shared_random) {}

    void configure_menu_state(const RetailMenuState& menu) noexcept override;
    [[nodiscard]] bool initialize(int player_index,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    bool shared_back_available() const noexcept override {
        return runtime_&&!runtime_->selected_piece();
    }
    bool shared_back_starts_replay() const noexcept override {return completion_started_;}

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
    retail::OriginalRetailRandom* shared_random_{};
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
    bool startup_pending_{};
    bool completion_started_{};
};

} // namespace btb::full_game
