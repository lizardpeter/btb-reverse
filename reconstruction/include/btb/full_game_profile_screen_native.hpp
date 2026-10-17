#pragma once
#include "btb/full_game_runtime.hpp"
#include "btb/original_asset_resolver.hpp"

#include <array>
#include <filesystem>
#include <optional>
#include <string>

namespace btb::full_game {

// Native DirectDraw/effects bridge for source 0x0042E110.
// Uses original image filenames (including the spaces in the 2002 retail
// files) and exact supplied brown/blue font coordinate tables.
class NativeProfileScreenAdapter final : public NativeProfileScreenDriver {
public:
    NativeProfileScreenAdapter(std::filesystem::path retail_root,
                               std::filesystem::path profile_dir);

    [[nodiscard]] bool initialize(std::string& error);
    [[nodiscard]] bool begin_frame(
        const ActivityFrameInput&,ActivityFrameOutput&,
        std::string& error) override;
    [[nodiscard]] bool end_frame(std::string& error) override;
    [[nodiscard]] std::span<const Rect>
        blue_font_rectangles() const noexcept override {
        return blue_font_;
    }
    [[nodiscard]] std::span<const Rect>
        brown_font_rectangles() const noexcept {
        return brown_font_;
    }
    [[nodiscard]] std::span<const Rect>
        badge_rectangles() const noexcept override {
        return badges_;
    }

    void draw_background() override;
    void update_help_button() override;
    void update_help_hover(bool) override;
    void enable_input() override;
    [[nodiscard]] bool any_managed_playing() override;
    [[nodiscard]] bool sound_playing(int id) override;
    [[nodiscard]] unsigned next_random() override;
    void play_voice(int,int,int) override;
    void stop_managed() override;
    void mark_last_voice_interruptible() override;
    [[nodiscard]] int glyph_width(int) override;
    void draw_glyph(int,int,int) override;
    void draw_badge(int,int,int) override;
    void draw_button(int,bool) override;
    void restore_quit_snapshot() override;
    void truncate_activity_file(const std::string&) override;
    [[nodiscard]] bool backing_present() override;
    void stop_backing() override;
    void release_backing() override;
    void open_delete_overlay() override;
    void use_delete_cursor(bool) override;

private:
    static constexpr std::size_t kRetailFontRows=191;
    [[nodiscard]] bool load_font(
        const char* name,std::array<Rect,kRetailFontRows>& out,
        std::string& error);

    OriginalAssetResolver assets_;
    std::filesystem::path profile_dir_;
    std::array<Rect,kRetailFontRows> brown_font_{};
    std::array<Rect,kRetailFontRows> blue_font_{};
    std::array<Rect,6> badges_{};
    ActivityFrameInput input_{};
    ActivityFrameOutput* output_{};
    std::string frame_error_{};
    bool initialized_{};
    bool backing_present_{};
    bool delete_cursor_{};
};

} // namespace btb::full_game
