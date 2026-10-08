#include "btb/full_game_generic_ui.hpp"

#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {
using namespace btb;
using namespace btb::full_game;

GenericUiCatalog catalog_fixture() {
    GenericUiCatalog catalog;
    for (std::size_t index=0; index<front_end::kScreenCount; ++index) {
        front_end::HotAreaScreen area;
        front_end::ReplacementScreen replacement;
        const auto size = front_end::kRetailHotAreaCounts[index];
        catalog.counts.push_back(size);
        for (int i=0; i<size; ++i) {
            const int x = 10+i*12;
            area.areas.push_back({
                {{{x,50},{x+10,50},{x+10,90},{x,90}}},
                {}
            });
            front_end::ReplacementRecord record{};
            record.target_state_or_action = 0x2c;
            record.hover_sound_ids = {80,-1,-1};
            record.click_sound_id = 120;
            replacement.records.push_back(std::move(record));
        }
        catalog.hot_areas.push_back(std::move(area));
        catalog.replacements.push_back(std::move(replacement));
    }
    catalog.replacements[9].records[0].target_state_or_action = -20;
    catalog.replacements[9].records[1].target_state_or_action = -21;
    return catalog;
}
}

int main() {
    using namespace btb;
    using namespace btb::full_game;
    using btb::game_flow::State;

    front_end::HotArea polygon;
    polygon.polygon = {
        {10,10},{60,10},{60,20},{20,20},{20,60},{10,60}
    };
    assert(point_in_original_polygon(polygon,15,40));
    assert(point_in_original_polygon(polygon,50,15));
    assert(!point_in_original_polygon(polygon,50,50));
    assert(!point_in_original_polygon(polygon,10,30));
    assert(!point_in_original_polygon(polygon,20,20));
    assert(!point_in_original_polygon(polygon,60,15));
    front_end::HotAreaScreen screen;
    screen.areas.push_back(polygon);
    assert(generic_ui_hit(screen,15,40) == 0);
    assert(!generic_ui_hit(screen,50,50));

    const auto catalog = catalog_fixture();
    std::string err;
    assert(catalog.validate(err));
    assert(kGenericFrontEndPairs.size() == 7);
    assert(generic_front_end_pair_for_state(State::MusicChooserSetup));
    assert(generic_front_end_pair_for_state(State::PlayAgainYesNoUpdate));
    assert(!generic_front_end_pair_for_state(State::GolfPregameUpdate));

    GameRoot root;
    assert(root.select_profile(0));
    assert(!root.install_generic_front_end_pair(
        State::GolfPregameSetup,
        std::make_unique<GenericUiScreenDriver>(
            catalog,front_end::Screen::MusicChooser,std::vector<int>(5,0))));
    std::ostringstream source_bitmaps;
    for (int i=0; i<22; ++i) {
        source_bitmaps << "Data/ui/backdrop" << i << ".bmp\\n";
    }
    source_bitmaps << "Data/ui/subact/music.bmp\\nEND.bmp\\n";
    std::istringstream names(source_bitmaps.str());
    OriginalUiBitmapCatalog backdrop_table;
    assert(backdrop_table.read(names,err));
    auto menu = std::make_unique<GenericUiScreenDriver>(
        catalog,front_end::Screen::MusicChooser,std::vector<int>(5,0));
    assert(menu->configure_backdrop(backdrop_table,22));
    assert(root.install_generic_front_end_pair(
        State::MusicChooserSetup,std::move(menu)));

    root.set_outer_state(State::MusicChooserSetup);
    auto setup = root.advance({});
    assert(setup.kind == FrameKind::FrontEndInitialized);
    assert(root.globals().dispatcher.current_state ==
           static_cast<int>(State::MusicChooserUpdate));

    ActivityFrameInput click{};
    click.pointer_x=15;
    click.pointer_y=65;
    click.click_pulse=true;
    auto hover_click = root.advance(click);
    assert(hover_click.kind == FrameKind::FrontEndUpdated);
    assert(hover_click.effects.draws.size() == 1);
    assert(hover_click.effects.draws[0].source_asset ==
           "Data/ui/subact/music.bmp");
    assert(hover_click.effects.audio.size() == 3);
    assert(hover_click.effects.audio[0].sound_id == 80);
    assert(hover_click.effects.audio[1].operation ==
           AudioOperation::StopManagedSounds);
    assert(hover_click.effects.audio[2].sound_id == 120);
    assert(!hover_click.effects.next_outer_state);

    click.click_pulse=false;
    click.managed_sound_playing=true;
    auto delayed=root.advance(click);
    assert(!delayed.effects.next_outer_state);
    click.managed_sound_playing=false;
    auto routed=root.advance(click);
    assert(routed.effects.next_outer_state == 0x2c);
    assert(root.globals().dispatcher.current_state == 0x2c);

    // The Yes/No replay screen emits negative action codes. These are NOT
    // negative indices into the 68-state table, and the original enclosing
    // replay state must eventually interpret them.
    assert(root.install_generic_front_end_pair(
        State::PlayAgainYesNoSetup,
        std::make_unique<GenericUiScreenDriver>(
            catalog,front_end::Screen::PlayAgainYesNo,
            std::vector<int>(2,0))));
    root.set_outer_state(State::PlayAgainYesNoSetup);
    setup=root.advance({});
    assert(setup.kind == FrameKind::FrontEndInitialized);
    click.click_pulse=true;
    hover_click=root.advance(click);
    assert(!hover_click.effects.next_outer_state);
    click.click_pulse=false;
    auto selected=root.advance(click);
    assert(selected.effects.negative_ui_action == -20);
    assert(root.globals().dispatcher.current_state ==
           static_cast<int>(State::PlayAgainYesNoUpdate));
}
