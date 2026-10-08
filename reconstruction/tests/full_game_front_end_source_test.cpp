#include "btb/full_game_front_end.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace {
using namespace btb;
using namespace btb::full_game;

GenericUiCatalog fixture_catalog() {
    GenericUiCatalog c;
    for (std::size_t screen = 0; screen < front_end::kScreenCount; ++screen) {
        const int n = front_end::kRetailHotAreaCounts[screen];
        c.counts.push_back(n);
        front_end::HotAreaScreen areas;
        front_end::ReplacementScreen replacements;
        for (int i=0; i<n; ++i) {
            const int x = 10 + i*10;
            const int y = 50;
            areas.areas.push_back({
                {{{x,y},{x+8,y},{x+8,y+30},{x,y+30}}},
                {},
            });
            front_end::ReplacementRecord r;
            r.target_state_or_action = 0x0c;
            r.click_sound_id = 451;
            r.hover_sound_ids = {39,-1,-1};
            replacements.records.push_back(r);
        }
        c.hot_areas.push_back(std::move(areas));
        c.replacements.push_back(std::move(replacements));
    }

    // Override the actual third Activity Select tile with its original
    // Firework hotspot / native outer-state value 0x22.
    auto& third = c.hot_areas[1].areas[2];
    third.polygon = {
        {400,339},{560,339},{560,450},{400,450},
    };
    c.replacements[1].records[2].target_state_or_action = 0x22;
    c.replacements[1].records[2].click_sound_id = -1;
    c.replacements[1].records[2].hover_sound_ids = {8,40,-1};

    // Back and Help are negative UI actions, not main state-table indexes.
    c.replacements[1].records[8].target_state_or_action = -1;
    c.replacements[1].records[9].target_state_or_action = -6;
    return c;
}
}

int main() {
    using namespace btb::full_game;
    using btb::game_flow::State;
    using btb::progress::Slot;

    auto catalog = fixture_catalog();
    std::string err;
    assert(catalog.validate(err));
    assert(activity_select_hit(catalog.hot_areas[1], 450, 390) == 2);
    assert(!activity_select_hit(catalog.hot_areas[1], 400, 390));
    assert(!activity_select_hit(catalog.hot_areas[1], 560, 390));
    assert(!activity_select_hit(catalog.hot_areas[1], 400, 339));

    GameRoot game;
    assert(game.select_profile(0));
    game.install_activity_select(std::make_unique<ActivitySelectDriver>(
        catalog, std::vector<int>(10,0)));
    game.set_outer_state(State::ActivitySelectSetup);

    auto initialized=game.advance({});
    assert(initialized.kind == FrameKind::FrontEndInitialized);
    assert(game.globals().finale_gate.locked);
    assert(game.globals().dispatcher.current_state ==
           static_cast<int>(State::ActivitySelectUpdate));

    ActivityFrameInput firework{};
    firework.pointer_x = 450;
    firework.pointer_y = 390;
    firework.click_pulse = true;
    auto armed = game.advance(firework);
    assert(armed.kind == FrameKind::FrontEndUpdated);
    // The locked special hover uses the native ASH_WEN_09 sound id 65.
    assert(!armed.effects.audio.empty());
    assert(armed.effects.audio[0].sound_id == 65);
    assert(!armed.effects.open_progress_screen);
    assert(!armed.effects.next_outer_state);

    firework.click_pulse = false;
    firework.managed_sound_playing = true;
    auto voice = game.advance(firework);
    assert(!voice.effects.open_progress_screen);
    assert(!game.globals().dispatcher.progress_screen_active);

    firework.managed_sound_playing = false;
    auto locked = game.advance(firework);
    assert(locked.effects.open_progress_screen);
    assert(game.globals().dispatcher.progress_screen_active);
    assert(game.globals().dispatcher.current_state ==
           static_cast<int>(State::ActivitySelectUpdate));

    // Native modal interception precedes any subsequent Activity Select.
    assert(game.advance(firework).kind == FrameKind::Intercept);

    // Complete the exact 13 prerequisites, return from modal and re-enter
    // state 4 to restore the native Activity Select dynamic hover metadata.
    for (int slot=50; slot<=62; ++slot) {
        game.globals().player_progress[0].values[
            static_cast<std::size_t>(slot)] = 1;
    }
    game.globals().dispatcher.progress_screen_active = false;
    game.set_outer_state(State::ActivitySelectSetup);
    assert(game.advance({}).kind == FrameKind::FrontEndInitialized);
    assert(!game.globals().finale_gate.locked);

    firework.click_pulse = true;
    auto unlocked_armed = game.advance(firework);
    assert(!unlocked_armed.effects.open_progress_screen);
    firework.click_pulse = false;
    const auto route = game.advance(firework);
    assert(route.effects.next_outer_state == 0x22);
    assert(!route.effects.open_progress_screen);
    assert(game.globals().dispatcher.current_state == 0x22);
    assert(game.advance({}).kind == FrameKind::FrontEndRequiresAdapter);

    // A different activity tile uses its normal data-driven outer route
    // only after its click sound has completed.
    game.set_outer_state(State::ActivitySelectSetup);
    assert(game.advance({}).kind == FrameKind::FrontEndInitialized);
    ActivityFrameInput other{};
    other.pointer_x = 13;
    other.pointer_y = 65;
    other.click_pulse = true;
    const auto other_armed = game.advance(other);
    assert(other_armed.effects.audio.size() >= 2);
    assert(!other_armed.effects.next_outer_state);
    other.click_pulse = false;
    other.managed_sound_playing = true;
    assert(!game.advance(other).effects.next_outer_state);
    other.managed_sound_playing = false;
    const auto other_route = game.advance(other);
    assert(other_route.effects.next_outer_state == 0x0c);
    assert(game.globals().dispatcher.current_state == 0x0c);

    // The Activity Select -1 branch differs from the four sub-choosers:
    // native 0x42A917 returns to the five profile signs at outer state 1.
    game.set_outer_state(State::ActivitySelectSetup);
    assert(game.advance({}).kind == FrameKind::FrontEndInitialized);
    ActivityFrameInput back{};
    back.pointer_x = 93; // fixture's Activity Select area 8
    back.pointer_y = 65;
    back.click_pulse = true;
    assert(game.advance(back).kind == FrameKind::FrontEndUpdated);
    back.click_pulse = false;
    const auto back_result = game.advance(back);
    assert(back_result.effects.negative_ui_action == -1);
    assert(back_result.menu_action);
    assert(back_result.menu_action->next_state ==
           State::PlayerProfileAndNameEntry);
    assert(game.globals().dispatcher.current_state == 0x01);

    // Activity Select Help -6 leaves the outer update state intact:
    // the original generic UI engine owns its contextual help mode.
    game.set_outer_state(State::ActivitySelectSetup);
    assert(game.advance({}).kind == FrameKind::FrontEndInitialized);
    ActivityFrameInput help{};
    help.pointer_x = 103; // fixture's Activity Select area 9
    help.pointer_y = 65;
    help.click_pulse = true;
    assert(game.advance(help).kind == FrameKind::FrontEndUpdated);
    help.click_pulse = false;
    const auto help_result = game.advance(help);
    assert(help_result.effects.negative_ui_action == -6);
    assert(help_result.menu_action);
    assert(help_result.menu_action->request_contextual_help);
    assert(!help_result.menu_action->next_state);
    assert(game.globals().dispatcher.current_state ==
           static_cast<int>(State::ActivitySelectUpdate));
}
