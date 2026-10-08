#pragma once

#include "btb/dino_presentation.hpp"
#include "btb/game_flow.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

namespace btb::host {

// This is the first integrated host for the source reconstruction, NOT the
// retail WinMain/DirectDraw/DirectSound implementation.
enum class Screen {
    ActivitySelect,
    Dinosaur,
    DinosaurComplete,
};

class Session {
public:
    [[nodiscard]] Screen screen() const noexcept { return screen_; }
    [[nodiscard]] std::int32_t retail_state() const noexcept { return retail_state_; }
    [[nodiscard]] game_flow::DispatcherStep dispatcher() const noexcept;
    [[nodiscard]] const std::string& error() const noexcept { return error_; }
    [[nodiscard]] std::int32_t last_sound_id() const noexcept { return last_sound_id_; }
    [[nodiscard]] const std::filesystem::path& level_path() const noexcept { return level_path_; }
    [[nodiscard]] dino::Species species() const noexcept { return species_; }
    [[nodiscard]] dino::Difficulty difficulty() const noexcept { return difficulty_; }

    [[nodiscard]] bool load_dinosaur(
        const std::filesystem::path& dino_txt,
        dino::Species species,
        dino::Difficulty difficulty);

    // Must be populated from source BMP dimensions before clicking a piece.
    void set_piece_dimensions(std::size_t slot, int width, int height);

    void pointer_down(dino::Vec2i cursor);
    void pointer_up(dino::Vec2i cursor);
    void tick();
    void back_to_activity_select() noexcept;

    [[nodiscard]] const dino::Runtime* dinosaur() const noexcept {
        return dinosaur_.get();
    }
    [[nodiscard]] const dino::FrameComposition& dino_frame() const noexcept {
        return dino_frame_;
    }
    [[nodiscard]] std::optional<std::size_t> carried_piece() const noexcept {
        return dinosaur_ ? dinosaur_->selected_piece() : std::nullopt;
    }
    [[nodiscard]] dino::Vec2i carried_piece_top_left(dino::Vec2i cursor) const noexcept {
        return {cursor.x + pickup_offset_.x, cursor.y + pickup_offset_.y};
    }

private:
    Screen screen_{Screen::ActivitySelect};
    std::int32_t retail_state_{
        static_cast<std::int32_t>(game_flow::State::ActivitySelectUpdate)};
    std::filesystem::path level_path_{};
    std::string error_{};
    std::unique_ptr<dino::Runtime> dinosaur_{};
    dino::CharacterAnimations animations_{};
    dino::FrameComposition dino_frame_{};
    dino::Vec2i pickup_offset_{};
    dino::Species species_{dino::Species::Raptor};
    dino::Difficulty difficulty_{dino::Difficulty::Easy};
    std::int32_t last_sound_id_{-1};
};

} // namespace btb::host
