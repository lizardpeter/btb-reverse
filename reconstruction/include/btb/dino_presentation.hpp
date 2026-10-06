#pragma once

#include "btb/dino_character_animation.hpp"
#include "btb/dino_runtime.hpp"
#include "btb/player_progress.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace btb::dino {

struct CharacterPosition {
    std::int32_t x{};
    std::int32_t y{};
};

// Exact destination tables built on the stack by 0x00409990.
inline constexpr std::array<CharacterPosition,3> kBobPositionsBySpecies{{
    {268,20}, // Raptor
    {312,20}, // Triceratops
    {308,20}, // Tyrannosaurus
}};

inline constexpr std::array<CharacterPosition,3> kEllisPositionsBySpecies{{
    {366,20}, // Raptor
    {414,21}, // Triceratops
    {394,20}, // Tyrannosaurus
}};

[[nodiscard]] constexpr CharacterPosition character_position(
    Character character,
    Species species) noexcept {

    const auto index = static_cast<std::size_t>(species);
    return character == Character::Bob
        ? kBobPositionsBySpecies[index]
        : kEllisPositionsBySpecies[index];
}

[[nodiscard]] constexpr Recti character_source_rect(
    Character character,
    std::int32_t frame) noexcept {

    const auto sheet =
        character == Character::Bob ? kBobSheet : kEllisSheet;

    const auto left = frame * sheet.frame_width;
    return {
        left,
        0,
        left + sheet.frame_width,
        sheet.frame_height,
    };
}

enum class DrawLayer {
    Background,
    Bob,
    Ellis,
    PlacedPiece,
    LoosePiece,
    SpecialPiece,
};

struct DrawCommand {
    DrawLayer layer{DrawLayer::Background};
    std::int32_t x{};
    std::int32_t y{};
    bool color_keyed{};
    std::optional<Character> character{};
    std::optional<std::size_t> piece_index{};
    std::optional<std::int32_t> piece_id{};
    std::optional<Recti> source_rect{};
};

struct FrameComposition {
    std::vector<DrawCommand> commands{};
};

// Exact 0x00409990 ordering:
//   1. opaque level background
//   2. advance both shared character animation channels
//   3. Bob
//   4. Ellis
//   5. all state-4 pieces at permanent targets
//   6. second pass over every piece:
//      - render_mode 2 at the special anchor/offset
//      - otherwise states 0..3 at current position when render_mode != 0
[[nodiscard]] FrameComposition compose_dino_frame(
    const Runtime& runtime,
    CharacterAnimations& animations,
    Species species,
    Vec2i special_anchor,
    std::int32_t special_offset_index);

[[nodiscard]] constexpr Species species_from_level_index(
    std::int32_t encoded_level_index) noexcept {
    return static_cast<Species>(encoded_level_index / 3);
}

[[nodiscard]] constexpr Difficulty difficulty_from_level_index(
    std::int32_t encoded_level_index) noexcept {
    return static_cast<Difficulty>(encoded_level_index % 3);
}

[[nodiscard]] constexpr progress::Slot progress_slot_for_species(
    Species species) noexcept {
    return progress::dino_species_slots()[
        static_cast<std::size_t>(species)];
}

inline constexpr std::array<std::string_view,3>
kCompletionArtworkBySpecies{{
    "data\\subgamedino\\vel.bmp",
    "data\\subgamedino\\tri.bmp",
    "data\\subgamedino\\trex.bmp",
}};

[[nodiscard]] constexpr std::string_view completion_artwork_filename(
    Species species) noexcept {
    return kCompletionArtworkBySpecies[
        static_cast<std::size_t>(species)];
}

struct StartupPresentationStep {
    bool stop_previous_activity_group{true};
    bool play_activity_music{true};
    std::int32_t activity_music_index{1}; // dinosaur.wav
    std::int32_t intro_sound_id{};
    std::int32_t intro_sound_priority{90};
    std::int32_t intro_arbitration_class{1};
    bool mark_intro_slot_input_interruptible{true};
};

