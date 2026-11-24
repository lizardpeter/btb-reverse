#pragma once

#include <array>
#include <cstddef>
#include <istream>
#include <optional>
#include <string>
#include <string_view>

namespace btb::full_game {

// Data comes from the installed loaddata/binkwalk.txt, parsed by the retail
// bulk-table loader. Preserve both columns, including legacy help names.
// 004281D0 consumes only the movie column; 0042A2C0 uses managed sound
// IDs for narration. The final three NULL rows are not playable movies.
inline constexpr std::size_t kRetailWalkthroughCount = 10;
inline constexpr std::size_t kRetailSubHelpCount = 3;

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
