#include "btb/original_asset_resolver.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>

int main() {
    using namespace btb::full_game;
    namespace fs = std::filesystem;

    const auto parent = fs::temp_directory_path() / "btb_original_asset_resolver";
    fs::remove_all(parent);
    const auto installed = parent / "Installed";
    const auto disc = parent / "CD";
    fs::create_directories(installed / "Data" / "SubGameOpen");
    fs::create_directories(installed / "loaddata");
    fs::create_directories(disc / "DATA" / "sound");

    {
        std::ofstream(installed / "Data" / "SubGameOpen" /
                      "BOBINSTAND.bmp", std::ios::binary) << "BMP";
        std::ofstream(installed / "loaddata" /
                      "uiHotAreaReplace.txt") << "UI";
        std::ofstream(disc / "DATA" / "sound" /
                      "binklist.txt") << "AS_BOB_01.wav 0\n";
    }

    OriginalAssetResolver lookup(installed, disc);
    const auto direct = lookup.resolve(
        "Data\\SubGameOpen\\BOBINSTAND.bmp");
    assert(direct.found());
    assert(direct.status == AssetResolutionStatus::FoundInstalled);
    assert(direct.absolute_path.filename() == "BOBINSTAND.bmp");

    const auto original_windows_case = lookup.resolve(
        "data\\subgameopen\\bobinstand.BMP");
    assert(original_windows_case.found());
    assert(original_windows_case.absolute_path == direct.absolute_path);

    const auto front_end = lookup.resolve(
        "loaddata\\uiHotAreaReplace.txt");
    assert(front_end.found());

    const auto fallback = lookup.resolve(
        "Data\\Sound\\binklist.txt");
    assert(fallback.found());
    assert(fallback.status == AssetResolutionStatus::FoundOnDisc);

    assert(lookup.resolve("..\\outside.txt").status ==
           AssetResolutionStatus::InvalidOriginalPath);
    assert(lookup.resolve("Data/../private").status ==
           AssetResolutionStatus::InvalidOriginalPath);
    assert(lookup.resolve("C:\\game.exe").status ==
           AssetResolutionStatus::InvalidOriginalPath);
    assert(lookup.resolve("/etc/passwd").status ==
           AssetResolutionStatus::InvalidOriginalPath);
    assert(lookup.resolve("Data\\\\").status ==
           AssetResolutionStatus::Missing);

#ifndef _WIN32
    // Windows cannot create both spellings simultaneously; Linux can.
    std::ofstream(installed / "Data" / "SubGameOpen" /
                  "MIX.bmp") << "a";
    std::ofstream(installed / "Data" / "SubGameOpen" /
                  "Mix.bmp") << "b";
    const auto ambiguous = lookup.resolve(
        "data/subgameopen/mix.BMP");
    assert(ambiguous.status == AssetResolutionStatus::AmbiguousCaseFold);
#endif

    fs::remove_all(parent);
}
