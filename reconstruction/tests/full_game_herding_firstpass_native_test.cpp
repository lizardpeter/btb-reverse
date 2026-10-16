#include "btb/full_game_herding.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"
#include "btb/herding_source_pickles_animation.hpp"

#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"native Pets Corner: %s at %s:%d\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)

#include <filesystem>
#include <fstream>
#include <memory>

int main() {
    namespace fs=std::filesystem;
    namespace fg=btb::full_game;
    using btb::game_flow::State;
    const auto root=fs::temp_directory_path()/
        "btb_native_herding_firstpass_source";
    fs::remove_all(root);
    fs::create_directories(root/"Data"/"SubGame1");
    {
        std::ofstream f(root/"Data"/"SubGame1"/"herd.txt");
        // Original 0x00419600 consumes SIX fixed setup pairs before
        // parsing sentinel-separated 12/5/6/1 coordinate groups.
        f<<"0 0\n100 200\n250 300\n350 390\n"
         <<"90 180\n120 240\n";
        constexpr int nav[][2]={
            {64,100},{800,100},{1300,100},{1300,200},
            {1300,900},{1000,900},{300,900},{64,900},
            {64,700},{64,400},{64,200},{64,150}
        };
        for(const auto& p:nav)f<<p[0]<<" "<<p[1]<<"\n";
        f<<"-1 -1\n";
        for(int i=0;i<5;++i)
            f<<100+i*20<<" "<<200+i*12<<"\n";
        f<<"-1 -1\n";
        for(int i=0;i<6;++i)
            f<<300+i*40<<" "<<350+i*10<<"\n";
        f<<"-1 -1\n";
        f<<"777 999\n-1 -1\n";
    }
    static_assert(btb::herding::food_pickup_in_range(
        btb::herding::FoodType::RabbitFood,350,390));
    static_assert(!btb::herding::food_pickup_in_range(
        btb::herding::FoodType::RabbitFood,365,394)); // exact 40px
    using btb::herding::original_pickles_keyboard_animation_step;
    using btb::herding::FoodType;
    // Real frame loop, not a synthetic render-only animation:
    // source switch 0x418CE4: carrying rows 20..29/33..38.
    static_assert(original_pickles_keyboard_animation_step(
        20,50,true,FoodType::RabbitFood)->countdown==40);
    static_assert(original_pickles_keyboard_animation_step(
        20,10,true,FoodType::RabbitFood)->frame==21);
    static_assert(original_pickles_keyboard_animation_step(
        29,10,true,FoodType::RabbitFood)->frame==20);
    static_assert(original_pickles_keyboard_animation_step(
        38,10,false,FoodType::RabbitFood)->frame==33);
    static_assert(original_pickles_keyboard_animation_step(
        15,10,false,FoodType::None)->frame==12);
    static_assert(original_pickles_keyboard_animation_step(
        2,60,false,FoodType::None)->frame==12);
    std::srand(123);
    for(int difficulty=0;difficulty<3;++difficulty) {
        fg::GameRoot game;
        assert(game.select_profile(0));
        auto simulator=std::make_unique<fg::HerdingEventSimulation>(
            std::make_unique<fg::NativePicklesMotionSource>());
        auto driver=std::make_unique<fg::HerdingDriver>(
            root/"Data"/"SubGame1",std::move(simulator),difficulty);
        auto* live=driver.get();
        game.install(fg::ActivityId::Herding,std::move(driver));
        game.set_outer_state(State::HerdingInit);
        auto first=game.advance({});
        if(first.kind!=fg::FrameKind::ActivityInitialized)
            std::fprintf(stderr,"Pets Corner init: frame=%d error=%s state=%d\\n",
                static_cast<int>(first.kind),first.error.c_str(),
                game.globals().dispatcher.current_state);
        assert(first.kind==fg::FrameKind::ActivityInitialized);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::HerdingRun));
        auto scene=game.advance({});
        assert(scene.kind==fg::FrameKind::ActivityUpdated);
        assert(!scene.effects.fatal_error);
        // Original 0x4182B0 proximity triggers the Rabbit bag without
        // requiring a guessed mouse click. Exact original x=350,y=390
        // is strictly <40 from the source (365,354) bag hotspot.
        assert(live->scene().selected_food==
               btb::herding::FoodType::RabbitFood);
        assert(!live->scene().visible_world_bags[1]);
        bool original_pickup_voice=false;
        for(const auto& audio:scene.effects.audio)
            original_pickup_voice|=audio.sound_id==583||
                                   audio.sound_id==610;
        assert(original_pickup_voice);
        assert(live->scene().entities.size()==
               static_cast<std::size_t>(15+3*difficulty+
                   (difficulty>0?1:0)));
        assert(live->scene().entities.front().x==350);
        assert(live->scene().entities.front().y==390);
        assert(live->scene().entities.front().entity_type()==
               btb::herding::EntityType::FarmerPickles);
        assert(live->scene().entities.front().animation_frame==33);
        // The retail dog exists on Medium/Hard only. Its first idle
        // frame must still advance animation and +0.01 speed, and the
        // source-controlled animation latch later permits translation.
        int scruffty_start_x=0,scruffty_start_y=0;
        bool found_scruffty=false;
        for(const auto& entity:live->scene().entities) {
            if(entity.entity_type()!=btb::herding::EntityType::Scruffty)
                continue;
            assert(!found_scruffty);
            found_scruffty=true;
            assert(entity.animation_frame==1);
            assert(entity.movement_speed>0.0f);
            scruffty_start_x=entity.x;
            scruffty_start_y=entity.y;
        }
        assert(found_scruffty==(difficulty>0));
        bool background=false,pickles=false,surround=false;
        for(const auto& draw:scene.effects.draws) {
            background|=draw.source_asset.find("PC_Background")!=
                std::string::npos;
            pickles|=draw.source_asset.find("Pickles")!=
                std::string::npos;
            surround|=draw.source_asset.find("Surround")!=
                std::string::npos;
        }
        assert(!scene.effects.draws.empty());
        assert(scene.effects.progress_writes.empty());
        scene=game.advance({.directional_input_bits=0x02});
        assert(scene.kind==fg::FrameKind::ActivityUpdated);
        assert(!scene.effects.fatal_error);
        assert(live->scene().entities.front().x==351);
        assert(live->scene().entities.front().x_float==351.5f);
        assert(live->scene().entities.front().y==390);
        assert(live->scene().entities.front().animation_frame==20);
        assert(live->scene().entities.front().animation_frame_countdown==50);
        assert(live->scene().selected_food==
               btb::herding::FoodType::RabbitFood);
        scene=game.advance({.directional_input_bits=0x01});
        assert(live->scene().entities.front().x==350);
        assert(live->scene().entities.front().x_float==350.f);
        assert(live->scene().entities.front().animation_frame==20);
        assert(live->scene().entities.front().animation_frame_countdown==40);
        // Actual native GameRoot updates advance the ten-frame walk
        // cycle using the source's -10 / reload-50 countdown.
        for(int frame=0;frame<4;++frame) {
            scene=game.advance({.directional_input_bits=0x02});
            assert(scene.kind==fg::FrameKind::ActivityUpdated);
            assert(!scene.effects.fatal_error);
        }
        assert(live->scene().entities.front().animation_frame==21);
        assert(live->scene().entities.front().animation_frame_countdown==50);
        scene=game.advance({});
        assert(live->scene().entities.front().animation_frame==33);
        assert(scene.effects.progress_writes.empty());
        for(int i=0;i<100;++i) {
            scene=game.advance({});
            assert(scene.kind==fg::FrameKind::ActivityUpdated);
            assert(scene.effects.progress_writes.empty());
        }
        // After 100 live frames, the native dog must have visibly
        // patrolled, not merely advanced a hidden source counter.
        if(difficulty>0) {
            bool moved_scruffty=false;
            for(const auto& entity:live->scene().entities) {
                if(entity.entity_type()!=btb::herding::EntityType::Scruffty)
                    continue;
                moved_scruffty=entity.x!=scruffty_start_x ||
                               entity.y!=scruffty_start_y;
                assert(entity.movement_speed>0.1f);
            }
            assert(moved_scruffty);
        }
        // No fabricated delivery: source animals require autonomous
        // movement/follower collision completion to award Pets Corner.
        assert(game.globals().player_progress[0].get(
            btb::progress::Slot::PetsCorner)==0);
        (void)background;(void)pickles;(void)surround;
    }
    fs::remove_all(root);
}
