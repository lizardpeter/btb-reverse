#pragma once
// Park Designer 0040DB50, 0040DA60, 0040D7D0, 0040D870, 0040D900.
// Native draw commands refer to the genuine installed resource paths.
#include "btb/full_game_runtime.hpp"
#include "btb/park_designer_retail_catalog.hpp"
#include <array>
#include <string>
#include <vector>

namespace btb::full_game {
struct RetailParkScenePlan {
    std::array<std::size_t,park_designer::kObjectRecordCount> sorted_slots{};
    std::vector<Draw> draws{};
};

// The original comparator callbacks are NOT strict weak orderings.
// Execute the recovered 00430B5C/00430CB0 partition/small-sort algorithm
// over record indices rather than using std::sort with invalid predicates.
[[nodiscard]] bool plan_retail_park_scene(park_designer::SaveData&,
    RetailParkScenePlan&,std::string& error);
}
