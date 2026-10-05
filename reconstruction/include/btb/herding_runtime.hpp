#pragma once

#include "btb/herding_data.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::herding {

// Exact retail type values from the 0x64-byte entity table.
enum class EntityType : std::int32_t {
    FarmerPickles = 0,
    Sheep = 1,
    Rabbit = 2,
    Duck = 3,

    // Exhaustive constructor/type-field writes in InitializeHerdingActivity
    // never create these IDs. They remain in generic retail branches only.
    DormantLegacy4 = 4,
    DormantLegacy5 = 5,
    DormantLegacy6 = 6,

    Scruffty = 7,

    Trailer1 = 12,
    TravisCab = 13,
    Trailer2 = 14,
    GateLeft = 15,
    GateRight = 16,
    Inactive = 17,
};

[[nodiscard]] constexpr bool entity_type_has_retail_constructor(
    EntityType type) noexcept {
    switch (type) {
        case EntityType::FarmerPickles:
        case EntityType::Sheep:
        case EntityType::Rabbit:
        case EntityType::Duck:
        case EntityType::Scruffty:
        case EntityType::Trailer1:
        case EntityType::TravisCab:
        case EntityType::Trailer2:
        case EntityType::GateLeft:
        case EntityType::GateRight:
        case EntityType::Inactive:
            return true;
        case EntityType::DormantLegacy4:
        case EntityType::DormantLegacy5:
        case EntityType::DormantLegacy6:
            return false;
    }
    return false;
}

enum class FoodType : std::int32_t {
    None = -1,
    DuckFood = 0,
    RabbitFood = 1,
    SheepFood = 2,
};

enum class HomeKind : std::int32_t {
    Pen,
    RabbitHutches,
    Pond,
};

[[nodiscard]] constexpr HomeKind home_for(EntityType type) noexcept {
    switch (type) {
        case EntityType::Sheep: return HomeKind::Pen;
        case EntityType::Rabbit: return HomeKind::RabbitHutches;
        case EntityType::Duck: return HomeKind::Pond;
        default: return HomeKind::Pen;
    }
}

// First-stage home entrance targets used by behavior states 10..15.
// Retail stores X as 679/855/1080 and subtracts 40 before steering.
[[nodiscard]] constexpr Vec2i home_entrance_target(EntityType type) noexcept {
    switch (type) {
        case EntityType::Sheep: return {639, 135};
        case EntityType::Rabbit: return {815, 91};
        case EntityType::Duck: return {1040, 264};
        default: return {-1, -1};
    }
}

// Final, post-initializer second-stage home waypoints used by behavior
// states 20..24. The retail initializer starts from 15 raw points at
// 0x0050B320, subtracts per-species sprite-anchor X values (40/48/50), then
// applies rabbit (-35,+10) and duck (-27,+36) adjustments.
//
// UpdateHerdingAnimal's index arithmetic:
//   table_index = species_index * 5 + (behavior_state - 20)
inline constexpr std::array<std::array<Vec2i, 5>, 3>
kHomeEntryWaypoints{{
    // Sheep / pen
    {{{503, 142}, {560, 128}, {707, 99}, {618, 118}, {656, 110}}},
    // Rabbit / hutches
    {{{738, 106}, {840, 77}, {759, 94}, {783, 98}, {807, 107}}},
    // Duck / pond
    {{{933, 255}, {1073, 331}, {1011, 337}, {964, 278}, {1022, 286}}},
}};

[[nodiscard]] constexpr std::optional<Vec2i> home_entry_target(
    EntityType type,
    std::int32_t behavior_state) noexcept {

    if (behavior_state < 20 || behavior_state > 24) {
        return std::nullopt;
    }

    std::size_t species{};
    switch (type) {
        case EntityType::Sheep: species = 0; break;
        case EntityType::Rabbit: species = 1; break;
        case EntityType::Duck: species = 2; break;
        default: return std::nullopt;
    }

    return kHomeEntryWaypoints[species][
        static_cast<std::size_t>(behavior_state - 20)];
}

inline constexpr float kHomeRouteArrivalDistance = 10.0F;

struct HomeRouteArrivalStep {
    std::int32_t next_behavior_state{};
    std::int32_t next_undelivered_count{};
    bool delivered{};
};

