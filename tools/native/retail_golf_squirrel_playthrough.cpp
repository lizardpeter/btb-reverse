// Read-only installed-data playthroughs using normal activity input.
#include "btb/full_game_golf.hpp"
#include "btb/full_game_squirrel.hpp"
#include "retail_activity_audit.hpp"
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
using namespace btb::retail_probe;
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
    // Easy offers only correct pieces; Medium and Hard have decoys.
    bool try_decoy=difficulty!=0;
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
                if(choices[i].correct){++accepted;try_decoy=difficulty!=0;}
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
            else check(accepted==3*(difficulty+1)&&rejected==(difficulty==0?0:accepted),"Squirrel selection count mismatch");
            completion(game,step.effects,golf?Slot::Golf:Slot::SquirrelRun,init,profile);
            std::cout<<(golf?"golf":"squirrel")<<" difficulty="<<difficulty
                <<" windowed="<<windowed<<" seed="<<seed<<" accepted="<<accepted
                <<" rejected="<<rejected<<" score="<<score;
            audit.report();return;
        }
    }
    if(s){
        std::cerr<<"timeout: level="<<s->level()<<" placed="<<s->placed_count()
                 <<" phase="<<int(s->placement().state)<<" section="<<s->run_section()
                 <<" conveyor="<<s->conveyor_phase()<<" complete="<<s->complete()
                 <<" accepted="<<accepted<<" rejected="<<rejected<<" try_decoy="<<try_decoy<<"\n";
        for(const auto& c:s->choices())
            std::cerr<<c.piece_id<<" correct="<<c.correct<<" active="<<c.active<<"\n";
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
