#include "btb/bobs_band_activity.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

using namespace btb::bobs_band;

namespace {

MachineData sample_data() {
    std::ostringstream data;
    for (int i=0; i<10; ++i) data << i*10 << " " << i*12 << "\n";
    for (int i=0; i<10; ++i) data << "42 39\n";
    for (int i=0; i<3; ++i) data << i*15 << " " << i*20 << "\n";
    for (int i=0; i<3; ++i) data << "85 78\n";
    data << "59 59 64\n";
    for (int i=0; i<10; ++i) {
        const int x = 100+i*10;
        data << x << " 200 " << x+9 << " 220\n";
    }
    data << "comments are not numeric and must be ignored";
    std::istringstream in(data.str());
    return parse_machine_data(in);
}

} // namespace

int main() {
    Composition grid;
    assert(grid.structurally_valid());
    assert(grid.at({0,0}) == kEmpty);
    assert(!grid.can_place({4,23},1));
    assert(grid.place({4,22},1));
    assert(grid.at({4,22}) == 1);
    assert(grid.at({4,23}) == kContinuation);
    assert(!grid.place({4,23},0));
    const auto erased = grid.erase_owner({4,23});
    assert(erased && erased->machine_type == 1);
    assert(erased->origin.second == 22);
    assert(erased->from_continuation);
    assert(grid.at({4,22}) == kEmpty);
    assert(grid.at({4,23}) == kEmpty);

    assert(grid.place({1,4},0));
    assert(grid.place({1,5},2));
    // Selecting a long brick and clicking a short occupied note removes
    // the old note first, but restores it if the next cell is occupied.
    const auto rejected = grid.replace_selected({1,4},1);
    assert(!rejected.placed);
    assert(!rejected.took_previous_note);
    assert(grid.at({1,4}) == 0);
    assert(grid.at({1,5}) == 2);
    assert(grid.place({2,4},3));
    const auto continuation = grid.replace_selected({2,5},0);
    assert(!continuation.placed);
    assert(continuation.rejected_by_occupied_continuation);
    assert(grid.at({2,4}) == 3);
    assert(grid.at({2,5}) == kContinuation);

    assert(grid.place({3,0},8));
    assert(grid.replace_selected({3,0},4).placed);
    assert(grid.at({3,0}) == 4);
    assert(grid.structurally_valid());

    const auto binary = grid.encode_le_dwords();
    assert(binary.size() == 480);
    Composition loaded;
    assert(loaded.decode_le_dwords(binary));
    assert(loaded.cells() == grid.cells());
    std::vector<std::uint8_t> broken = binary;
    broken.pop_back();
    assert(!loaded.decode_le_dwords(broken));
    assert(loaded.cells() == grid.cells());

    assert(grid_hit(44,310)->row == 0);
    assert(grid_hit(595,404)->second == 23);
    assert(!grid_hit(596,405));
    assert(pitch_sample_index(0,9) == 49);
    assert(pitch_sample_index(4,0) == 0);
    assert(sample_wav_path(0,0) ==
           "Data\\SubGameOpen\\roley1_5.wav");
    assert(sample_wav_path(4,9) ==
           "Data\\SubGameOpen\\scoop2_1.wav");
    assert(sequence_filename(Conductor::FarmerPickles,4) ==
           "musicfarmer5.txt");

    const auto md = sample_data();
    assert(md.machine_positions[3].x == 30);
    assert(md.machine_sizes[5].width == 42);
    assert(md.conductor_idle_frame_counts[2] == 64);
    assert(md.machine_palette_hit_rects[9].left == 190);

    MachineAnimations machines;
    assert(machines.trigger(1));
    assert(machines.channel(0).state ==
           MachineAnimationState::TwoSeconds);
    assert(!machines.trigger(0)); // native never interrupts live machine anim
    machines.advance(4);
    assert(machines.channel(0).frame == 1);
    for (int i=0; i<29; ++i) machines.advance(4);
    assert(machines.channel(0).state == MachineAnimationState::Idle);
    assert(machines.channel(0).frame == 0);

    ConductorEditorAnimation conductor;
    for (int i=0; i<6; ++i) {
        static_cast<void>(conductor.advance(1));
    }
    assert(conductor.frame == 1);
    for (int i=0; i<19*6; ++i) {
        static_cast<void>(conductor.advance(1));
    }
    assert(conductor.frame == 0); // frame 20, 2-in-3 short-idle branch

    Activity app(md, Conductor::Wendy, 3);
    assert(app.active_sequence_filename() == "musicwendy4.txt");

    FrameInput click_palette{};
    click_palette.mouse_x=105;
    click_palette.mouse_y=210;
    click_palette.click_pulse=true;
    const auto palette=app.advance(click_palette);
    assert(app.editor().state() == InternalState::BrickSelected);
    assert(!palette.audio.empty());
    assert(palette.audio.front().filename ==
           "Data\\SubGameOpen\\roley1_3.wav");

    FrameInput click_wall{};
    click_wall.mouse_x=45;
    click_wall.mouse_y=311;
    click_wall.click_pulse=true;
    static_cast<void>(app.advance(click_wall));
    assert(app.editor().composition().at({0,0}) == 0);
    assert(app.editor().state() == InternalState::EditIdle);

    FrameInput play{};
    play.mouse_x=329;
    play.mouse_y=421;
    play.click_pulse=true;
    auto voice=app.advance(play);
    assert(app.editor().state() == InternalState::EditIdle);
    assert(app.editor().play_pending());
    assert(voice.audio.size() == 2);
    assert(voice.audio[1].managed_sound_id == 524); // Wendy base + 1

    FrameInput voice_still_playing{};
    voice_still_playing.managed_voice_playing=true;
    static_cast<void>(app.advance(voice_still_playing));
    assert(app.editor().play_pending());

    FrameInput voice_done{};
    const auto start=app.advance(voice_done);
    assert(app.editor().state() == InternalState::Playback);
    assert(start.progress && start.progress->slot == 53);
    assert(start.progress->shared_completion_code == 6);
    assert(start.audio[0].kind == AudioRequest::Kind::StartBacking);
    assert(start.audio[0].filename == "Data\\SubGameOpen\\Wendymt.wav");

    FrameInput at_second_zero{};
    const auto sequence=app.advance(at_second_zero);
    assert(sequence.audio.size() == 1);
    assert(sequence.audio[0].filename ==
           "Data\\SubGameOpen\\roley1_5.wav");
    assert(app.playback().active());

    FrameInput same_second{};
    same_second.elapsed_centiseconds=40;
    const auto deduplicated=app.advance(same_second);
    assert(deduplicated.audio.empty());

    app.leave_to_play_again();
    const auto leave=app.advance({});
    assert(leave.save_and_exit_to_play_again);
    assert(leave.new_outer_game_flow_state == 0x3c);
    assert(leave.play_again_context == 0x2e);
    assert(!app.advance({}).save_and_exit_to_play_again);
}
