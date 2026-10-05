#include "btb/fireworks_layout.hpp"

#include <stdexcept>
#include <vector>

namespace btb::fireworks {
namespace {

std::vector<Recti> parse_rectangles(std::istream& in, std::size_t count) {
    std::vector<Recti> out;
    out.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        std::int32_t x0{}, y0{}, x1{}, y1{}, x2{}, y2{}, x3{}, y3{};
        if (!(in >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3)) {
            throw std::runtime_error("Fireworks rectangle table ended early");
        }

        // The retail executable reads all four corners but retains only
        // vertex 0 and vertex 2.
        out.push_back({x0, y0, x2, y2});
    }

    std::int32_t extra{};
    if (in >> extra) {
        throw std::runtime_error("Fireworks rectangle table contains extra data");
    }

    return out;
}

} // namespace

LayoutData parse_layout(std::istream& placement, std::istream& palette) {
    const auto placement_rects = parse_rectangles(placement, 18);
    const auto palette_rects = parse_rectangles(palette, 12);

    LayoutData result;
    for (std::size_t i = 0; i < result.placement_source.size(); ++i) {
        result.placement_source[i] = placement_rects[i];

        // Net retail behavior after parse-time (-20,-30) and runtime
        // transfer (+0,+30): placement hit rectangles are shifted left 20.
        result.placement_runtime[i] = {
            placement_rects[i].left - 20,
            placement_rects[i].top,
            placement_rects[i].right - 20,
            placement_rects[i].bottom,
        };
    }

    for (std::size_t i = 0; i < result.palette.size(); ++i) {
        result.palette[i] = palette_rects[i];
    }

    return result;
}

} // namespace btb::fireworks
