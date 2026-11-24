#pragma once

#include "btb/maze_data.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace btb::maze {

enum class SurfaceKind {
    WestBackground,
    MiddleBackground,
    EastBackground,
    Player,
    Spud,
    Box,
    BoxGhost,
    SmallBox,
    Numerals,
    Timer,
};

struct SurfaceBinding {
    SurfaceKind kind{};
    std::uint32_t global_address{};
    std::string_view filename{};
    std::int32_t width{};
    std::int32_t height{};
    bool color_keyed{};
    std::uint32_t color_key{};
};

// Exact InitializeMazeActivity (0x0041A730) load order and globals.
//
// Retail contains Data\SubGameMaze\timer.bmp on disk, but Maze does not load
// it. The timer global is deliberately loaded from SubGameSpudMaze\timer.bmp.
// Numerals is also loaded/keyed even though no Maze runtime read of 0x512120
// follows initialization.
inline constexpr std::array<SurfaceBinding,10> kSurfaceBindings{{
    {SurfaceKind::WestBackground,   0x00512100,
     "Data\\SubGameMaze\\LEFT_MAZE.bmp", 640,480,false,0},
    {SurfaceKind::MiddleBackground, 0x00512104,
     "Data\\SubGameMaze\\MID_MAZE.bmp", 640,480,false,0},
    {SurfaceKind::EastBackground,   0x00512108,
     "Data\\SubGameMaze\\RIGHT_MAZE.bmp", 640,480,false,0},
    {SurfaceKind::Player,           0x0051210C,
     "Data\\SubGameMaze\\player.bmp", 1024,512,true,0x00FF00FF},
    {SurfaceKind::Spud,             0x00512110,
     "Data\\SubGameMaze\\spud.bmp", 1792,512,true,0x00FF00FF},
    {SurfaceKind::Box,              0x00512114,
     "Data\\SubGameMaze\\box.bmp", 26,29,true,0x00FF00FF},
    {SurfaceKind::BoxGhost,         0x00512118,
     "Data\\SubGameMaze\\boxghost.bmp", 16,19,true,0x00FF00FF},
    {SurfaceKind::SmallBox,         0x0051211C,
     "Data\\SubGameMaze\\smallbox.bmp", 16,19,true,0x00FF00FF},
    {SurfaceKind::Numerals,         0x00512120,
     "Data\\SubGameMaze\\numerals.bmp", 200,40,true,0x00000000},
    {SurfaceKind::Timer,            0x005144D0,
     "Data\\SubGameSpudMaze\\timer.bmp", 103,25,true,0x00FF00FF},
}};

[[nodiscard]] constexpr const SurfaceBinding*
surface_binding(SurfaceKind kind) noexcept {
    for (const auto& binding : kSurfaceBindings) {
        if (binding.kind == kind) {
            return &binding;
        }
    }
    return nullptr;
}

[[nodiscard]] constexpr SurfaceKind background_surface(
    Screen screen) noexcept {
    switch (screen) {
    case Screen::West:   return SurfaceKind::WestBackground;
    case Screen::Middle: return SurfaceKind::MiddleBackground;
    case Screen::East:   return SurfaceKind::EastBackground;
    }
    return SurfaceKind::MiddleBackground;
}

struct Recti {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};

    friend bool operator==(const Recti&, const Recti&) = default;
};

inline constexpr std::int32_t kActorFrameSize = 128;

// DrawMazeActivity maps source column 4 back to column 3 before forming the
// 128x128 source rectangle. Other columns are retained verbatim.
[[nodiscard]] constexpr std::int32_t retail_actor_column(
    std::int32_t column) noexcept {
    return column == 4 ? 3 : column;
}

[[nodiscard]] constexpr Recti actor_source_rect(
    std::int32_t column,
    std::int32_t row) noexcept {
    column = retail_actor_column(column);
    return {
        column * kActorFrameSize,
        row * kActorFrameSize,
        (column + 1) * kActorFrameSize,
        (row + 1) * kActorFrameSize,
    };
}

struct Pointi {
    std::int32_t x{};
    std::int32_t y{};

    friend bool operator==(const Pointi&, const Pointi&) = default;
};

// 0x0041C110 subtracts 65/64 from the player and 64/64 from Spud.
// 0x004304D0 forces x87 conversion to truncate toward zero.
[[nodiscard]] inline Pointi player_destination(
    float x,
    float y) noexcept {
    return {
        static_cast<std::int32_t>(x - 65.0f),
        static_cast<std::int32_t>(y - 64.0f),
    };
}

[[nodiscard]] inline Pointi spud_destination(
    float x,
    float y,
    std::int32_t horizontal_screen_offset) noexcept {
    return {
        static_cast<std::int32_t>(
            x + static_cast<float>(horizontal_screen_offset) - 64.0f),
        static_cast<std::int32_t>(y - 64.0f),
    };
}

inline constexpr Pointi kTimerDestination{422,430};
inline constexpr std::int32_t kTimerSourceHeight = 25;
inline constexpr float kTimerFillPixels = 102.0f;

[[nodiscard]] inline std::int32_t timer_fill_width(
    std::int32_t remaining_seconds,
    std::int32_t initial_seconds) noexcept {
    return static_cast<std::int32_t>(
        (static_cast<float>(remaining_seconds) /
         static_cast<float>(initial_seconds)) * kTimerFillPixels);
}

[[nodiscard]] inline Recti timer_source_rect(
    std::int32_t remaining_seconds,
    std::int32_t initial_seconds) noexcept {
    return {
        0,
        0,
        timer_fill_width(remaining_seconds, initial_seconds),
        kTimerSourceHeight,
    };
}

inline constexpr std::array<std::int32_t,3> kPackageCountByDifficulty{
    4, 8, 12
};

struct PackageMarker {
    SurfaceKind surface{SurfaceKind::BoxGhost};
    Pointi destination{};
};

// Exact marker loop in DrawMazeTimer (0x0041BF80).
//
// The normal game totals (4/8/12) stay on one row. The >250 wrap branch is
// preserved because it exists in retail even though those totals do not reach
// it.
[[nodiscard]] inline std::vector<PackageMarker> package_progress_markers(
    std::int32_t total,
    std::int32_t completed) {

    std::vector<PackageMarker> result;
    if (total <= 0) {
        return result;
    }

    result.reserve(static_cast<std::size_t>(total));

    const auto start_x = 244 - (total / 2) * 20;
    std::int32_t x_offset = 0;
    std::int32_t y_offset = 0;

    for (std::int32_t index = 0; index < total; ++index) {
        if (x_offset > 250) {
            x_offset = 0;
            y_offset = 20;
        }

        result.push_back({
            index < completed
                ? SurfaceKind::SmallBox
                : SurfaceKind::BoxGhost,
            {start_x + x_offset, 430 + y_offset},
        });

        x_offset += 20;
    }

    return result;
}

struct TimerPresentation {
    Recti timer_source{};
    Pointi timer_destination{kTimerDestination};
    std::vector<PackageMarker> package_markers{};
};

[[nodiscard]] inline TimerPresentation timer_presentation(
    std::int32_t remaining_seconds,
    std::int32_t initial_seconds,
    std::int32_t total_packages,
    std::int32_t completed_packages) {
    return {
        timer_source_rect(remaining_seconds, initial_seconds),
        kTimerDestination,
        package_progress_markers(total_packages, completed_packages),
    };
}

} // namespace btb::maze
