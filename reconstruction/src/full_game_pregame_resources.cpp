#include "btb/full_game_pregame_resources.hpp"

#include <utility>

namespace btb::full_game {

std::optional<OriginalPregameResources> resolve_original_pregame_resources(
    game_flow::State setup,
    int selected_subgame,
    const OriginalUiBitmapCatalog& bitmap_table,
    const OriginalWalkthroughCatalog& walkthrough_table) {

    const auto* pair = retail_pregame_for_state(setup);
    if (!pair || pair->setup != setup) {
        return std::nullopt;
    }
    const auto backdrop_slot = retail_pregame_backdrop_index(
        setup,selected_subgame);
    const auto movie_index = retail_walkthrough_index(
        setup,selected_subgame);
    if (!backdrop_slot || !movie_index) {
        return std::nullopt;
    }
    auto backdrop = bitmap_table.backdrop(*backdrop_slot);
    const auto* movie = walkthrough_table.entry(*movie_index);
    if (!backdrop || !movie) {
        return std::nullopt;
    }
    return OriginalPregameResources{
        *backdrop_slot,std::move(*backdrop),*movie_index,
        movie->movie,movie->spoken_help,
        pair->starts_global_movie_mode14
    };
}

std::unique_ptr<GenericUiScreenDriver>
make_original_pregame_screen_driver(
    GenericUiCatalog ui_tables,
    const OriginalUiBitmapCatalog& bitmap_table,
    game_flow::State setup,
    int selected_subgame,
    std::vector<int> original_initial_hover_random_draws) {

    const auto* pair = retail_pregame_for_state(setup);
    if (!pair || pair->setup != setup) {
        return {};
    }
    const auto backdrop_index = retail_pregame_backdrop_index(
        setup,selected_subgame);
    if (!backdrop_index || !bitmap_table.filename(*backdrop_index)) {
        return {};
    }
    auto driver = std::make_unique<GenericUiScreenDriver>(
        std::move(ui_tables),pair->screen,
        std::move(original_initial_hover_random_draws));
    if (!driver->configure_backdrop(bitmap_table,*backdrop_index)) {
        return {};
    }
    return driver;
}

} // namespace btb::full_game
