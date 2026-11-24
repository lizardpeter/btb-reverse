#include "btb/maze_data.hpp"

#include <sstream>
#include <stdexcept>

namespace btb::maze {
namespace {

bool starts_with_next(const std::string& token) {
    return token.size() >= 4 && token.compare(0, 4, "NEXT") == 0;
}

Node read_node(std::istream& in, const std::string& label) {
    Node node;
    if (!(in >> node.id
             >> node.source_position.x
             >> node.source_position.y
             >> node.direction_bits
             >> node.node_type
             >> node.links[0]
             >> node.links[1]
             >> node.links[2]
             >> node.links[3])) {
        throw std::runtime_error("could not parse Maze node after label: " + label);
    }

    node.retail_position = node.source_position;
    node.retail_position.y -= 11;
    return node;
}

} // namespace

Data parse_data(const std::string& text) {
    std::istringstream in(text);
    Data out;
    out.screens[0].source_marker = "WestScreen";
    out.screens[1].source_marker = "MiddleScreen";
    out.screens[2].source_marker = "EastScreen";

    std::string token;
    if (!(in >> token)) {
        throw std::runtime_error("empty maze_nodes.txt");
    }

    std::size_t screen = 0;
    while (in >> token) {
        if (token == "END") {
            break;
        }

        if (starts_with_next(token)) {
            ++screen;
            if (screen >= out.screens.size()) {
                throw std::runtime_error("too many Maze screen sections");
            }
            continue;
        }

        auto node = read_node(in, token);
        if (node.id < 0) {
            throw std::runtime_error("negative Maze node id");
        }
        out.screens[screen].nodes.push_back(node);
    }

    if (token != "END") {
        throw std::runtime_error("maze_nodes.txt missing END marker");
    }

    for (auto& screen_boxes : out.box_locations) {
        for (auto& box : screen_boxes) {
            if (!(in >> box.position.x >> box.position.y >> box.node_id)) {
                throw std::runtime_error("could not parse Maze box-location table");
            }
        }
    }

    if (!(in >> out.player_speed)) {
        throw std::runtime_error("could not parse Maze player speed");
    }

    for (auto& value : out.spud_speed_regular) {
        if (!(in >> value)) throw std::runtime_error("could not parse regular Spud speed");
    }
    for (auto& value : out.spud_speed_package) {
        if (!(in >> value)) throw std::runtime_error("could not parse package Spud speed");
    }
    for (auto& value : out.timer_by_difficulty) {
        if (!(in >> value)) throw std::runtime_error("could not parse Maze timer");
    }
    for (auto& value : out.spud_spawn_frames_by_difficulty) {
        if (!(in >> value)) throw std::runtime_error("could not parse Spud spawn interval");
    }
    if (!(in >> out.spud_animation_delay)) {
        throw std::runtime_error("could not parse Spud animation delay");
    }

    return out;
}

} // namespace btb::maze
