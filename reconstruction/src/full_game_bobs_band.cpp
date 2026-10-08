#include "btb/full_game_bobs_band.hpp"

#include <exception>
#include <fstream>
#include <utility>

namespace btb::full_game {

bool BobsBandDriver::initialize(int player_index, std::string& error) {
    if (activity_) {
        error = "Bob's Band is already initialized; release it first";
        return false;
    }
    if (player_index < 0 ||
        player_index >= static_cast<int>(progress::kPlayerCount)) {
        error = "Bob's Band player index outside original five-profile table";
        return false;
    }

    std::ifstream in(original_data_dir_ / "machinedata.txt");
    if (!in) {
        error = "cannot open original Data/SubGameOpen/machinedata.txt";
        return false;
    }

    try {
        auto data = bobs_band::parse_machine_data(in);
        auto next = std::make_unique<bobs_band::Activity>(
            std::move(data), conductor_, player_index);

        // Retail initializer clears the entire 5x24 grid before trying to
        // load the conductor/player binary file. Absence is normal on the
        // first run; malformed/short data is not silently repaired.
        auto file = bobs_band::load_sequence_file(
            save_dir_, conductor_, player_index);
        if (!file.error.empty() && !file.opened) {
            error = file.error;
            return false;
        }
        next->restore_saved_composition(file.composition);
        activity_ = std::move(next);
        player_index_ = player_index;
        error.clear();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

Audio BobsBandDriver::convert_audio(
    const bobs_band::AudioRequest& request) {

    Audio result;
    result.source_asset = request.filename;
    result.sound_id = request.managed_sound_id;
    result.priority = request.priority;
    result.arbitration_class = request.arbitration_class;

    switch (request.kind) {
    case bobs_band::AudioRequest::Kind::StartBacking:
        result.operation = AudioOperation::StartBackingTrack;
        break;
    case bobs_band::AudioRequest::Kind::PlayMachineSample:
        result.operation = AudioOperation::PlaySampleFile;
        break;
    case bobs_band::AudioRequest::Kind::StopBacking:
        result.operation = AudioOperation::StopBackingTrack;
        break;
    case bobs_band::AudioRequest::Kind::StopManagedSounds:
        result.operation = AudioOperation::StopManagedSounds;
        break;
    case bobs_band::AudioRequest::Kind::PlayManagedVoice:
        result.operation = AudioOperation::ManagedSoundId;
        break;
    }
    return result;
}

Draw BobsBandDriver::convert_draw(
    const bobs_band::DrawCommand& command) const {

    Draw out;
    out.x = command.x;
    out.y = command.y;
    out.color_keyed = command.color_keyed;
    const auto& data = activity_->machine_data();

    switch (command.kind) {
    case bobs_band::DrawKind::Background:
        out.source_asset = "Data\\SubGameOpen\\music_01.bmp";
        break;
    case bobs_band::DrawKind::NoteBrick:
        out.source_asset = bobs_band::sound_brick_bitmap_path(
            command.index, false);
        break;
    case bobs_band::DrawKind::Machine: {
        const auto sound_type = command.sprite_sound_type;
        if (!bobs_band::valid_machine(sound_type)) {
            break;
        }
        const auto i = static_cast<std::size_t>(sound_type);
        const auto size = data.machine_sizes[i];
        out.source_asset = std::string(bobs_band::kShortAndLongSprites[i]);
        out.source_rectangle = Rect{
            command.source_frame * size.width,
            0,
            (command.source_frame + 1) * size.width,
            size.height,
        };
        break;
    }
    case bobs_band::DrawKind::Conductor: {
        if (command.index < 0 ||
            command.index >= bobs_band::kConductorCount) {
            break;
        }
        const auto i = static_cast<std::size_t>(command.index);
        const auto size = data.conductor_sizes[i];
        out.source_asset = std::string(bobs_band::kConductorSprites[i]);
        out.source_rectangle = Rect{
            command.source_frame * size.width,
            0,
            (command.source_frame + 1) * size.width,
            size.height,
        };
        break;
    }
    case bobs_band::DrawKind::Toolbar:
        out.source_asset = "Data\\SubGameOpen\\toolbar.bmp";
        break;
    }
    return out;
}

ActivityFrameOutput BobsBandDriver::advance(
    const ActivityFrameInput& input) {

    ActivityFrameOutput out;
    if (!activity_) {
        return out;
    }

    const bobs_band::FrameInput band_input{
        input.pointer_x,
        input.pointer_y,
        input.click_pulse,
        input.managed_sound_playing,
        input.backing_track_playing,
        input.confirmation_resolved,
        input.confirmation_yes,
        input.elapsed_centiseconds,
        input.frame_delta,
        input.random_value,
    };

    auto frame = activity_->advance(band_input);
    out.draws.reserve(frame.layers.size());
    for (const auto& command : frame.layers) {
        out.draws.push_back(convert_draw(command));
    }
    out.audio.reserve(frame.audio.size());
    for (const auto& request : frame.audio) {
        out.audio.push_back(convert_audio(request));
    }
    if (frame.progress) {
        const int slot = frame.progress->slot;
        if (slot >= 52 && slot <= 54) {
            out.progress_writes.push_back({
                static_cast<progress::Slot>(slot),
                frame.progress->value,
            });
            out.shared_completion_code =
                frame.progress->shared_completion_code;
        }
    }
    if (frame.open_clear_confirmation) {
        out.open_yes_no_confirmation = true;
        out.yes_no_context = 2; // Native Deletebricks.bmp modal context.
    }
    if (frame.save_and_exit_to_play_again) {
        out.save_and_unload = true;
        out.next_saved_state =
            static_cast<int>(game_flow::State::BobsBandInit);
        out.next_replay_class = 1;
        out.next_outer_state = frame.new_outer_game_flow_state;
        out.next_ui_context = frame.play_again_context;
    }
    return out;
}

bool BobsBandDriver::unload(std::string& error) {
    if (!activity_) {
        return true;
    }
    // The original 0x41D920 exit performs both writes. The common game
    // root must not switch its outer state until the adapter returns.
    std::error_code ec;
    std::filesystem::create_directories(save_dir_, ec);
    if (ec) {
        error = "cannot create Bob's Band profile save directory: " +
                ec.message();
        return false;
    }
    if (!bobs_band::save_sequence_file(
            save_dir_, conductor_, player_index_,
            activity_->editor().composition())) {
        error = "cannot save Bob's Band 480-byte composition";
        return false;
    }
    if (!bobs_band::write_native_last_file(save_dir_, conductor_)) {
        error = "cannot save Bob's Band last.txt";
        return false;
    }
    activity_.reset();
    player_index_ = -1;
    error.clear();
    return true;
}

} // namespace btb::full_game
