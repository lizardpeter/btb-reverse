#pragma once

#include "btb/full_game_pregame.hpp"
#include "btb/full_game_generic_ui.hpp"
#include "btb/full_game_ui_bitmaps.hpp"
#include "btb/full_game_walkthrough_catalog.hpp"

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>
#include <string>

namespace btb::full_game {

// The original instruction-screen presentation is the combination of a
// backdrop in loaddata/uiBitmapName.txt and a separately indexed looping
// Bink + hover/help WAV in loaddata/binkwalk.txt. The two indices are NOT
// the same as front_end::Screen (2, 7, 8). The actual decoder, sound device,
// source-frame animations and retained COM surfaces remain host-owned.
struct OriginalPregameResources {
    std::size_t backdrop_slot{};
    Draw backdrop{};
    int walkthrough_index{};
    std::string walkthrough_movie{};
    std::string spoken_help_wav{};
    bool global_intro_mode14{};
};

[[nodiscard]] std::optional<OriginalPregameResources>
resolve_original_pregame_resources(
    game_flow::State setup,
    int selected_subgame,
    const OriginalUiBitmapCatalog& bitmap_table,
    const OriginalWalkthroughCatalog& walkthrough_table);


// Construct the real table-driven pregame UI with the source-backed
// background selected from uiBitmapName.txt. The caller still installs
// the resulting driver into GameRoot, and hosts the separate Bink movie.
[[nodiscard]] std::unique_ptr<GenericUiScreenDriver>
make_original_pregame_screen_driver(
    GenericUiCatalog ui_tables,
    const OriginalUiBitmapCatalog& bitmap_table,
    game_flow::State setup,
    int selected_subgame,
    std::vector<int> original_initial_hover_random_draws);

} // namespace btb::full_game
