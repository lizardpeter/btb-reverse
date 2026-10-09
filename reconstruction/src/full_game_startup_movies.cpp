#include "btb/full_game_startup_movies.hpp"

#include <utility>

namespace btb::full_game {

bool OriginalStartupMovieCatalog::read(
    std::istream& in, std::string& error) {

    std::array<std::string,kRetailStartupMovieCount> candidate{};
    for (auto& file : candidate) {
        if (!(in >> file) || file.empty() || file == "NULL") {
            error = "startupmovie.txt requires eight original Bink paths";
            return false;
        }
    }
    std::string extra;
    if ((in >> extra) || in.bad()) {
        error = "startupmovie.txt contains extra or unreadable records";
        return false;
    }
    files_ = std::move(candidate);
    initialized_ = true;
    error.clear();
    return true;
}

} // namespace btb::full_game
