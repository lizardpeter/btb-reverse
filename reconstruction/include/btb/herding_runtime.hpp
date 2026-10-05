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

    // 4..6 are referenced by some retail collision/state paths but are not
    // assigned semantic names until their construction/use is fully closed.
    Scruffty = 7,

    Trailer1 = 12,
    TravisCab = 13,
    Trailer2 = 14,
    GateLeft = 15,
    GateRight = 16,
    Inactive = 17,
};

enum class FoodType : std::int32_t {
    None = -1,
    DuckFood = 0,
    RabbitFood = 1,
    SheepFood = 2,
};

[[nodiscard]] constexpr bool is_herd_animal(EntityType type) noexcept {
    return type == EntityType::Sheep ||
           type == EntityType::Rabbit ||
           type == EntityType::Duck;
}

[[nodiscard]] constexpr FoodType required_food_for(EntityType type) noexcept {
    switch (type) {
        case EntityType::Sheep: return FoodType::SheepFood;
        case EntityType::Rabbit: return FoodType::RabbitFood;
        case EntityType::Duck: return FoodType::DuckFood;
        default: return FoodType::None;
    }
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
    float x_float{};                   // +0x1C
    float y_float{};                   // +0x20
    std::int32_t unknown_24{};         // +0x24
    std::int32_t unknown_28{};         // +0x28
    std::int32_t animation_timer{};    // +0x2C
    std::int32_t type{};               // +0x30
    std::int32_t source_left{};        // +0x34
    std::int32_t source_top{};         // +0x38
    std::int32_t source_right{};       // +0x3C
    std::int32_t source_bottom{};      // +0x40
    std::uint32_t surface_ptr32{};     // +0x44
    float movement_speed{};            // +0x48
    float movement_scalar{};           // +0x4C
    std::int32_t behavior_state{};      // +0x50
    std::int32_t unknown_54{};         // +0x54
    std::int32_t target_flag_or_timer{};// +0x58
    std::int32_t target_x{};           // +0x5C
    std::int32_t target_y{};           // +0x60

    [[nodiscard]] EntityType entity_type() const noexcept {
        return static_cast<EntityType>(type);
    }

    [[nodiscard]] BehaviorClass behavior_class() const noexcept {
        return classify_behavior_state(behavior_state);
    }
};

static_assert(sizeof(RetailEntityRecord32) == 0x64);
static_assert(offsetof(RetailEntityRecord32, type) == 0x30);
static_assert(offsetof(RetailEntityRecord32, surface_ptr32) == 0x44);
static_assert(offsetof(RetailEntityRecord32, behavior_state) == 0x50);

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

} // namespace btb::herding
