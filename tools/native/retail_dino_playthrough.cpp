// Read-only installed-data puzzle diagnostic. All placements use mouse pulses.
#include "btb/full_game_dino.hpp"
#include "btb/retail_crt_random.hpp"
#include "retail_activity_audit.hpp"
#include <memory>

namespace {
using namespace btb::retail_probe;
namespace dino=btb::dino;
void play(const fs::path& root,int species,int difficulty,unsigned seed) {
    Audit audit{fg::OriginalAssetResolver(root)};
    VoiceModel voices;
    const int profile=(seed+species+difficulty)%btb::progress::kPlayerCount;
    fg::GameRoot game;
    check(game.select_profile(profile),"profile rejected");
    game.globals().menu.selected_subgame=species;
    game.globals().menu.subgame_selection_origin=State::DinoChooserUpdate;
    game.globals().menu.source_variant=difficulty;
    game.globals().menu.variant_selection_origin=State::DinoPregameUpdate;
    btb::retail::OriginalRetailRandom rng(seed);
    auto driver=std::make_unique<fg::DinoDriver>(root,&rng);
    auto* live=driver.get();game.install(fg::ActivityId::Dinosaur,std::move(driver));
    game.set_outer_state(State::DinoInit);
    auto step=game.advance({});
    check(step.kind==fg::FrameKind::ActivityInitialized,"Dinosaur init failed: "+step.error);
    const auto count=live->runtime()->pieces().size();
    int accepted=0,rejected=0,certificates=0,printed=0,hovered=0;
    bool wrong_pending=true;
    for(int tick=0;tick<30000;++tick) {
        const auto* r=live->runtime();check(r!=nullptr,"Dinosaur unloaded early");
        fg::ActivityFrameInput input{};voices.sample(input);
        input.elapsed_centiseconds=tick*2;
        input.pointer_position_valid=true;
        if(live->shared_back_starts_replay()) {
            if(hovered==0){input.pointer_x=320;input.pointer_y=440;++hovered;}
            else if(printed==0){input.pointer_x=320;input.pointer_y=440;input.click_pulse=true;++printed;}
            else {input.pointer_x=590;input.pointer_y=440;input.click_pulse=true;}
        }else if(r->mode()==dino::InteractionMode::Idle&&!r->complete()) {
            // Inclusive rectangle selection follows original first-loose-slot scan.
            const auto& pieces=r->pieces();
            for(const auto& p:pieces)if(p.state==dino::PieceState::Loose){
                input.pointer_x=p.current.x+1;input.pointer_y=p.current.y+1;
                input.click_pulse=true;wrong_pending=true;break;
            }
        }else if(r->mode()==dino::InteractionMode::Carrying) {
            const auto& p=r->pieces().at(*r->selected_piece());
            input.pointer_x=wrong_pending?5:p.target.x;
            input.pointer_y=wrong_pending?5:p.target.y;
            input.click_pulse=true;
            if(wrong_pending)++rejected;else ++accepted;
            wrong_pending=false;
        }
        step=game.advance(input);
        check(step.kind==fg::FrameKind::ActivityUpdated,"Dinosaur advance failed: "+step.error);
        audit.observe(step.effects);voices.observe(step.effects);
        const auto slot=dino::progress_slot_for_species(dino::Species(species));
        if(!step.effects.progress_writes.empty()) {
            check(step.effects.progress_writes.size()==1&&step.effects.progress_writes[0].slot==slot&&step.effects.progress_writes[0].value==1,"incorrect Dinosaur progress");
            ++certificates;
        }
        if(printed==1&&input.pointer_x==320&&input.click_pulse)
            check(step.effects.request_original_frame_print,"certificate print request missing");
        if(step.effects.save_and_unload) {
            check(!live->runtime()&&accepted==int(count)&&rejected==accepted&&certificates==1&&printed==1,"Dinosaur completion mismatch");
            for(int p=0;p<int(btb::progress::kPlayerCount);++p)
                check(game.globals().player_progress[p].get(slot)==(p==profile?1:0),"Dinosaur progress isolation failed");
            check(game.globals().dispatcher.current_state==int(State::PlayAgainYesNoSetup)&&game.globals().dispatcher.saved_state==int(State::DinoInit)&&game.globals().menu.replay_class==1,"Dinosaur replay handoff failed");
            std::cout<<"dino species="<<species<<" difficulty="<<difficulty<<" seed="<<seed<<" placed="<<accepted<<" rejected="<<rejected<<" certificate="<<certificates<<" print="<<printed;
            audit.report();return;
        }
    }
    throw std::runtime_error("Dinosaur diagnostic exhausted frame budget");
}
}
int main(int argc,char** argv) {
    if(argc<2||argc>3){std::cerr<<"usage: btb_retail_dino_playthrough <installed-root> [seed-count]\n";return 2;}
    try {
        int seeds=argc==3?std::stoi(argv[2]):1;
        check(seeds>=1&&seeds<=100,"seed count outside 1..100");
        for(int seed=1;seed<=seeds;++seed)for(int species=0;species<3;++species)for(int difficulty=0;difficulty<3;++difficulty)
            play(argv[1],species,difficulty,seed);
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
