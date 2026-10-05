#include "btb/spud_maze_data.hpp"

#include <stdexcept>

namespace btb::spud_maze {
namespace {

bool starts_with_next(const std::string& token) {
    return token.size() >= 4 &&
           token[0] == 'N' &&
           token[1] == 'E' &&
           token[2] == 'X' &&
           token[3] == 'T';
}

} // namespace

Data parse_data(std::istream& in) {
    Data out;

    std::string token;
    if (!(in >> token)) {
        throw std::runtime_error("empty spudmaze_nodes.txt");
    }

    std::size_t screen = 0;

    while (in >> token) {
        if (token == "END") {
            break;
        }

        if (starts_with_next(token)) {
            if (++screen >= kScreenCount) {
                throw std::runtime_error("too many Spud Maze screens");
            }
            if (!(in >> token)) {
                throw std::runtime_error("missing Spud Maze token after NEXT");
            }
            // The shipped file is inconsistent: "NEXT_2nd Screen" has a
            // separate Screen token, while later NEXT markers include Screen
            // in the marker itself. Normalize both spellings to the same
            // effective retail graph.
            if (token == "Screen") {
                if (!(in >> token)) {
                    throw std::runtime_error("missing first node label after NEXT_2nd Screen");
                }
            }
        }

        Node node;
        if (!(in >> node.index
                 >> node.x
                 >> node.y
                 >> node.direction_bits
                 >> node.node_type
                 >> node.links[0]
                 >> node.links[1]
                 >> node.links[2]
                 >> node.links[3])) {
            throw std::runtime_error("short Spud Maze node record");
        }

        if (node.index < 0 ||
            node.index >= static_cast<std::int32_t>(kMaxNodesPerScreen)) {
            throw std::runtime_error("Spud Maze node index out of retail range");
        }

        node.y -= 11;
        out.screens[screen].push_back(node);
    }

    for (auto& screen_refs : out.references) {
        for (auto& ref : screen_refs) {
            if (!(in >> ref.x >> ref.y >> ref.node)) {
                throw std::runtime_error("short Spud Maze reference triple table");
            }
        }
    }

    if (!(in >> out.file_player_speed)) {
        throw std::runtime_error("missing Spud Maze file player speed");
    }
    out.retail_player_speed = 4.0f;

    for (auto& value : out.spud_speed_regular) {
        if (!(in >> value)) throw std::runtime_error("short regular Spud speed table");
    }
    for (auto& value : out.spud_speed_package) {
        if (!(in >> value)) throw std::runtime_error("short package Spud speed table");
    }
    for (auto& value : out.timer_values) {
        if (!(in >> value)) throw std::runtime_error("short Spud Maze timer table");
    }
    for (auto& value : out.spud_spawn_times) {
        if (!(in >> value)) throw std::runtime_error("short Spud spawn-time table");
    }

    if (!(in >> out.spud_animation_delay)) {
        throw std::runtime_error("missing Spud animation delay");
    }

    for (std::size_t s = 0; s < kScreenCount; ++s) {
        auto& points = out.repair_points[s];
        points.reserve(kRepairPointCounts[s]);
        for (std::size_t i = 0; i < kRepairPointCounts[s]; ++i) {
            std::array<std::int32_t,2> point{};
            if (!(in >> point[0] >> point[1])) {
                throw std::runtime_error("short trailing Spud Maze screen-point table");
            }
            points.push_back(point);
        }
    }

    return out;
}

} // namespace btb::spud_maze