// Exact normal-retail arrival transitions. First-stage states 10..14 become
// 20..24 when distance to the species entrance falls below 10.0. Second-stage
// states 20..24 become 99 and decrement the shared undelivered-animal count.
//
// State handlers 15/25 exist in the retail jump-table machinery, but normal
// Easy/Medium/Hard population allocation never produces them.
[[nodiscard]] constexpr std::optional<HomeRouteArrivalStep>
home_route_arrival_step(
    std::int32_t behavior_state,
    std::int32_t undelivered_count) noexcept {

    if (behavior_state >= 10 && behavior_state <= 14) {
        return HomeRouteArrivalStep{
            behavior_state + 10,
            undelivered_count,
            false,
        };
    }

    if (behavior_state >= 20 && behavior_state <= 24) {
        return HomeRouteArrivalStep{
            99,
            undelivered_count - 1,
            true,
        };
    }

    return std::nullopt;
}

[[nodiscard]] constexpr std::int32_t animals_per_species(
    std::int32_t difficulty_index) noexcept {
    return difficulty_index + 3;
}

[[nodiscard]] constexpr bool scruffty_enabled(
    std::int32_t difficulty_index) noexcept {
    return difficulty_index > 0;
}

[[nodiscard]] constexpr std::int32_t initial_undelivered_animal_count(
    std::int32_t difficulty_index) noexcept {
    return 3 * animals_per_species(difficulty_index);
}

[[nodiscard]] constexpr bool is_herd_animal(EntityType type) noexcept {
    return type == EntityType::Sheep ||
           type == EntityType::Rabbit ||
           type == EntityType::Duck;
}

struct SoundPair {
    std::int32_t a{};
    std::int32_t b{};
};

struct FoodPickupSoundChoices {
    std::int32_t pickles_line{};
    std::int32_t travis_line{};
};

[[nodiscard]] constexpr FoodType required_food_for(EntityType type) noexcept {
    switch (type) {
        case EntityType::Sheep: return FoodType::SheepFood;
        case EntityType::Rabbit: return FoodType::RabbitFood;
        case EntityType::Duck: return FoodType::DuckFood;
        default: return FoodType::None;
    }
}

// When an animal first joins Pickles, the retail code selects one of two
// species/food-specific lines using 0x249 + food_index*2 + rand()%2.
// Picking a food bag chooses between a Pickles line (0x246+food) and a
// Travis line (0x261+food).
[[nodiscard]] constexpr FoodPickupSoundChoices food_pickup_sound_ids(
    FoodType food) noexcept {
    switch (food) {
        case FoodType::DuckFood: return {582, 609};   // PC_PIC_02 / PC_TR_02
        case FoodType::RabbitFood: return {583, 610}; // PC_PIC_03 / PC_TR_03
        case FoodType::SheepFood: return {584, 611};  // PC_PIC_04 / PC_TR_04
        default: return {-1, -1};
    }
}

[[nodiscard]] constexpr SoundPair attraction_sound_ids(FoodType food) noexcept {
    switch (food) {
        case FoodType::DuckFood: return {585, 586};   // PC_PIC_05 / PC_PIC_07
        case FoodType::RabbitFood: return {587, 588}; // PC_PIC_06 / PC_PIC_08
        case FoodType::SheepFood: return {589, 590};  // PC_PIC_09 / PC_PIC_11
        default: return {-1, -1};
    }
}

[[nodiscard]] constexpr std::int32_t species_home_route_sound_id(
    EntityType type) noexcept {
    switch (type) {
        case EntityType::Sheep: return 596;  // PC_PIC_16
        case EntityType::Rabbit: return 597; // PC_PIC_17
        case EntityType::Duck: return 598;   // PC_PIC_18
        default: return -1;
    }
}

enum class HomeRouteGateTrigger {
    None,
    LeftGate,
    RightGate,
};

inline constexpr std::int32_t kHomeRouteGateAnimationTimer = 50;

struct BeginHomeRouteStep {
    std::int32_t assigned_behavior_state{-1};
    std::int32_t next_species_route_counter{};
    bool final_species_animal{};
    std::int32_t sound_id{-1};
    HomeRouteGateTrigger gate_trigger{HomeRouteGateTrigger::None};
    std::int32_t gate_animation_timer{};
};

