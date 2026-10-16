#pragma once
#include "btb/maze_spud_frame_batch.hpp"
#include "btb/spud_maze_data.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace btb::spud_maze {

// Native integration of source 0x00423630 Pilchard phase dispatch and
// 0x00423470 collision. Paths use the already recovered shortest-path
// implementation. Full frame-perfect motion/animation parity and the
// original per-ID DirectSound busy flags remain under reconstruction.
struct PilchardNative {
    int phase{};                 // 0 wait, 1 spawn, 2 arrive, 4/5 roam, 6/7 return
    int screen{-1};
    int spawn_wait{-1};
    int current_node{};
    int route_index{};
    int roaming_direction{};
    int collision_latch{};
    int offscreen_frames{};
    int animation_frame{9};
    int animation_wait{7};
    float x{},y{};
    std::vector<int> route{};
};

struct PilchardBob {
    int screen{},direction{};
    float x{},y{};
    bool in_repair{};
};

struct PilchardFrame {
    std::vector<maze_spud_batch::Effect> sound_and_hammer{};
    bool hammer_dropped{};
    bool actor_visible{};
    bool unsupported_source_branch{};
};

using OriginalRandom = std::function<std::uint32_t()>;

// Stable original state and original 60px collision/500-frame cooldown.
// Does not create sound or drop a hammer when Pilchard is not on the
// player screen. A phase 0 actor is not active or rendered.
[[nodiscard]] PilchardFrame update_pilchard_native(
    PilchardNative& pilchard,
    maze_spud_batch::CollisionState& collision,
    const Data& data,int difficulty,
    const PilchardBob& bob,
    const OriginalRandom& random,
    bool bob_reaction_sound_playing=false,
    bool pilchard_reaction_sound_playing=false);

[[nodiscard]] bool pickup_dropped_hammer(
    maze_spud_batch::CollisionState& collision,
    const PilchardBob& bob);

} // namespace btb::spud_maze
