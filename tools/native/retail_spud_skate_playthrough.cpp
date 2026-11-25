// Real installed Bink streams through the same native bank as the Windows host.
#include "btb/native_spud_skate_movie_bank.hpp"
#include "retail_activity_audit.hpp"
#include <memory>
namespace {
using namespace btb::retail_probe;
namespace skate=btb::spud_skate;
void play(const fs::path& root,int difficulty,int quality,bool maze_completed) {
    Audit audit{fg::OriginalAssetResolver(root)};
    VoiceModel voices;
    const int profile=(difficulty+quality)%btb::progress::kPlayerCount;
    fg::GameRoot game;
    check(game.select_profile(profile),"profile rejected");
    game.globals().player_progress[(profile+1)%btb::progress::kPlayerCount].set(Slot::SpudMaze,1);
    if(maze_completed)game.globals().player_progress[profile].set(Slot::SpudMaze,1);
    game.globals().menu.source_variant=difficulty;
    game.globals().menu.variant_selection_origin=State::SpudSkatePregameUpdate;
    std::ifstream file(audit.resolve("Data/SubGameSpudSkate/spuddata"+std::to_string(difficulty+1)+".txt"));
    const auto timing=skate::parse_timing(file);
    btb::retail::OriginalRetailRandom rng(17+difficulty*4+quality);
    std::array<unsigned,5> uploads{};
    auto bank=std::make_unique<fg::NativeSpudSkateMovieBank>(fg::NativeSpudSkateMovieBank::Pace{},
        [&](std::size_t stream,const auto& frame,std::string& error){
            if(frame.width!=600||frame.height!=380||frame.planes[0].size()!=600*380||
               frame.planes[1].size()!=300*190||frame.planes[2].size()!=300*190||frame.planes[3].size()!=600*380) {
                error="invalid decoded frame dimensions";return false;
            }
            ++uploads.at(stream);return true;
        });
    auto* movies=bank.get();
    auto driver=std::make_unique<fg::SpudSkateDriver>(root,std::move(bank),&rng,
        [&](int p){return game.globals().player_progress[p].get(Slot::SpudMaze)==1;});
    auto* live=driver.get();game.install(fg::ActivityId::SpudSkate,std::move(driver));
    game.set_outer_state(State::SpudSkateInit);
    auto step=game.advance({});
    check(step.kind==fg::FrameKind::ActivityInitialized,"Spud Skate init failed: "+step.error);
    unsigned rewinds=0,end_transitions=0,awards=0,presses=0;
    int score=0;
    for(int tick=0;tick<3000;++tick) {
        fg::ActivityFrameInput input{};voices.sample(input);
        const auto prior_phase=live->phase();
        const auto prior_score=live->score();
        const int next=int(movies->decoded_frame(0))+1;
        if(prior_phase==skate::PlaybackPhase::SynchronizedRun&&quality!=0) {
            for(const auto& w:timing.windows) {
                const int frame=quality==1?w.normal_start:quality==2?w.okay_start:w.good_start;
                if(next==frame) {
                    if(presses++%2==0)input.click_pulse=true;
                    else input.keyboard_state[0x39]=0x80;
                }
            }
        }
        step=game.advance(input);
        check(step.kind==fg::FrameKind::ActivityUpdated,"Spud Skate advance failed: "+step.error);
        audit.observe(step.effects);voices.observe(step.effects);
        const auto digits=skate::score_render_plan(prior_score);
        std::size_t digit_index=0;
        for(const auto& d:step.effects.draws)if(d.source_asset=="Data/SubGameSpudSkate/skatespudscore.bmp"){
            check(digit_index<digits.count&&d.source_rectangle&&d.source_rectangle->left==digits.digits[digit_index].source_left,"incorrect pre-update score display");++digit_index;
        }
        check(digit_index==digits.count,"missing score digits");
        if(prior_phase==skate::PlaybackPhase::SynchronizedRun&&next==679) {
            ++rewinds;
            check(movies->displayed_stream()==3,"original resync must present Good frame immediately");
            for(int i=0;i<4;++i)check(movies->decoded_frame(i)==44,"resync did not decode frame 44");
        }
        if(prior_phase==skate::PlaybackPhase::SynchronizedRun&&live->phase()==skate::PlaybackPhase::EndMovie) {
            ++end_transitions;
            check(next==660&&movies->displayed_stream()==4&&movies->decoded_frame(4)==1,"second pass did not immediately present end.bik frame 1");
        }
        if(!step.effects.progress_writes.empty()) {
            ++awards;score=live->score();
            check(step.effects.progress_writes.size()==1&&step.effects.progress_writes[0].slot==Slot::SpudSkate&&step.effects.progress_writes[0].value==1,"incorrect Skate progress");
            check(step.effects.shared_completion_code.has_value()==maze_completed,"wrong selected-profile prerequisite");
            if(maze_completed)check(step.effects.shared_completion_code==2,"wrong shared movie code");
        }
        if(step.effects.save_and_unload) {
            check(!live->initialized()&&rewinds==1&&end_transitions==1&&awards==1,"Skate completion lifecycle mismatch");
            check(score==16*quality,"score differs from 16 original opportunities: "+std::to_string(score));
            check(game.globals().player_progress[profile].get(Slot::SpudSkate)==1,"Skate badge not stored");
            for(int p=0;p<int(btb::progress::kPlayerCount);++p)if(p!=profile)
                check(game.globals().player_progress[p].get(Slot::SpudSkate)==0,"Skate badge leaked to another profile");
            check(game.globals().dispatcher.current_state==int(State::PlayAgainYesNoSetup)&&game.globals().dispatcher.saved_state==int(State::SpudSkateInit)&&game.globals().menu.replay_class==0,"Skate replay handoff failed");
            check(uploads[4]==44,"original end movie should stop at displayed frame 44");
            std::cout<<"skate difficulty="<<difficulty<<" quality="<<quality<<" maze="<<maze_completed<<" score="<<score<<" presses="<<presses<<" uploads=";
            unsigned total=0;for(auto n:uploads)total+=n;
            std::cout<<total<<" rewind="<<rewinds<<" end="<<end_transitions;
            audit.report();return;
        }
    }
    throw std::runtime_error("Spud Skate diagnostic exhausted frame budget");
}
}
int main(int argc,char** argv){
    if(argc!=2){std::cerr<<"usage: btb_retail_spud_skate_playthrough <installed-root>\n";return 2;}
    try{
        for(int difficulty=0;difficulty<3;++difficulty)for(int quality=0;quality<4;++quality)
            play(argv[1],difficulty,quality,(difficulty+quality)%2!=0);
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