// Exact state-1 allocation at 0x00416B70 for the shipped retail difficulty
// domain (0=Easy, 1=Medium, 2=Hard). Each species owns a counter initialized
// to zero. The current counter selects state 10+counter, then increments.
// The last animal of that species plays PC_PIC_16/17/18.
//
// With retail difficulties the highest assigned first-stage state is 14
// (Hard has five animals/species). Handler state 15 exists in the jump table
// but is not reached by normal shipped population counts.
[[nodiscard]] constexpr std::optional<BeginHomeRouteStep>
begin_home_route_step(
    EntityType type,
    std::int32_t current_species_route_counter,
    std::int32_t difficulty_index) noexcept {

    if (!is_herd_animal(type) ||
        difficulty_index < 0 || difficulty_index > 2) {
        return std::nullopt;
    }

    const auto count = animals_per_species(difficulty_index);
    if (current_species_route_counter < 0 ||
        current_species_route_counter >= count) {
        return std::nullopt;
    }

    const auto next = current_species_route_counter + 1;
    const bool final = next == count;

    HomeRouteGateTrigger gate = HomeRouteGateTrigger::None;
    if (final) {
        if (type == EntityType::Sheep) {
            gate = HomeRouteGateTrigger::LeftGate;
        } else if (type == EntityType::Rabbit) {
            gate = HomeRouteGateTrigger::RightGate;
        }
    }

    return BeginHomeRouteStep{
        10 + current_species_route_counter,
        next,
        final,
        final ? species_home_route_sound_id(type) : -1,
        gate,
        gate == HomeRouteGateTrigger::None
            ? 0
            : kHomeRouteGateAnimationTimer,
    };
}

inline constexpr Vec2i kAnimalExclusionEscapeTarget{650, 486};

// Exact herd.txt group semantics now closed from runtime consumers.
[[nodiscard]] const std::vector<Vec2i>& animal_exclusion_polygon(
    const Data& data);

[[nodiscard]] std::optional<Vec2i> animal_navigation_recovery_target(
    const Data& data) noexcept;

[[nodiscard]] constexpr Vec2i farmer_pickles_start(
    const Data& data) noexcept {
    // InitializeHerdingActivity reads fixed setup pair #3 directly.
    return data.setup_positions[kFarmerPicklesSetupPositionIndex];
}

enum class CompletionAction : std::int32_t {
    None,
    PlayFinalLine,
    ExitToPlayAgain,
};

struct CompletionStep {
    std::int32_t stage{};
    CompletionAction action{CompletionAction::None};
    std::int32_t sound_id{-1};
};

[[nodiscard]] constexpr CompletionStep herding_completion_step(
    std::int32_t undelivered_animals,
    std::int32_t completion_stage,
    bool any_managed_sound_playing,
    std::int32_t random_bit) noexcept {

    if (undelivered_animals > 0 || any_managed_sound_playing) {
        return {completion_stage, CompletionAction::None, -1};
    }
    if (completion_stage == 0) {
        return {
            1,
            CompletionAction::PlayFinalLine,
            599 + (random_bit & 1), // PC_PIC_19 / PC_PIC_20
        };
    }
    if (completion_stage == 1) {
        return {1, CompletionAction::ExitToPlayAgain, -1};
    }
    return {completion_stage, CompletionAction::None, -1};
}

enum class BehaviorClass : std::int32_t {
    FreeRoamFollowAndCollision,
    BeginHomeRoute,
    ApproachHomeEntrance,
    EnterHome,
    Delivered,
    RetailNoOp,
};

[[nodiscard]] constexpr BehaviorClass classify_behavior_state(
    std::int32_t state) noexcept {
    if (state == 0) return BehaviorClass::FreeRoamFollowAndCollision;
    if (state == 1) return BehaviorClass::BeginHomeRoute;
    if (state >= 10 && state <= 15) return BehaviorClass::ApproachHomeEntrance;
    if (state >= 20 && state <= 25) return BehaviorClass::EnterHome;
    if (state == 99) return BehaviorClass::Delivered;
    return BehaviorClass::RetailNoOp;
}

