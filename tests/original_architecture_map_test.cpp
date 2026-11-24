#include "btb/original_architecture_map.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace {
constexpr bool addresses_are_unique() {
    constexpr auto& map = btb::evidence::kOriginalArchitecture200;
    for (std::size_t i = 0; i < map.size(); ++i) {
        for (std::size_t j = i + 1; j < map.size(); ++j) {
            if (map[i].address == map[j].address) return false;
        }
    }
    return true;
}
}

int main() {
    constexpr auto& architecture = btb::evidence::kOriginalArchitecture200;
    static_assert(architecture.size() == 200);
    static_assert(addresses_are_unique());
    static_assert(architecture[0].address == 0x00402cf0U);
    static_assert(architecture[0].original_thiscall);
    static_assert(architecture[0].callers > 40);
    static_assert(architecture[0].module_candidate == "shared_platform");
    // This test verifies mapping metadata ONLY, not recovered implementation.
    std::size_t direct_callers = 0;
    std::size_t observed_globals = 0;
    for (const auto& row : architecture) {
        assert(!row.symbol.empty());
        assert(!row.module_candidate.empty());
        assert(!row.cpp_owner_candidate.empty());
        assert(row.address >= 0x00400000U);
        direct_callers += row.callers;
        observed_globals += row.globals;
    }
    assert(direct_callers > 100);
    assert(observed_globals > 100);
}
