#include "btb/bobs_band.hpp"

#include <algorithm>
#include <bit>
#include <cstdint>

namespace btb::bobs_band {

bool Composition::can_place(Cell cell, int type) const noexcept {
    if (!valid_cell(cell.row, cell.second) || !valid_machine(type)) {
        return false;
    }
    const int span = machine_span(type);
    if (cell.second + span > kSeconds) {
        return false;
    }
    for (int step = 0; step < span; ++step) {
        if (at({cell.row, cell.second + step}) != kEmpty) {
            return false;
        }
    }
    return true;
}

bool Composition::place(Cell cell, int type) noexcept {
    if (!can_place(cell, type)) {
        return false;
    }
    cells_[cell_index(cell.row, cell.second)] = type;
    for (int step = 1; step < machine_span(type); ++step) {
        cells_[cell_index(cell.row, cell.second + step)] = kContinuation;
    }
    return true;
}

std::optional<ErasedNote> Composition::erase_owner(Cell cell) noexcept {
    if (!valid_cell(cell.row, cell.second)) {
        return std::nullopt;
    }
    const int tapped = at(cell);
    if (tapped == kEmpty || (tapped != kContinuation && !valid_machine(tapped))) {
        return std::nullopt;
    }

    int origin = cell.second;
    if (tapped == kContinuation) {
        // Native RemoveSoundBrick 0x41F430 walks BACK through cells whose
        // values are >9, then erases the original type and its full duration.
        while (origin > 0 &&
               at({cell.row, origin}) == kContinuation) {
            --origin;
        }
        if (!valid_machine(at({cell.row, origin}))) {
            return std::nullopt; // Guard malformed/imported data.
        }
    }
    const int type = at({cell.row, origin});
    if (!valid_machine(type)) {
        return std::nullopt;
    }

    const int span = machine_span(type);
    cells_[cell_index(cell.row, origin)] = kEmpty;
    for (int step = 1; step < span && origin + step < kSeconds; ++step) {
        cells_[cell_index(cell.row, origin + step)] = kEmpty;
    }
    return ErasedNote{
        type, {cell.row, origin}, tapped == kContinuation,
    };
}

Composition::ReplaceResult Composition::replace_selected(
    Cell cell, int selected_type) noexcept {

    ReplaceResult result;
    if (!valid_cell(cell.row, cell.second) || !valid_machine(selected_type)) {
        return result;
    }

    const int touched = at(cell);
    if (touched == kContinuation) {
        // State 1 does not resolve the owner of a continuation. It tries to
        // place at the occupied coordinate and fails the occupancy check.
        result.rejected_by_occupied_continuation = true;
        return result;
    }

    if (valid_machine(touched)) {
        int owner_second = cell.second;
        if (touched == 9) {
            // Literal 0x41F604 comparison is 'jl' after cmp type,9.
            // Hence only type 9 takes the backwards-scan branch. This
            // surprising retail quirk is distinct from normal erase_owner().
            while (owner_second > 0 &&
                   at({cell.row, owner_second - 1}) >= 9) {
                --owner_second;
            }
        }
        const auto removed = erase_owner({cell.row, owner_second});
        if (!removed) {
            return result;
        }
        result.took_previous_note = true;
        result.previous_machine = removed->machine_type;
        result.placed = place(cell, selected_type);
        if (!result.placed) {
            // Original 0x41F651 failure path restores the old event at
            // the removed event's origin, not necessarily at click position.
            static_cast<void>(place(removed->origin, removed->machine_type));
            result.took_previous_note = false;
            result.previous_machine = -1;
        }
        return result;
    }

    result.placed = place(cell, selected_type);
    return result;
}

bool Composition::decode_le_dwords(
    const std::vector<std::uint8_t>& bytes) noexcept {

    if (bytes.size() != cells_.size()*sizeof(std::int32_t)) {
        return false;
    }
    std::array<std::int32_t,120> parsed{};
    for (std::size_t i = 0; i < parsed.size(); ++i) {
        const std::size_t off = i * 4;
        const std::uint32_t bits =
            static_cast<std::uint32_t>(bytes[off]) |
            (static_cast<std::uint32_t>(bytes[off+1]) << 8) |
            (static_cast<std::uint32_t>(bytes[off+2]) << 16) |
            (static_cast<std::uint32_t>(bytes[off+3]) << 24);
        parsed[i] = std::bit_cast<std::int32_t>(bits);
    }
    cells_ = parsed;
    return true;
}

std::vector<std::uint8_t> Composition::encode_le_dwords() const {
    std::vector<std::uint8_t> bytes;
    bytes.reserve(cells_.size() * 4);
    for (const auto cell : cells_) {
        const auto value = std::bit_cast<std::uint32_t>(cell);
        for (unsigned shift = 0; shift < 32; shift += 8) {
            bytes.push_back(
                static_cast<std::uint8_t>((value >> shift) & 0xffU));
        }
    }
    return bytes;
}

bool Composition::structurally_valid() const noexcept {
    for (int row = 0; row < kRows; ++row) {
        for (int second = 0; second < kSeconds; ++second) {
            const int type = at({row, second});
            if (type == kEmpty) {
                continue;
            }
            if (type == kContinuation) {
                // Exactly one preceding long event must own this slot.
                if (second == 0 ||
                    !valid_machine(at({row, second-1})) ||
                    machine_span(at({row, second-1})) != 2) {
                    return false;
                }
                continue;
            }
            if (!valid_machine(type)) {
                return false;
            }
            if (machine_span(type) == 2) {
                if (second == kSeconds - 1 ||
                    at({row, second+1}) != kContinuation) {
                    return false;
                }
            }
        }
    }
    return true;
}

EditResult Editor::choose_machine(int type) noexcept {
    if (!valid_machine(type) ||
        (state_ != InternalState::EditIdle &&
         state_ != InternalState::BrickSelected)) {
        return {};
    }

    delete_mode_ = false;
    if (state_ == InternalState::BrickSelected &&
        selected_machine_ == type) {
        state_ = InternalState::EditIdle;
        selected_machine_ = -1;
        return {EditEvent::PaletteUnselected, type, false, false, false};
    }

    state_ = InternalState::BrickSelected;
    selected_machine_ = type;
    return {EditEvent::PaletteSelected, type, true, false, false};
}

EditResult Editor::click_grid(Cell cell) noexcept {
    if (!valid_cell(cell.row, cell.second)) {
        return {};
    }

    if (state_ == InternalState::EditIdle) {
        const auto removed = composition_.erase_owner(cell);
        if (!removed) {
            return {};
        }
        if (delete_mode_) {
            // The original frame's delete-interaction latch is separate from
            // the persistent delete-mode flag, handled by the caller.
            return {
                EditEvent::NoteRemoved, removed->machine_type, false, false, false
            };
        }
        state_ = InternalState::BrickSelected;
        selected_machine_ = removed->machine_type;
        return {
            EditEvent::NotePickedUp, selected_machine_, false, false, false
        };
    }

    if (state_ != InternalState::BrickSelected ||
        !valid_machine(selected_machine_)) {
        return {};
    }

    const auto outcome =
        composition_.replace_selected(cell, selected_machine_);
    if (!outcome.placed) {
        return {EditEvent::NoteRejected, selected_machine_, false, false, false};
    }
    if (outcome.took_previous_note) {
        selected_machine_ = outcome.previous_machine;
        return {
            EditEvent::NotePickedUp, selected_machine_, true, false, false
        };
    }
    const int placed = selected_machine_;
    selected_machine_ = -1;
    state_ = InternalState::EditIdle;
    return {EditEvent::NotePlaced, placed, false, false, false};
}

EditResult Editor::toolbar_click(Toolbar button) noexcept {
    switch (button) {
    case Toolbar::Play:
        if (state_ != InternalState::EditIdle || delete_mode_) {
            return {};
        }
        play_pending_ = true;
        return {EditEvent::PlayPending, -1, false, true, false};
    case Toolbar::Stop:
        if (state_ != InternalState::Playback || delete_mode_) {
            return {};
        }
        stop_playback();
        return {EditEvent::StopPlayback, -1, false, false, false};
    case Toolbar::ClearAll:
        if (state_ != InternalState::EditIdle || delete_mode_) {
            return {};
        }
        clear_confirmation_pending_ = true;
        return {EditEvent::ClearConfirmation, -1, false, false, true};
    case Toolbar::Delete:
        if (state_ != InternalState::EditIdle) {
            return {};
        }
        delete_mode_ = !delete_mode_;
        return {
            delete_mode_ ? EditEvent::EnterDeleteMode
                         : EditEvent::LeaveDeleteMode,
            -1, false, false, false,
        };
    }
    return {};
}

void Editor::resolve_clear_confirmation(bool affirmative) noexcept {
    if (clear_confirmation_pending_ && affirmative) {
        composition_.clear();
    }
    clear_confirmation_pending_ = false;
}

bool Editor::advance_play_pending(bool managed_sound_playing) noexcept {
    if (!play_pending_ || managed_sound_playing) {
        return false;
    }
    play_pending_ = false;
    state_ = InternalState::BeginPlayback;
    return true;
}

bool Editor::update_delete_mode_lifetime(
    int cursor_y, bool deletion_latched_this_frame) noexcept {

    if (!delete_mode_) {
        return false;
    }

    // 0x4200C3..0x4200EB: native cancels delete mode outside the
    // strict 289 < Y < 401 editor band when no deletion occurred.
    if (!deletion_latched_this_frame &&
        (cursor_y <= 289 || cursor_y >= 401)) {
        delete_mode_ = false;
        return true;
    }
    return false;
}

void Editor::start_playback() noexcept {
    if (state_ == InternalState::BeginPlayback) {
        state_ = InternalState::Playback;
        selected_machine_ = -1;
        delete_mode_ = false;
    }
}

void Editor::stop_playback() noexcept {
    state_ = InternalState::EditIdle;
    play_pending_ = false;
    selected_machine_ = -1;
}

void Playback::begin(Conductor conductor) noexcept {
    if (static_cast<int>(conductor) < 0 ||
        static_cast<int>(conductor) >= kConductorCount) {
        conductor = Conductor::Bob;
    }
    conductor_ = conductor;
    last_elapsed_centiseconds_ = 999999;
    conductor_tick_ = 0;
    frame_ = 50;
    active_ = true;
}

PlaybackFrame Playback::advance(
    const Composition& composition,
    int elapsed_centiseconds) noexcept {

    PlaybackFrame out;
    out.conductor_frame = frame_;
    if (!active_ || elapsed_centiseconds < 0) {
        return out;
    }
    const int second = elapsed_centiseconds / 100;
    out.second = second;
    out.within_composition_window = second < kSeconds;
    if (!out.within_composition_window) {
        // 0x41FCEA exits before triggering notes once >= 24 seconds.
        return out;
    }

    // Retail redraw runs on each tick, but note triggers happen only on
    // a new 100-centisecond bucket, including second 0 (999999 sentinel).
    out.new_second =
        second != last_elapsed_centiseconds_ / 100;
    if (out.new_second) {
        for (int row = 0; row < kRows; ++row) {
            const int type = composition.at({row, second});
            if (!valid_machine(type)) {
                continue; // ignore -1 and duration continuation 10
            }
            out.started_samples.push_back({
                row, second, type, pitch_sample_index(row, type)
            });
        }
    }
    last_elapsed_centiseconds_ = elapsed_centiseconds;

    ++conductor_tick_;
    if (conductor_tick_ > 3) {
        conductor_tick_ = 0;
        ++frame_;
        const int end = kConductorFrameEnd[
            static_cast<std::size_t>(conductor_)];
        if (frame_ >= end) {
            frame_ = 50;
        }
    }
    out.conductor_frame = frame_;
    return out;
}

std::vector<DrawCommand> editor_draw_order(
    const Composition& composition,
    Conductor conductor,
    const std::array<int,5>& machine_frames,
    int conductor_frame) {

    std::vector<DrawCommand> draw;
    draw.reserve(129);
    draw.push_back({DrawKind::Background, 0, 0, 0, 0, false});

    for (int row = 0; row < kRows; ++row) {
        for (int second = 0; second < kSeconds; ++second) {
            const int type = composition.at({row, second});
            if (valid_machine(type)) {
                draw.push_back({
                    DrawKind::NoteBrick,
                    kGridX + second*kCellWidth,
                    kGridY + row*kCellHeight,
                    type, 0, true
                });
            }
        }
    }

    // Exact painter order: Scoop, Dizzy, Lofty, Muck, Roley. Position
    // is the shared short-animation origin from machinedata.txt.
    constexpr std::array<int,5> x{0,86,225,342,393};
    constexpr std::array<int,5> y{130,93,2,105,75};
    for (int machine = 4; machine >= 0; --machine) {
        draw.push_back({
            DrawKind::Machine,
            x[static_cast<std::size_t>(machine)],
            y[static_cast<std::size_t>(machine)],
            machine,
            machine_frames[static_cast<std::size_t>(machine)],
            true
        });
    }

    constexpr std::array<int,3> cx{499,504,493};
    constexpr std::array<int,3> cy{133,132,136};
    const int idx = std::clamp(static_cast<int>(conductor), 0, 2);
    draw.push_back({
        DrawKind::Conductor,
        cx[static_cast<std::size_t>(idx)],
        cy[static_cast<std::size_t>(idx)],
        idx, conductor_frame, true
    });
    draw.push_back({DrawKind::Toolbar,0,0,0,0,true});
    return draw;
}

} // namespace btb::bobs_band
