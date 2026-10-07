#include "btb/maze_presentation.hpp"

#include <cassert>

using namespace btb::maze;

int main() {
    static_assert(kSurfaceBindings.size() == 10);

    constexpr auto* west = surface_binding(SurfaceKind::WestBackground);
    static_assert(west != nullptr);
    static_assert(west->global_address == 0x00512100);
    static_assert(west->filename == "Data\\SubGameMaze\\LEFT_MAZE.bmp");
    static_assert(west->width == 640 && west->height == 480);
    static_assert(!west->color_keyed);

    constexpr auto* player = surface_binding(SurfaceKind::Player);
    static_assert(player != nullptr);
    static_assert(player->global_address == 0x0051210C);
    static_assert(player->width == 1024 && player->height == 512);
    static_assert(player->color_keyed);
    static_assert(player->color_key == 0x00FF00FF);

    constexpr auto* spud = surface_binding(SurfaceKind::Spud);
    static_assert(spud != nullptr);
    static_assert(spud->width == 1792 && spud->height == 512);

    constexpr auto* numerals = surface_binding(SurfaceKind::Numerals);
    static_assert(numerals != nullptr);
    static_assert(numerals->color_keyed);
    static_assert(numerals->color_key == 0x00000000);

    // Retail's Maze timer intentionally names the Spud Maze copy.
    constexpr auto* timer = surface_binding(SurfaceKind::Timer);
    static_assert(timer != nullptr);
    static_assert(timer->global_address == 0x005144D0);
    static_assert(
        timer->filename == "Data\\SubGameSpudMaze\\timer.bmp");
    static_assert(timer->width == 103 && timer->height == 25);

    static_assert(background_surface(Screen::West) ==
                  SurfaceKind::WestBackground);
    static_assert(background_surface(Screen::Middle) ==
                  SurfaceKind::MiddleBackground);
    static_assert(background_surface(Screen::East) ==
                  SurfaceKind::EastBackground);

    static_assert(retail_actor_column(0) == 0);
    static_assert(retail_actor_column(3) == 3);
    static_assert(retail_actor_column(4) == 3);

    constexpr auto actor = actor_source_rect(4, 2);
    static_assert(actor.left == 384);
    static_assert(actor.top == 256);
    static_assert(actor.right == 512);
    static_assert(actor.bottom == 384);

    assert(player_destination(247.9f, 267.9f) == (Pointi{182,203}));
    assert(spud_destination(200.9f, 100.9f, -20) == (Pointi{116,36}));

    assert(timer_fill_width(240,240) == 102);
    assert(timer_fill_width(120,240) == 51);
    assert(timer_fill_width(15,120) == 12);

    const auto half = timer_presentation(120,240,4,2);
    assert(half.timer_source == (Recti{0,0,51,25}));
    assert(half.timer_destination == (Pointi{422,430}));
    assert(half.package_markers.size() == 4);
    assert(half.package_markers[0].surface == SurfaceKind::SmallBox);
    assert(half.package_markers[1].surface == SurfaceKind::SmallBox);
    assert(half.package_markers[2].surface == SurfaceKind::BoxGhost);
    assert(half.package_markers[3].surface == SurfaceKind::BoxGhost);
    assert(half.package_markers[0].destination == (Pointi{204,430}));
    assert(half.package_markers[3].destination == (Pointi{264,430}));

    const auto hard = package_progress_markers(12,3);
    assert(hard.size() == 12);
    assert(hard.front().destination == (Pointi{124,430}));
    assert(hard.back().destination == (Pointi{344,430}));

    // Preserve the dormant two-row branch too.
    const auto extended = package_progress_markers(14,0);
    assert(extended.size() == 14);
    assert(extended[12].destination.y == 430);
    assert(extended[13].destination.y == 450);
}
