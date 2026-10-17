#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <string_view>
#include <string>

// The 23 original entries of retail LoadFireworkMovieBank (0x004115C0).
// Keep order and filename capitalization: authored row-one events apply
// type+12, while indices 20..22 are topmiddle and the two crowd movies.
namespace btb::fireworks {
inline constexpr std::array<std::string_view,23> kOriginalFireworkBinkNames{{
    "airbombredleft.bik",
    "smallgreenleft.bik",
    "mediumleftred.bik",
    "bigblueleft.bik",
    "bigredleft.bik",
    "Mediumgreenleft.bik",
    "smallblueleft.bik",
    "airbombblueleft.bik",
    "Candlered.bik",
    "Wheelred.bik",
    "Wheelgreen.bik",
    "Candleblue.bik",
    "airbombredright.bik",
    "smallgreenright.bik",
    "mediumrightred.bik",
    "bigblueright.bik",
    "bigredright.bik",
    "Mediumgreenright.bik",
    "smallblueright.bik",
    "airbombblueright.bik",
    "topmiddle.bik",
    "fireworkcrowdloop.bik",
    "fireworkcrowdend.bik"
}};
// LoadFireworkMovieBank (PE32 0x004115C0) constructs one DDSURFACEDESC2
// per index, with its ORIGINAL width/height fields. Those are NOT to be
// guessed from a Bink header. The source uses one 1,500,000-byte fread
// scratch buffer for every stream, then retains the copied byte count.
inline constexpr std::size_t kOriginalFireworkReadCapacity=1500000;
struct OriginalFireworkSurfaceSize {
    int width{};
    int height{};
};
[[nodiscard]] constexpr OriginalFireworkSurfaceSize
original_firework_surface_size(std::size_t index) noexcept {
    if(index<8)return {220,340};
    if(index<12)return {220,140};
    if(index<20)return {200,340};
    if(index==20)return {220,200};
    if(index<23)return {640,140};
    return {};
}
inline constexpr std::size_t kOriginalFireworkMovieCount=
    kOriginalFireworkBinkNames.size();
inline constexpr std::size_t kOriginalTopMiddleMovie=20;
inline constexpr std::size_t kOriginalCrowdLoopMovie=21;
inline constexpr std::size_t kOriginalCrowdEndMovie=22;

[[nodiscard]] inline std::array<std::filesystem::path,23>
original_firework_movie_paths(
    const std::filesystem::path& installed_subgame_firework) {
    std::array<std::filesystem::path,23> result{};
    for(std::size_t i=0;i<result.size();++i)
        result[i]=installed_subgame_firework/"graph"/
            std::string(kOriginalFireworkBinkNames[i]);
    return result;
}
}
