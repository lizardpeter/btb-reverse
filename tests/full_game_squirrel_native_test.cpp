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
        int connector_art=0,nut_piles=0,conveyor_art=0,squirrel_art=0;
        for(const auto& draw:rendered.effects.draws) {
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
                assert(draw.x==36);
                assert(draw.y==234||draw.y==205||draw.y==172);
            }
        }
        assert(connector_art==4);
        assert(nut_piles==(difficulty==0?1:0));
        assert(conveyor_art==1);
        assert(squirrel_art==1);
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
                assert(wrong>=0);
                const auto xy=sq::kConveyorItemOrigins[
                    static_cast<std::size_t>(wrong)];
                frame=game.advance({
                    .pointer_x=xy.x+2,.pointer_y=xy.y+2,
                    .click_pulse=true
                });
                assert(frame.kind==fg::FrameKind::ActivityUpdated);
                assert(!driver->animating());
                assert(driver->placed_count()==selected%3);
                assert(frame.effects.progress_writes.empty());
            }
            const auto point=sq::kConveyorItemOrigins[
                static_cast<std::size_t>(chosen)];
            // Strict edge clicks must not select a piece.
            frame=game.advance({.pointer_x=point.x,.pointer_y=point.y+2,.click_pulse=true});
            assert(!driver->animating());
            frame=game.advance({
                .pointer_x=point.x+2,.pointer_y=point.y+2,
                .click_pulse=true
            });
            assert(frame.kind==fg::FrameKind::ActivityUpdated);
            assert(driver->animating());
            int ticks=0;
            while(driver->animating()&&ticks<150) {
                frame=game.advance({});
                ++ticks;
            }
            // Recovered 0x00425F40: the whole source motion has exactly
            // 14 seven-tick substeps with three overlapping boundaries.
            assert(ticks==95);
            assert(!driver->animating());
            assert(driver->shared_back_available()); // no permanent selection lock
            assert(frame.effects.progress_writes.empty());
            if(selected%3<2) {
                assert(driver->conveyor_phase()==1 && driver->conveyor_offset()==475);
                // A click while the conveyor slides cannot choose a piece.
                frame=game.advance({.pointer_x=point.x+2,.pointer_y=point.y+2,.click_pulse=true});
                assert(!driver->animating());
                const auto before_regeneration=rng.state();
                for(int i=0;i<4;++i)frame=game.advance({});
                assert(driver->conveyor_phase()==1 && driver->conveyor_offset()==500);
                assert(rng.state()==before_regeneration);
                frame=game.advance({});
                assert(driver->conveyor_phase()==2 && driver->conveyor_offset()==-420);
                assert(rng.state()!=before_regeneration);
                for(const auto& draw:frame.effects.draws) {
                    if(draw.source_asset=="Data\\SubGameSquirrel\\allconveyor.bmp") {
                        assert(draw.x>=0 && draw.source_rectangle);
                        assert(draw.x+draw.source_rectangle->right-draw.source_rectangle->left<=640);
                    }
                }
                const auto generated=rng.state();
                for(int i=0;i<83;++i)frame=game.advance({});
                assert(driver->conveyor_phase()==2 && driver->conveyor_offset()==-5);
                frame=game.advance({});
                assert(driver->conveyor_phase()==0 && driver->conveyor_offset()==0);
                assert(rng.state()==generated);
            } else assert(driver->conveyor_phase()==0);
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
    fs::remove_all(root);
}