// A byte-for-byte model of the original 32-bit retail entity record.
// COM pointers are kept as uint32_t so the layout remains exactly 0x64 bytes
// even when this reconstruction is compiled on a 64-bit host.
struct RetailEntityRecord32 {
    std::int32_t entity_id{};          // +0x00
    std::int32_t animation_frame{};    // +0x04
    std::int32_t direction{};          // +0x08
    std::int32_t x{};                  // +0x0C
    std::int32_t y{};                  // +0x10
    std::int32_t previous_x{};         // +0x14
    std::int32_t previous_y{};         // +0x18
    float direction_degrees{};         // +0x1C (direction * 45.0)
    float x_float{};                   // +0x20
    float y_float{};                   // +0x24
    std::int32_t movement_input_mask{};// +0x28 (Pickles direction bits)
    std::int32_t animation_timer{};    // +0x2C
    std::int32_t type{};               // +0x30
    std::int32_t source_left{};        // +0x34
    std::int32_t source_top{};         // +0x38
    std::int32_t source_right{};       // +0x3C
    std::int32_t source_bottom{};      // +0x40
    std::uint32_t surface_ptr32{};      // +0x44
    std::int32_t movement_active{};      // +0x48 (retail writes 0/1)
    float movement_speed{};              // +0x4C
    std::int32_t behavior_state{};        // +0x50
    std::int32_t unused_54{};            // +0x54 (no retail references)
    std::int32_t temporary_target_timer{};// +0x58
    std::int32_t target_x{};              // +0x5C
    std::int32_t target_y{};              // +0x60

    [[nodiscard]] EntityType entity_type() const noexcept {
        return static_cast<EntityType>(type);
    }

    [[nodiscard]] BehaviorClass behavior_class() const noexcept {
        return classify_behavior_state(behavior_state);
    }
};

static_assert(sizeof(RetailEntityRecord32) == 0x64);
static_assert(offsetof(RetailEntityRecord32, direction_degrees) == 0x1C);
static_assert(offsetof(RetailEntityRecord32, x_float) == 0x20);
static_assert(offsetof(RetailEntityRecord32, y_float) == 0x24);
static_assert(offsetof(RetailEntityRecord32, movement_input_mask) == 0x28);
static_assert(offsetof(RetailEntityRecord32, type) == 0x30);
static_assert(offsetof(RetailEntityRecord32, surface_ptr32) == 0x44);
static_assert(offsetof(RetailEntityRecord32, movement_active) == 0x48);
static_assert(offsetof(RetailEntityRecord32, movement_speed) == 0x4C);
static_assert(offsetof(RetailEntityRecord32, behavior_state) == 0x50);
static_assert(offsetof(RetailEntityRecord32, temporary_target_timer) == 0x58);

// Retail uses a 20-entry int32 entity-index list at 0x0050AF14 for animals
// currently following Farmer Pickles. -1 marks an unused slot.
class FollowerList {
public:
    static constexpr std::size_t kCapacity = 20;

    FollowerList() noexcept;

    [[nodiscard]] bool contains(std::int32_t entity_index) const noexcept;
    [[nodiscard]] bool add(std::int32_t entity_index) noexcept;
    [[nodiscard]] bool remove(std::int32_t entity_index) noexcept;
    [[nodiscard]] std::size_t count() const noexcept;
    [[nodiscard]] const std::array<std::int32_t, kCapacity>& slots() const noexcept {
        return slots_;
    }

private:
    std::array<std::int32_t, kCapacity> slots_{};
};

// Herding group 1 is consumed by Scruffty as a cyclic waypoint list.
[[nodiscard]] const std::vector<Vec2i>& scruffty_patrol_path(const Data& data);

// Exact non-audio part of the Scruffty collision branch. If the animal is
// following Pickles, Scruffty removes it from the follower list and gives it
// a 200-tick horizontal wandering target. random_mod_400 must be 0..399.
[[nodiscard]] bool apply_scruffty_distraction(
    RetailEntityRecord32& animal,
    FollowerList& followers,
    std::int32_t entity_index,
    std::int32_t random_mod_400) noexcept;

// On food change retail releases all followers and gives each one a random
// temporary target. Each random argument is the already-reduced rand()%400.
[[nodiscard]] constexpr Vec2i food_change_wander_target(
    std::int32_t random_x_mod_400,
    std::int32_t random_y_mod_400) noexcept {
    return {286 + random_x_mod_400, 450 + random_y_mod_400};
}

[[nodiscard]] bool release_follower_for_food_change(
    RetailEntityRecord32& animal,
    FollowerList& followers,
    std::int32_t entity_index,
    std::int32_t random_x_mod_400,
    std::int32_t random_y_mod_400) noexcept;

} // namespace btb::herding