[[nodiscard]] constexpr StartupPresentationStep startup_presentation(
    Species species) noexcept {
    return {
        true,
        true,
        1,
        intro_sound_id(static_cast<std::int32_t>(species)),
        90,
        1,
        true,
    };
}

struct CompletionBeginStep {
    bool stop_all_managed_sounds{true};
    std::int32_t completion_sound_id{};
    std::int32_t completion_sound_priority{50};
    std::int32_t completion_arbitration_class{1};

    progress::Slot progress_slot{progress::Slot::DinoRaptor};
    bool write_progress_one{};

    bool set_shared_completion_code_1{true};
    std::string_view artwork_filename{};
    bool load_completion_artwork{true};
    bool register_completion_artwork{true};
    std::int32_t completion_phase_after{1};
};

[[nodiscard]] constexpr CompletionBeginStep begin_completion(
    Species species,
    std::int32_t current_progress_value) noexcept {

    return {
        true,
        completion_sound_id(static_cast<std::int32_t>(species)),
        50,
        1,
        progress_slot_for_species(species),
        current_progress_value == 0,
        true,
        completion_artwork_filename(species),
        true,
        true,
        1,
    };
}

struct CompletionGate {
    bool wait_for_managed_feedback{};
    bool begin_completion{};
    bool update_certificate_print_ui{};
    bool dormant_phase10_play_again{};
};

// Exact completion gate at 0x00409E69..0x0040A058.
// Phase 1 is deliberately exempt from the "wait for managed sound" branch so
// the certificate remains interactive while its completion voice is playing.
[[nodiscard]] constexpr CompletionGate completion_gate(
    bool all_pieces_complete,
    bool any_managed_sound_playing,
    std::int32_t completion_phase) noexcept {

    if (!all_pieces_complete) {
        return {};
    }

    if (any_managed_sound_playing && completion_phase != 1) {
        return {true,false,false,false};
    }

    if (completion_phase == 0) {
        return {false,true,true,false};
    }

    if (completion_phase == 10) {
        return {false,false,false,true};
    }

    return {false,false,true,false};
}

inline constexpr std::int32_t kPlayAgainOuterState = 0x3C;
inline constexpr std::int32_t kDinoChooserOuterState = 0x10;
inline constexpr std::int32_t kSharedMovieOuterState = 0x40;

struct DormantPhase10Transition {
    bool prepare_play_again{true};
    std::int32_t saved_frontend_state{0x10};
    std::int32_t outer_state{kPlayAgainOuterState};
    bool set_shared_transition_flag{true};
    std::int32_t next_ui_context{0x14};
    bool unload_dino_resources{true};
};

inline constexpr DormantPhase10Transition kDormantPhase10Transition{};

enum class PrintVisual {
    None,
    Hover,
    Pressed,
};

struct PrintButtonInput {
    std::int32_t mouse_x{};
    std::int32_t mouse_y{};
    bool click_active{};
    bool pressed_visual{};
};

struct PrintButtonState {
    bool hover_voice_latched{};
};

struct PrintButtonStep {
    bool draw_completion_artwork{true};
    bool inside{};
    PrintVisual visual{PrintVisual::None};
    std::int32_t overlay_x{};
    std::int32_t overlay_y{};
    bool draw_print_bar_before_print{};
    bool stop_all_managed_sounds{};
    bool invoke_shared_print{};
    std::optional<std::int32_t> hover_sound_id{};
    std::int32_t hover_sound_priority{};
    std::int32_t hover_arbitration_class{};
};

inline constexpr Recti kPrintButtonHitRect{298,421,347,465};
inline constexpr CharacterPosition kPrintButtonOverlayPosition{292,417};
inline constexpr std::int32_t kPrintHoverSoundId = 143;

[[nodiscard]] PrintButtonStep update_print_button(
    PrintButtonState& state,
    const PrintButtonInput& input) noexcept;

} // namespace btb::dino
