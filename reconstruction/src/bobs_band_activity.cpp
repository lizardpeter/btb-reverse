#include "btb/bobs_band_activity.hpp"

#include <algorithm>
#include <utility>

namespace btb::bobs_band {

Activity::Activity(MachineData data, Conductor conductor, int player_index)
    : data_(std::move(data)),
      conductor_(conductor),
      player_index_(std::clamp(player_index, 0, 4)) {
    if (static_cast<int>(conductor_) < 0 ||
        static_cast<int>(conductor_) >= kConductorCount) {
        conductor_ = Conductor::Bob;
    }
}

std::string Activity::active_sequence_filename() const {
    return sequence_filename(conductor_, player_index_);
}

std::optional<Toolbar> Activity::toolbar_hit(int x, int y) noexcept {
    for (int i = 0; i < static_cast<int>(kToolbarBounds.size()); ++i) {
        if (kToolbarBounds[static_cast<std::size_t>(i)]
                .strict_contains(x,y)) {
            return static_cast<Toolbar>(i);
        }
    }
    return std::nullopt;
}

EditResult Activity::click(int x, int y) {
    // Runtime state 9 has a second hard-coded Stop region corresponding to
    // action 22 in the 132-record editor hit table.
    if (editor_.state() == InternalState::Playback &&
        x > 251 && x < 301 && y > 415 && y < 463) {
        editor_.stop_playback();
        playback_.stop();
        return {EditEvent::StopPlayback};
    }

    if (auto control = toolbar_hit(x,y)) {
        return editor_.toolbar_click(*control);
    }

    if (editor_.state() == InternalState::Playback) {
        return {};
    }

    for (int i = 0; i < static_cast<int>(data_.machine_palette_hit_rects.size()); ++i) {
        if (data_.machine_palette_hit_rects[static_cast<std::size_t>(i)]
                .strict_contains(x,y)) {
            return editor_.choose_machine(i);
        }
    }

    if (const auto cell = grid_hit(x,y)) {
        return editor_.click_grid(*cell);
    }
    return {};
}

void Activity::emit_edit_result(
    EditResult change, BandFrame& frame, int random_value) {

    const int voice = kVoiceBase[static_cast<std::size_t>(conductor_)];
    switch (change.event) {
    case EditEvent::PaletteSelected:
        if (change.preview_sound) {
            // Palette preview uses the middle one of five pitch banks:
            // pointer table 0x4FC0D4 = bank 2 of 0x4FC084.
            frame.audio.push_back({
                AudioRequest::Kind::PlayMachineSample,
                sample_wav_path(2, change.machine)
            });
            static_cast<void>(machines_.trigger(change.machine));
        }
        break;
    case EditEvent::NotePlaced:
        static_cast<void>(machines_.trigger(change.machine));
        break;
    case EditEvent::PlayPending:
        frame.audio.push_back({AudioRequest::Kind::StopManagedSounds});
        frame.audio.push_back({
            AudioRequest::Kind::PlayManagedVoice,
            {},
            voice+1,50,1
        });
        break;
    case EditEvent::StopPlayback:
        frame.audio.push_back({AudioRequest::Kind::StopBacking});
        frame.audio.push_back({
            AudioRequest::Kind::PlayManagedVoice,
            {},
            voice+3+(random_value%2),50,1
        });
        break;
    case EditEvent::ClearConfirmation:
        clear_confirmation_outstanding_ = true;
        frame.open_clear_confirmation = true;
        break;
    case EditEvent::EnterDeleteMode:
        frame.audio.push_back({
            AudioRequest::Kind::PlayManagedVoice,
            {},
            voice+9+(random_value%2),50,1
        });
        break;
    case EditEvent::None:
    case EditEvent::PaletteUnselected:
    case EditEvent::NoteRemoved:
    case EditEvent::NotePickedUp:
    case EditEvent::NoteRejected:
    case EditEvent::LeaveDeleteMode:
        break;
    }
}

BandFrame Activity::advance(FrameInput input) {
    BandFrame result;

    if (editor_.state() == InternalState::ExitToPlayAgain) {
        if (!emit_exit_once_) {
            emit_exit_once_ = true;
            result.audio.push_back({AudioRequest::Kind::StopBacking});
            result.save_and_exit_to_play_again = true;
            result.new_outer_game_flow_state = kBandPlayAgainOuterState;
            result.play_again_context = kBandPlayAgainContext;
        }
        return result;
    }

    if (input.clear_confirmation_resolved && clear_confirmation_outstanding_) {
        editor_.resolve_clear_confirmation(input.clear_confirmation_yes);
        clear_confirmation_outstanding_ = false;
    }

    EditResult edited;
    if (input.click_pulse && !clear_confirmation_outstanding_) {
        edited = click(input.mouse_x, input.mouse_y);
        emit_edit_result(edited, result, input.random_value);
    }

    // Native 0x4200C3 only retains delete mode outside the wall band when
    // this frame has the separate 0x513F28 interaction latch set.
    if (editor_.state() == InternalState::EditIdle) {
        static_cast<void>(editor_.update_delete_mode_lifetime(
            input.mouse_y, edited.event == EditEvent::NoteRemoved));
    }

    // The Play click starts a managed voice first. Even if no sound was
    // active at the start of THIS frame, native never starts the backing
    // track until a later update sees that voice finish.
    const bool started = edited.event != EditEvent::PlayPending &&
        editor_.advance_play_pending(input.managed_voice_playing);
    if (started) {
        editor_.start_playback();
        playback_.begin(conductor_);
        result.progress = ProgressWrite{
            52 + static_cast<int>(conductor_), 1, 6
        };
        result.audio.push_back({
            AudioRequest::Kind::StartBacking,
            std::string(kBackingWavs[static_cast<std::size_t>(conductor_)])
        });
    } else if (editor_.state() == InternalState::Playback) {
        if (!input.backing_track_playing) {
            editor_.stop_playback();
            playback_.stop();
        } else {
            const auto notes =
                playback_.advance(editor_.composition(),
                                  input.elapsed_centiseconds);
            for (const auto& sample : notes.started_samples) {
                result.audio.push_back({
                    AudioRequest::Kind::PlayMachineSample,
                    sample_wav_path(sample.row, sample.machine_type)
                });
                static_cast<void>(machines_.trigger(sample.machine_type));
            }
        }
    }

    machines_.advance(input.frame_delta);

    int conductor_frame = 0;
    if (editor_.state() == InternalState::Playback) {
        conductor_frame = playback_.animation_frame();
    } else {
        static_cast<void>(editor_conductor_.advance(input.random_value));
        conductor_frame = editor_conductor_.frame;
    }

    std::array<int,5> frames{};
    for (int i = 0; i < 5; ++i) {
        frames[static_cast<std::size_t>(i)] = machines_.channel(i).frame;
    }
    result.layers = editor_draw_order(
        editor_.composition(), conductor_, frames, conductor_frame);

    for (auto& command : result.layers) {
        if (command.kind != DrawKind::Machine) {
            continue;
        }
        const auto sprite = machines_.sprite(data_, command.index);
        command.x = sprite.destination.x;
        command.y = sprite.destination.y;
        command.source_frame = sprite.source_frame;
        command.sprite_sound_type = sprite.type;
    }

    return result;
}

void Activity::leave_to_play_again() noexcept {
    playback_.stop();
    editor_.exit_to_play_again();
    emit_exit_once_ = false;
}

} // namespace btb::bobs_band
