// Read-only installed-data playthroughs using normal activity input.
#include "btb/full_game_golf.hpp"
#include "btb/full_game_squirrel.hpp"
#include "btb/original_asset_resolver.hpp"
#include "btb/full_game_blt_clip.hpp"
#include <array>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

namespace fg=btb::full_game;
namespace fs=std::filesystem;
using btb::game_flow::State;
using btb::progress::Slot;
namespace {
void check(bool ok,const std::string& message) {
    if(!ok)throw std::runtime_error(message);
}
std::uint32_t word(const std::array<unsigned char,26>& b,int off) {
    return std::uint32_t(b[off])|(std::uint32_t(b[off+1])<<8)|
           (std::uint32_t(b[off+2])<<16)|(std::uint32_t(b[off+3])<<24);
}
struct Audit {
    fg::OriginalAssetResolver resolver;
    std::map<std::string,std::pair<int,int>> bitmaps;
    std::map<std::string,fs::path> sounds;
    std::uint64_t frames{},draws{},visible{};
    fs::path resolve(const std::string& name) {
        const auto a=resolver.resolve(name);
        check(a.found(),name+": "+a.error);
        return a.absolute_path;
    }
    void observe(const fg::ActivityFrameOutput& out) {
        check(!out.fatal_error,out.fatal_error.value_or(""));
        for(const auto& d:out.draws) {
            auto it=bitmaps.find(d.source_asset);
            if(it==bitmaps.end()) {
                const auto path=resolve(d.source_asset);
                std::ifstream in(path,std::ios::binary);
                std::array<unsigned char,26> b{};
                in.read(reinterpret_cast<char*>(b.data()),b.size());
                check(bool(in)&&b[0]=='B'&&b[1]=='M'&&word(b,14)>=40,
                      "invalid BMP header: "+path.string());
                const int w=std::bit_cast<std::int32_t>(word(b,18));
                const int h=std::bit_cast<std::int32_t>(word(b,22));
                check(w>0&&h!=0&&h!=std::numeric_limits<int>::min(),"invalid BMP dimensions");
                it=bitmaps.emplace(d.source_asset,std::pair{w,h<0?-h:h}).first;
            }
            const auto [w,h]=it->second;
            const auto r=d.source_rectangle.value_or(fg::Rect{0,0,w,h});
            check(r.left>=0&&r.top>=0&&r.right>r.left&&r.bottom>r.top&&r.right<=w&&r.bottom<=h,
                "invalid DirectDraw source crop: "+d.source_asset+" rect="+
                std::to_string(r.left)+","+std::to_string(r.top)+","+
                std::to_string(r.right)+","+std::to_string(r.bottom)+" extent="+
                std::to_string(w)+"x"+std::to_string(h));
            const auto clipped=fg::retail_clip_blt_to_rect(r,d.x,d.y,
                d.destination_clip.value_or(fg::Rect{0,0,640,480}));
            if(clipped.visible) {
                const auto& s=clipped.source;
                check(s.left>=r.left&&s.top>=r.top&&s.right<=r.right&&s.bottom<=r.bottom,
                      "clipped source escaped original rectangle");
                check(s.right-s.left==clipped.destination.right-clipped.destination.left&&
                      s.bottom-s.top==clipped.destination.bottom-clipped.destination.top,
                      "clip changed sprite scale");
                ++visible;
            }
            ++draws;
        }
        for(const auto& a:out.audio)
            if(!a.source_asset.empty()&&!sounds.contains(a.source_asset))
                sounds.emplace(a.source_asset,resolve(a.source_asset));
        ++frames;
    }
    void report() const {
        std::cout<<" frames="<<frames<<" draws="<<draws<<" visible="<<visible
                 <<" bitmap_paths="<<bitmaps.size()<<" audio_paths="<<sounds.size()
                 <<" completion=1\n";
    }
};
struct VoiceModel {
    int wait{},id{-1};
    void sample(fg::ActivityFrameInput& input) {
        input.managed_sound_playing=wait>0;
        input.squirrel_lofty_voice_playing.emplace();
        input.squirrel_lofty_voice_playing->fill(false);
        if(wait>0&&id>=633&&id<=638)(*input.squirrel_lofty_voice_playing)[id-633]=true;
        if(wait>0)--wait;
    }
    void observe(const fg::ActivityFrameOutput& out) {
        for(const auto& a:out.audio) {
            if(a.operation==fg::AudioOperation::StopManagedSounds){wait=0;id=-1;}
            if(a.operation==fg::AudioOperation::ManagedSoundId){wait=8;id=a.sound_id;}
        }
    }
};
void completion(const fg::GameRoot& game,const fg::ActivityFrameOutput& out,
                Slot slot,State init,int profile) {
    check(out.save_and_unload&&out.progress_writes.size()==1&&
          out.progress_writes[0].slot==slot&&out.progress_writes[0].value==1,
          "invalid completion progress write");
    for(int i=0;i<int(btb::progress::kPlayerCount);++i)
        check(game.globals().player_progress[i].get(slot)==(i==profile?1:0),
              "progress isolation failed");
    check(game.globals().dispatcher.current_state==int(State::PlayAgainYesNoSetup)&&
          game.globals().dispatcher.saved_state==int(init)&&
          game.globals().menu.replay_class==1,"incorrect replay transition");
}
void play(const fs::path& root,bool golf,int difficulty,bool windowed,unsigned seed,int profile) {
    Audit audit{fg::OriginalAssetResolver(root)};
    VoiceModel voices;
    btb::retail::OriginalRetailRandom random(seed);
    fg::GameRoot game;
    check(game.select_profile(profile),"invalid profile");
    fg::GolfDriver* g=nullptr;
    fg::SquirrelDriver* s=nullptr;
    const auto init=golf?State::GolfInit:State::SquirrelInit;
    if(golf){
        auto d=std::make_unique<fg::GolfDriver>(
            audit.resolve("Data/SubGameGolf/golfdata.txt").parent_path(),difficulty,&random);
        g=d.get();game.install(fg::ActivityId::Golf,std::move(d));
    }else{
        auto d=std::make_unique<fg::SquirrelDriver>(root,difficulty,&random);
        s=d.get();game.install(fg::ActivityId::Squirrel,std::move(d));
    }
    game.set_outer_state(init);
    fg::ActivityFrameInput input{};
    input.original_windowed_display=windowed;
    auto step=game.advance(input);
    check(step.kind==fg::FrameKind::ActivityInitialized,"init failed: "+step.error);
    int accepted=0,rejected=0,charged=0,score=0;
    bool try_decoy=true;
    constexpr std::array<int,5> aims{0,34,60,44,88},powers{400,700,850,990,50};
    for(int tick=0;tick<40000;++tick) {
        input.click_pulse=false;input.elapsed_centiseconds=tick;voices.sample(input);
        if(g) {
            check(g->round()!=nullptr,"Golf unloaded early");
            const auto& round=*g->round();const auto f=round.snapshot();
            if(!round.finished()&&f.state==btb::golf::RoundState::Aim){
                check(accepted<5,"unexpected sixth shot");
                input.pointer_x=(88-aims[accepted])*7;input.click_pulse=true;charged=0;
            }else if(f.state==btb::golf::RoundState::PowerMeter){
                check(accepted<5,"invalid power meter attempt");
                if(f.power>=powers[accepted]||charged++>=1100){
                    input.click_pulse=true;++accepted;
                }
            }
        }else if(!s->complete()&&!s->level_transition_pending()&&s->shared_back_available()&&
                 s->conveyor_phase()==0&&s->placed_count()<3&&s->run_section()<6){
            const auto& choices=s->choices();
            for(std::size_t i=0;i<choices.size();++i){
                if(!choices[i].active||choices[i].correct==try_decoy)continue;
                const auto p=btb::squirrel::kConveyorItemOrigins[i];
                input.pointer_x=p.x+10;input.pointer_y=p.y+10;input.click_pulse=true;
                if(choices[i].correct){++accepted;try_decoy=true;}
                else{++rejected;try_decoy=false;}
                break;
            }
        }
        step=game.advance(input);
        check(step.kind==fg::FrameKind::ActivityUpdated,"advance failed: "+step.error);
        audit.observe(step.effects);voices.observe(step.effects);
        if(g&&g->round())score=g->round()->score();
        if(step.effects.save_and_unload){
            if(g)check(accepted==5&&g->round()==nullptr,"Golf attempt/unload count mismatch");
            else check(accepted==3*(difficulty+1)&&rejected==accepted,"Squirrel selection count mismatch");
            completion(game,step.effects,golf?Slot::Golf:Slot::SquirrelRun,init,profile);
            std::cout<<(golf?"golf":"squirrel")<<" difficulty="<<difficulty
                <<" windowed="<<windowed<<" seed="<<seed<<" accepted="<<accepted
                <<" rejected="<<rejected<<" score="<<score;
            audit.report();return;
        }
    }
    throw std::runtime_error("playthrough exhausted frame budget");
}
}
int main(int argc,char** argv){
    if(argc<2||argc>4){
        std::cerr<<"usage: btb_retail_golf_squirrel_playthrough <installed-root> [both|golf|squirrel] [seed-count]\n";
        return 2;
    }
    try{
        const std::string activity=argc>=3?argv[2]:"both";
        const int seeds=argc>=4?std::stoi(argv[3]):1;
        check(activity=="both"||activity=="golf"||activity=="squirrel","unknown activity");
        check(seeds>=1&&seeds<=100,"seed count outside 1..100");
        for(int difficulty=0;difficulty<3;++difficulty)for(bool windowed:{false,true})
            for(int seed=1;seed<=seeds;++seed){
                const int profile=(seed+difficulty+int(windowed))%5;
                if(activity!="squirrel")play(argv[1],true,difficulty,windowed,seed,profile);
                if(activity!="golf")play(argv[1],false,difficulty,windowed,seed,profile);
            }
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
