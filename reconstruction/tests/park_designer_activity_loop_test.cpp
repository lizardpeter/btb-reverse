#include "btb/park_designer_activity_loop.hpp"
#include <cassert>
#include <string>
#include <vector>
using namespace btb::park_designer;
namespace {
std::vector<char> calls;
bool draw(const SaveData&,std::string&){calls.push_back('D');return true;}
bool edit(SaveData& d,std::string&){calls.push_back('E');d.objects[1].object_code=7;return true;}
}
int main(){
    LoopState game{};
    for(auto& o:game.save.objects)o.object_code=42;
    game.save.state(TrailingStateIndex::PondLinkedRecord4)=99;
    game.save.state(TrailingStateIndex::NextDecorateRecord)=360;
    game.save.state(TrailingStateIndex::NextBandstandRecord)=260;
    game.save.state(TrailingStateIndex::PondPalettePage)=1;
    game.reset_all_pending=true;
    calls.clear();
    auto f=advance_park_designer_loop(game,{},draw,edit);
    assert(!f.error&&f.performed_reset&&f.requested_draw&&f.requested_editor);
    assert((calls==std::vector<char>{'D','E'}));
    assert(game.save.state(TrailingStateIndex::PondLinkedRecord4)==0);
    assert(game.save.state(TrailingStateIndex::PondLinkedRecord0)==-1);
    assert(game.save.state(TrailingStateIndex::NextDecorateRecord)==300);
    assert(game.save.state(TrailingStateIndex::NextBandstandRecord)==200);
    assert(game.save.state(TrailingStateIndex::NextPondPrimaryRecord)==1);
    assert(game.save.state(TrailingStateIndex::PondPalettePage)==0);
    assert(game.save.objects[0].object_code==-1);
    assert(game.save.objects[1].object_code==-1);
    // Original editor mutation happens before family compaction.
    assert(game.save.objects[0].object_code==7);
    assert(game.save.objects[0].record_index==0);

    // Normal mode includes draw, edit, compaction, delayed voice ONLY
    // if all managed sounds stopped. Exactly source call order.
    game.queued_voice=275;game.queued_voice_context=99;
    calls.clear();
    LoopInput busy{};busy.managed_sound_playing=true;
    f=advance_park_designer_loop(game,busy,draw,edit);
    assert(!f.error&&f.sound.empty()&&game.queued_voice==275);
    assert((calls==std::vector<char>{'D','E'}));
    calls.clear();
    f=advance_park_designer_loop(game,{},draw,edit);
    assert(f.sound.size()==1&&f.sound[0].id==275);
    assert(f.sound[0].context==99&&game.queued_voice==-1);

    // Intro/music interruption: draw, stop intro, play indexed track 2,
    // first-frame special managed voice 0x123, NO editor.
    game.intro_music_pending=true;
    game.intro_managed_voice_pending=true;
    calls.clear();
    f=advance_park_designer_loop(game,{},draw,edit);
    assert(!f.error&&f.requested_draw&&!f.requested_editor);
    assert((calls==std::vector<char>{'D'}));
    assert(f.sound.size()==3);
    assert(f.sound[0].kind==LoopSoundKind::StopIntroSound);
    assert(f.sound[1].kind==LoopSoundKind::StartParkBackingTrack);
    assert(f.sound[1].id==2);
    assert(f.sound[2].kind==LoopSoundKind::ManagedVoice&&f.sound[2].id==291);
    assert(!game.intro_managed_voice_pending&&game.intro_music_pending);
    f=advance_park_designer_loop(game,busy,draw,edit);
    assert(game.intro_music_pending);
    f=advance_park_designer_loop(game,{},draw,edit);
    assert(!game.intro_music_pending);
    calls.clear();
    f=advance_park_designer_loop(game,{},draw,edit);
    assert(f.requested_editor&&(calls==std::vector<char>{'D','E'}));

    // Exit takes exactly three source stages: voice 246..248, wait, unload.
    game.exit_requested=true;
    LoopInput roll{};roll.random_value=5;
    calls.clear();
    f=advance_park_designer_loop(game,roll,draw,edit);
    assert(game.outro_stage==1&&f.sound.size()==1&&f.sound[0].id==248);
    assert(f.requested_draw&&!f.requested_editor&&!f.completed);
    assert((calls==std::vector<char>{'D'}));
    calls.clear();
    f=advance_park_designer_loop(game,busy,draw,edit);
    assert(game.outro_stage==1&&!f.completed);
    f=advance_park_designer_loop(game,{},draw,edit);
    assert(game.outro_stage==2&&!f.completed);
    f=advance_park_designer_loop(game,{},draw,edit);
    assert(f.completed&&f.performed_unload&&f.mark_park_progress);
    assert(!f.return_to_activity_select);
    assert(game.outer_state==0);

    // Independent shutdown flag has priority over editor, and sets state 4.
    LoopState exit{};
    exit.shutdown_requested=true;exit.outro_stage=2;
    f=advance_park_designer_loop(exit,{},draw,edit);
    assert(f.completed&&!f.mark_park_progress&&f.return_to_activity_select);
    assert(exit.outer_state==4&&!exit.shutdown_requested);

    // Missing original draw/editor service must not silently claim success.
    LoopState missing{};
    f=advance_park_designer_loop(missing,{},SceneDraw{},edit);
    assert(f.error&&f.error->find("DrawParkDesignerActivity")!=std::string::npos);
    assert(!f.requested_editor);
    f=advance_park_designer_loop(missing,{},draw,EditorUpdate{});
    assert(f.error&&f.error->find("UpdateParkDesignerEditorInteraction")!=std::string::npos);
    LoopState broken{};
    f=advance_park_designer_loop(broken,{},draw,
        [](SaveData&,std::string& err){err="original cursor bitmap unavailable";return false;});
    assert(f.error&&*f.error=="original cursor bitmap unavailable");
}
