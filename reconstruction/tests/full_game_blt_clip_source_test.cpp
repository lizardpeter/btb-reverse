#include "btb/full_game_blt_clip.hpp"

#include <cassert>

using namespace btb::full_game;

constexpr auto original=retail_clip_blt({4,8,36,40},100,70);
static_assert(original.visible);
static_assert(original.source.left==4);
static_assert(original.source.top==8);
static_assert(original.destination.left==100);
static_assert(original.destination.right==132);

constexpr auto left=retail_clip_blt({5,10,45,30},-13,18);
static_assert(left.visible);
static_assert(left.source.left==18);
static_assert(left.source.right==45);
static_assert(left.destination.left==0);
static_assert(left.destination.right==27);

constexpr auto right=retail_clip_blt({20,10,60,30},630,20);
static_assert(right.visible);
static_assert(right.source.left==20);
static_assert(right.source.right==30);
static_assert(right.destination.right==640);

constexpr auto top=retail_clip_blt({30,40,62,60},50,-8);
static_assert(top.visible);
static_assert(top.source.top==48);
static_assert(top.destination.top==0);
static_assert(top.destination.bottom==12);

constexpr auto bottom=retail_clip_blt({0,0,40,40},12,465);
static_assert(bottom.visible);
static_assert(bottom.source.bottom==15);
static_assert(bottom.destination.bottom==480);

constexpr auto outside=retail_clip_blt({0,0,40,40},650,80);
static_assert(!outside.visible);
constexpr auto malformed=retail_clip_blt({50,0,20,40},0,0);
static_assert(!malformed.visible);

int main() {
    const auto corner=retail_clip_blt({100,200,140,240},-5,475);
    assert(corner.visible);
    assert(corner.source.left==105);
    assert(corner.source.top==200);
    assert(corner.source.right==140);
    assert(corner.source.bottom==205);
    assert(corner.destination.left==0);
    assert(corner.destination.top==475);
    assert(corner.destination.right==35);
    assert(corner.destination.bottom==480);

    assert(!retail_clip_blt({0,0,30,30},-31,0).visible);
    assert(!retail_clip_blt({0,0,30,30},640,0).visible);
    assert(!retail_clip_blt({0,0,30,30},0,480).visible);
}
