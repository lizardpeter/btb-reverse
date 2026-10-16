#pragma once

#include "btb/full_game_herding.hpp"
#include "btb/herding_behavior_bridge.hpp"
#include "btb/herding_navigation_boundary.hpp"
#include "btb/herding_retail_rect_contact.hpp"
#include "btb/herding_source_steering.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_roaming_loop.hpp"
#include "btb/herding_source_temporary_target.hpp"
#include "btb/herding_source_scruffty_patrol.hpp"
#include "btb/herding_source_far_roam.hpp"
#include "btb/herding_source_far_boundary_recovery.hpp"
#include "btb/herding_source_follower_approach.hpp"
#include "btb/herding_source_registration_point.hpp"
#include "btb/herding_source_home_motion.hpp"
#include "btb/herding_source_delivered_animation.hpp"
#include "btb/herding_source_ordinary_following.hpp"

#include <cstddef>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace btb::full_game {

// These event kinds are emitted only after the unfinished original
// continuous-motion/collision source has verified its retail trigger.
// The event processor may update food, followers and home routes, but
// it must not perform guessed proximity or frame-timer calculations.
enum class HerdingObservedKind {
    PickUpFood,
    JoinFollower,
    ScrufftyCollision,
    TrackedTargetArrived,
    EnterHomeRoute,
    ReachHomeWaypoint
};

struct HerdingCollisionEvidence {
    // Ordered exactly as the two pointers passed to retail 0x4169A0.
    // Swapping first/second can change the native result.
    herding::RetailCornerRect first{};
    herding::RetailCornerRect second{};
};

struct HerdingObservedEvent {
    HerdingObservedKind kind{};
    std::size_t entity_index{};
    herding::FoodType food{herding::FoodType::None};
    int random_mod_2{};
    int random_mod_400{};
    bool retail_trigger_confirmed{};
    // The original dog/animal collision must be independently
    // reproduced by the asymmetric 0x4169A0 corner check.
    std::optional<HerdingCollisionEvidence> retail_rect_contact{};
    // For PickUpFood: original rand()%400 X/Y pairs, indexed by the
    // original entity-table index, one for every current follower.
    std::vector<herding::WanderRandom> follower_release_random{};
    // Reserved for recovered Scruffty collision reaction audio once
    // the exact executable branch has been verified.
    int source_verified_sound_id{-1};
};

// Source's raw pre-polygon Pickles candidate. The independently
// recovered 0x418C2D..0x418CE4 validator checks that the source really
// performed the retail axis-slide response, not a rectangle clamp.
struct HerdingPicklesBoundaryEvidence {
    float attempted_x{};
    float attempted_y{};
    std::int32_t previous_x{};
    std::int32_t previous_y{};
    std::int32_t active_directional_axes{};
    std::int32_t source_mouse_navigation_mode_443a9c{};
};

struct HerdingSteeringEvidence {
    std::size_t entity_index{};
    float original_x{};
    float original_y{};
    std::int32_t native_heading_degrees{};
    // Once source target selection is recovered, include its exact
    // integer point to independently re-evaluate native 0x415D70.
    std::optional<herding::Vec2i> original_target{};
    std::int32_t original_actor_x{};
    std::int32_t original_actor_y{};
    float magnitude_before_step{};
    bool native_roaming_speed_ramp{};
};

struct HerdingRoamingRecoveryEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
    std::uint32_t rng_state_before{};
    std::uint32_t rng_state_after{};
    std::uint64_t attempts{};
};

// A narrow original free-roam branch: animals farther than 80 from
// Pickles and not following drift through the exact retail 8-way
// table when the new point stays inside main navigation polygon.
struct HerdingFarRoamEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
};

// Post-roam group 0 rejection is followed by one original 3-unit
// steering step toward the single group 3 return point, with
// previous integer coordinates restored before heading calculation.
struct HerdingFarBoundaryEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
    herding::Vec2i group3_target{};
};

// Full ordinary-follower formation branch from 0x417603..0x417994.
// The native entity world motion is independently replayed using
// Pickles' post-navigation pose and source per-frame follower ordinal.
struct HerdingOrdinaryFollowerEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
    herding::Vec2i formation_target{};
    int formation_ordinal{};
    bool held_position{};
};

// On the original tracked-target list, a follower runs the source
// <30 registration-point approach. The state-one arrival is emitted
// as an event after movement import, never forged as a source-state
// mutation. This before/after proof is checked transactionally.
struct HerdingTrackedApproachEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
    herding::Vec2i registered_point{};
    herding::Vec2i source_actor_anchor{};
    bool arrived{};
};

// Native first and second stage home routing: the original actor
// anchor, species waypoint, movement and strict pre-move <10 trigger.
// State changes remain owned by the transactional event bridge.
struct HerdingHomeMotionEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
    herding::Vec2i source_actor_anchor{};
    herding::Vec2i source_target{};
    bool arrived{};
};

struct HerdingDeliveredAnimationEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
};

