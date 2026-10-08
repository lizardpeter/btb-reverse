#pragma once

#include "btb/bobs_band.hpp"

#include <array>
#include <istream>
#include <string>
#include <string_view>

namespace btb::bobs_band {

struct Point {
    int x{};
    int y{};
};
struct Size {
    int width{};
    int height{};
};
struct HitRectangle {
    int left{};
    int top{};
    int right{};
    int bottom{};
    [[nodiscard]] constexpr bool strict_contains(int x, int y) const noexcept {
        return x > left && x < right && y > top && y < bottom;
    }
};

// Source file data/subgameOpen/machinedata.txt, recovered loader 0x41D7C0.
// The 10 machine geometry entries alternate short/long for each of five
// machines. The source tail after these numbers is explanatory text.
struct MachineData {
    std::array<Point,10> machine_positions{};
    std::array<Size,10> machine_sizes{};
    std::array<Point,3> conductor_positions{};
    std::array<Size,3> conductor_sizes{};
    std::array<int,3> conductor_idle_frame_counts{};
    std::array<HitRectangle,10> machine_static_animation_rects{};
};
[[nodiscard]] MachineData parse_machine_data(std::istream& input);

inline constexpr std::array<std::string_view,10> kShortAndLongSprites{
    "Data\\SubGameOpen\\ROLEY1SEC.bmp",
    "Data\\SubGameOpen\\ROLEY2SEC.bmp",
    "Data\\SubGameOpen\\MUCK1SEC.bmp",
    "Data\\SubGameOpen\\MUCK2SEC.bmp",
    "Data\\SubGameOpen\\lofty1sec.bmp",
    "Data\\SubGameOpen\\lofty2sec.bmp",
    "Data\\SubGameOpen\\dizzy1sec.BMP",
    "Data\\SubGameOpen\\dizzy2sec.BMP",
    "Data\\SubGameOpen\\SCOOP1SEC.bmp",
    "Data\\SubGameOpen\\SCOOP2SEC.bmp",
};

inline constexpr std::array<std::string_view,3> kConductorSprites{
    "Data\\SubGameOpen\\BOBINSTAND.bmp",
    "Data\\SubGameOpen\\wendyINSTAND.bmp",
    "Data\\SubGameOpen\\picklesINSTAND.bmp",
};

inline constexpr std::array<std::string_view,3> kBackingWavs{
    "Data\\SubGameOpen\\bobmt.wav",
    "Data\\SubGameOpen\\Wendymt.wav",
    "Data\\SubGameOpen\\fpmt.wav",
};

inline constexpr std::array<std::array<std::string_view,5>,3>
kSequenceFileNames{{
    {{"musicbob1.txt", "musicbob2.txt", "musicbob3.txt",
      "musicbob4.txt", "musicbob5.txt"}},
    {{"musicwendy1.txt", "musicwendy2.txt", "musicwendy3.txt",
      "musicwendy4.txt", "musicwendy5.txt"}},
    {{"musicfarmer1.txt", "musicfarmer2.txt", "musicfarmer3.txt",
      "musicfarmer4.txt", "musicfarmer5.txt"}},
}};

// Runtime uses 50 DirectSound sample objects, with five pitch banks of ten
// machine types. Filenames are the explicit original executable string table.
[[nodiscard]] std::string sample_wav_path(int row, int type);
[[nodiscard]] std::string sound_brick_bitmap_path(int type, bool hover);
[[nodiscard]] std::string sequence_filename(Conductor conductor, int player_index);

} // namespace btb::bobs_band
