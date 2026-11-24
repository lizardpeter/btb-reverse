#include "btb/full_game_ui_bitmaps.hpp"

#include <utility>

namespace btb::full_game {

bool OriginalUiBitmapCatalog::read(
    std::istream& in, std::string& error) {

    std::array<std::string,kRetailBackdropCount> candidate{};
    for (std::size_t i=0; i<kRetailBackdropCount; ++i) {
        if (!(in >> candidate[i]) ||
            candidate[i].empty() ||
            candidate[i] == "END.bmp") {
            error = "uiBitmapName.txt is missing the 23 original paths";
            return false;
        }
    }

    std::string sentinel;
    std::string extra;
    if (!(in >> sentinel) || sentinel != "END.bmp" || (in >> extra)) {
        error = "uiBitmapName.txt must terminate at literal END.bmp";
        return false;
    }
    if (in.bad()) {
        error = "I/O failure reading native UI bitmap table";
        return false;
    }

    paths_ = std::move(candidate);
    loaded_ = true;
    error.clear();
    return true;
}

const std::string* OriginalUiBitmapCatalog::filename(
    std::size_t source_slot) const noexcept {

    if (!loaded_ || source_slot >= kRetailBackdropCount) {
        return nullptr;
    }
    return &paths_[source_slot];
}

std::optional<Draw> OriginalUiBitmapCatalog::backdrop(
    std::size_t source_slot) const {

    const auto* path = filename(source_slot);
    if (!path) {
        return std::nullopt;
    }
    return Draw{*path,0,0,std::nullopt,false};
}

} // namespace btb::full_game
