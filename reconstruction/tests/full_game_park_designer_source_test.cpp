#include "btb/full_game_park_designer.hpp"

#include <array>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace fs=std::filesystem;
using namespace btb::full_game;
using btb::park_designer::SaveData;
namespace {
struct FakeNativePark final : ParkDesignerNativeService {
    int draws{},editors{},music{},voices{};
    bool fail_draw{},fail_sound{};
    bool emit_confirm{};
    bool render(const SaveData& data,const btb::park_designer::BoundAreas& bounds,
                ActivityFrameOutput& output,std::string& error) override {
        ++draws;
        if(fail_draw){error="no original Park bitmap surface";return false;}
        assert(bounds[0][0][0].vertices.size()==4);
        (void)data;
        output.draws.push_back({"fixture:original-DYP-backdrop",0,0});
        return true;
    }
    bool update_editor(SaveData& data,const btb::park_designer::BoundAreas&,
                       const ActivityFrameInput& input,
                       ActivityFrameOutput& output,std::string&) override {
        ++editors;
        if(input.click_pulse&&input.pointer_x==100) {
            data.objects[2].object_code=7;
            data.objects[2].record_index=2;
        }
        if(input.click_pulse&&input.pointer_x==200) {
            output.open_yes_no_confirmation=true;
            output.yes_no_context=3;
        }
        return true;
    }
    bool emit_sound(const btb::park_designer::LoopSound& sound,
                    ActivityFrameOutput& output,std::string& error) override {
        if(fail_sound){error="native audio bank unavailable";return false;}
        if(sound.kind==btb::park_designer::LoopSoundKind::StartParkBackingTrack) {
            assert(sound.id==2);++music;
            output.audio.push_back({AudioOperation::StartBackingTrack,"fixture:DYP-track-2"});
        } else if(sound.kind==btb::park_designer::LoopSoundKind::ManagedVoice||
                 sound.kind==btb::park_designer::LoopSoundKind::InputInterruptibleVoice) {
            ++voices;
            output.audio.push_back({AudioOperation::ManagedSoundId,{},sound.id,50,1});
        } else {
            output.audio.push_back({AudioOperation::StopBackingTrack});
        }
        return true;
    }
};

void polygon_file(const fs::path& file) {
    std::ofstream out(file);
    assert(out);
    for(std::size_t i=0;i<60;++i) {
        out<<"0 0 40 0 40 40 0 40 -1 -1\n";
    }
}
} // namespace

