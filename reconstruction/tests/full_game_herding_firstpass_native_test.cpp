#include "btb/full_game_herding.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"
#include "btb/herding_source_pickles_animation.hpp"
#include "btb/herding_source_far_roam.hpp"
#include "btb/herding_source_attraction.hpp"
#include "btb/herding_source_temporary_target.hpp"
#include "btb/herding_source_registration_point.hpp"
#include "btb/herding_source_follower_approach.hpp"
#include "btb/herding_source_home_motion.hpp"
#include "btb/herding_source_delivered_animation.hpp"
#include "btb/herding_retail_rect_contact.hpp"
#include "btb/retail_crt_random.hpp"
#include "btb/herding_source_ordinary_following.hpp"

#include <algorithm>
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
    // The same original CRT generator is owned by the Windows host
    // and reused across frames. A rejected NativeGameSession update
    // must NOT consume RNG or permanently mutate Scruffty/food state.
    {
        std::ifstream input(root/"Data"/"SubGame1"/"herd.txt");
        assert(input.good());
        auto data=btb::herding::parse_data(input);
        btb::retail::OriginalRetailRandom shared_rng{0x12345678u};
        fg::NativePicklesMotionSource native{shared_rng};
        std::vector<btb::herding::RetailEntityRecord32> entities{};
        std::string error;
        assert(native.initialize(data,1,entities,error));
        assert(entities.back().entity_type()==
               btb::herding::EntityType::Scruffty);
        auto seed_before=shared_rng.state();
        entities[0].x=240;entities[0].y=414;
        entities[0].x_float=240.f;entities[0].y_float=414.f;
        fg::HerdingObservedFrame first{},replayed{};
        assert(native.begin_frame(error));
        assert(!native.begin_frame(error)); // reject nested frame
        assert(native.advance(data,{}, {},entities,first,error));
        assert(first.scruffty_patrol_evidence);
        const auto picked=std::find_if(
            first.events.begin(),first.events.end(),
            [](const auto& e) {
                return e.kind==fg::HerdingObservedKind::PickUpFood &&
                       e.food==btb::herding::FoodType::SheepFood;
            });
        assert(picked!=first.events.end());
        const int original_voice= picked->random_mod_2;
        const auto after_first_rng=shared_rng.state();
        assert(after_first_rng!=seed_before);
        const auto dog_index=first.scruffty_patrol_evidence->entity_index;
        const auto first_dog_x=first.motion_records[dog_index].x_float;
        const auto first_dog_speed=
            first.motion_records[dog_index].movement_speed;
        native.rollback_frame();
        assert(shared_rng.state()==seed_before);
        assert(native.begin_frame(error));
        assert(native.advance(data,{}, {},entities,replayed,error));
        assert(shared_rng.state()==after_first_rng);
        const auto repeated_pick=std::find_if(
            replayed.events.begin(),replayed.events.end(),
            [](const auto& e) {
                return e.kind==fg::HerdingObservedKind::PickUpFood &&
                       e.food==btb::herding::FoodType::SheepFood;
            });
        assert(repeated_pick!=replayed.events.end());
        assert(repeated_pick->random_mod_2==original_voice);
        assert(replayed.scruffty_patrol_evidence);
        assert(replayed.motion_records[dog_index].x_float==first_dog_x);
        assert(replayed.motion_records[dog_index].movement_speed==
               first_dog_speed);
        native.commit_frame();
        native.rollback_frame(); // completed transaction cannot rewind
        assert(shared_rng.state()==after_first_rng);

        // Follower insertion and its source-random voice MUST also
        // roll back. Retry the same attempted Pickles/sheep contact:
        // the same sheep must join exactly once, with the same roll.
        entities=replayed.motion_records;
        entities[0].x=entities[1].x+
            (entities[1].source_left-entities[1].source_right)/2;
        entities[0].y=entities[1].y+
            (entities[1].source_bottom-entities[1].source_top)/2-
            (entities[0].source_bottom-entities[0].source_top)+20;
        entities[0].x_float=static_cast<float>(entities[0].x);
        entities[0].y_float=static_cast<float>(entities[0].y);
        const auto pre_join_rng=shared_rng.state();
        fg::HerdingObservedFrame joining{},joined_again{};
        assert(native.begin_frame(error));
        assert(native.advance(data,{}, {},entities,joining,error));
        const auto first_join=std::find_if(
            joining.events.begin(),joining.events.end(),
            [](const auto& e) {
                return e.kind==fg::HerdingObservedKind::JoinFollower &&
                       e.entity_index==1;
            });
        assert(first_join!=joining.events.end());
        const auto first_join_voice=first_join->random_mod_2;
        const auto after_join_rng=shared_rng.state();
        assert(after_join_rng!=pre_join_rng);
        native.rollback_frame();
        assert(shared_rng.state()==pre_join_rng);
        assert(native.begin_frame(error));
        assert(native.advance(data,{}, {},entities,joined_again,error));
        const auto second_join=std::find_if(
            joined_again.events.begin(),joined_again.events.end(),
            [](const auto& e) {
                return e.kind==fg::HerdingObservedKind::JoinFollower &&
                       e.entity_index==1;
            });
        assert(second_join!=joined_again.events.end());
        assert(second_join->random_mod_2==first_join_voice);
        assert(shared_rng.state()==after_join_rng);
        native.commit_frame();
        entities=joined_again.motion_records;
        fg::HerdingObservedFrame following{};
        assert(native.begin_frame(error));
        assert(native.advance(data,{}, {},entities,following,error));
        for(const auto& e:following.events)
            assert(!(e.kind==fg::HerdingObservedKind::JoinFollower &&
                     e.entity_index==1));
        native.commit_frame();
        assert(native.unload(error));
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
    // Recovered x86 0x417603 follower-slot geometry and source
    // speed response: slot 0 holds at 15 source units; beyond that
    // it steers into Pickles' own facing-dependent orbit.
    {
        btb::herding::RetailEntityRecord32 hero{};
        hero.type=static_cast<int>(btb::herding::EntityType::FarmerPickles);
        hero.x=500;hero.y=500;
        hero.source_bottom=100;
        hero.direction=2;
        btb::herding::RetailEntityRecord32 animal{};
        animal.type=static_cast<int>(btb::herding::EntityType::Sheep);
        animal.source_right=100;
        animal.source_bottom=80;
        animal.x=525;animal.y=520;
        animal.x_float=525.f;animal.y_float=520.f;
        animal.movement_speed=0.5f;
        animal.animation_frame_countdown=100;
        const auto held=
            btb::herding::original_herding_ordinary_follower_step(
                animal,hero,0);
        assert(held && held->holds_formation_position);
        assert(held->moved.x==525 && held->moved.y==520);
        assert(held->moved.direction==2);
        assert(held->moved.movement_speed<animal.movement_speed);
        assert(held->moved.movement_active==1);
        assert(held->source_formation_ordinal==0);

        animal.x=630;animal.y=520;
        animal.x_float=630.f;animal.y_float=520.f;
        animal.movement_speed=0.f;
        const auto following=
            btb::herding::original_herding_ordinary_follower_step(
                animal,hero,0);
        assert(following && !following->holds_formation_position);
        assert(following->source_formation_target.x==445);
        // The retail float radian constant is slightly larger than
        // mathematical pi/180. At 270 degrees this moves target Y
        // a fraction below 560, and original FTOL truncates to 559.
        assert(following->source_formation_target.y==559);
        assert(following->moved.movement_speed>0.f);
        assert(following->moved.x_float<animal.x_float);
        // The formation slot applies two different spacing formulas:
        // orbit radius=slot*25+15 and speed gap=slot*50+15.
        const auto second=
            btb::herding::original_herding_ordinary_follower_step(
                animal,hero,1);
        assert(second && !second->holds_formation_position);
        assert(second->source_formation_target.x==420);
    }

    // This native source adapter test places the retail Pickles
    // character first at a genuine Sheep food bag and then in the
    // exact original <80 attraction radius of spawned sheep #1.
    // No synthetic click or fake home/delivery event is supplied.
    {
        std::ifstream infile(root/"Data"/"SubGame1"/"herd.txt");
        assert(infile.good());
        auto data=btb::herding::parse_data(infile);
        btb::retail::OriginalRetailRandom game_rng{123};
        fg::NativePicklesMotionSource native{game_rng};
        std::vector<btb::herding::RetailEntityRecord32> animals{};
        std::string error{};
        assert(native.initialize(data,0,animals,error));
        auto expected_first_selection=game_rng;
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
        const auto expected_voice=expected_first_selection.next_rand()%2;
        const auto pickup=std::find_if(
            observation.events.begin(),observation.events.end(),
            [](const auto& event) {
                return event.kind==fg::HerdingObservedKind::PickUpFood &&
                       event.food==FoodType::SheepFood;
            });
        assert(pickup!=observation.events.end());
        assert(pickup->random_mod_2==expected_voice);
        animals=observation.motion_records;
        animals[0].x=500;animals[0].y=520;
        animals[0].x_float=500.f;animals[0].y_float=520.f;
        // Spawn locations depend on the exact CRT seed. Place this
        // fixture sheep at the original source attraction anchor,
        // rather than relying on unrelated host std::rand behavior.
        animals[1].x=animals[0].x+
            (animals[1].source_right-animals[1].source_left)/2;
        animals[1].y=animals[0].y+
            (animals[0].source_bottom-animals[0].source_top)-20-
            (animals[1].source_bottom-animals[1].source_top)/2;
        animals[1].x_float=static_cast<float>(animals[1].x);
        animals[1].y_float=static_cast<float>(animals[1].y);
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
        // Original 0x4175C5 join falls THROUGH to 0x417603:
        // the newly admitted sheep moves in formation immediately,
        // not with a fabricated one-frame delay.
        assert(std::any_of(
            observation.ordinary_follower_evidence.begin(),
            observation.ordinary_follower_evidence.end(),
            [](const auto& proof){ return proof.entity_index==1; }));
        assert(observation.motion_records[1].movement_active==1);
        animals=observation.motion_records;
        const auto original_follow_pos=animals[1].x_float;
        assert(native.advance(data,{}, {},animals,observation,error));
        const auto proof=std::find_if(
            observation.ordinary_follower_evidence.begin(),
            observation.ordinary_follower_evidence.end(),
            [](const auto& step){return step.entity_index==1;});
        assert(proof!=observation.ordinary_follower_evidence.end());
        assert(observation.motion_records[1].movement_active==1);
        assert(observation.motion_records[1].movement_speed>0.f);
        assert(observation.motion_records[1].x_float!=original_follow_pos);
        for(const auto& e:observation.events)
            assert(!(e.kind==fg::HerdingObservedKind::JoinFollower &&
                     e.entity_index==1)); // no repeated join/voice
        // Simulate a follower physically reaching the genuine sheep
        // registration area (not a new synthetic gameplay shortcut).
        // The original two-list dispatcher admits it on one tick,
        // steers on the NEXT tick, and then emits state 0->1 only
        // after strictly <30 from point (736,183).
        animals=observation.motion_records;
        animals[1].x=760;animals[1].y=210;
        animals[1].x_float=760.f;animals[1].y_float=210.f;
        const auto registered_point=
            btb::herding::original_herding_species_registration_point(
                btb::herding::EntityType::Sheep);
        assert(registered_point && registered_point->x==736 &&
               registered_point->y==183);
        assert(native.advance(data,{}, {},animals,observation,error));
        assert(observation.tracked_approach_evidence.empty());
        animals=observation.motion_records;
        const auto tracked_before=animals[1];
        assert(native.advance(data,{}, {},animals,observation,error));
        assert(observation.tracked_approach_evidence.size()==1);
        assert(!observation.tracked_approach_evidence.front().arrived);
        const auto expected_approach=
            btb::herding::original_herding_follower_approach(
                tracked_before,*registered_point,
                *btb::herding::original_herding_animal_tracking_anchor(
                    tracked_before));
        assert(expected_approach);
        assert(observation.motion_records[1].x_float==
               expected_approach->updated.x_float);
        animals=observation.motion_records;
        // Place the actor at the actual source target, correcting for
        // the signed original half-sprite X/Y anchor.
        animals[1].x=736+(animals[1].source_right-
            animals[1].source_left)/2;
        animals[1].y=183-(animals[1].source_bottom-
            animals[1].source_top)/2;
        animals[1].x_float=static_cast<float>(animals[1].x);
        animals[1].y_float=static_cast<float>(animals[1].y);
        assert(native.advance(data,{}, {},animals,observation,error));
        assert(observation.tracked_approach_evidence.size()==1);
        assert(observation.tracked_approach_evidence.front().arrived);
        bool arrived_once=false;
        for(const auto& e:observation.events)
            arrived_once |=
                e.kind==fg::HerdingObservedKind::TrackedTargetArrived &&
                e.entity_index==1;
        assert(arrived_once);
        animals=observation.motion_records;
        // In the actual game the recovered event machine owns the
        // state transition. Test its following-frame state-one route
        // handoff without double-emitting tracked arrival.
        animals[1].behavior_state=1;
        assert(native.advance(data,{}, {},animals,observation,error));
        bool entered_route=false;
        for(const auto& e:observation.events)
            entered_route |=
                e.kind==fg::HerdingObservedKind::EnterHomeRoute &&
                e.entity_index==1;
        assert(entered_route);
        animals=observation.motion_records;
        animals[1].behavior_state=0; // other fixture checks food release
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
        // The recovered event machine sets a 200-tick escape target
        // on the released sheep. Feed that genuine record transition
        // back to the native source on its next frame: do not pretend
        // a food-change itself moves the animal immediately.
        animals=observation.motion_records;
        animals[1].temporary_target_timer=200;
        animals[1].target_x=1000;
        animals[1].target_y=800;
        const auto release_before=animals[1];
        const auto predicted=
            btb::herding::original_herding_temporary_target_update(
                release_before);
        assert(predicted && predicted->branch_taken);
        assert(!predicted->early_return);
        assert(native.advance(data,{}, {},animals,observation,error));
        assert(observation.temporary_target_evidence.size()==1);
        const auto& escape=observation.motion_records[1];
        assert(escape.x==predicted->moved.x &&
               escape.y==predicted->moved.y);
        assert(escape.x_float==predicted->moved.x_float &&
               escape.y_float==predicted->moved.y_float);
        assert(escape.movement_speed==3.f);
        assert(escape.temporary_target_timer==199);
        for(const auto& e:observation.events)
            assert(!(e.kind==fg::HerdingObservedKind::JoinFollower &&
                     e.entity_index==1));

        // With only one timeout tick remaining the source decrements
        // 1 -> 0 before ordinary animal AI, without stale target motion.
        animals=observation.motion_records;
        animals[1].temporary_target_timer=1;
        assert(native.advance(data,{}, {},animals,observation,error));
        assert(observation.motion_records[1].temporary_target_timer==0);
        assert(observation.temporary_target_evidence.empty());
        // The source may execute a legitimate far-roaming movement
        // after timer expiration. Integer positions can still be
        // unchanged for subpixel steps; do not mistake that for a
        // fresh temporary-target movement. Any far-roam probe is
        // independently verified by HerdingEventSimulation.

        // A target inside the original STRICT 120px gate clears the
        // temporary timer immediately on completion.
        animals=observation.motion_records;
        animals[1].temporary_target_timer=200;
        animals[1].target_x=animals[1].x+30;
        animals[1].target_y=animals[1].y+30;
        assert(native.advance(data,{}, {},animals,observation,error));
        assert(observation.temporary_target_evidence.size()==1);
        assert(observation.temporary_target_evidence.front().source_returned_early);
        assert(observation.motion_records[1].temporary_target_timer==0);
        assert(native.unload(error));
    }
    // Exercise genuine first/second stage home-route updates with
    // source animal sprite origins, NOT actor-space target positions.
    // The native adapter only supplies source-confirmed events; the
    // event simulation owns all state transitions and progress.
    {
        std::ifstream infile(root/"Data"/"SubGame1"/"herd.txt");
        assert(infile.good());
        auto data=btb::herding::parse_data(infile);
        fg::NativePicklesMotionSource motion{};
        std::vector<btb::herding::RetailEntityRecord32> animals{};
        std::string error{};
        std::srand(456);
        assert(motion.initialize(data,0,animals,error));
        constexpr int animal_indices[]{1,4,7};
        const btb::herding::EntityType species[]{
            btb::herding::EntityType::Sheep,
            btb::herding::EntityType::Rabbit,
            btb::herding::EntityType::Duck
        };
        fg::HerdingObservedFrame frame{};
        for(int stage=0;stage<2;++stage) {
            for(int n=0;n<3;++n) {
                auto& animal=animals[animal_indices[n]];
                animal.behavior_state=stage==0?10:20;
                const auto target=btb::herding::original_herding_current_home_target(
                    species[n],animal.behavior_state);
                assert(target);
                const auto origin=
                    btb::herding::original_herding_home_entity_origin_for_target(
                        species[n],*target);
                assert(origin);
                animal.x=origin->x;animal.y=origin->y;
                animal.x_float=static_cast<float>(animal.x);
                animal.y_float=static_cast<float>(animal.y);
                animal.movement_speed=0.0f;
            }
            assert(motion.advance(data,{}, {},animals,frame,error));
            assert(frame.home_motion_evidence.size()==3);
            int home_event_count=0;
            for(const auto& event:frame.events)
                if(event.kind==fg::HerdingObservedKind::ReachHomeWaypoint)
                    ++home_event_count;
            assert(home_event_count==3);
            for(const auto& probe:frame.home_motion_evidence) {
                assert(probe.arrived);
                const auto expected=
                    btb::herding::original_herding_home_route_motion(
                        probe.before);
                assert(expected);
                const auto& actual=frame.motion_records[probe.entity_index];
                assert(actual.x==expected->moved.x);
                assert(actual.y==expected->moved.y);
                assert(actual.movement_speed==expected->moved.movement_speed);
                assert(actual.animation_frame_countdown==
                       expected->moved.animation_frame_countdown);
            }
            animals=frame.motion_records;
        }
        // The actual retail behavior-99 animation continues after
        // an animal is delivered; each species has a separate frame
        // family, all driven by the original -5 / reload-150 timer.
        const int last_home_frames[]{5,9,11};
        const int first_home_frames[]{3,7,9};
        for(int n=0;n<3;++n) {
            auto& home=animals[animal_indices[n]];
            home.behavior_state=99;
            home.animation_frame=last_home_frames[n];
            home.animation_frame_countdown=5;
        }
        assert(motion.advance(data,{}, {},animals,frame,error));
        assert(frame.delivered_animation_evidence.size()==3);
        int sprite_index=0;
        for(const auto& proof:frame.delivered_animation_evidence) {
            const auto& updated=frame.motion_records[proof.entity_index];
            assert(updated.animation_frame==first_home_frames[sprite_index]);
            assert(updated.animation_frame_countdown==150);
            assert(updated.direction==4);
            assert(updated.behavior_state==99);
            assert(updated.x==proof.before.x && updated.y==proof.before.y);
            ++sprite_index;
        }
        animals=frame.motion_records;
        assert(motion.advance(data,{}, {},animals,frame,error));
        assert(frame.delivered_animation_evidence.size()==3);
        for(const auto& proof:frame.delivered_animation_evidence) {
            const auto& updated=frame.motion_records[proof.entity_index];
            assert(updated.animation_frame_countdown==145);
            assert(updated.x==proof.before.x && updated.y==proof.before.y);
        }
        animals=frame.motion_records;

        // Strict boundary: standing 10 world units away from the
        // original offset ACTOR target must never deliver early.
        auto& sheep=animals[1];
        sheep.behavior_state=10;
        const auto target=btb::herding::home_entrance_target(
            btb::herding::EntityType::Sheep);
        const auto origin=
            btb::herding::original_herding_home_entity_origin_for_target(
                btb::herding::EntityType::Sheep,target);
        assert(origin);
        sheep.x=origin->x+10;sheep.y=origin->y;
        sheep.x_float=static_cast<float>(sheep.x);
        sheep.y_float=static_cast<float>(sheep.y);
        assert(motion.advance(data,{}, {},animals,frame,error));
        for(const auto& event:frame.events)
            assert(!(event.kind==fg::HerdingObservedKind::ReachHomeWaypoint &&
                     event.entity_index==1));
        assert(motion.unload(error));
    }
    // Medium/Hard: source animal-FIRST rectangle collision really
    // releases an ordinary follower when Scruffty crosses it. Test
    // uses the actual initialized sprite rectangles and patrol.
    {
        std::ifstream infile(root/"Data"/"SubGame1"/"herd.txt");
        assert(infile.good());
        auto data=btb::herding::parse_data(infile);
        btb::retail::OriginalRetailRandom game_rng{321};
        fg::NativePicklesMotionSource native{game_rng};
        std::vector<btb::herding::RetailEntityRecord32> animals{};
        std::string error{};
        assert(native.initialize(data,1,animals,error));
        fg::HerdingObservedFrame frame{};
        assert(animals.back().entity_type()==
               btb::herding::EntityType::Scruffty);
        // Retail food bag, then retail first sheep attraction.
        animals[0].x=240; animals[0].y=414;
        animals[0].x_float=240.f;animals[0].y_float=414.f;
        assert(native.advance(data,{}, {},animals,frame,error));
        animals=frame.motion_records;
        animals[0].x=500; animals[0].y=520;
        animals[0].x_float=500.f;animals[0].y_float=520.f;
        animals[1].x=animals[0].x+
            (animals[1].source_right-animals[1].source_left)/2;
        animals[1].y=animals[0].y+
            (animals[0].source_bottom-animals[0].source_top)-20-
            (animals[1].source_bottom-animals[1].source_top)/2;
        animals[1].x_float=static_cast<float>(animals[1].x);
        animals[1].y_float=static_cast<float>(animals[1].y);
        assert(native.advance(data,{}, {},animals,frame,error));
        bool followed=false;
        for(const auto& event:frame.events)
            followed|=event.kind==fg::HerdingObservedKind::JoinFollower &&
                      event.entity_index==1;
        assert(followed);
        animals=frame.motion_records;
        const auto& dog=animals.back();
        animals[1].x=dog.x+5;
        animals[1].y=dog.y+5;
        animals[1].x_float=static_cast<float>(animals[1].x);
        animals[1].y_float=static_cast<float>(animals[1].y);
        assert(native.advance(data,{}, {},animals,frame,error));
        int scruffty_events=0;
        for(const auto& event:frame.events) {
            if(event.kind!=fg::HerdingObservedKind::ScrufftyCollision)
                continue;
            ++scruffty_events;
            assert(event.entity_index==1);
            assert(event.retail_trigger_confirmed);
            assert(event.retail_rect_contact);
            assert(event.random_mod_400>=0 &&
                   event.random_mod_400<400);
            assert(event.source_verified_sound_id==0x250 ||
                   event.source_verified_sound_id==0x251 ||
                   event.source_verified_sound_id==0x264 ||
                   event.source_verified_sound_id==0x266 ||
                   event.source_verified_sound_id==0x267);
            assert(event.retail_rect_contact->first==
                btb::herding::original_herding_entity_rect(
                    frame.motion_records[1]));
            assert(event.retail_rect_contact->second==
                btb::herding::original_herding_entity_rect(
                    frame.motion_records.back()));
        }
        assert(scruffty_events==1);
        // A removed ordinary follower cannot generate another
        // collision/release by overlapping the same dog next tick.
        animals=frame.motion_records;
        assert(native.advance(data,{}, {},animals,frame,error));
        for(const auto& event:frame.events)
            assert(!(event.kind==fg::HerdingObservedKind::ScrufftyCollision &&
                     event.entity_index==1));
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
