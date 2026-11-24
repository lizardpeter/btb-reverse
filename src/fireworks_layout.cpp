#include "btb/fireworks_layout.hpp"

#include <array>
#include <cstdint>
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

        // Exact retail behavior:
        //   1. parser subtracts 20 from all four retained coordinates
        //   2. runtime-table transfer adds 30 to the two X coordinates
        // Final placement hit region = source X + 10, source Y - 20.
        result.placement_runtime[i] = {
            placement_rects[i].left + 10,
            placement_rects[i].top - 20,
            placement_rects[i].right + 10,
            placement_rects[i].bottom - 20,
        };
    }

    for (std::size_t i = 0; i < result.palette.size(); ++i) {
        result.palette[i] = palette_rects[i];
    }

    std::size_t region = 0;
    for (const auto& rect : result.placement_runtime) {
        result.interactive_regions[region++] = {
            rect,
            static_cast<std::int32_t>(EditorAction::PlacementSlot),
        };
    }
    for (std::size_t i = 0; i < result.palette.size(); ++i) {
        result.interactive_regions[region++] = {
            result.palette[i],
            static_cast<std::int32_t>(i),
        };
    }

    result.interactive_regions[region++] = {
        {292, 416, 346, 472},
        static_cast<std::int32_t>(EditorAction::Play),
    };
    result.interactive_regions[region++] = {
        {102, 416, 156, 472},
        static_cast<std::int32_t>(EditorAction::DeleteAll),
    };
    result.interactive_regions[region++] = {
        {483, 416, 537, 472},
        static_cast<std::int32_t>(EditorAction::DeleteSelected),
    };

    return result;
}

RetailGrid read_retail_grid(std::istream& in) {
    RetailGrid result{};
    for (auto& value : result) {
        std::array<unsigned char, 4> bytes{};
        if (!in.read(reinterpret_cast<char*>(bytes.data()), bytes.size())) {
            throw std::runtime_error("Fireworks firedata file ended early");
        }
        const auto raw =
            static_cast<std::uint32_t>(bytes[0]) |
            (static_cast<std::uint32_t>(bytes[1]) << 8U) |
            (static_cast<std::uint32_t>(bytes[2]) << 16U) |
            (static_cast<std::uint32_t>(bytes[3]) << 24U);
        value = static_cast<std::int32_t>(raw);
    }

    char extra{};
    if (in.get(extra)) {
        throw std::runtime_error("Fireworks firedata file contains trailing bytes");
    }
    return result;
}

void write_retail_grid(std::ostream& out, const RetailGrid& grid) {
    for (const auto value : grid) {
        const auto raw = static_cast<std::uint32_t>(value);
        const std::array<unsigned char, 4> bytes{
            static_cast<unsigned char>(raw & 0xffU),
            static_cast<unsigned char>((raw >> 8U) & 0xffU),
            static_cast<unsigned char>((raw >> 16U) & 0xffU),
            static_cast<unsigned char>((raw >> 24U) & 0xffU),
        };
        out.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
        if (!out) {
            throw std::runtime_error("failed writing Fireworks firedata file");
        }
    }
}

} // namespace btb::fireworks
