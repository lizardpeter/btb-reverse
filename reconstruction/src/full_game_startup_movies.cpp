#include "btb/full_game_startup_movies.hpp"

#include <utility>

namespace btb::full_game {

bool OriginalBootMovieCatalog::read(std::istream& in,std::string& error) {
    std::vector<BootMovie> staged;
    std::string name;
    while (in >> name) {
        int flag{};
        if (staged.size()==10 || name=="NULL" || name.size()>=260 || !(in>>flag)) {
            error="videoseq.txt requires at most ten original path/flag pairs";
            return false;
        }
        staged.push_back({std::move(name),flag});
    }
    if (in.bad() || !in.eof() || staged.empty()) {
        error="videoseq.txt is empty or unreadable";return false;
    }
    files_=std::move(staged);loaded_=true;error.clear();return true;
}

bool OriginalCompletionMovieCatalog::read(std::istream& in,std::string& error) {
    std::array<std::string,8> next{};
    for (auto& name:next) {
        if (!(in>>name) || name=="NULL") {
            error="completedmovie.txt requires eight original Bink paths";return false;
        }
    }
    std::string extra;
    if ((in>>extra) || in.bad()) {
        error="completedmovie.txt has extra or unreadable records";return false;
    }
    files_=std::move(next);loaded_=true;error.clear();return true;
}

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
