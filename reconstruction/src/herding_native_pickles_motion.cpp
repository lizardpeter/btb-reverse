#include "btb/herding_native_pickles_motion.hpp"
#include "btb/herding_navigation_boundary.hpp"
#include "btb/original_herding_init.hpp"
#include "btb/herding_source_scruffty_patrol.hpp"
#include "btb/herding_source_pickles_animation.hpp"
#include "btb/herding_source_attraction.hpp"
#include "btb/herding_source_temporary_target.hpp"
#include "btb/herding_source_follower_approach.hpp"
#include "btb/herding_source_registration_point.hpp"
#include "btb/herding_source_home_motion.hpp"
#include "btb/herding_source_delivered_animation.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

namespace btb::full_game {
namespace {
struct HerdingOriginalInitServices final :
    original_herding_init::IO {
    explicit HerdingOriginalInitServices(const herding::Data& source)
        :data(source) {}
    const herding::Data& data;
    std::uint32_t handles{};

    herding::Data load_original_coordinate_data() override {
        return data;
    }
    void register_original_unload() override {}
    void set_global_game_volume(std::int32_t) override {
        // Audio routing is owned by the real host GameRoot, not this
        // source-only initialization adapter.
    }
    std::uint32_t load_registered_bitmap(
        std::uint32_t, std::uint32_t) override {
        // Nonzero native handle IDs satisfy the exact 18 registrations
        // of 0x00419600. Real BMP names/DirectDraw blits are instead
        // resolved by the source HerdingScene compositor and host.
        return ++handles;
    }
    void key_bitmap(
        std::uint32_t,std::uint32_t,std::uint32_t) override {}
    std::uint32_t original_random32() override {
        return static_cast<std::uint32_t>(std::rand());
    }
    float original_angle_to_radians(
        std::int32_t facing) override {
        return static_cast<float>(facing)*0.7853981633974483096f;
    }
};

herding::RetailEntityRecord32 convert_original_entity(
    const original_herding_init::Entity& entity) {
    herding::RetailEntityRecord32 out{};
    out.entity_id=entity.id;
    out.direction=entity.facing;
    out.type=entity.type;
    out.x=entity.x;
    out.y=entity.y;
    out.previous_x=entity.x;
    out.previous_y=entity.y;
    out.x_float=entity.precise_x;
    out.y_float=entity.precise_y;
    out.direction_degrees=static_cast<float>(entity.facing*45);
    out.animation_frame=entity.frame_counter;
    out.animation_frame_countdown=entity.timer;
    out.behavior_state=entity.behavior;
    out.source_left=0;
    out.source_top=0;
    out.source_right=entity.width;
    out.source_bottom=entity.height;
    out.surface_ptr32=entity.bitmap;
    out.movement_active=0;
    out.movement_speed=0.0f;
    return out;
}
} // namespace

bool NativePicklesMotionSource::initialize(
    const herding::Data& data,int difficulty,
    std::vector<herding::RetailEntityRecord32>& originals,
    std::string& error) {
    if(active_) {
        error="Pets Corner Pickles source already initialized";
        return false;
    }
    try {
        original_herding_init::State native{};
        native.difficulty=difficulty;
        HerdingOriginalInitServices io(data);
        original_herding_init::initialize_herding(native,io);
        if(!native.initialized||native.entities.empty()||
           io.handles!=18)
            throw std::runtime_error("incomplete source Pets Corner population");
        originals.clear();
        originals.reserve(native.entities.size());
        for(const auto& item:native.entities)
            originals.push_back(convert_original_entity(item));
        active_=true;
        selected_food_=herding::FoodType::None;
        follower_indices_.clear();
        tracked_slots_=herding::RetailTrackedTargetSlots{};
        // Original InitializeHerdingActivity 0x00419600 writes ZERO
        // to global 0x50B31C. It is an independent signed counter,
        // not the number of animals remaining in the scene.
        remaining_to_register_=0;
        // The original initializer resets its patrol waypoint, but not
        // the process-global frame movement latch 0x443B20.
        scruffty_patrol_state_.waypoint_index=0;
        error.clear();
        return true;
    } catch(const std::exception& e) {
        originals.clear();
        error=std::string("original Pets Corner initialization: ")+e.what();
        return false;
    }
}

bool NativePicklesMotionSource::advance(
    const herding::Data& data,
    const ActivityFrameInput&,
    const herding::PicklesKeyboardMotion& motion,
    const std::vector<herding::RetailEntityRecord32>& source,
    HerdingObservedFrame& observed,std::string& error) {
    if(!active_||source.empty()||
       source.front().entity_type()!=herding::EntityType::FarmerPickles) {
        error="native Pickles source uninitialized or missing original hero";
        return false;
    }
    observed={};
    observed.motion_records=source;

    // UpdateHerdingActivity 0x418F23..0x41908D moves Scruffty once
    // per actual game frame, including frames where Pickles is idle.
    // Reuse the independently recovered patrol kernel and supply its
    // before/after evidence to HerdingEventSimulation for validation.
    // The other animals remain stationary until their source dispatch
    // and follower branches are connected; no delivery is invented.
    for(std::size_t i=0;i<source.size();++i) {
        if(source[i].entity_type()!=herding::EntityType::Scruffty)
            continue;
        if(observed.scruffty_patrol_evidence) {
            error="multiple retail Scruffty entities in Pets Corner";
            return false;
        }
        const auto before=scruffty_patrol_state_;
        const auto step=herding::original_scruffty_patrol_step(
            source[i],herding::scruffty_patrol_path(data),before);
        if(!step) {
            error="original 0x418F23 Scruffty patrol source invalid";
            return false;
        }
        observed.motion_records[i]=step->animal;
        observed.scruffty_patrol_evidence=
            HerdingScrufftyPatrolEvidence{i,source[i],before,step->state};
        scruffty_patrol_state_=step->state;
    }

    const auto source_food_pickup=[&] {
        // Source 0x004182B0 checks every original food hotspot using
        // the final integer Pickles position; it does not require
        // an invented click or screen-relative cursor hitbox.
        constexpr std::array<herding::FoodType,3> foods{{
            herding::FoodType::DuckFood,
            herding::FoodType::RabbitFood,
            herding::FoodType::SheepFood
        }};
        const auto& pickles=observed.motion_records.front();
        for(const auto food:foods) {
            if(food==selected_food_ ||
               !herding::food_pickup_in_range(
                   food,pickles.x,pickles.y))continue;
            HerdingObservedEvent event{};
            event.kind=HerdingObservedKind::PickUpFood;
            event.food=food;
            event.retail_trigger_confirmed=true;
            event.random_mod_2=std::rand()%2;
            // Original food changes release ALL existing ordinary
            // followers in first-vacant slot order. After the voice
            // selection RNG, retail consumes two rand()%400 values
            // per follower. The event bridge validates and commits
            // the exact 200-frame wander target/clear transition.
            event.follower_release_random.resize(source.size());
            for(const auto index:follower_indices_) {
                event.follower_release_random[index]={
                    std::rand()%400,std::rand()%400};
            }
            observed.events.push_back(std::move(event));
            follower_indices_.clear();
            selected_food_=food;
            break;
        }
    };
    // In retail, the food selection loop precedes the Pickles
    // walk/idle animation branch. Use that same order on both idle
    // and moving frames to switch between carried/empty sprite rows.
    const auto animate_pickles=[&]() -> bool {
        auto& hero=observed.motion_records.front();
        const auto animation=herding::original_pickles_keyboard_animation_step(
            hero.animation_frame,hero.animation_frame_countdown,
            motion.movement_mask!=0,selected_food_);
        if(!animation) {
            error="original 0x418CE4 Pickles animation source invalid";
            return false;
        }
        hero.animation_frame=animation->frame;
        hero.animation_frame_countdown=animation->countdown;
        return true;
    };
    // Original dispatch runs temporary escape targeting BEFORE
    // ordinary animal roaming/attraction. Released animals therefore
    // move in the real world until their source <120 return condition
    // clears the timer. Other uncovered animal AI stays unmodified.
    const auto source_far_animal_roam=[&]() -> bool {
        const auto& hero=observed.motion_records.front();
        const auto nav=data.retail_transformed_group0();
        for(std::size_t i=1;i<observed.motion_records.size();++i) {
            auto before=observed.motion_records[i];
            if(!herding::is_herd_animal(before.entity_type()))
                continue;

            // Original UpdateHerdingAnimal 0x416F57 gives an animal
            // with a positive temporary target exclusive steering
            // priority over food attraction, following and free roam.
            // This state is set when Pickles changes food, and must
            // actually move the released animal in subsequent frames.
            if(before.temporary_target_timer>0) {
                const auto recovery=
                    herding::original_herding_temporary_target_update(before);
                if(!recovery) {
                    error="retail released-animal temporary target invalid";
                    return false;
                }
                observed.motion_records[i]=recovery->moved;
                if(recovery->branch_taken) {
                    observed.temporary_target_evidence.push_back({
                        i,before,recovery->early_return});
                    // Both the <120 arrival and distant +0x58
                    // branches RETURN from original animal update.
                    // Never combine with attraction or generic roam.
                    continue;
                }
                // Source countdown 1 -> 0 resumes ordinary animal
                // dispatch on this same frame. The motion validator
                // still independently verifies any ensuing far-roam.
                if(!recovery->continue_normal_update ||
                   recovery->moved.temporary_target_timer!=0) {
                    error="original target timer expiration differs from source";
                    return false;
                }
                before=recovery->moved;
            }

            if(before.behavior_state==99) {
                const auto animation=
                    herding::original_herding_delivered_animal_animation(
                        before);
                if(!animation) {
                    error="original delivered animal sprite update invalid";
                    return false;
                }
                observed.motion_records[i]=animation->updated;
                observed.delivered_animation_evidence.push_back({i,before});
                continue;
            }
            if(before.behavior_state==1) {
                // Original state-one dispatch allocates route 10+N on
                // a later update than the <30 tracked-point arrival.
                HerdingObservedEvent route{};
                route.kind=HerdingObservedKind::EnterHomeRoute;
                route.entity_index=i;
                route.retail_trigger_confirmed=true;
                observed.events.push_back(std::move(route));
                continue;
            }
            // Retail states 10..14 steer to each species' entrance;
            // states 20..24 steer to the selected animal's final home
            // waypoint. All share the 0x415D70 native heading and
            // source's signed actor origin rather than sprite top-left.
            if((before.behavior_state>=10 &&
                before.behavior_state<=14) ||
               (before.behavior_state>=20 &&
                before.behavior_state<=24)) {
                const auto home_step=
                    herding::original_herding_home_route_motion(before);
                if(!home_step) {
                    error="retail home entrance/waypoint motion invalid";
                    return false;
                }
                observed.motion_records[i]=home_step->moved;
                observed.home_motion_evidence.push_back({
                    i,before,home_step->source_anchor,
                    home_step->source_target,home_step->source_arrived});
                if(home_step->source_arrived) {
                    HerdingObservedEvent arrival{};
                    arrival.kind=HerdingObservedKind::ReachHomeWaypoint;
                    arrival.entity_index=i;
                    arrival.retail_trigger_confirmed=true;
                    observed.events.push_back(std::move(arrival));
                }
                continue;
            }
            if(before.behavior_state!=0)continue;
            const auto original_point=
                herding::original_herding_species_registration_point(
                    before.entity_type());
            const auto original_anchor=
                herding::original_herding_animal_tracking_anchor(before);
            if(!original_point || !original_anchor) {
                error="retail follower source lacks original tracking coordinates";
                return false;
            }

            // 0x41721E scans the tracked-target slots BEFORE ordinary
            // follower membership. Membership survives even if a
            // later food selection releases ordinary followers.
            if(tracked_slots_.contains(static_cast<int>(i))) {
                const auto approach=herding::original_herding_follower_approach(
                    before,*original_point,*original_anchor);
                if(!approach) {
                    error="original 0x4172B0 tracked-point movement invalid";
                    return false;
                }
                observed.tracked_approach_evidence.push_back({
                    i,before,*original_point,*original_anchor,
                    approach->reached_tracked_point});
                if(approach->reached_tracked_point) {
                    if(!tracked_slots_.remove(static_cast<int>(i))) {
                        error="retail tracked arrival lost source membership";
                        return false;
                    }
                    // Arrival consumes one discarded original rand.
                    static_cast<void>(std::rand());
                    HerdingObservedEvent arrival{};
                    arrival.kind=HerdingObservedKind::TrackedTargetArrived;
                    arrival.entity_index=i;
                    arrival.retail_trigger_confirmed=true;
                    observed.events.push_back(std::move(arrival));
                } else {
                    observed.motion_records[i]=approach->updated;
                }
                continue;
            }

            const auto existing=std::find(
                follower_indices_.begin(),follower_indices_.end(),i);
            const bool following=existing!=follower_indices_.end();

            if(following) {
                herding::FollowerList ordinary{};
                for(const auto slot:follower_indices_) {
                    if(!ordinary.add(static_cast<int>(slot))) {
                        error="original ordinary follower list invalid";
                        return false;
                    }
                }
                const auto admission=tracked_slots_.admit_from_follower(
                    static_cast<int>(i),*original_anchor,*original_point,
                    ordinary,remaining_to_register_);
                if(admission.status==
                    herding::RetailTrackedAdmissionStatus::Inserted) {
                    // The retail registration branch returns from
                    // this entity update; first approach is next tick.
                    continue;
                }
                if(admission.status==
                    herding::RetailTrackedAdmissionStatus::InvalidInput ||
                   admission.status==
                    herding::RetailTrackedAdmissionStatus::MissingOrdinaryFollower) {
                    error="source registered animal to invalid tracked list";
                    return false;
                }
            }
            const auto contact=herding::original_herding_food_attraction_contact(
                before,hero,selected_food_,following);
            if(!contact) {
                error="invalid original herd-animal 80px attraction probe";
                return false;
            }
            if(*contact) {
                if(follower_indices_.size()>=20) {
                    error="original ordinary follower table is full";
                    return false;
                }
                HerdingObservedEvent joined{};
                joined.kind=HerdingObservedKind::JoinFollower;
                joined.entity_index=i;
                joined.retail_trigger_confirmed=true;
                joined.random_mod_2=std::rand()%2;
                observed.events.push_back(std::move(joined));
                follower_indices_.push_back(i);
                // The follower's close-range physics will be brought
                // online separately; do not substitute default drift.
                continue;
            }
            if(following)continue;
            const auto step=herding::original_far_untracked_animal_roam_step(
                before,hero,nav);
            switch(step.coverage) {
            case herding::RetailFarAnimalRoamCoverage::NotCovered:
            case herding::RetailFarAnimalRoamCoverage::RequiresOriginalBoundaryRecovery:
                break;
            case herding::RetailFarAnimalRoamCoverage::AdvancedWithinPolygon:
                observed.motion_records[i]=step.animal;
                observed.far_roam_evidence.push_back({i,before});
                break;
            case herding::RetailFarAnimalRoamCoverage::InvalidInput:
                error="original far-roam source polygon/animal state invalid";
                return false;
            }
        }
        return true;
    };
    if(motion.movement_mask==0) {
        source_food_pickup();
        if(!animate_pickles()||!source_far_animal_roam())return false;
        error.clear();
        return true;
    }

    auto& player=observed.motion_records.front();
    const float attempt_x=player.x_float+motion.delta_x;
    const float attempt_y=player.y_float+motion.delta_y;
    const auto before_x=player.x,before_y=player.y;
    const int axes=(motion.delta_x!=0.0f?1:0)+
                   (motion.delta_y!=0.0f?1:0);
    constexpr int original_mouse_navigation_mode=0;
    const auto result=herding::retail_herding_navigation_boundary_step(
        data.retail_transformed_group0(),
        attempt_x,attempt_y,before_x,before_y,
        herding::retail_herding_axis_recovery_enabled(
            axes,original_mouse_navigation_mode));
    if(!result) {
        error="Pickles retail navigation polygon/source division failed";
        return false;
    }
    observed.pickles_boundary_evidence={
        attempt_x,attempt_y,before_x,before_y,
        axes,original_mouse_navigation_mode
    };
    player.previous_x=before_x;
    player.previous_y=before_y;
    player.x_float=result->x;
    player.y_float=result->y;
    player.x=static_cast<std::int32_t>(result->x);
    player.y=static_cast<std::int32_t>(result->y);
    player.movement_direction_mask=motion.movement_mask;
    if(const auto facing=herding::pickles_facing_direction_from_mask(
            motion.movement_mask)) {
        player.direction=*facing;
        player.direction_degrees=static_cast<float>(*facing*45);
    }
    player.movement_active=1;

    // Source-confirmed food pickup, animal attraction and Scruffty
    // patrol are active; follower steering/deliveries are not faked.
    source_food_pickup();
    if(!animate_pickles()||!source_far_animal_roam())return false;
    error.clear();
    return true;
}

bool NativePicklesMotionSource::unload(std::string& error) {
    active_=false;
    selected_food_=herding::FoodType::None;
    follower_indices_.clear();
    tracked_slots_=herding::RetailTrackedTargetSlots{};
    remaining_to_register_=0;
    error.clear();
    return true;
}
} // namespace btb::full_game
