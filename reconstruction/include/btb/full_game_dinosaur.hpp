#pragma once
#include "btb/full_game_runtime.hpp"
#include "btb/dino_presentation.hpp"
#include "btb/original_coordinators2.hpp"
#include "btb/original_asset_resolver.hpp"
#include "btb/retail_crt_random.hpp"

namespace btb::full_game {

// Live 0x14/0x15 adapter. Runs recovered 00409E00 against the original
// nine dino.txt files and bitmap dimensions; no preview Session or artwork.
class DinosaurDriver final : public ActivityDriver,
                             private original_coordinators2::DinoIO {
public:
    DinosaurDriver(std::filesystem::path root,retail::OriginalRetailRandom& random)
        : assets_(std::move(root)),random_(random) {}
    void configure_menu_state(const RetailMenuState&) noexcept override;
    bool initialize(int player,std::string& error) override;
    ActivityFrameOutput advance(const ActivityFrameInput&) override;
    bool unload(std::string& error) override;
    void request_leave() noexcept { state_.menu_requested=true; }
    const original_coordinators2::DinoState& state() const noexcept { return state_; }
private:
    void enable_input_processing() override {}
    bool any_managed_sound_playing() override { return input_.managed_sound_playing; }
    void stop_all_managed_sounds() override;
    void sound(int id,int priority,int group) override;
    std::uint32_t random32() override;
    bool cursor_in_rectangle(original_coordinators2::Rect) override;
    bool cursor_near_point(int x,int y,int radius) override;
    void set_cursor_surface(std::uint32_t surface) override { carried_surface_=surface; }
    void load_completion_sprite(int species) override;
    void draw_dino_activity() override;
    void update_print_button() override;
    void unload_dino_activity() override;
    void prepare_play_again() override;
    void stop_activity_music() override;
    void play_activity_music(int index) override;
    void mark_activity_intro_sound(int species) override;
    OriginalAssetResolver assets_;
    retail::OriginalRetailRandom& random_;
    original_coordinators2::DinoState state_{};
    dino::CharacterAnimations animations_{};
    dino::PrintButtonState print_button_{};
    ActivityFrameInput input_{};
    ActivityFrameOutput* output_{};
    std::vector<std::string> pieces_{};
    std::string background_{},certificate_{};
    int species_{},difficulty_{};
    std::uint32_t carried_surface_{};
    bool initialized_{},progress_emitted_{};
};
} // namespace btb::full_game
