#include "btb/dino_presentation.hpp"

namespace btb::dino {

FrameComposition compose_dino_frame(
    const Runtime& runtime,
    CharacterAnimations& animations,
    Species species,
    Vec2i special_anchor,
    std::int32_t special_offset_index) {

    FrameComposition result;
    result.commands.reserve(runtime.pieces().size() * 2 + 3);

    result.commands.push_back({
        DrawLayer::Background,
        0,
        0,
        false,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
    });

    animations.advance();

    for (const auto character : {Character::Bob, Character::Ellis}) {
        const auto& channel = animations.channel(character);
        const auto position = character_position(character, species);
        result.commands.push_back({
            character == Character::Bob
                ? DrawLayer::Bob
                : DrawLayer::Ellis,
            position.x,
            position.y,
            true,
            character,
            std::nullopt,
            std::nullopt,
            character_source_rect(character, channel.frame()),
        });
    }

    const auto& pieces = runtime.pieces();

    for (std::size_t i = 0; i < pieces.size(); ++i) {
        const auto& piece = pieces[i];
        if (piece.state != PieceState::Placed) {
            continue;
        }

        result.commands.push_back({
            DrawLayer::PlacedPiece,
            piece.target.x,
            piece.target.y,
            true,
            std::nullopt,
            i,
            piece.piece_id,
            piece.source_rect,
        });
    }

    for (std::size_t i = 0; i < pieces.size(); ++i) {
        const auto& piece = pieces[i];

        if (piece.render_mode == 2) {
            const auto special =
                special_render_position(
                    special_anchor,
                    special_offset_index);
            if (special) {
                result.commands.push_back({
                    DrawLayer::SpecialPiece,
                    special->x,
                    special->y,
                    true,
                    std::nullopt,
                    i,
                    piece.piece_id,
                    piece.source_rect,
                });
            }
            continue;
        }

        const auto state = static_cast<std::int32_t>(piece.state);
        if (state < static_cast<std::int32_t>(PieceState::Loose) ||
            state > static_cast<std::int32_t>(PieceState::RejectedDrop) ||
            piece.render_mode == 0) {
            continue;
        }

        result.commands.push_back({
            DrawLayer::LoosePiece,
            piece.current.x,
            piece.current.y,
            true,
            std::nullopt,
            i,
            piece.piece_id,
            piece.source_rect,
        });
    }

    return result;
}

PrintButtonStep update_print_button(
    PrintButtonState& state,
    const PrintButtonInput& input) noexcept {

    PrintButtonStep result;

    result.inside =
        input.mouse_x > kPrintButtonHitRect.left &&
        input.mouse_x < kPrintButtonHitRect.right &&
        input.mouse_y > kPrintButtonHitRect.top &&
        input.mouse_y < kPrintButtonHitRect.bottom;

    if (!result.inside) {
        state.hover_voice_latched = false;
        return result;
    }

    if (input.click_active) {
        result.draw_print_bar_before_print = true;
        result.stop_all_managed_sounds = true;
        result.invoke_shared_print = true;
        return result;
    }

    result.overlay_x = kPrintButtonOverlayPosition.x;
    result.overlay_y = kPrintButtonOverlayPosition.y;
    result.visual = input.pressed_visual
        ? PrintVisual::Pressed
        : PrintVisual::Hover;

    if (!state.hover_voice_latched) {
        result.hover_sound_id = kPrintHoverSoundId;
        result.hover_sound_priority = 50;
        result.hover_arbitration_class = 2;
        state.hover_voice_latched = true;
    }

    return result;
}

} // namespace btb::dino
