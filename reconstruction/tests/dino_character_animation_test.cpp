#include "btb/dino_character_animation.hpp"

#include <cassert>

using namespace btb::dino;

int main() {
    CharacterAnimationChannel ch;

    // Retail globals start at frame 0/mode 0; the first draw normalizes the
    // idle animation to its real frame range.
    ch.advance();
    assert(ch.mode() == CharacterAnimationMode::Idle);
    assert(ch.frame() == 80);

    ch.queue(CharacterAnimationMode::PickupA);
    ch.advance();
    assert(ch.mode() == CharacterAnimationMode::PickupA);
    assert(ch.frame() == 0);

    // A frame advances only after seven draw/update calls.
    for (int i = 0; i < 6; ++i) {
        ch.advance();
    }
    assert(ch.frame() == 1);

    // Run through the rest of the 0..19 range; completion returns to Idle.
    for (int i = 0; i < 19 * 7; ++i) {
        ch.advance();
    }
    assert(ch.mode() == CharacterAnimationMode::Idle);
    assert(ch.frame() >= 80 && ch.frame() < 100);

    CharacterAnimations animations;

    animations.queue_pickup(0, 0);
    assert(animations.channel(Character::Bob).has_pending());
    animations.advance();
    assert(animations.channel(Character::Bob).mode() == CharacterAnimationMode::PickupA);

    // Correct feedback IDs 164..166 belong to Ellis; 167..169 to Bob.
    assert(animations.queue_correct_drop(0) == Character::Ellis);
    assert(correct_drop_sound_id(0) == 164);
    assert(animations.queue_correct_drop(5) == Character::Bob);
    assert(correct_drop_sound_id(5) == 169);

    // Wrong-drop sound and animation groups follow the same 3/3 character split.
    CharacterAnimations wrong;
    assert(wrong.queue_wrong_drop(1, 0) == Character::Ellis);
    wrong.advance();
    assert(wrong.channel(Character::Ellis).mode() == CharacterAnimationMode::WrongDropA);
    assert(wrong_drop_sound_id(1) == 171);

    assert(intro_sound_id(0) == 185);
    assert(intro_sound_id(2) == 187);
    assert(completion_sound_id(0) == 188);
    assert(completion_sound_id(2) == 190);

    static_assert(kBobSheet.frame_width == 113);
    static_assert(kBobSheet.frame_height == 97);
    static_assert(kEllisSheet.frame_width == 93);
    static_assert(kEllisSheet.frame_height == 105);
    static_assert(kCharacterFrameRanges[5].end_exclusive == 131);
}
