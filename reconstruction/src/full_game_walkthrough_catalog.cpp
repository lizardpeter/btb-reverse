#include "btb/full_game_walkthrough_catalog.hpp"

#include <utility>

namespace btb::full_game {

bool OriginalWalkthroughCatalog::read(
    std::istream& input, std::string& error) {

    std::array<WalkthroughEntry,kRetailWalkthroughCount> candidate{};
    std::array<std::string,kRetailSubHelpCount> extra{};

    for (auto& entry : candidate) {
        if (!(input >> entry.movie >> entry.spoken_help) ||
            entry.movie.empty() || entry.movie == "NULL" ||
            entry.spoken_help.empty() || entry.spoken_help == "NULL") {
            error = "binkwalk.txt missing original walkthrough Bink/help pair";
            return false;
        }
    }
    for (auto& help : extra) {
        std::string sentinel;
        if (!(input >> sentinel >> help) ||
            sentinel != "NULL" ||
            help.empty() || help == "NULL") {
            error = "binkwalk.txt malformed secondary NULL-movie/help pair";
            return false;
        }
    }
    std::string trailing;
    if ((input >> trailing) || input.bad()) {
        error = "binkwalk.txt contains trailing or unreadable data";
        return false;
    }

    // A broken update never destroys a previously successful catalog.
    entries_ = std::move(candidate);
    sub_help_ = std::move(extra);
    initialized_ = true;
    error.clear();
    return true;
}

} // namespace btb::full_game
