#pragma once

#include "btb/full_game_runtime.hpp"

#include <array>
#include <cstddef>
#include <istream>
#include <optional>
#include <string>

namespace btb::full_game {

// loaddata/uiBitmapName.txt contains 23 original bitmap names plus a literal
// END.bmp sentinel. It is a state/page backdrop table; it is NOT indexed by
// the generic 12-screen UI enum. Outer state transition logic must supply
// the correct source slot explicitly.
inline constexpr std::size_t kRetailBackdropCount = 23;

class OriginalUiBitmapCatalog {
public:
    [[nodiscard]] bool read(std::istream& in, std::string& error);
    [[nodiscard]] std::optional<Draw> backdrop(std::size_t source_slot) const;
    [[nodiscard]] const std::string* filename(
        std::size_t source_slot) const noexcept;
    [[nodiscard]] bool loaded() const noexcept { return loaded_; }

private:
    std::array<std::string,kRetailBackdropCount> paths_{};
    bool loaded_{};
};

} // namespace btb::full_game
