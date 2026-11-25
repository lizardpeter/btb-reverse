#pragma once
#include <array>
#include <vector>
#include <cstdint>
namespace btb::full_game {
// Canonical display planes only; no codec-private reference state.
struct OpaqueGlobalVideo {
    std::array<std::vector<std::uint8_t>,3> planes{};
    bool valid() const noexcept{return planes[0].size()==640*480&&planes[1].size()==320*240&&planes[2].size()==320*240;}
    void clear() noexcept {for(auto& plane:planes)plane.clear();}
};
}
