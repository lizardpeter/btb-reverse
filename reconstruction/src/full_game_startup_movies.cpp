#include "btb/full_game_startup_movies.hpp"

#include <utility>

namespace btb::full_game {

bool OriginalVideoSequenceCatalog::read(
    std::istream& in, std::string& error) {
    std::vector<OriginalVideoSequenceEntry> candidate;
    std::string path;
    while (in >> path) {
        std::int32_t mode{};
        if (!(in >> mode)) {
            error = "videoseq.txt record requires an original path and input mode";
            return false;
        }
        // The retail loader reserves 0x104 bytes for each path. Reject a
        // path that would overflow that slot rather than emulate memory damage.
        if (path.empty() || path.size() >= 0x104U) {
            error = "videoseq.txt path exceeds the original 0x104-byte slot";
            return false;
        }
        if (candidate.size() >= kRetailVideoSequenceCapacity) {
            error = "videoseq.txt exceeds the ten original movie slots";
            return false;
        }
        candidate.push_back({std::move(path), mode});
        path.clear();
    }
    if (in.bad() || (!in.eof() && in.fail())) {
        error = "videoseq.txt could not be read completely";
        return false;
    }
    if (candidate.empty()) {
        error = "videoseq.txt contains no startup movie records";
        return false;
    }
    entries_ = std::move(candidate);
    initialized_ = true;
    error.clear();
    return true;
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