int main() {
    const auto rootpath=fs::temp_directory_path()/"btb_park_driver_00410d10";
    fs::remove_all(rootpath);
    const auto source=rootpath/"Data"/"SubGameDYP";
    const auto saves=rootpath/"profiles";
    fs::create_directories(source);
    polygon_file(source/"boundareas.txt");

    GameRoot game;
    assert(game.select_profile(0));
    auto backend=std::make_shared<FakeNativePark>();
    auto driver=std::make_unique<ParkDesignerDriver>(source,saves,backend);
    auto* live=driver.get();
    game.install(ActivityId::ParkDesigner,std::move(driver));
    game.set_outer_state(btb::game_flow::State::ParkDesignerInit);

    auto f=game.advance({});
    assert(f.kind==FrameKind::ActivityInitialized&&live->initialized());
    assert(backend->draws==0);
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityUpdated&&backend->draws==1&&backend->editors==1);
    assert(live->save().state(btb::park_designer::TrailingStateIndex::NextDecorateRecord)==300);
    assert(live->save().objects[0].object_code==-1);

    ActivityFrameInput click{};
    click.click_pulse=true;click.pointer_x=100;
    f=game.advance(click);
    assert(f.kind==FrameKind::ActivityUpdated);
    assert(live->save().objects[0].object_code==7);
    assert(live->save().objects[0].record_index==0); // compact after editor
    assert(f.effects.progress_writes.empty());

    live->queue_original_voice(275,99);
    ActivityFrameInput speaking{};
    speaking.managed_sound_playing=true;
    f=game.advance(speaking);
    assert(f.kind==FrameKind::ActivityUpdated&&f.effects.audio.empty());
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityUpdated&&
           f.effects.audio.size()==1&&f.effects.audio[0].sound_id==275);
    live->start_original_music_intro();
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityUpdated);
    assert(backend->music==1&&backend->voices==2);
    assert(f.effects.audio.size()==3);

    // Park Designer View-mode Delete All uses shared modal confirmation.
    ActivityFrameInput confirm_click{};
    confirm_click.click_pulse=true;confirm_click.pointer_x=200;
    f=game.advance(confirm_click);
    // Intro voice resumes editor only after managed samples finish:
    f=game.advance({});
    f=game.advance(confirm_click);
    assert(f.kind==FrameKind::ActivityUpdated);
    assert(f.effects.open_yes_no_confirmation&&f.effects.yes_no_context==3);
    ActivityFrameInput decline{};
    decline.confirmation_resolved=true;
    f=game.advance(decline);
    assert(f.kind==FrameKind::ActivityUpdated);
    assert(live->save().objects[0].object_code==7);
    f=game.advance(confirm_click);
    assert(f.effects.open_yes_no_confirmation);
    ActivityFrameInput accept{};
    accept.confirmation_resolved=true;accept.confirmation_yes=true;
    f=game.advance(accept);
    assert(f.kind==FrameKind::ActivityUpdated);
    assert(live->save().objects[0].object_code==-1);

    // Save/unload at the actual 3-state 00410D10 exit, not a made-up
    // immediate switch in a partially supported in-game click handler.
    f=game.advance(click);assert(live->save().objects[0].object_code==7);
    live->request_exit(true);
    ActivityFrameInput roll{};roll.random_value=1;
    f=game.advance(roll);
    assert(f.kind==FrameKind::ActivityUpdated);
    assert(!f.effects.audio.empty()&&f.effects.audio[0].sound_id==247);
    f=game.advance(speaking);
    assert(f.kind==FrameKind::ActivityUpdated&&live->initialized());
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityUpdated&&live->initialized());
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityUpdated&&!live->initialized());
    assert(f.effects.save_and_unload&&f.state_changed);
    assert(f.effects.next_outer_state==
        static_cast<int>(btb::game_flow::State::ActivitySelectSetup));
    assert(game.globals().player_progress[0].get(
        btb::progress::Slot::ParkDesigner)==1);

    const auto file=saves/"dypdata1.txt";
    assert(fs::file_size(file)==btb::park_designer::kRetailSaveBytes);
    std::ifstream persisted(file,std::ios::binary);
    const auto saved=btb::park_designer::read_save(persisted);
    assert(saved.objects[0].object_code==7);

    // Re-entering preserves the original binary state rather than a
    // synthetic new profile and never prompts a Delete All reset.
    game.set_outer_state(btb::game_flow::State::ParkDesignerInit);
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityInitialized);
    assert(live->save().objects[0].object_code==7);

    // No native scene provider means a failed activity frame, not a
    // successful empty render or fabricated terrain/surfaces.
    std::string error;
    assert(game.unload_current(error));
    backend->fail_draw=true;
    game.set_outer_state(btb::game_flow::State::ParkDesignerInit);
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityInitialized);
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityFailed);
    assert(f.error=="no original Park bitmap surface");
    assert(f.effects.draws.empty());
    assert(game.unload_current(error));

    // Existing but corrupt retail save must be rejected at init,
    // including trailing data; never silently overwritten.
    {
        std::ofstream wrong(file,std::ios::binary|std::ios::trunc);
        wrong<<"malformed";
    }
    game.set_outer_state(btb::game_flow::State::ParkDesignerInit);
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityFailed&&!live->initialized());
    assert(f.error.find("30512")!=std::string::npos);

    // The binary save cannot substitute for the source authored polygon data.
    fs::remove(source/"boundareas.txt");
    f=game.advance({});
    assert(f.kind==FrameKind::ActivityFailed);
    assert(f.error.find("boundareas")!=std::string::npos);
    fs::remove_all(rootpath);
}
