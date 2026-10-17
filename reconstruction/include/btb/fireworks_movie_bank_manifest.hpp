#pragma once

#include <array>
#include <cstddef>
#include <filesystem>
#include <string_view>

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
