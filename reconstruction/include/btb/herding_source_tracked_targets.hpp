#pragma once

#include "btb/herding_runtime.hpp"

#include <array>
#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Two independent original arrays, each 20 signed DWORD slots:
//  - 0x50AF14..0x50AF64: ordinary followers of Farmer Pickles
//  - 0x50AF78..0x50AFC8: registered tracked-target/home approach.
// The source tests the SECOND array before the FIRST at
// 0x41721E..0x417249. They must never be merged.
//
// When an ordinary follower is within strictly 150 units of the
// species point (0x50B2F4 + entity_type*8), 0x417290..0x4172AB
// searches first vacant tracked slot. 0x417586 then records its
// original table index and decrements global remaining-to-register
// at 0x50B31C. Reaching <30 later removes that entity ONLY from
// tracked slots and changes behavior state to 1.
inline constexpr std::int64_t kRetailTrackedAdmissionRadiusSquared=22500;

enum class RetailTrackedRoute {
    TrackedTargetApproach,
    OrdinaryFollower,
    UntrackedFreeRoam
};

enum class RetailTrackedAdmissionStatus {
    NotWithinThreshold,
    AlreadyTracked,
    MissingOrdinaryFollower,
    TableFull,
    Inserted,
    InvalidInput
};

struct RetailTrackedAdmission {
    RetailTrackedAdmissionStatus status{};
    int slot{-1};
    int remaining_to_register{};
    bool reached_registration_target{};
};

struct RetailTrackedTargetSlots {
    static constexpr std::size_t kCapacity=20;
    std::array<std::int32_t,kCapacity> slot{};

    constexpr RetailTrackedTargetSlots() noexcept {
        slot.fill(-1);
    }

    [[nodiscard]] constexpr bool contains(
        std::int32_t entity_index) const noexcept {
        for (const auto value:slot) {
            if (value==entity_index) return true;
        }
        return false;
    }

    [[nodiscard]] constexpr bool remove(
        std::int32_t entity_index) noexcept {
        for (auto& value:slot) {
            if (value==entity_index) {
                value=-1;
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] constexpr std::optional<int> first_vacant() const noexcept {
        for (std::size_t i=0;i<slot.size();++i) {
            if (slot[i]==-1) return static_cast<int>(i);
        }
        return std::nullopt;
    }

    [[nodiscard]] constexpr RetailTrackedRoute route_for(
        std::int32_t entity_index,
        const FollowerList& ordinary_followers) const noexcept {
        if (contains(entity_index)) {
            return RetailTrackedRoute::TrackedTargetApproach;
        }
        if (ordinary_followers.contains(entity_index)) {
            return RetailTrackedRoute::OrdinaryFollower;
        }
        return RetailTrackedRoute::UntrackedFreeRoam;
    }

    [[nodiscard]] constexpr RetailTrackedAdmission admit_from_follower(
        std::int32_t entity_index,
        Vec2i entity_point,
        Vec2i species_registration_point,
        const FollowerList& ordinary_followers,
        int& remaining_to_register) noexcept {

        if (entity_index<0 || remaining_to_register<=0) {
            return {RetailTrackedAdmissionStatus::InvalidInput,-1,
                    remaining_to_register,false};
        }
        if (contains(entity_index)) {
            return {RetailTrackedAdmissionStatus::AlreadyTracked,-1,
                    remaining_to_register,false};
        }
        if (!ordinary_followers.contains(entity_index)) {
            return {RetailTrackedAdmissionStatus::MissingOrdinaryFollower,-1,
                    remaining_to_register,false};
        }

        const std::int64_t dx=
            static_cast<std::int64_t>(entity_point.x) -
            species_registration_point.x;
        const std::int64_t dy=
            static_cast<std::int64_t>(entity_point.y) -
            species_registration_point.y;
        if (dx<=-150 || dx>=150 ||
            dy<=-150 || dy>=150 ||
            dx*dx+dy*dy>=kRetailTrackedAdmissionRadiusSquared) {
            return {RetailTrackedAdmissionStatus::NotWithinThreshold,-1,
                    remaining_to_register,false};
        }

        const auto free=first_vacant();
        if (!free) {
            return {RetailTrackedAdmissionStatus::TableFull,-1,
                    remaining_to_register,false};
        }
        slot[static_cast<std::size_t>(*free)]=entity_index;
        --remaining_to_register;
        return {RetailTrackedAdmissionStatus::Inserted,*free,
                remaining_to_register,remaining_to_register==0};
    }
};

} // namespace btb::herding
