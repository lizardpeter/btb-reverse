#include "btb/spud_maze_runtime.hpp"

#include <cassert>

using namespace btb::spud_maze;

int main() {
    static_assert(
        classify_screen_transition(0x80) == ScreenTransition::LeftUpper);
    static_assert(
        classify_screen_transition(0x100) == ScreenTransition::RightUpper);
    static_assert(
        classify_screen_transition(0x400) == ScreenTransition::LeftLower);
    static_assert(
        classify_screen_transition(0x800) == ScreenTransition::RightLower);
    static_assert(
        classify_screen_transition(0) == ScreenTransition::None);

    constexpr auto left = apply_screen_transition(
        2, 7, ScreenTransition::LeftLower);
    static_assert(left.screen == 1);
    static_assert(left.node == 5);
    static_assert(left.retail_direction == 1);

    constexpr auto right = apply_screen_transition(
        2, 5, ScreenTransition::RightUpper);
    static_assert(right.screen == 3);
    static_assert(right.node == 7);
    static_assert(right.retail_direction == 2);

    std::vector<Node> graph{
        Node{0, 0, 0, 0, 0, {1, 2, -1, -1}},
        Node{1, 10, 0, 0, 0, {0, 3, -1, -1}},
        Node{2, 0, 20, 0, 0, {0, 3, -1, -1}},
        Node{3, 20, 0, 0, 0, {1, 2, -1, -1}},
    };

    const auto shortest = shortest_graph_path(graph, 0, 3);
    assert(shortest.found);
    assert(shortest.node_count == 3);
    assert(shortest.nodes[0] == 0);
    assert(shortest.nodes[1] == 1);
    assert(shortest.nodes[2] == 3);
    assert(shortest.total_cost == 20);

    const auto same = shortest_graph_path(graph, 2, 2);
    assert(same.found);
    assert(same.node_count == 1);
    assert(same.total_cost == 0);

    const auto missing = shortest_graph_path(graph, 0, 99);
    assert(!missing.found);

    for (int seed = 1; seed <= 4; ++seed) {
        const auto easy = select_repair_damage(Difficulty::Easy, seed);
        assert(easy.repairs_remaining == 4);

        const auto medium = select_repair_damage(Difficulty::Medium, seed);
        assert(medium.repairs_remaining == (seed == 4 ? 7 : 6));

        const auto hard = select_repair_damage(Difficulty::Hard, seed);
        assert(hard.repairs_remaining == ((seed == 2 || seed == 3) ? 11 : 10));
    }

    assert(activity_outcome(4, 100) == ActivityOutcome::Continue);
    assert(activity_outcome(0, 100) == ActivityOutcome::Success);
    assert(activity_outcome(-1, 100) == ActivityOutcome::Success);
    assert(activity_outcome(4, -1) == ActivityOutcome::Timeout);

    constexpr auto drop_right = make_hammer_drop(200, 300, 1, 3);
    static_assert(drop_right.screen == 3);
    static_assert(drop_right.x == 210);
    static_assert(drop_right.y == 341);
    static_assert(drop_right.collision_cooldown == 500);

    constexpr auto drop_left = make_hammer_drop(200, 300, 0, 3);
    static_assert(drop_left.x == 190);

    static_assert(can_pick_up_hammer_x(100, 119));
    static_assert(!can_pick_up_hammer_x(100, 120));

    static_assert(repair_sound_id(0) == 702);
    static_assert(repair_sound_id(6) == 708);
    static_assert(no_hammer_sound_id(0) == 709);
    static_assert(no_hammer_sound_id(1) == 710);
    static_assert(pilchard_collision_bob_sound_id(0) == 696);
    static_assert(pilchard_collision_bob_sound_id(5) == 701);
    static_assert(pilchard_reaction_sound_id(0) == 720);
    static_assert(pilchard_reaction_sound_id(1) == 721);

    for(int direction:{-1,1}) {
        SlideState slide{};slide.previous=2;slide.destination=2+direction;slide.phase=1;
        int frames=0;
        while(slide.phase) {
            const auto f=advance_slide(slide);
            ++frames;
            assert(frames<500);
            if(!f.complete) {
                assert(f.left.left>=20&&f.left.right<=620);
                assert(f.right.left==20&&f.right.right<=620);
                assert(f.left.screen==(direction>0?2:1));
                assert(f.right.screen==(direction>0?3:2));
            }
        }
        assert(frames>10);
    }
    for(bool failed:{false,true}) {
        BentleyState b{};
        std::vector<int> voices;
        bool done=false;
        for(int i=0;i<3000&&!done;++i) {
            // Simulate actual completion of each submitted voice, keeping
            // the phase-3 response busy for additional calls.
            const bool playing=b.phase==3&&b.wait>1;
            const auto f=advance_bentley(b,failed,playing,1);
            assert(b.frame>=0&&b.frame<56);
            if(f.sound_id>=0)voices.push_back(f.sound_id);
            done=f.complete;
        }
        assert(done&&b.visible);
        assert((voices==std::vector<int>{713,failed?714:715,failed?717:727}));
    }
    static_assert(kStartupMusicIndex == 7);
    static_assert(kStartupVoiceSoundId == 694);
    static_assert(kLowTimerWarningSoundId == 994);
}
