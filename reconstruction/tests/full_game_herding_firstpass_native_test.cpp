#include "btb/full_game_herding.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"
#include "btb/herding_source_pickles_animation.hpp"
#include "btb/herding_source_far_roam.hpp"
#include "btb/herding_source_attraction.hpp"

#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"native Pets Corner: %s at %s:%d\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)

#include <filesystem>
#include <fstream>
#include <memory>
#include <utility>
#include <vector>

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
    // Retail 0x416A30 measures Pickles proximity using the
    // animal's NEGATIVE half-sprite X anchor. A normal center would
    // incorrectly classify this as far enough to free-roam.
    {
        btb::herding::RetailEntityRecord32 hero{};
        hero.type=static_cast<int>(btb::herding::EntityType::FarmerPickles);
        hero.x=100;
        hero.y=200;
        hero.source_bottom=100; // original Pickles reference Y=280
        btb::herding::RetailEntityRecord32 sheep{};
        sheep.type=static_cast<int>(btb::herding::EntityType::Sheep);
        sheep.x=151;
        sheep.y=233;
        sheep.x_float=151.f;
        sheep.y_float=233.f;
        sheep.source_right=102;
        sheep.source_bottom=95; // source animal center -> (100,280)
        sheep.direction=2;
        sheep.movement_speed=1.0f;
        const std::vector<btb::herding::Vec2i> polygon{{
            {0,0},{600,0},{600,700},{0,700}
        }};
        const auto near=
            btb::herding::original_far_untracked_animal_roam_step(
                sheep,hero,polygon);
        assert(near.coverage==
            btb::herding::RetailFarAnimalRoamCoverage::NotCovered);
        sheep.x=250;
        sheep.x_float=250.f;
        const auto far=
            btb::herding::original_far_untracked_animal_roam_step(
                sheep,hero,polygon);
        assert(far.coverage==
            btb::herding::RetailFarAnimalRoamCoverage::AdvancedWithinPolygon);
        assert(far.animal.x_float>sheep.x_float);
    }
    // This native source adapter test places the retail Pickles
    // character first at a genuine Sheep food bag and then in the
    // exact original <80 attraction radius of spawned sheep #1.
    // No synthetic click or fake home/delivery event is supplied.
    {
        std::ifstream infile(root/"Data"/"SubGame1"/"herd.txt");
        assert(infile.good());
        auto data=btb::herding::parse_data(infile);
        fg::NativePicklesMotionSource native{};
        std::vector<btb::herding::RetailEntityRecord32> animals{};
        std::string error{};
        std::srand(123);
        assert(native.initialize(data,0,animals,error));
        assert(animals.size()==15);
        animals[0].x=240;animals[0].y=414;
        animals[0].x_float=240.f;animals[0].y_float=414.f;
        fg::HerdingObservedFrame observation{};
        assert(native.advance(data,{}, {},animals,observation,error));
        bool selected_sheep=false;
        for(const auto& e:observation.events)
            selected_sheep |= e.kind==fg::HerdingObservedKind::PickUpFood &&
                              e.food==FoodType::SheepFood;
        assert(selected_sheep);
        animals=observation.motion_records;
        animals[0].x=500;animals[0].y=520;
        animals[0].x_float=500.f;animals[0].y_float=520.f;
        const auto near=btb::herding::original_herding_food_attraction_contact(
            animals[1],animals[0],FoodType::SheepFood,false);
        assert(near.has_value()&&*near);
        assert(native.advance(data,{}, {},animals,observation,error));
        bool joined_first_sheep=false;
        for(const auto& e:observation.events)
            joined_first_sheep |=
                e.kind==fg::HerdingObservedKind::JoinFollower &&
                e.entity_index==1;
        assert(joined_first_sheep);
        animals=observation.motion_records;
        assert(native.advance(data,{}, {},animals,observation,error));
        for(const auto& e:observation.events)
            assert(!(e.kind==fg::HerdingObservedKind::JoinFollower &&
                     e.entity_index==1)); // no repeated join/voice
        animals=observation.motion_records;
        animals[0].x=365;animals[0].y=354;
        animals[0].x_float=365.f;animals[0].y_float=354.f;
        assert(native.advance(data,{}, {},animals,observation,error));
        bool released_on_food_change=false;
        for(const auto& e:observation.events) {
            if(e.kind!=fg::HerdingObservedKind::PickUpFood ||
               e.food!=FoodType::RabbitFood)continue;
            released_on_food_change=true;
            assert(e.follower_release_random.size()==animals.size());
            assert(e.follower_release_random[1].x_mod_400>=0);
            assert(e.follower_release_random[1].x_mod_400<400);
            assert(e.follower_release_random[1].y_mod_400>=0);
            assert(e.follower_release_random[1].y_mod_400<400);
        }
        assert(released_on_food_change);
        assert(native.unload(error));
    }
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
        // Snapshot the actual retail animal coordinates after first
        // frame. Original 0x419100 default far-roam motion should
        // advance these over subsequent native GameRoot updates.
        std::vector<std::pair<int,int>> first_animals{};
        for(const auto& e:live->scene().entities) {
            if(btb::herding::is_herd_animal(e.entity_type()))
                first_animals.emplace_back(e.x,e.y);
        }
        assert(first_animals.size()==static_cast<std::size_t>(
            3*(3+difficulty)));
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
        bool some_real_animal_moved=false;
        std::size_t animal_ordinal=0;
        for(const auto& e:live->scene().entities) {
            if(!btb::herding::is_herd_animal(e.entity_type()))
                continue;
            assert(animal_ordinal<first_animals.size());
            some_real_animal_moved |=
                e.x!=first_animals[animal_ordinal].first ||
                e.y!=first_animals[animal_ordinal].second;
            ++animal_ordinal;
        }
        // The binary's exact float direction table, not an arbitrary
        // drift animation, must now move animals in the real world.
        if(!some_real_animal_moved) {
            const auto& a=live->scene().entities[1];
            const auto& hero=live->scene().entities.front();
            std::fprintf(stderr,
                "far animal remained still difficulty=%d "
                "first=(%d,%d) now=(%d,%d) floats=(%.3f,%.3f) "
                "speed=%.4f dir=%d active=%d behavior=%d timer=%d "
                "source_rect=(%d,%d,%d,%d) hero=(%d,%d)\\n",
                difficulty,first_animals.front().first,
                first_animals.front().second,a.x,a.y,
                static_cast<double>(a.x_float),
                static_cast<double>(a.y_float),
                static_cast<double>(a.movement_speed),
                a.direction,a.movement_active,a.behavior_state,
                a.temporary_target_timer,
                a.source_left,a.source_top,
                a.source_right,a.source_bottom,hero.x,hero.y);
        }
        assert(some_real_animal_moved);
        // No fabricated delivery: source animals require autonomous
        // movement/follower collision completion to award Pets Corner.
        assert(game.globals().player_progress[0].get(
            btb::progress::Slot::PetsCorner)==0);
        (void)background;(void)pickles;(void)surround;
    }
    fs::remove_all(root);
}
