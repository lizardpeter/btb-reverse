#include "btb/retail_game_lifecycle_15.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace btb::game_batch_15;
namespace {
[[nodiscard]] std::size_t count(const ActivityResources& s,Op kind) {
    return static_cast<std::size_t>(std::count_if(
        s.actions.begin(),s.actions.end(),[&](const Action& a){
            return a.kind==kind;
        }));
}
[[nodiscard]] std::size_t count(const FireState& s,FireAction kind) {
    return static_cast<std::size_t>(std::count_if(
        s.events.begin(),s.events.end(),[&](const FireEvent& a){
            return a.kind==kind;
        }));
}
std::string fixture_bound_areas() {
    std::string s;
    for(std::size_t i=0;i<60;++i)
        s+="0 0 20 0 20 20 0 20 -1 -1\n";
    return s;
}
}
int main() {
    // 004110E0: 4 fixed editor surface slots and exactly 12 alternating
    // indexed slots (6 pairs). Unregister called even for null slots.
    {
        ActivityResources s{};
        s.slot[0x0050ab18]=0xabc;
        s.slot[0x0050a49c]=0xbbb;
        s.slot[0x0050a4a0]=0xccc;
        unload_fireworks_editor(s);
        assert(count(s,Op::UnregisterBitmap)==16);
        assert(count(s,Op::ReleaseBitmap)==3);
        assert(s.slot[0x0050ab18]==0&&s.slot[0x0050a49c]==0);
        assert(s.actions[0]==Action(Op::UnregisterBitmap,0x0050ab18,0));
        assert(s.actions[1]==Action(Op::ReleaseBitmap,0x0050ab18,0xabc));
        const auto old=s.actions.size();unload_fireworks_editor(s);
        assert(s.actions.size()==old+16); // no second release
    }
    // 004111C0: exactly 23 Bink movies, each close -> bitmap unregister/
    // optional release -> buffer free, followed by four trailing bitmaps.
    {
        ActivityResources s{};
        s.slot[0x0050aaac]=0x2001; // movie #0
        s.slot[0x0050937c]=0x3001; // movie render surface
        s.slot[0x0050a50c]=0x4001; // decoded movie heap record
        s.slot[0x0050aaac+22*4]=0x2002;
        s.slot[0x0050a5cc]=0x5001;
        unload_fireworks_movie_bank(s);
        assert(count(s,Op::CloseBink)==23);
        assert(count(s,Op::UnregisterBitmap)==27);
        assert(count(s,Op::FreeMovieBuffer)==23);
        assert(count(s,Op::ReleaseBitmap)==2);
        assert((s.actions[0]==Action{Op::CloseBink,0x0050aaac,0x2001}));
        assert((s.actions[1]==Action{Op::UnregisterBitmap,0x0050937c,0}));
        assert((s.actions[2]==Action{Op::ReleaseBitmap,0x0050937c,0x3001}));
        assert((s.actions[3]==Action{Op::FreeMovieBuffer,0x0050a50c,0x4001}));
        assert(s.slot[0x0050aaac]==0&&s.slot[0x0050aaac+22*4]==0);
    }
    // 0040A530: 3 fixed surfaces + variable count + sounds20 +
    // trailing 3. Explicit result verifies audio and cursor order.
    {
        ActivityResources s{};
        s.slot[0x004fc9e8]=15;s.slot[0x004fc9ec]=16;
        s.slot[0x004fc084]=17;s.slot[0x004fc0d0]=18;
        unload_dino(s,2);
        assert(count(s,Op::UnregisterBitmap)==8);
        assert(count(s,Op::DeleteSound)==2);
        assert(count(s,Op::ResetCursor)==1&&count(s,Op::StopMusic)==1);
        assert(s.actions.back().kind==Op::StopMusic);
        assert(s.actions[s.actions.size()-2].kind==Op::ResetCursor);
        assert(s.slot[0x004fc084]==0&&s.slot[0x004fc9ec]==0);
    }
    // 00419390: original odd bitmap ordering (510738 after 510744).
    {
        ActivityResources s{};
        s.herding_character_latch_a=3;s.herding_character_latch_b=4;
        s.audio_latch=7;
        unload_herding(s);
        assert(count(s,Op::UnregisterBitmap)==18);
        assert(count(s,Op::StopMusic)==1);
        const auto index=[&](U32 addr) {
            for(std::size_t i=0;i<s.actions.size();++i)
                if(s.actions[i].kind==Op::UnregisterBitmap&&
                   s.actions[i].address==addr)return i;
            return s.actions.size();
        };
        assert(index(0x00510744)<index(0x00510738));
        assert(s.herding_character_latch_a==0&&s.herding_character_latch_b==0);
        assert(s.audio_latch==0);
    }
    // 0041A390: 3 grouped + 7 singular bitmap slots, 20 sounds,
    // shared UI state 51C334 reset AFTER stopping activity music.
    {
        ActivityResources s{};
        s.shared_ui_latch=8;s.audio_latch=9;s.slot[0x00512100]=81;
        unload_maze(s);
        assert(count(s,Op::UnregisterBitmap)==10);
        assert(count(s,Op::ReleaseBitmap)==1);
        assert(s.shared_ui_latch==0&&s.audio_latch==0);
        assert(s.actions.back().kind==Op::StopMusic);
    }
    // 00414410: three interleaved arrays (3*3) and eight trailing.
    {
        ActivityResources s{};
        s.slot[0x0050ab88]=1;s.slot[0x0050adfc]=2;
        unload_golf(s);
        assert(s.skate_input_blocked==1);
        assert(count(s,Op::UnregisterBitmap)==18);
        assert(count(s,Op::ReleaseBitmap)==2);
        assert(s.actions[0].address==0x0050aedc);
        assert(s.actions[1].address==0x0050ab88);
        assert(s.actions[2].kind==Op::ReleaseBitmap);
        assert(s.actions[3].address==0x0050adfc);
        assert(s.actions.back().kind==Op::StopMusic);
    }
    // 00424100: exactly five Binks and their buffers, 9 bitmap surfaces,
    // no StopActivityMusic, no ResetCursor in the actual source body.
    {
        ActivityResources s{};
        s.slot[0x00514b2c]=101;s.slot[0x00514e8c]=102;
        s.slot[0x00514620]=103;s.slot[0x004fc084]=104;
        s.audio_latch=1;
        unload_spud_skate(s);
        assert(s.skate_input_blocked==1&&s.audio_latch==0);
        assert(count(s,Op::CloseBink)==5);
        assert(count(s,Op::FreeMovieBuffer)==5);
        assert(count(s,Op::UnregisterBitmap)==10);
        assert(count(s,Op::DeleteSound)==1);
        assert(count(s,Op::StopMusic)==0);
        assert(s.slot[0x004fc084]==0&&s.slot[0x00514e8c]==0);
    }
    // 00424F00: sixteen original squirrel bitmaps, 20 sound slots and
    // final cursor-reset/music-stop pair.
    {
        ActivityResources s{};
        s.slot[0x005150c8]=200;s.slot[0x004fc0d0]=201;
        unload_squirrel(s);
        assert(count(s,Op::UnregisterBitmap)==16);
        assert(count(s,Op::ReleaseBitmap)==1);
        assert(count(s,Op::DeleteSound)==1);
        assert(s.actions[s.actions.size()-2].kind==Op::ResetCursor);
        assert(s.actions.back().kind==Op::StopMusic);
    }
    // 00420360: 5 contiguous, 10 named, variable 5x11 family surfaces.
    {
        ActivityResources s{};
        const std::array<I32,5> invalid{1,2,12,3,4};
        assert(!unload_spud_maze(s,invalid)&&s.actions.empty());
        const std::array<I32,5> groups{3,0,1,4,2};
        s.slot[0x005141ec]=100;
        s.slot[0x005141ec+44*2]=101;
        assert(unload_spud_maze(s,groups));
        assert(count(s,Op::UnregisterBitmap)==25);
        assert(count(s,Op::ReleaseBitmap)==2);
        assert(s.slot[0x005141ec]==0);
        assert(s.actions.back().kind==Op::StopMusic);
    }
    // 0040AC90 Save and 0040AEA0 Load: original 30,512 bytes
    // for 400 native 0x4C records + 28 trailing int32 values.
    {
        btb::park_designer::SaveData original{};
        for(auto& obj:original.objects)obj.object_code=-1;
        original.objects[301].object_code=406;
        original.objects[301].special_collision_origin_x=211;
        original.objects[301].special_collision_origin_y=345;
        original.state(btb::park_designer::TrailingStateIndex::Season)=1;
        original.state(btb::park_designer::TrailingStateIndex::NextDecorateRecord)=302;
        std::ostringstream output(std::ios::binary);
        assert(!save_park_designer(original,nullptr));
        assert(save_park_designer(original,&output));
        assert(output.str().size()==btb::park_designer::kRetailSaveBytes);
        std::istringstream polygons(fixture_bound_areas());
        std::istringstream binary(output.str(),std::ios::binary);
        btb::park_designer::SaveData roundtrip{};
        btb::park_designer::BoundAreas bounds{};
        int seasonal_callbacks=0;
        auto status=load_park_designer(polygons,&binary,roundtrip,bounds,
            [&](btb::park_designer::Season season) {
                ++seasonal_callbacks;
                return season==btb::park_designer::Season::Winter;
            });
        assert(status.polygons_loaded&&status.save_loaded);
        assert(status.applied_winter&&status.error.empty());
        assert(seasonal_callbacks==1);
        assert(roundtrip.objects[301].object_code==406);
        assert(roundtrip.objects[301].special_collision_origin_x==211);
        assert(roundtrip.objects[301].special_collision_origin_y==345);
        assert(roundtrip.state(btb::park_designer::TrailingStateIndex::NextDecorateRecord)==302);
        assert(bounds[0][0][0].vertices.size()==4);
        assert(bounds[2][3][4].vertices[1].x==20);

        // Missing save leaves the original initialized record values intact.
        btb::park_designer::SaveData blank=roundtrip;
        std::istringstream polygons2(fixture_bound_areas());
        auto no_save=load_park_designer(polygons2,nullptr,blank,bounds,{});
        assert(no_save.polygons_loaded&&!no_save.save_loaded);
        assert(blank.objects[301].object_code==406);

        // A truncated source polygon file cannot be accepted or guessed.
        std::istringstream wrong_polygons("0 0 -1 -1");
        auto bad=load_park_designer(wrong_polygons,nullptr,blank,bounds,{});
        assert(!bad.error.empty()&&!bad.polygons_loaded);
        std::istringstream polygons3(fixture_bound_areas());
        std::istringstream short_save("bad");
        bad=load_park_designer(polygons3,&short_save,blank,bounds,{});
        assert(bad.polygons_loaded&&!bad.save_loaded&&!bad.error.empty());
    }
    // 0040DBE0: two GetSurfaceDesc vtable calls, constant 6x124 bounds,
    // depth category from original table, and record_index repair.
    {
        btb::park_designer::SaveData data{};
        data.objects[7].object_code=27;
        data.objects[7].left=100;data.objects[7].top=200;
        std::array<I32,100> depth{};
        depth[0]=12;depth[27]=9;
        std::vector<U32> queried;
        auto provider=[&](U32 category){queried.push_back(category);return true;};
        assert(refresh_park_geometry(data,7,depth,provider));
        const auto& obj=data.objects[7];
        assert((queried==std::vector<U32>{27,27}));
        assert(obj.left==100&&obj.top==200);
        assert(obj.right==106&&obj.bottom==324);
        assert(obj.sprite_width==6&&obj.sprite_height==124);
        assert(obj.enabled_or_active==1&&obj.sort_layer==9&&obj.record_index==7);
        queried.clear();
        data.objects[9].object_code=401;
        assert(refresh_park_geometry(data,9,depth,provider));
        assert((queried==std::vector<U32>{0,0})&&data.objects[9].sort_layer==12);
        assert(!refresh_park_geometry(data,401,depth,provider));
        queried.clear();
        data.objects[8].object_code=-1;
        assert(!refresh_park_geometry(data,8,depth,provider));
        assert(queried.empty());
    }
    // 00412BD0: Fireworks palette events, latch behavior, genuine delete
    // modal, and selected occupied grid removal.
    {
        FireState state{};
        begin_fireworks_action(state,3,0,0,2,{});
        assert(state.cursor_type==3&&state.selected_type==3);
        assert(state.bob_actor_state==1);
        assert(count(state,FireAction::Sound)==1);
        assert((state.events.back()==FireEvent{FireAction::Sound,864,50,2}));
        begin_fireworks_action(state,5,0,0,3,{});
        assert(state.selected_type==3);
        assert(count(state,FireAction::Sound)==1);
        begin_fireworks_action(state,10,0,0,4,{});
        assert(state.wendy_actor_state==1&&state.selected_type==10);
        assert((state.events.back()==FireEvent{FireAction::Sound,903,50,2}));
        state.cells[2*6+3]=4;
        int removed=0;
        begin_fireworks_action(state,12,2,3,0,
            [&](I32 row,I32 col){++removed;assert(row==2&&col==3);});
        assert(removed==1&&count(state,FireAction::RemoveCell)==1);
        state.cells[2*6+3]=-1;
        begin_fireworks_action(state,12,2,3,0,
            [&](I32,I32){++removed;});
        assert(removed==1);
        begin_fireworks_action(state,25,0,0,0,{});
        assert(state.open_delete_all&&count(state,FireAction::DeleteAllConfirm)==1);
        begin_fireworks_action(state,27,0,0,0,{});
        assert(state.editor_state==14&&count(state,FireAction::StartShow)==1);
    }
    // 00412D50: completion uses Bob on rows 0/1, Wendy on row 2;
    // only invalid (-1 or >=12) grid entries admit a placement.
    {
        FireState state{};
        complete_fireworks_action(state,9,0,0,0,{});
        assert(state.selected_type==9&&count(state,FireAction::SelectPalette)==1);
        int placed=0;
        complete_fireworks_action(state,12,1,4,1,
            [&](I32 r,I32 c,I32 type){
                ++placed;assert(r==1&&c==4&&type==9);return true;
            });
        assert(state.bob_actor_state==10&&state.cursor_cleared);
        assert(state.actor_follow_bias==0);
        assert(count(state,FireAction::ClearPlacementTool)==1);
        assert((state.events[state.events.size()-3]==FireEvent{FireAction::Sound,868,50,2}));
        state.cursor_cleared=false;state.cells[1*6+4]=2;
        complete_fireworks_action(state,12,1,4,1,
            [&](I32,I32,I32){++placed;return true;});
        assert(placed==1&&!state.cursor_cleared);
        state.cells[2*6+5]=-1;
        complete_fireworks_action(state,12,2,5,3,
            [&](I32,I32,I32){++placed;return false;});
        assert(placed==2&&state.wendy_actor_state==10);
        assert((state.events[state.events.size()-2]==FireEvent{FireAction::Sound,907,50,2}));
        assert(count(state,FireAction::ClearPlacementTool)==1);
    }
    // 00424D10: Spud Skate's original completion/play-again state,
    // regular music startup and unusual EXIT CALL TO MAZE UNLOAD.
    {
        SkateLoop game{};
        game.cursor_y=399;game.startup_sound_pending=1;
        std::vector<Action> actions;
        assert(advance_spud_skate_activity(game,actions)==0);
        assert(game.ui_input_blocked==0&&game.startup_sound_pending==0);
        assert(actions.size()==5);
        assert(actions[0].kind==Op::EnableInput);
        assert(actions[1].kind==Op::UpdatePlayback);
        assert(actions[2].kind==Op::StopIntroVoice);
        assert(actions[3]==Action(Op::StartMusic,0,8));
        assert(actions[4]==Action(Op::ManagedSound,0,758));
        game.phase=2;actions.clear();
        assert(advance_spud_skate_activity(game,actions)==0);
        assert((actions.size()==3&&actions[1].kind==Op::PreparePlayAgain&&
                actions[2].kind==Op::UnloadSkate));
        assert(game.outer_state==60&&game.game_selection==22);
        assert(game.replay_class==26&&game.playback_latch==0);
        SkateLoop leave{};
        leave.global_shutdown=1;leave.shared_completion=-1;
        actions.clear();
        assert(advance_spud_skate_activity(leave,actions)==1);
        assert(leave.ui_input_blocked==1&&leave.global_shutdown==0);
        assert(leave.outer_state==22);
        assert(actions.size()==2&&actions[1].kind==Op::UnloadMaze);
        leave.global_leave_flag=1;leave.shared_completion=7;
        actions.clear();
        assert(advance_spud_skate_activity(leave,actions)==1);
        assert(leave.outer_state==64);
    }
}
