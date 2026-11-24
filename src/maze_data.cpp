#include "btb/maze_data.hpp"

#include <algorithm>
#include <sstream>
#include <utility>
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


bool validate_package_routes(
    const Data& data,Screen start_screen,std::int32_t start_node,
    std::string& error) {
    const auto start_index=static_cast<std::int32_t>(start_screen);
    if(start_index<0||start_index>=static_cast<std::int32_t>(data.screens.size())) {
        error="invalid Maze starting screen";
        return false;
    }
    const auto find_node=[&](std::int32_t screen,std::int32_t id)->const Node* {
        if(screen<0||screen>=static_cast<std::int32_t>(data.screens.size()))
            return nullptr;
        const Node* found=nullptr;
        for(const auto& node:data.screens[static_cast<std::size_t>(screen)].nodes) {
            if(node.id!=id)continue;
            if(found)return nullptr;
            found=&node;
        }
        return found;
    };
    for(std::size_t screen=0;screen<data.screens.size();++screen) {
        const auto& nodes=data.screens[screen].nodes;
        for(std::size_t i=0;i<nodes.size();++i) {
            for(std::size_t j=i+1;j<nodes.size();++j) {
                if(nodes[i].id==nodes[j].id) {
                    error="duplicate Maze node ID "+std::to_string(nodes[i].id)+
                        " on screen "+std::to_string(screen);
                    return false;
                }
            }
        }
    }
    if(!find_node(start_index,start_node)) {
        error="Maze starting node is missing or duplicated";
        return false;
    }

    std::array<std::vector<std::int32_t>,3> reached{};
    std::vector<std::pair<std::int32_t,std::int32_t>> pending;
    const auto enqueue=[&](std::int32_t screen,std::int32_t id) {
        auto& seen=reached[static_cast<std::size_t>(screen)];
        if(std::find(seen.begin(),seen.end(),id)!=seen.end())return;
        seen.push_back(id);
        pending.emplace_back(screen,id);
    };
    enqueue(start_index,start_node);
    for(std::size_t cursor=0;cursor<pending.size();++cursor) {
        const auto [screen,id]=pending[cursor];
        const auto* node=find_node(screen,id);
        if(!node) {
            error="Maze route reached an ambiguous node";
            return false;
        }
        for(const auto link:node->links) {
            if(link==-1)continue;
            if(!find_node(screen,link)) {
                error="Maze node "+std::to_string(id)+" on screen "+
                    std::to_string(screen)+" links to a missing or duplicate node "+
                    std::to_string(link);
                return false;
            }
            enqueue(screen,link);
        }
        const auto destination=node->portal_destination();
        if(!destination)continue;
        const auto destination_index=static_cast<std::int32_t>(*destination);
        std::int32_t reciprocal_count=0;
        std::int32_t reciprocal_id=-1;
        for(const auto& candidate:data.screens[
                static_cast<std::size_t>(destination_index)].nodes) {
            const auto candidate_destination=candidate.portal_destination();
            if(candidate_destination&&
               *candidate_destination==static_cast<Screen>(screen)) {
                ++reciprocal_count;
                reciprocal_id=candidate.id;
            }
        }
        if(reciprocal_count!=1) {
            error="Maze portal node "+std::to_string(id)+" on screen "+
                std::to_string(screen)+" has "+
                std::to_string(reciprocal_count)+
                " reciprocal entry nodes";
            return false;
        }
        enqueue(destination_index,reciprocal_id);
    }

    for(std::size_t screen=0;screen<data.box_locations.size();++screen) {
        for(std::size_t location=0;location<data.box_locations[screen].size();++location) {
            const auto id=data.box_locations[screen][location].node_id;
            if(!find_node(static_cast<std::int32_t>(screen),id)) {
                error="Maze package location "+std::to_string(location)+
                    " on screen "+std::to_string(screen)+
                    " references missing or duplicate node "+std::to_string(id);
                return false;
            }
            const auto& seen=reached[screen];
            if(std::find(seen.begin(),seen.end(),id)==seen.end()) {
                error="Maze package location "+std::to_string(location)+
                    " on screen "+std::to_string(screen)+
                    " is unreachable from Middle node 8 (node "+
                    std::to_string(id)+")";
                return false;
            }
        }
    }
    error.clear();
    return true;
}

} // namespace btb::maze
