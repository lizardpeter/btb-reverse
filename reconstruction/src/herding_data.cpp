#include "btb/herding_data.hpp"

#include <stdexcept>

namespace btb::herding {

Data parse_data(std::istream& in) {
    Data out;

    for (auto& p : out.setup_positions) {
        if (!(in >> p.x >> p.y)) {
            throw std::runtime_error("herd.txt ended inside fixed setup positions");
        }
    }

    std::vector<Vec2i> current;
    Vec2i p{};
    while (in >> p.x >> p.y) {
        if (p.x == -1 && p.y == -1) {
            out.coordinate_groups.push_back(current);
            current.clear();
            continue;
        }
        current.push_back(p);
    }

    if (!current.empty()) {
        // Preserve unexpected unterminated data rather than silently dropping it.
        out.coordinate_groups.push_back(current);
    }

    return out;
}

std::vector<Vec2i> Data::retail_transformed_group0() const {
    std::vector<Vec2i> result;
    if (coordinate_groups.empty()) {
        return result;
    }

    result.reserve(coordinate_groups[0].size());
    for (const auto& p : coordinate_groups[0]) {
        result.push_back({p.x - 64, p.y - 100});
    }
    return result;
}

} // namespace btb::herding
