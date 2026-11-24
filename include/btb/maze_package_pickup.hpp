#pragma once
#include "btb/maze_box_layout.hpp"
#include "btb/maze_data.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>

namespace btb::maze {

// Literal float32 46.0 at 0x0043B460 of original retail PE:
// SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Used by 0x0041B140 UpdateMazePlayerMovement for BOTH normal (status 1)
// and opponent-dropped (status 3) box pickup distance.
inline constexpr int kRetailPickupRadiusExclusive=46;

struct PickupResult {
    std::size_t screen{};
    std::size_t slot{};
    int remaining{};
};

// The original uses signed truncation of both player floats, scans the
// current screen's four 20-byte package records in original order, and
// consumes the FIRST eligible record strictly closer than 46 pixels.
// This changes the source status to zero; it does not grant the final
// game progress slot (0x0041D4D0 grants that after feedback).
[[nodiscard]] inline std::optional<PickupResult> collect_source_box(
    BoxAllocation& allocation,
    const Data& data,
    int screen,
    float player_x,float player_y) {
    if(screen<0 || screen>=3)
        throw std::out_of_range("original Maze screen outside 0..2");
    const auto si=static_cast<std::size_t>(screen);
    const auto px=static_cast<std::int32_t>(player_x);
    const auto py=static_cast<std::int32_t>(player_y);
    auto& bank=allocation.screens[si];
    for(std::size_t index=0;index<bank.size();++index) {
        auto& slot=bank[index];
        if(slot.node_id<0)continue;
        int x{},y{};
        if(slot.status==kSpudAvailablePackageStatus) {
            if(slot.node_id>=4)
                throw std::out_of_range("source box location index outside 0..3");
            const auto& point=data.box_locations[si][
                static_cast<std::size_t>(slot.node_id)];
            x=point.position.x;
            y=point.position.y;
        } else if(slot.status==kSpudDroppedPackageStatus) {
            x=slot.dropped_x;
            y=slot.dropped_y;
        } else continue;
        const std::int64_t dx=static_cast<std::int64_t>(x)-px;
        const std::int64_t dy=static_cast<std::int64_t>(y)-py;
        if(dx*dx+dy*dy >=
            static_cast<std::int64_t>(kRetailPickupRadiusExclusive)*
                kRetailPickupRadiusExclusive)
            continue;
        if(allocation.remaining<=0)
            throw std::logic_error("Maze source package counter underflow");
        slot.status=0;
        --allocation.remaining;
        return PickupResult{si,index,allocation.remaining};
    }
    return std::nullopt;
}
} // namespace btb::maze
