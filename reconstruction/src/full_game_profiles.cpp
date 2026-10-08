#include "btb/full_game_runtime.hpp"

#include <array>
#include <exception>
#include <fstream>
#include <string>
#include <utility>

namespace btb::full_game {

bool GameRoot::load_profile_files(
    const std::filesystem::path& directory,
    std::string& error) {

    if (initialized_activity_) {
        error = "cannot replace player records while an activity is running";
        return false;
    }

    // Stage every file before changing the live player or unlock state.
    // Retail stores playerinfo.txt separately from five ASCII progress
    // records, each with exactly 100 signed int32 values.
    profiles::ProfileTable metadata{};
    std::array<progress::Record, progress::kPlayerCount> records{};
    try {
        const auto info_file = directory / "playerinfo.txt";
        if (std::filesystem::exists(info_file)) {
            std::ifstream in(info_file, std::ios::binary);
            if (!in) {
                error = "cannot open original playerinfo.txt";
                return false;
            }
            metadata = profiles::read_playerinfo(in);
        }

        for (std::size_t i = 0; i < progress::kPlayerCount; ++i) {
            const auto filename =
                "player" + std::to_string(i + 1) + ".txt";
            const auto source = directory / filename;
            if (!std::filesystem::exists(source)) {
                continue; // Original first-run profiles have no save yet.
            }

            std::ifstream in(source, std::ios::binary);
            if (!in) {
                error = "cannot open original " + filename;
                return false;
            }
            records[i] = progress::read_record(in);
        }
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }

    // Do not silently keep old profile state after loading new game data.
    globals_.profile_metadata = std::move(metadata);
    globals_.player_progress = std::move(records);
    globals_.active_profile.reset();
    globals_.finale_gate = {};
    profile_directory_ = directory;
    activity_select_initialized_ = false;
    generic_front_end_initialized_.fill(false);
    error.clear();
    return true;
}

bool GameRoot::save_profile_files(
    const std::filesystem::path& directory,
    std::string& error) const {

    try {
        std::filesystem::create_directories(directory);
        {
            std::ofstream out(directory / "playerinfo.txt",
                              std::ios::binary | std::ios::trunc);
            if (!out) {
                error = "cannot create playerinfo.txt";
                return false;
            }
            profiles::write_playerinfo(
                out, globals_.profile_metadata);
            out.flush();
            if (!out) {
                error = "cannot write playerinfo.txt";
                return false;
            }
        }
        for (std::size_t i = 0; i < progress::kPlayerCount; ++i) {
            const auto filename =
                "player" + std::to_string(i + 1) + ".txt";
            std::ofstream out(directory / filename,
                              std::ios::binary | std::ios::trunc);
            if (!out) {
                error = "cannot create " + filename;
                return false;
            }
            progress::write_record(
                out, globals_.player_progress[i]);
            out.flush();
            if (!out) {
                error = "cannot write " + filename;
                return false;
            }
        }
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
    error.clear();
    return true;
}

} // namespace btb::full_game
