#include "btb/full_game_front_end.hpp"

#include <cassert>
#include <memory>
#include <sstream>
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

    // Real 00429150 numbered/un-numbered frame convention and
    // 00428600 draw order: hover, optional keyed, depressed, idle.
    {
        std::vector<btb::front_end::ReplacementRecord> records(2);
        records[0].hover_bitmap_base="Data/ui/actanims/petsh";
        records[0].hover_frame_count=20;
        records[0].x=385;records[0].y=170;
        records[0].pressed_bitmap="Data/ui/pressed.bmp";
        records[0].optional_surface="Data/ui/adornment.bmp";
        records[0].secondary_x=10;records[0].secondary_y=20;
        records[1].secondary_bitmap_base="Data/ui/actanims/dinob";
        records[1].secondary_frame_count=10;
        records[1].secondary_x=77;records[1].secondary_y=329;
        std::vector<btb::front_end::ReplacementRuntimeState> runtime(2);
        std::vector<Draw> art;
        int delay=0;
        append_generic_ui_art(art,records,runtime,0,true,delay);
        assert(art.size()==4);
        assert(art[0].source_asset=="Data/ui/actanims/petsh.bmp");
        assert(art[0].x==385&&art[0].y==170&&!art[0].color_keyed);
        assert(art[1].source_asset=="Data/ui/adornment.bmp"&&art[1].color_keyed);
        assert(art[2].source_asset=="Data/ui/pressed.bmp"&&!art[2].color_keyed);
        assert(art[3].source_asset=="Data/ui/actanims/dinob1.bmp");
        assert(art[3].x==77&&art[3].y==329);
        runtime[0].hover_animation_frame=19;
        for(int i=0;i<6;++i){
            art.clear();
            append_generic_ui_art(art,records,runtime,0,false,delay);
        }
        assert(art.size()==3);
        assert(art[0].source_asset=="Data/ui/actanims/petsh19.bmp");
        assert(art[2].source_asset=="Data/ui/actanims/dinob2.bmp");
        runtime[1].secondary_animation_frame=9;delay=0;art.clear();
        append_generic_ui_art(art,records,runtime,0,false,delay,false);
        assert(art.size()==1&&art[0].source_asset=="Data/ui/actanims/dinob.bmp");
    }
    {
        // Real down/drag/up semantics: do not accept release on a
        // different tile or a release with no captured press.
        std::optional<std::size_t> captured;
        ActivityFrameInput input{};input.menu_release_input=true;
        input.click_pulse=true;input.pointer_button_held=true;
        auto step=resolve_menu_pointer(captured,0,0,input);
        assert(step.depressed&&!step.activate&&captured==0);
        input.click_pulse=false;
        step=resolve_menu_pointer(captured,1,1,input);
        assert(!step.hover&&!step.depressed&&!step.activate);
        input.pointer_button_held=false;input.pointer_release_pulse=true;
        step=resolve_menu_pointer(captured,1,1,input);
        assert(!step.activate&&!captured);
        step=resolve_menu_pointer(captured,1,1,input);
        assert(!step.activate);
        input.click_pulse=true; // fast tap in one sampled frame
        step=resolve_menu_pointer(captured,1,0,input);
        assert(!step.activate);
        step=resolve_menu_pointer(captured,1,1,input);
        assert(step.activate&&!captured);
        std::vector<btb::front_end::ReplacementRecord> rows(2);
        rows[0].target_state_or_action=-2;rows[0].hover_bitmap_base="easy";
        rows[1].target_state_or_action=-3;rows[1].hover_bitmap_base="medium";
        rows[1].secondary_x=340;rows[1].secondary_y=317;
        rows[0].hover_frame_count=rows[1].hover_frame_count=1;
        std::vector<btb::front_end::ReplacementRuntimeState> runtime(2);
        std::vector<Draw> draws;
        append_generic_ui_difficulty_art(draws,rows,runtime,1);
        assert(draws.size()==1&&draws[0].source_asset=="medium.bmp");
        assert(draws[0].x==340&&draws[0].y==317);
    }
    auto catalog = fixture_catalog();
    std::string err;
    assert(catalog.validate(err));
    assert(activity_select_hit(catalog.hot_areas[1], 450, 390) == 2);
    assert(!activity_select_hit(catalog.hot_areas[1], 400, 390));
    assert(!activity_select_hit(catalog.hot_areas[1], 560, 390));
    assert(!activity_select_hit(catalog.hot_areas[1], 400, 339));

    auto game_storage=std::make_unique<GameRoot>();
    auto& game=*game_storage;
    assert(game.select_profile(0));
    // Native DirectDraw7 uses source slot 2 of the separate 23-entry
    // uiBitmapName.txt table for original Activity Select background.
    std::ostringstream retail_bitmaps;
    for(int i=0;i<23;++i)
        retail_bitmaps << "Data/ui/authored" << i << ".bmp\n";
    retail_bitmaps << "END.bmp\n";
    OriginalUiBitmapCatalog bitmaps;
    std::string bitmap_error;
    std::istringstream bitmap_stream(retail_bitmaps.str());
    assert(bitmaps.read(bitmap_stream,bitmap_error));
    auto chooser=std::make_unique<ActivitySelectDriver>(
        catalog,std::vector<int>(10,0));
    assert(chooser->configure_backdrop(bitmaps,2));
    game.install_activity_select(std::move(chooser));
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
    assert(armed.effects.draws.size()==1);
    assert(armed.effects.draws[0].source_asset==
           "Data/ui/authored2.bmp");
    assert(!armed.effects.draws[0].color_keyed);
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

    // Exercise the real locked-Finale modal opening, voice gate, original
    // artwork and release-to-return, not a direct reset of its global flag.
    firework.managed_sound_playing=true;
    auto overlay=game.advance(firework);
    assert(overlay.kind==FrameKind::Intercept);
    assert(overlay.effects.draws.size()==1);
    assert(overlay.effects.draws[0].source_asset=="Data/ui/Progress-screen.bmp");
    assert(overlay.effects.audio.empty());
    firework.managed_sound_playing=false;
    overlay=game.advance(firework);
    assert(overlay.effects.audio.size()==1&&overlay.effects.audio[0].sound_id==954);
    assert(overlay.effects.audio[0].input_interruptible);
    assert(game.advance(firework).effects.audio.empty());
    ActivityFrameInput overlay_back{};
    overlay_back.pointer_position_valid=true;
    overlay_back.pointer_x=600;
    overlay_back.pointer_y=444;
    overlay_back.pointer_button_held=true;
    assert(game.advance(overlay_back).kind==FrameKind::Intercept);
    assert(game.globals().dispatcher.progress_screen_active);
    overlay_back.pointer_button_held=false;
    overlay_back.pointer_release_pulse=true;
    assert(game.advance(overlay_back).kind==FrameKind::Intercept);
    assert(!game.globals().dispatcher.progress_screen_active);
    assert(game.globals().dispatcher.current_state==static_cast<int>(State::ActivitySelectUpdate));
    assert(game.advance({}).kind==FrameKind::FrontEndUpdated);

    // Complete the exact 13 prerequisites and re-enter state 4 to
    // restore native Activity Select dynamic hover metadata.
    for (int slot=50; slot<=62; ++slot) {
        game.globals().player_progress[0].values[
            static_cast<std::size_t>(slot)] = 1;
    }
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
