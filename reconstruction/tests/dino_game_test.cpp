#include "btb/dino_game.hpp"

#include <cassert>
#include <sstream>
#include <vector>

using namespace btb::dino;

static LevelData make_level() {
    std::istringstream in(R"(
2
100 100
200 200
10 10
40 40
80 300
100 120
-1 -1
1 0
)");
    return parse_level(in);
}

int main() {
    std::vector<Vec2i> sizes{{20, 20}, {20, 20}};
    GameModel game(make_level(), sizes, 10);

    // Slot 0 contains piece ID 1 and starts at 10,10.
    assert(game.pieces()[0].piece_id == 1);
    assert(game.pieces()[0].target == Vec2i{200, 200});
    assert(game.pieces()[0].start == Vec2i{10, 10});

    assert(game.begin_drag({15, 15}));
    assert(game.mode() == InteractionMode::Dragging);
    assert(game.pieces()[0].state == PieceState::Dragging);

    assert(game.release({150, 150}) == DropResult::Incorrect);
    assert(game.mode() == InteractionMode::ResolveDrop);
    assert(game.pieces()[0].state == PieceState::IncorrectReturnTransition);
    game.resolve_drop();
    assert(game.mode() == InteractionMode::Idle);
    assert(game.pieces()[0].state == PieceState::Loose);
    assert(game.completed_piece_count() == 0);

    assert(game.begin_drag({15, 15}));
    assert(game.release({205, 195}) == DropResult::Correct);
    assert(game.pieces()[0].state == PieceState::CorrectSnapTransition);
    game.resolve_drop();
    assert(game.pieces()[0].state == PieceState::Placed);
    assert(game.completed_piece_count() == 1);

    // Placed pieces are no longer selectable.
    assert(!game.begin_drag({15, 15}));

    // Slot 1 contains piece ID 0 and starts at 40,40.
    assert(game.begin_drag({45, 45}));
    assert(game.release({100, 100}) == DropResult::Correct);
    game.resolve_drop();
    assert(game.complete());

    static_assert(kCorrectDropSoundFirst == 164);
    static_assert(kCorrectDropSoundLast == 169);
    static_assert(kIncorrectDropSoundFirst == 170);
    static_assert(kIncorrectDropSoundLast == 175);
    static_assert(kDinoIntroSoundFirst == 185);
    static_assert(kDinoCompletionSoundLast == 190);
}
