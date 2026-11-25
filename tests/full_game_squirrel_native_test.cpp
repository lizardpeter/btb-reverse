#include "btb/full_game_squirrel.hpp"
#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"native Squirrel Run: %s at %s:%d\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)
#include <filesystem>
#include <fstream>
#include <memory>

int main() {
    namespace fs=std::filesystem;
    namespace fg=btb::full_game;
    namespace sq=btb::squirrel;
    static_assert(sq::kSourceSquirrelFrameWidth==102);
    static_assert(sq::kSourceSquirrelFrameHeight==92);
    static_assert(sq::kSourceSquirrelFrameCount==86);
    static_assert(sq::source_squirrel_frame_rect(85)->right==8772);
    static_assert(!sq::source_squirrel_frame_rect(86));
    static_assert(sq::source_squirrel_start_position(0)->y==234);
    static_assert(sq::source_squirrel_start_position(1)->y==205);
    static_assert(sq::source_squirrel_start_position(2)->y==172);
    using btb::game_flow::State;
    const auto root=fs::temp_directory_path()/"btb_native_squirrel_fixture";
    fs::remove_all(root);
    fs::create_directories(root/"Data"/"SubGameSquirrel");
    {
        std::ofstream f(root/"Data"/"SubGameSquirrel"/"sqdata.txt");
        f<<"0 3 10 27 8 -4 -4\n";
        f<<"0 0 1 -1 0 0 0\n"
         <<"0 0 0 0 0 0 0\n"
         <<"0 0 1 -17 0 0 0\n"
         <<"0 0 1 -33 0 0 0\n"
         <<"0 0 -1 17 0 0 0\n"
         <<"0 0 19 13 0 0 0\n"
         <<"0 0 0 -32 0 0 0\n"
         <<"0 0 0 32 0 0 0\n";
        f<<"7\n105 79\n1 2 2 3\n70\n";
        f<<"110 0 234 146 384 177 362 203 172 0\n";
    }
    // The retail actor and squirrel run advance independently. An early
    // selection in the initial approach must not be dropped merely
    // because 00425F40 is still animating.
    {
        btb::retail::OriginalRetailRandom early_rng{7};
        fg::SquirrelDriver early(root,0,&early_rng);
        std::string error;
        assert(early.initialize(0,error));
        assert(early.animating()&&early.run_section()==0);
        assert(early.choices()[0].active&&early.choices()[0].correct);
        const auto origin=sq::kConveyorItemOrigins[0];
        auto frame=early.advance({.pointer_x=origin.x+10,
            .pointer_y=origin.y+10,.click_pulse=true});
        assert(!frame.fatal_error);
        assert(early.animating()&&early.run_section()==0);
        assert(early.placement().state!=sq::PlacementState::IdleSelect);
        assert(early.placement().selected==0&&early.placed_count()==0);
        int ticks=0;
        while(early.placed_count()==0&&ticks++<1000) {
            frame=early.advance({});
            assert(!frame.fatal_error);
        }
        assert(early.placed_count()==1&&early.conveyor_phase()==1);
        assert(early.unload(error));
    }
    btb::retail::OriginalRetailRandom rng{7};
    for(int difficulty=0;difficulty<3;++difficulty) {
        fg::GameRoot game;
        assert(game.select_profile(2));
        const auto before_rng=rng.state();
        auto activity=std::make_unique<fg::SquirrelDriver>(root,difficulty,&rng);
        auto* driver=activity.get();
        game.install(fg::ActivityId::Squirrel,std::move(activity));
        game.set_outer_state(State::SquirrelInit);
        auto frame=game.advance({});
        assert(frame.kind==fg::FrameKind::ActivityInitialized);
        assert(rng.state()!=before_rng);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::SquirrelRun));
        assert(driver->level()==0);
        auto rendered=game.advance({});
        assert(rendered.kind==fg::FrameKind::ActivityUpdated);
        assert(rendered.effects.audio.size()==2);
        assert(rendered.effects.audio[0].source_asset=="Data/music/squirrelrun.wav");
        assert(rendered.effects.audio[1].sound_id==643 && rendered.effects.audio[1].input_interruptible);
        auto waiting=game.advance({.managed_sound_playing=true});
        assert(waiting.effects.audio.empty());
        auto intro2=game.advance({});
        assert(intro2.effects.audio.size()==1 && intro2.effects.audio[0].sound_id==632);
        auto intro3=game.advance({});
        assert(intro3.effects.audio.size()==1 && intro3.effects.audio[0].sound_id==644);
        assert(rendered.effects.draws[0].source_asset==
            "Data\\SubGameSquirrel\\level"+std::to_string(difficulty+1)+".bmp");
        assert(rendered.effects.draws[0].source_rectangle->right==620);
        int connector_art=0,nut_piles=0,conveyor_art=0,squirrel_art=0;
        int hook_layer=-1,first_choice=-1,squirrel_layer=-1,layer=0;
        for(const auto& draw:rendered.effects.draws) {
            if(draw.source_asset=="Data\\SubGameSquirrel\\hook.bmp") {
                hook_layer=layer;
                assert(draw.x==200 && draw.y==0 && draw.source_rectangle);
                assert(draw.source_rectangle->top==436 && draw.source_rectangle->right==47);
                assert(draw.source_rectangle->bottom==445);
            }
            if(draw.source_asset=="Data\\SubGameSquirrel\\eyes.bmp") {
                assert(draw.source_rectangle && draw.source_rectangle->left==5*128);
            }
            if(draw.source_asset=="Data\\SubGameSquirrel\\mouth.bmp") {
                assert(draw.source_rectangle && draw.source_rectangle->left==120);
            }
            if(draw.source_asset=="Data\\SubGameSquirrel\\allconveyor.bmp" && first_choice<0)first_choice=layer;
            if(draw.source_asset=="Data\\SubGameSquirrel\\squriel_1_86.bmp")squirrel_layer=layer;
            ++layer;
            if(draw.source_asset=="Data\\SubGameSquirrel\\all.bmp" &&
               draw.source_rectangle &&
               (draw.x==36||draw.x==196||draw.x==356||draw.x==516)) {
                ++connector_art;
                assert(draw.source_rectangle->right-
                       draw.source_rectangle->left==89);
                assert(draw.source_rectangle->bottom==133);
            }
            if(draw.source_asset=="Data\\SubGameSquirrel\\nutpile.bmp")
                ++nut_piles;
            if(draw.source_asset=="Data\\SubGameSquirrel\\conveyor.bmp")
                ++conveyor_art;
            if(draw.source_asset==
                    "Data\\SubGameSquirrel\\squriel_1_86.bmp") {
                ++squirrel_art;
                assert(draw.source_rectangle);
                assert(draw.source_rectangle->left==0);
                assert(draw.source_rectangle->right==102);
                assert(draw.source_rectangle->top==0);
                assert(draw.source_rectangle->bottom==92);
                assert(draw.x==-34); // original base 36 minus loaded alignment 70
                assert(draw.y==234||draw.y==205||draw.y==172);
            }
        }
        assert(connector_art==4);
        assert(nut_piles==(difficulty==0?1:0));
        assert(conveyor_art==1);
        assert(squirrel_art==1);
        assert(squirrel_layer>=0 && hook_layer>squirrel_layer && first_choice>hook_layer);
        for(int i=0;i<100 && driver->animating();++i) {
            auto initial=game.advance({});
            assert(initial.kind==fg::FrameKind::ActivityUpdated);
        }
        assert(driver->run_section()==1 && !driver->animating());
        assert(driver->squirrel_displacement().x==10);
        for(int selected=0;selected<3*(difficulty+1);++selected) {
            const int level=selected/3;
            assert(driver->level()==level);
            assert(driver->placed_count()==selected%3);
            int offered=0,correct=0,decoy=0,chosen=-1,wrong=-1;
            for(std::size_t i=0;i<4;++i) {
                const auto& choice=driver->choices()[i];
                if(!choice.active)continue;
                ++offered;correct+=choice.correct;
                decoy+=!choice.correct;
                assert(choice.piece_id>=0&&choice.piece_id<36);
                if(choice.correct&&chosen<0)
                    chosen=static_cast<int>(i);
                if(!choice.correct&&wrong<0)
                    wrong=static_cast<int>(i);
            }
            assert(offered==(difficulty==0?3:4));
            assert(correct==(difficulty==2?2:3));
            assert(decoy==(difficulty==0?0:difficulty==1?1:2));
            assert(chosen>=0);
            if(decoy>0) {
                const auto xy=sq::kConveyorItemOrigins[static_cast<std::size_t>(wrong)];
                const auto piece=driver->choices()[static_cast<std::size_t>(wrong)].piece_id;
                frame=game.advance({.pointer_x=xy.x+2,.pointer_y=xy.y+2,.click_pulse=true});
                assert(driver->placement().state==sq::PlacementState::MoveToSelectedDecoy);
                bool dropped=false,retrieved=false,carried=false;
                for(int ticks=0;ticks<2500&&!driver->shared_back_available();++ticks) {
                    frame=game.advance({});
                    assert(frame.kind==fg::FrameKind::ActivityUpdated);
                    assert(driver->placed_count()==selected%3);
                    assert(frame.effects.progress_writes.empty());
                    carried|=driver->placement().carrying;
                    dropped|=driver->placement().state==sq::PlacementState::ReturnAfterDecoy;
                    retrieved|=driver->placement().state==sq::PlacementState::ReturnDecoyToConveyor;
                }
                assert(carried&&dropped&&retrieved&&driver->shared_back_available());
                assert(driver->choices()[static_cast<std::size_t>(wrong)].active);
                assert(driver->choices()[static_cast<std::size_t>(wrong)].piece_id==piece);
                assert(!driver->animating()&&driver->conveyor_phase()==0);
            }
            const auto point=sq::kConveyorItemOrigins[static_cast<std::size_t>(chosen)];
            frame=game.advance({.pointer_x=point.x,.pointer_y=point.y+2,.click_pulse=true});
            assert(driver->shared_back_available());
            frame=game.advance({.pointer_x=point.x+2,.pointer_y=point.y+2,.click_pulse=true});
            assert(driver->placement().state==sq::PlacementState::MoveToSelectedCorrect);
            assert(driver->placed_count()==selected%3);
            bool committed=false,carried=false;
            int section_completions=0,outgoing=0,incoming=0;
            bool observed_transition_latch=false;
            bool final=selected+1==3*(difficulty+1);
            for(int tick=0;tick<3000;++tick) {
                const int prior_section=driver->run_section();
                const int prior_phase=driver->conveyor_phase();
                frame=game.advance({});
                assert(frame.kind==fg::FrameKind::ActivityUpdated);
                assert(frame.effects.progress_writes.empty());
                carried|=driver->placement().carrying;
                committed|=driver->placed_count()>selected%3;
                if(driver->run_section()==prior_section+1)++section_completions;
                if(driver->level_transition_pending()) {
                    observed_transition_latch=true;
                    assert(driver->level()==level && driver->run_section()==7);
                }
                if(prior_phase==1)++outgoing;
                if(prior_phase==2)++incoming;
                for(const auto& draw:frame.effects.draws)
                    if(draw.source_asset=="Data\\SubGameSquirrel\\allconveyor.bmp" &&
                       !driver->placement().carrying) {
                        assert(draw.x>=0 && draw.source_rectangle);
                        assert(draw.x+draw.source_rectangle->right-draw.source_rectangle->left<=640);
                    }
                if(final&&driver->complete())break;
                if(!final&&driver->shared_back_available()&&!driver->animating()&&
                   driver->conveyor_phase()==0&&
                   (selected%3<2?(driver->placed_count()==selected%3+1 &&
                       driver->run_section()==(selected%3+1)*2+1):driver->level()==level+1))
                    break;
            }
            assert(committed&&carried&&section_completions==2);
            assert(driver->shared_back_available());
            if(!final) {
                assert(outgoing==101 && incoming==84);
                assert(observed_transition_latch==(selected%3==2));
                assert(driver->conveyor_phase()==0 && driver->conveyor_offset()==0);
            } else {
                assert(driver->complete());
                assert(frame.effects.audio.size()==2);
                assert(frame.effects.audio.back().sound_id>=664 && frame.effects.audio.back().sound_id<=667);
            }
        }
        frame=game.advance({}); // Wendy final line
        assert(frame.kind==fg::FrameKind::ActivityUpdated);
        assert(frame.effects.audio.size()>=2);
        assert(frame.effects.audio.back().sound_id>=665);
        assert(frame.effects.audio.back().sound_id<=667);
        frame=game.advance({.managed_sound_playing=true});
        assert(frame.kind==fg::FrameKind::ActivityUpdated);
        assert(!frame.effects.save_and_unload);
        frame=game.advance({.managed_sound_playing=false});
        assert(!frame.effects.save_and_unload);
        frame=game.advance({});
        assert(frame.effects.save_and_unload);
        assert(frame.effects.progress_writes.size()==1);
        assert(frame.effects.progress_writes[0].slot==
               btb::progress::Slot::SquirrelRun);
        assert(game.globals().player_progress[2].get(
            btb::progress::Slot::SquirrelRun)==1);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::PlayAgainYesNoSetup));
    }
    // Drive complete native windowed games using the actual 50-pixel,
    // alternate-update conveyor branch. Compare the portable kernel to
    // the live driver on every frame, even when the conveyor is idle.
    for(int difficulty=0;difficulty<3;++difficulty)for(int seed=0;seed<12;++seed) {
        btb::retail::OriginalRetailRandom random(static_cast<std::uint32_t>(seed+91));
        fg::SquirrelDriver driver(root,difficulty,&random);
        std::string error;
        assert(driver.initialize(0,error));
        int selected=0,frames=0,regenerated=0;
        for(;frames<30000 && !driver.complete();++frames) {
            fg::ActivityFrameInput input{};
            input.original_windowed_display=true;
            const auto old=driver.conveyor_state();
            const bool transitioning=driver.level_transition_pending();
            auto expected=old;
            if(!transitioning)
                btb::retail_activity::update_squirrel_conveyor(expected,true,nullptr);
            if(!transitioning && driver.shared_back_available() &&
                driver.conveyor_phase()==0 && driver.placed_count()<3 &&
                driver.run_section()<6) {
                for(std::size_t i=0;i<driver.choices().size();++i)
                    if(driver.choices()[i].active && driver.choices()[i].correct) {
                        const auto p=sq::kConveyorItemOrigins[i];
                        input.pointer_x=p.x+10;input.pointer_y=p.y+10;
                        input.click_pulse=true;++selected;break;
                    }
            }
            const auto output=driver.advance(input);
            assert(!output.fatal_error);
            const auto now=driver.conveyor_state();
            assert(now.window_counter==expected.window_counter);
            assert(now.position==expected.position);
            assert(now.animation_countdown==expected.animation_countdown);
            assert(now.animation_frame==expected.animation_frame);
            if(old.phase==1 && expected.phase==2)++regenerated;
            if(expected.phase!=0)assert(now.phase==expected.phase);
        }
        assert(driver.complete()&&frames<30000);
        assert(selected==3*(difficulty+1));
        assert(regenerated==selected-1);
        const auto prior=driver.conveyor_state().window_counter;
        assert(driver.unload(error));
        assert(driver.initialize(0,error));
        assert(driver.conveyor_state().window_counter==prior);
        auto frame=driver.advance({.original_windowed_display=true});
        assert(!frame.fatal_error&&driver.conveyor_state().window_counter==prior+1);
        frame=driver.advance({.original_windowed_display=false});
        assert(!frame.fatal_error&&driver.conveyor_state().window_counter==prior+1);
        assert(driver.unload(error));
    }
    fs::remove_all(root);
}
