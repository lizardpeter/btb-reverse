#pragma once

#include "btb/full_game_herding.hpp"
#include "btb/herding_behavior_bridge.hpp"
#include "btb/herding_navigation_boundary.hpp"
#include "btb/herding_retail_rect_contact.hpp"

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

struct HerdingObservedFrame {
    std::optional<HerdingPicklesBoundaryEvidence> pickles_boundary_evidence{};
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
};

} // namespace btb::full_game