// The original 0x416F57 branch executes before generic behavior
// dispatch whenever a herd animal has +0x58 > 0. Record its unmodified
// pre-branch entity so the verified adapter can independently check
// the exact 3-unit motion, resulting facing and 120-unit return gate.
struct HerdingTemporaryTargetEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
    bool source_returned_early{};
};

// Native dog branch 0x418F23 is deterministic given the entity,
// group-1 waypoint table and activity-global animation latch. Its
// source-controlled before/after states are independently replayed.
struct HerdingScrufftyPatrolEvidence {
    std::size_t entity_index{};
    herding::RetailEntityRecord32 before{};
    herding::RetailScrufftyPatrolState state_before{};
    herding::RetailScrufftyPatrolState state_after{};
};

struct HerdingObservedFrame {
    std::optional<HerdingPicklesBoundaryEvidence> pickles_boundary_evidence{};
    // Optional, branch-specific probes for native 0x416C45/0x416DCE
    // steering. These are not manufactured for unrelated AI branches.
    std::vector<HerdingSteeringEvidence> steering_evidence{};
    std::vector<HerdingRoamingRecoveryEvidence> recovery_evidence{};
    std::vector<HerdingFarRoamEvidence> far_roam_evidence{};
    std::vector<HerdingFarBoundaryEvidence> far_boundary_evidence{};
    std::vector<HerdingOrdinaryFollowerEvidence> ordinary_follower_evidence{};
    std::vector<HerdingTrackedApproachEvidence> tracked_approach_evidence{};
    std::vector<HerdingHomeMotionEvidence> home_motion_evidence{};
    std::vector<HerdingDeliveredAnimationEvidence> delivered_animation_evidence{};
    std::vector<HerdingTemporaryTargetEvidence> temporary_target_evidence{};
    std::optional<HerdingScrufftyPatrolEvidence> scruffty_patrol_evidence{};
    // Same ordering/entity IDs as the original 0x64-byte record table;
    // updates to behavior states and animation are ignored here because
    // the reconstructed behavior/compositor own those fields.
    std::vector<herding::RetailEntityRecord32> motion_records{};
    std::vector<HerdingObservedEvent> events{};
};

// SOURCE CONTRACT: the full UpdateHerdingActivity/UpdateHerdingAnimal
// movement, steering, point-in-polygon and collision arithmetic belongs
// to this component. The class below never generates contacts itself.
class OriginalHerdingMotionSource {
public:
    virtual ~OriginalHerdingMotionSource() = default;
    // Recoverable native-source transaction. An observed frame is
    // PROVISIONAL until HerdingEventSimulation independently verifies
    // motion, collision evidence and all behavior events. Derived
    // adapters can snapshot internal follower/target/RNG state here.
    // Default no-op keeps existing deterministic source test adapters.
    [[nodiscard]] virtual bool begin_frame(std::string& error) {
        error.clear();return true;
    }
    virtual void commit_frame() noexcept {}
    virtual void rollback_frame() noexcept {}
    [[nodiscard]] virtual bool initialize(
        const herding::Data& data,
        int difficulty,
        std::vector<herding::RetailEntityRecord32>& original_entities,
        std::string& error) = 0;
    [[nodiscard]] virtual bool advance(
        const herding::Data& data,
        const ActivityFrameInput& input,
        const herding::PicklesKeyboardMotion& pickles_motion,
        const std::vector<herding::RetailEntityRecord32>& source_entities,
        HerdingObservedFrame& observed,
        std::string& error) = 0;
    [[nodiscard]] virtual bool unload(std::string& error) = 0;
};

// Actual HerdingSimulationProvider implementation. It owns one recovered
// behavior state and integrates source-confirmed movement/contact events
// transactionally. A missing original motion source is a hard init error,
// never silent AI, placeholder entities, or fabricated home deliveries.
class HerdingEventSimulation final : public HerdingSimulationProvider {
public:
    explicit HerdingEventSimulation(
        std::unique_ptr<OriginalHerdingMotionSource> motion)
        : motion_(std::move(motion)) {}

    [[nodiscard]] bool initialize(
        const herding::Data& original_data,
        int difficulty,
        std::string& error) override;

    [[nodiscard]] bool advance(
        const ActivityFrameInput& input,
        const herding::PicklesKeyboardMotion& normalized_motion,
        HerdingScene& out,
        int& undelivered_animals,
        std::vector<Audio>& source_audio_events,
        std::string& error) override;

    [[nodiscard]] bool unload(std::string& error) override;

    [[nodiscard]] bool initialized() const noexcept {
        return state_ != nullptr;
    }

private:
    std::unique_ptr<OriginalHerdingMotionSource> motion_{};
    std::unique_ptr<herding::HerdingRecoveredBehavior> state_{};
    herding::Data data_{};
    // Original global 0x443B20 lives in process .data. Unlike the
    // waypoint index it is not reset by InitializeHerdingActivity.
    herding::RetailScrufftyPatrolState scruffty_patrol_state_{};
};

} // namespace btb::full_game
