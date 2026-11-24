#include "btb/spud_maze_runtime.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace btb::spud_maze {

RepairSelection select_repair_damage(
    Difficulty difficulty,
    std::int32_t phase_seed_1_to_4) {

    if (phase_seed_1_to_4 < 1 || phase_seed_1_to_4 > 4) {
        throw std::runtime_error(
            "Spud Maze repair phase seed must match retail rand()%4 + 1");
    }

    RepairSelection result;
    const auto divisor = repair_damage_divisor(difficulty);

    std::int32_t counter = phase_seed_1_to_4;
    for (std::size_t i = 0; i < kRepairPointCount; ++i) {
        const bool damaged = (counter % divisor) == 0;
        result.damaged[i] = damaged;
        if (damaged) {
            ++result.repairs_remaining;
        }
        ++counter;
    }

    return result;
}

namespace {

const Node* find_node(
    const std::vector<Node>& nodes,
    std::int32_t index) noexcept {

    for (const auto& node : nodes) {
        if (node.index == index) {
            return &node;
        }
    }
    return nullptr;
}

std::int32_t integer_edge_distance(
    const Node& a,
    const Node& b) noexcept {

    const auto dx = static_cast<double>(b.x - a.x);
    const auto dy = static_cast<double>(b.y - a.y);
    return static_cast<std::int32_t>(
        std::sqrt(dx * dx + dy * dy));
}

bool path_contains(
    const std::array<std::int32_t, kRetailPathNodeLimit>& path,
    std::size_t count,
    std::int32_t node) noexcept {

    for (std::size_t i = 0; i < count; ++i) {
        if (path[i] == node) {
            return true;
        }
    }
    return false;
}

void search_path(
    const std::vector<Node>& nodes,
    std::int32_t target,
    GraphPath& best,
    std::array<std::int32_t, kRetailPathNodeLimit>& current,
    std::size_t current_count,
    std::int32_t current_cost) {

    if (current_count == 0 || current_count >= kRetailPathNodeLimit) {
        return;
    }

    const auto* from = find_node(nodes, current[current_count - 1]);
    if (from == nullptr) {
        return;
    }

    for (const auto linked : from->links) {
        if (linked < 0) {
            continue;
        }
        if (current_count >= kRetailPathNodeLimit) {
            return;
        }
        if (path_contains(current, current_count, linked)) {
            continue;
        }

        const auto* next = find_node(nodes, linked);
        if (next == nullptr) {
            continue;
        }

        const auto next_cost =
            current_cost + integer_edge_distance(*from, *next);

        if (best.found && next_cost >= best.total_cost) {
            continue;
        }

        current[current_count] = linked;
        const auto next_count = current_count + 1;

        if (linked == target) {
            best.nodes = current;
            best.node_count = next_count;
            best.total_cost = next_cost;
            best.found = true;
            continue;
        }

        search_path(
            nodes,
            target,
            best,
            current,
            next_count,
            next_cost);
    }
}

} // namespace

GraphPath shortest_graph_path(
    const std::vector<Node>& nodes,
    std::int32_t start_node,
    std::int32_t target_node) {

    GraphPath result;
    result.total_cost = std::numeric_limits<std::int32_t>::max();

    if (find_node(nodes, start_node) == nullptr ||
        find_node(nodes, target_node) == nullptr) {
        return result;
    }

    std::array<std::int32_t, kRetailPathNodeLimit> current{};
    current[0] = start_node;

    if (start_node == target_node) {
        result.nodes = current;
        result.node_count = 1;
        result.total_cost = 0;
        result.found = true;
        return result;
    }

    search_path(
        nodes,
        target_node,
        result,
        current,
        1,
        0);

    return result;
}

SlideFrame advance_slide(SlideState& s) noexcept {
    SlideFrame f{};
    if(s.phase==0) {f.complete=true;return f;}
    if(s.phase==1) {
        s.phase=2;
        s.east=s.destination>s.previous;
        s.speed=10;
        s.left=s.east?20:618;
        s.right=s.east?22:620;
    }
    const int step=s.speed/5;
    if(s.east) {
        s.left+=step;s.right+=step;
        f.left={s.previous,s.left,619,20};
        f.right={s.destination,20,s.right,619-s.left+20};
        f.old_offset=20-s.left;
        f.new_offset=619-s.right;
    } else {
        s.left-=step;s.right-=step;
        f.left={s.destination,s.left,619,20};
        f.right={s.previous,20,s.right,619-s.left+20};
        f.old_offset=619-s.left;
        f.new_offset=20-s.left;
    }
    const int remaining=s.east?619-s.right:s.right-20;
    if(remaining<=0) {s.phase=0;f.complete=true;}
    s.speed=std::max(10,std::min({200,s.speed+1,remaining}));
    return f;
}

BentleyStep advance_bentley(BentleyState& s,bool failed,
                            bool playing,int random_bit) noexcept {
    BentleyStep out{};
    switch(s.phase) {
    case -1:
        s.visible=true;
        if(--s.wait<1) {
            s.wait=9;s.x+=5;
            if(++s.frame>20) {
                s.frame=12;
                if(s.x>149) {
                    s.phase=0;s.frame=0;
                    out.stop_managed=true;out.sound_id=713;
                }
            }
        }
        break;
    case 0:
        if(--s.wait<1) {
            s.wait=9;
            if(++s.frame>7) {
                s.frame=0;
                if(!playing) {s.frame=12;s.phase=1;}
            }
        }
        break;
    case 1:
        // Source decrements the counter again on the common phase-2 tail.
        if(--s.wait<1) {
            s.wait=9;s.x+=5;
            if(++s.frame>20) {
                s.frame=12;
                if(s.x>299) {s.frame=25;s.phase=2;break;}
            }
        }
        [[fallthrough]];
    case 2:
        if(--s.wait<1) {
            s.wait=9;
            if(++s.frame>39) {
                s.frame=25;
                if(s.pause_cycles--<1) {
                    s.phase=3;s.failed=failed;
                    s.frame=failed?40:44;
                    out.sound_id=failed?714:715;
                }
            }
        }
        break;
    case 3:
    case 4:
        if(--s.wait<1) {
            s.wait=9;
            if(++s.frame>=(s.failed?44:56))s.frame=s.failed?40:44;
        }
        if(!playing) {
            if(s.phase==3) {
                out.sound_id=(s.failed?716:726)+(random_bit&1);
                s.phase=4;
            } else out.complete=true;
        }
        break;
    }
    return out;
}

} // namespace btb::spud_maze
