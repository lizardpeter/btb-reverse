// Installed machine data, actual bitmap bounds, sample files and profile saves.
#include "btb/full_game_bobs_band.hpp"
#include "retail_activity_audit.hpp"
#include <memory>
#include <set>
namespace {
using namespace btb::retail_probe;
namespace band=btb::bobs_band;
struct Run {
    Audit audit;
    VoiceModel voices;
    std::unique_ptr<fg::GameRoot> game=std::make_unique<fg::GameRoot>();
    btb::retail::OriginalRetailRandom rng{17};
    fg::BobsBandDriver* live{};
    int time=40000;
    Run(const fs::path& root,const fs::path& save,int conductor,int profile):audit{fg::OriginalAssetResolver(root)} {
        check(game->select_profile(profile),"profile rejected");
        auto driver=std::make_unique<fg::BobsBandDriver>(audit.resolve("Data/SubGameOpen/machinedata.txt").parent_path(),save,band::Conductor(conductor),&rng);
        live=driver.get();game->install(fg::ActivityId::BobsBand,std::move(driver));
        game->set_outer_state(State::BobsBandInit);
        const auto step=game->advance({});
        check(step.kind==fg::FrameKind::ActivityInitialized,"Band init: "+step.error);
    }
    fg::ActivityFrameOutput update(int x=0,int y=0,bool click=false,bool backing=true) {
        fg::ActivityFrameInput input{};voices.sample(input);
        input.elapsed_centiseconds=time;time+=2;
        input.pointer_x=x;input.pointer_y=y;input.pointer_position_valid=true;
        input.click_pulse=click;input.backing_track_playing=backing;
        auto step=game->advance(input);
        check(step.kind==fg::FrameKind::ActivityUpdated,"Band update: "+step.error);
        audit.observe(step.effects);voices.observe(step.effects);return step.effects;
    }
};
void play(const fs::path& root,const fs::path& save,int conductor,int profile) {
    Run run(root,save,conductor,profile);
    const auto intro=run.update();
    check(!intro.audio.empty()&&intro.audio.back().sound_id==band::kVoiceBase[conductor]&&intro.audio.back().input_interruptible,"missing conductor opening voice");
    std::array<std::int32_t,120> expected{};expected.fill(-1);
    for(int row=0;row<5;++row)for(int type=0;type<10;++type){
        const auto box=band::kPaletteHitboxes[type];
        run.update((box.left+box.right)/2,(box.top+box.bottom)/2,true);
        const int second=type*2;
        run.update(band::kGridX+second*band::kCellWidth+1,band::kGridY+row*band::kCellHeight+1,true);
        expected[band::cell_index(row,second)]=type;
        if(type%2)expected[band::cell_index(row,second+1)]=band::kContinuation;
    }
    check(run.live->activity()->editor().composition().cells()==expected,"composition placement mismatch");
    run.update(350,441,true);
    int awards=0,starts=0;
    for(int i=0;i<40&&!starts;++i){const auto out=run.update();awards+=out.progress_writes.size();for(const auto& a:out.audio)starts+=a.operation==fg::AudioOperation::StartBackingTrack;}
    check(starts==1&&awards==1,"backing/progress gate failed");
    std::set<std::string> samples;
    unsigned count=0;
    for(int i=0;i<1210;++i){const auto out=run.update();for(const auto& a:out.audio)if(a.operation==fg::AudioOperation::PlaySampleFile){++count;samples.insert(a.source_asset);}}
    check(count==50&&samples.size()==50,"incorrect song sample count: "+std::to_string(count));
    run.update(0,0,false,false);
    check(run.live->activity()->editor().state()==band::InternalState::EditIdle,"backing EOS did not restore editor");
    const auto slot=static_cast<Slot>(52+conductor);
    check(run.game->globals().player_progress[profile].get(slot)==1,"profile badge missing");
    for(int p=0;p<int(btb::progress::kPlayerCount);++p)if(p!=profile)check(run.game->globals().player_progress[p].get(slot)==0,"profile badge leaked");
    check(run.update(590,440,true).open_leave_activity_confirmation,"Back did not open leave modal");
    fg::ActivityFrameInput yes{};yes.pointer_x=260;yes.pointer_y=280;yes.pointer_position_valid=true;yes.pointer_release_pulse=true;
    (void)run.game->advance(yes);
    auto exit=run.game->advance({});
    check(exit.effects.save_and_unload&&!run.live->activity(),"confirmed leave did not persist/unload");
    const auto file=save/band::sequence_filename(band::Conductor(conductor),profile);
    check(fs::file_size(file)==480&&fs::file_size(save/"last.txt")==20,"incorrect saved byte count");
    Run reload(root,save,conductor,profile);
    check(reload.live->activity()->editor().composition().cells()==expected,"saved sequence did not reload");
    std::string error;check(reload.game->unload_current(error),"reload unload: "+error);
    std::cout<<"band conductor="<<conductor<<" profile="<<profile<<" notes="<<count<<" persisted=1";run.audit.report();
}
}
int main(int argc,char** argv){
    if(argc!=3){std::cerr<<"usage: btb_retail_bobs_band_playthrough <installed-root> <new-temporary-save-directory>\n";return 2;}
    try{
        const fs::path save=argv[2];check(!fs::exists(save),"save directory must not exist");
        for(int conductor=0;conductor<3;++conductor)for(int profile=0;profile<5;++profile)play(argv[1],save,conductor,profile);
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
