#pragma once

#include "btb/game_flow.hpp"

#include <array>
#include <cstddef>
#include <istream>
#include <optional>
#include <string>
#include <vector>

namespace btb::full_game {

// 00407770 loads at most ten path/flag pairs from videoseq.txt. This is
// the application boot sequence, separate from the eight activity intros.
struct BootMovie {
    std::string file{};
    int flag{};
    [[nodiscard]] bool skippable() const noexcept { return flag != 1; }
};
class OriginalBootMovieCatalog {
public:
    [[nodiscard]] bool read(std::istream&,std::string& error);
    [[nodiscard]] const std::vector<BootMovie>& entries() const noexcept { return files_; }
    [[nodiscard]] bool loaded() const noexcept { return loaded_; }
private:
    std::vector<BootMovie> files_{};
    bool loaded_{};
};

// Original loaddata/startupmovie.txt contains exactly eight bitmap-independent
// startup Bink entries. The four pregame states that call 0x408EB0 use fixed
// 256-byte slots in the loaded string table (base 0x493970).
inline constexpr std::size_t kRetailStartupMovieCount = 8;

[[nodiscard]] constexpr std::optional<int>
retail_pregame_global_intro_index(
    game_flow::State setup) noexcept {
    using game_flow::State;
    switch (setup) {
    case State::HerdingPregameSetup: return 0;      // 0x493970
    case State::FireworksPregameSetup: return 4;    // 0x493D70
    case State::SquirrelPregameSetup: return 5;     // 0x493E70
    case State::ParkDesignerPregameSetup: return 7; // 0x494070
    default: return std::nullopt;
    }
}

class OriginalStartupMovieCatalog {
public:
    [[nodiscard]] bool read(std::istream& in, std::string& error);

    [[nodiscard]] const std::string* movie(int index) const noexcept {
        return initialized_ && index >= 0 &&
            index < static_cast<int>(files_.size())
            ? &files_[static_cast<std::size_t>(index)] : nullptr;
    }
    [[nodiscard]] bool loaded() const noexcept {
        return initialized_;
    }
private:
    std::array<std::string,kRetailStartupMovieCount> files_{};
    bool initialized_{};
};

// Independent eight-entry loaddata/completedmovie.txt table at 0x0048daf0.
class OriginalCompletionMovieCatalog {
public:
    [[nodiscard]] bool read(std::istream& in,std::string& error);
    [[nodiscard]] const std::string* movie(int index) const noexcept {
        return loaded_ && index>=0 && index<8?&files_[static_cast<std::size_t>(index)]:nullptr;
    }
private:
    std::array<std::string,8> files_{};
    bool loaded_{};
};

} // namespace btb::full_game
