#pragma once
#include "btb/full_game_resources.hpp"
#include "btb/packed_frame_snapshot.hpp"
#include <memory>
#include <variant>
namespace btb::full_game {
// Bitmap draw commands and genuine already-decoded RGB surfaces share the
// same painter-order stream. A decoded movie surface is opaque at this
// boundary: the native decoder has already blended its alpha into it.
// Captured underlays likewise contain complete source pixels.
struct PackedCompositionLayer {
    std::shared_ptr<const display::PackedFrameSnapshot> pixels;
    int x{},y{};
};
using FrameCompositionLayer=std::variant<ResolvedDraw,PackedCompositionLayer>;
}
