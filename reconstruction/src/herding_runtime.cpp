#include "btb/herding_runtime.hpp"

namespace btb::herding {

FollowerList::FollowerList() noexcept {
    slots_.fill(-1);
}

bool FollowerList::contains(std::int32_t entity_index) const noexcept {
    for (const auto value : slots_) {
        if (value == entity_index) {
            return true;
        }
    }
    return false;
}

bool FollowerList::add(std::int32_t entity_index) noexcept {
    if (entity_index < 0 || contains(entity_index)) {
        return false;
    }
    for (auto& value : slots_) {
        if (value == -1) {
            value = entity_index;
            return true;
        }
    }
    return false;
}

bool FollowerList::remove(std::int32_t entity_index) noexcept {
    for (auto& value : slots_) {
        if (value == entity_index) {
            value = -1;
            return true;
        }
    }
    return false;
}

std::size_t FollowerList::count() const noexcept {
    std::size_t result = 0;
    for (const auto value : slots_) {
        if (value != -1) {
            ++result;
        }
    }
    return result;
}

const std::vector<Vec2i>& scruffty_patrol_path(const Data& data) {
    static const std::vector<Vec2i> empty;
    return data.coordinate_groups.size() > 1
        ? data.coordinate_groups[1]
        : empty;
}

bool apply_scruffty_distraction(
    RetailEntityRecord32& animal,
    FollowerList& followers,
    std::int32_t entity_index,
    std::int32_t random_mod_400) noexcept {

    if (random_mod_400 < 0 || random_mod_400 >= 400) {
        return false;
    }
    if (!followers.remove(entity_index)) {
        return false;
    }

    animal.target_flag_or_timer = 200;
    animal.target_x = 286 + random_mod_400;

    const auto height = animal.source_bottom - animal.source_top;
    animal.target_y = animal.y + height / 2;
    return true;
}

} // namespace btb::herding
