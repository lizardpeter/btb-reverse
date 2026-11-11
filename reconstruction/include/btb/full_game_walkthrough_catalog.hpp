#pragma once

#include <array>
#include <cstddef>
#include <istream>
#include <optional>
#include <string>
#include <string_view>

namespace btb::full_game {

// Data comes from the installed loaddata/binkwalk.txt, parsed by the retail
// bulk-table loader. Ten primary walkthrough movies have one spoken-help
// WAV each. The last three table rows have a literal NULL movie and an
// additional spoken-help WAV; they must NOT create three phantom movies.
inline constexpr std::size_t kRetailWalkthroughCount = 10;
inline constexpr std::size_t kRetailSubHelpCount = 3;

// Retail UpdateWalkthroughMovie (0x00428120) checks the ORIGINAL
// pointer region (49,140)-(365,350), strictly excluding its edges.
// The enclosing dispatcher writes DAT_00446FD8 for each pregame
// case (0x0042AAED..0x0042C84E), in binkwalk.txt movie-index order.
// Narration is sound priority 0x5A, class 1 and must not start
// when ANY managed sound is already playing.
inline constexpr std::array<int,kRetailWalkthroughCount>
    kRetailWalkthroughHoverVoiceIds{{
        0x1f5, // 0  Pets Corner
        0x1f0, // 1  Dinosaur
        0x1fa, // 2  Spud Skate (source state 0x19)
        0x1f9, // 3  Spud Maze (source state 0x39)
        0x1e8, // 4  Maze (source state 0x1F)
        0x1e9, // 5  Golf (source state 0x35)
        0x1f3, // 6  Fireworks
        0x1f7, // 7  Squirrel
        0x1f4, // 8  Bob's Band
        0x1f1, // 9  Park Designer
    }};

[[nodiscard]] constexpr std::optional<int>
retail_walkthrough_hover_voice(
    int movie_index,int pointer_x,int pointer_y,
    bool any_managed_sound_playing) noexcept {
    if(movie_index<0 ||
       movie_index>=static_cast<int>(kRetailWalkthroughCount) ||
       any_managed_sound_playing ||
       !(pointer_x>0x31 && pointer_x<0x16d &&
         pointer_y>0x8c && pointer_y<0x15e))
        return std::nullopt;
    return kRetailWalkthroughHoverVoiceIds[
        static_cast<std::size_t>(movie_index)];
}

struct WalkthroughEntry {
    std::string movie{};
    std::string spoken_help{};
};

class OriginalWalkthroughCatalog {
public:
    [[nodiscard]] bool read(std::istream& input, std::string& error);

    [[nodiscard]] const WalkthroughEntry* entry(int index) const noexcept {
        return initialized_ && index >= 0 &&
            index < static_cast<int>(entries_.size())
                ? &entries_[static_cast<std::size_t>(index)]
                : nullptr;
    }
    [[nodiscard]] const std::string* extra_help(
        int index) const noexcept {
        return initialized_ && index >= 0 &&
            index < static_cast<int>(sub_help_.size())
                ? &sub_help_[static_cast<std::size_t>(index)]
                : nullptr;
    }
    [[nodiscard]] bool loaded() const noexcept {
        return initialized_;
    }

private:
    std::array<WalkthroughEntry,kRetailWalkthroughCount> entries_{};
    std::array<std::string,kRetailSubHelpCount> sub_help_{};
    bool initialized_{};
};

} // namespace btb::full_game
