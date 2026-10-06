#include "btb/dino_runtime.hpp"

#include <cassert>
#include <sstream>

using namespace btb::dino;

static LevelData sample_level() {
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

int main() {
    Runtime runtime(sample_level());

    // Slot 0 carries piece ID 1, so its target is target_positions[1].
    assert((runtime.pieces()[0].target == Vec2i{200, 200}));
    assert((runtime.pieces()[0].current == Vec2i{10, 10}));
    assert(runtime.pieces()[0].piece_id == 1);

    runtime.set_piece_dimensions(0, 50, 40);
    runtime.set_piece_dimensions(1, 20, 20);

    const auto selected = runtime.begin_drag({20, 20});
    assert(selected && *selected == 0);
    assert(runtime.mode() == InteractionMode::Carrying);
    assert(runtime.pieces()[0].state == PieceState::Dragging);
    assert(runtime.pieces()[0].render_mode == 0);

    assert(runtime.try_drop({100, 100}, 20) == DropResult::Rejected);
    assert(runtime.mode() == InteractionMode::Carrying);
    assert(runtime.pieces()[0].state == PieceState::RejectedDrop);

    assert(runtime.try_drop({210, 190}, 20) == DropResult::Accepted);
    assert(runtime.mode() == InteractionMode::FinalizeAcceptedDrop);
    assert(runtime.pieces()[0].state == PieceState::AcceptedDrop);

    assert(runtime.finalize_accepted_drop());
    assert(runtime.mode() == InteractionMode::Idle);
    assert(runtime.pieces()[0].state == PieceState::Placed);
    assert(runtime.pieces()[0].render_mode == 1);
    assert(runtime.completed_piece_count() == 1);
    assert(!runtime.complete());

    // Placed pieces are not selectable again.
    assert(!runtime.begin_drag({20, 20}));

    assert(Runtime::snap_tolerance(2) == 9);
    assert(Runtime::snap_tolerance(0) == 20);
    assert(Runtime::snap_tolerance(8) == 20);

    static_assert(kInitialSpecialRenderOffsetIndex == 4);
    static_assert(kSpecialRenderOffsets[0] == Vec2i{0, -40});
    static_assert(kSpecialRenderOffsets[4] == Vec2i{0, 40});
    static_assert(kSpecialRenderOffsets[6] == Vec2i{-20, 0});

    constexpr auto special0 =
        special_render_position({456, 248}, 0);
    static_assert(special0.has_value());
    static_assert(*special0 == Vec2i{456, 208});

    constexpr auto special4 =
        special_render_position({456, 248}, 4);
    static_assert(special4.has_value());
    static_assert(*special4 == Vec2i{456, 288});

    static_assert(!special_render_position({80, 300}, 8).has_value());
}
