#include "btb/dino_presentation.hpp"

#include <cassert>
#include <sstream>

using namespace btb::dino;

namespace {

LevelData sample_level() {
    std::istringstream in(R"(
2
100 100
200 200
10 10
300 300
80 300
100 120
-1 -1
1 0
)");
    return parse_level(in);
}

} // namespace

int main() {
    static_assert(
        character_position(Character::Bob, Species::Raptor).x == 268);
    static_assert(
        character_position(Character::Bob, Species::Triceratops).x == 312);
    static_assert(
        character_position(Character::Bob, Species::Tyrannosaurus).x == 308);

    static_assert(
        character_position(Character::Ellis, Species::Raptor).x == 366);
    static_assert(
        character_position(Character::Ellis, Species::Triceratops).x == 414);
    static_assert(
        character_position(Character::Ellis, Species::Triceratops).y == 21);
    static_assert(
        character_position(Character::Ellis, Species::Tyrannosaurus).x == 394);

    constexpr auto bob80 =
        character_source_rect(Character::Bob, 80);
    static_assert(bob80.left == 80 * 113);
    static_assert(bob80.top == 0);
    static_assert(bob80.right == 81 * 113);
    static_assert(bob80.bottom == 97);

    constexpr auto ellis80 =
        character_source_rect(Character::Ellis, 80);
    static_assert(ellis80.left == 80 * 93);
    static_assert(ellis80.right == 81 * 93);
    static_assert(ellis80.bottom == 105);

    Runtime runtime(sample_level());
    runtime.set_piece_dimensions(0, 50, 40);
    runtime.set_piece_dimensions(1, 20, 30);

    runtime.pieces()[0].state = PieceState::Placed;
    runtime.pieces()[0].render_mode = 1;
    runtime.pieces()[1].state = PieceState::Loose;
    runtime.pieces()[1].render_mode = 1;

    CharacterAnimations animations;
    auto frame = compose_dino_frame(
        runtime,
        animations,
        Species::Raptor,
        {456,248},
        4);

    assert(frame.commands.size() == 5);

    assert(frame.commands[0].layer == DrawLayer::Background);
    assert(frame.commands[0].x == 0);
    assert(frame.commands[0].y == 0);
    assert(!frame.commands[0].color_keyed);

    assert(frame.commands[1].layer == DrawLayer::Bob);
    assert(frame.commands[1].character == Character::Bob);
    assert(frame.commands[1].x == 268);
    assert(frame.commands[1].y == 20);
    assert(frame.commands[1].source_rect);
    assert(frame.commands[1].source_rect->left == 80 * 113);
    assert(frame.commands[1].source_rect->right == 81 * 113);

    assert(frame.commands[2].layer == DrawLayer::Ellis);
    assert(frame.commands[2].character == Character::Ellis);
    assert(frame.commands[2].x == 366);
    assert(frame.commands[2].y == 20);
    assert(frame.commands[2].source_rect);
    assert(frame.commands[2].source_rect->left == 80 * 93);

    assert(frame.commands[3].layer == DrawLayer::PlacedPiece);
    assert(frame.commands[3].piece_index == 0);
    assert(frame.commands[3].piece_id == 1);
    assert(frame.commands[3].x == 200);
    assert(frame.commands[3].y == 200);

    assert(frame.commands[4].layer == DrawLayer::LoosePiece);
    assert(frame.commands[4].piece_index == 1);
    assert(frame.commands[4].piece_id == 0);
    assert(frame.commands[4].x == 300);
    assert(frame.commands[4].y == 300);

    // render_mode 0 is hidden because the cursor owns the carried bone.
    runtime.pieces()[1].render_mode = 0;
    frame = compose_dino_frame(
        runtime,
        animations,
        Species::Triceratops,
        {456,248},
        4);
    assert(frame.commands.size() == 4);
    assert(frame.commands[1].x == 312);
    assert(frame.commands[2].x == 414);
    assert(frame.commands[2].y == 21);

    // Dormant render_mode 2 is drawn in the second pass from the special
    // anchor/offset table. If a state-4 piece were given mode 2, retail would
    // draw both its placed copy and this special copy.
    runtime.pieces()[0].render_mode = 2;
    frame = compose_dino_frame(
        runtime,
        animations,
        Species::Tyrannosaurus,
        {456,248},
        4);

    bool saw_special = false;
    bool saw_placed = false;
    for (const auto& command : frame.commands) {
        if (command.layer == DrawLayer::SpecialPiece &&
            command.piece_index == 0) {
            saw_special = true;
            assert(command.x == 456);
            assert(command.y == 288);
        }
        if (command.layer == DrawLayer::PlacedPiece &&
            command.piece_index == 0) {
            saw_placed = true;
        }
    }
    assert(saw_special);
    assert(saw_placed);

    static_assert(
        species_from_level_index(0) == Species::Raptor);
    static_assert(
        species_from_level_index(4) == Species::Triceratops);
    static_assert(
        species_from_level_index(8) == Species::Tyrannosaurus);

    static_assert(
        difficulty_from_level_index(0) == Difficulty::Easy);
    static_assert(
        difficulty_from_level_index(4) == Difficulty::Medium);
    static_assert(
        difficulty_from_level_index(8) == Difficulty::Hard);

    static_assert(
        progress_slot_for_species(Species::Raptor) ==
        btb::progress::Slot::DinoRaptor);
    static_assert(
        progress_slot_for_species(Species::Triceratops) ==
        btb::progress::Slot::DinoTriceratops);
    static_assert(
        progress_slot_for_species(Species::Tyrannosaurus) ==
        btb::progress::Slot::DinoTyrannosaurus);

    static_assert(
        completion_artwork_filename(Species::Raptor) ==
        "data\\subgamedino\\vel.bmp");
    static_assert(
        completion_artwork_filename(Species::Triceratops) ==
        "data\\subgamedino\\tri.bmp");
    static_assert(
        completion_artwork_filename(Species::Tyrannosaurus) ==
        "data\\subgamedino\\trex.bmp");

    constexpr auto startup =
        startup_presentation(Species::Tyrannosaurus);
    static_assert(startup.activity_music_index == 1);
    static_assert(startup.intro_sound_id == 190);
    static_assert(startup.intro_sound_priority == 90);
    static_assert(startup.intro_arbitration_class == 1);
    static_assert(startup.mark_intro_slot_input_interruptible);

    constexpr auto begin_new =
        begin_completion(Species::Triceratops, 0);
    static_assert(begin_new.completion_sound_id == 186);
    static_assert(begin_new.completion_sound_priority == 50);
    static_assert(begin_new.completion_arbitration_class == 1);
    static_assert(
        begin_new.progress_slot ==
        btb::progress::Slot::DinoTriceratops);
    static_assert(begin_new.write_progress_one);
    static_assert(begin_new.set_shared_completion_code_1);
    static_assert(
        begin_new.artwork_filename ==
        "data\\subgamedino\\tri.bmp");
    static_assert(begin_new.completion_phase_after == 1);

    constexpr auto begin_existing =
        begin_completion(Species::Triceratops, 1);
    static_assert(!begin_existing.write_progress_one);

    constexpr auto not_complete =
        completion_gate(false, false, 0);
    static_assert(!not_complete.wait_for_managed_feedback);
    static_assert(!not_complete.begin_completion);

    constexpr auto wait =
        completion_gate(true, true, 0);
    static_assert(wait.wait_for_managed_feedback);
    static_assert(!wait.begin_completion);

    constexpr auto begin =
        completion_gate(true, false, 0);
    static_assert(begin.begin_completion);
    static_assert(begin.update_certificate_print_ui);

    constexpr auto phase1_sound =
        completion_gate(true, true, 1);
    static_assert(!phase1_sound.wait_for_managed_feedback);
    static_assert(phase1_sound.update_certificate_print_ui);

    constexpr auto phase10 =
        completion_gate(true, false, 10);
    static_assert(phase10.dormant_phase10_play_again);
    static_assert(!phase10.update_certificate_print_ui);

    static_assert(kDormantPhase10Transition.saved_frontend_state == 0x10);
    static_assert(kDormantPhase10Transition.outer_state == 0x3C);
    static_assert(kDormantPhase10Transition.next_ui_context == 0x14);
    static_assert(kDormantPhase10Transition.unload_dino_resources);

    static_assert(kPrintButtonHitRect.left == 298);
    static_assert(kPrintButtonHitRect.top == 421);
    static_assert(kPrintButtonHitRect.right == 347);
    static_assert(kPrintButtonHitRect.bottom == 465);
    static_assert(kPrintButtonOverlayPosition.x == 292);
    static_assert(kPrintButtonOverlayPosition.y == 417);
    static_assert(kPrintHoverSoundId == 143);

    PrintButtonState print_state;

    auto print = update_print_button(
        print_state, {320,440,false,false});
    assert(print.draw_completion_artwork);
    assert(print.inside);
    assert(print.visual == PrintVisual::Hover);
    assert(print.overlay_x == 292);
    assert(print.overlay_y == 417);
    assert(print.hover_sound_id && *print.hover_sound_id == 143);
    assert(print.hover_sound_priority == 50);
    assert(print.hover_arbitration_class == 2);
    assert(print_state.hover_voice_latched);

    print = update_print_button(
        print_state, {320,440,false,true});
    assert(print.visual == PrintVisual::Pressed);
    assert(!print.hover_sound_id);

    // Strict bounds: exact edges are outside and reset the one-shot hover latch.
    print = update_print_button(
        print_state, {298,440,false,false});
    assert(!print.inside);
    assert(!print_state.hover_voice_latched);

    print = update_print_button(
        print_state, {320,440,true,false});
    assert(print.inside);
    assert(print.draw_print_bar_before_print);
    assert(print.stop_all_managed_sounds);
    assert(print.invoke_shared_print);
    assert(print.visual == PrintVisual::None);
}
