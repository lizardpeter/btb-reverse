#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace btb::bobs_band {

// The retail composition at 0x005123B8 has exactly 120 signed dword cells:
// five pitch rows, each containing 24 one-second time slots.
inline constexpr int kRows = 5;
inline constexpr int kSeconds = 24;
inline constexpr int kEmpty = -1;
inline constexpr int kContinuation = 10;
inline constexpr int kMachineCount = 10;
inline constexpr int kGridX = 44;
inline constexpr int kGridY = 310;
inline constexpr int kCellWidth = 23;
inline constexpr int kCellHeight = 19;
inline constexpr int kConductorCount = 3;

inline constexpr std::array<int, kMachineCount> kMachineSpans{
    1,2,1,2,1,2,1,2,1,2,
};
inline constexpr std::array<std::string_view, 5> kMachineNames{
    "Roley", "Muck", "Lofty", "Dizzy", "Scoop",
};
inline constexpr std::array<std::string_view, kMachineCount> kWavStems{
    "roley1", "roley2", "muck1", "muck2", "lofty1",
    "lofty2", "dizzy1", "dizzy2", "scoop1", "scoop2",
};

[[nodiscard]] constexpr bool valid_cell(int row, int second) noexcept {
    return row >= 0 && row < kRows && second >= 0 && second < kSeconds;
}
[[nodiscard]] constexpr bool valid_machine(int type) noexcept {
    return type >= 0 && type < kMachineCount;
}
[[nodiscard]] constexpr std::size_t cell_index(int row, int second) noexcept {
    return static_cast<std::size_t>(row * kSeconds + second);
}
[[nodiscard]] constexpr int machine_span(int type) noexcept {
    return valid_machine(type) ? kMachineSpans[static_cast<std::size_t>(type)] : 0;
}
[[nodiscard]] constexpr int pitch_sample_index(int row, int type) noexcept {
    // Retail playback at 0x41FE96 indexes its five descending pitch banks
    // as 40, 30, 20, 10, 0 + machine type.
    return valid_cell(row, 0) && valid_machine(type)
        ? (kRows - 1 - row) * kMachineCount + type : -1;
}
[[nodiscard]] constexpr int machine_owner(int type) noexcept {
    return valid_machine(type) ? type / 2 : -1;
}

struct Cell {
    int row{};
    int second{};
};
[[nodiscard]] constexpr std::optional<Cell> grid_hit(int x, int y) noexcept {
    if (x < kGridX || y < kGridY ||
        x >= kGridX + kSeconds*kCellWidth ||
        y >= kGridY + kRows*kCellHeight) {
        return std::nullopt;
    }
    return Cell{(y-kGridY)/kCellHeight, (x-kGridX)/kCellWidth};
}

struct ErasedNote {
    int machine_type{};
    Cell origin{};
    bool from_continuation{};
};

class Composition {
public:
    Composition() noexcept { clear(); }
    void clear() noexcept { cells_.fill(kEmpty); }

    [[nodiscard]] const std::array<std::int32_t, 120>& cells() const noexcept {
        return cells_;
    }
    [[nodiscard]] std::int32_t at(Cell cell) const noexcept {
        return valid_cell(cell.row, cell.second)
            ? cells_[cell_index(cell.row, cell.second)] : kEmpty;
    }
    [[nodiscard]] bool can_place(Cell cell, int type) const noexcept;
    [[nodiscard]] bool place(Cell cell, int type) noexcept;
    [[nodiscard]] std::optional<ErasedNote> erase_owner(Cell cell) noexcept;

    // State-1 editor path, including the historical type-9 special-case:
    // if a start note is under the cursor, remove it, attempt the replacement,
    // then restore it if there is insufficient space. Continuations are NOT
    // owner-resolved in this path; they reject replacement.
    struct ReplaceResult {
        bool placed{};
        bool took_previous_note{};
        int previous_machine{-1};
        bool rejected_by_occupied_continuation{};
    };
    [[nodiscard]] ReplaceResult replace_selected(Cell cell, int selected_type) noexcept;

    // Shipped sequence files are 120 signed dwords (480 bytes). These
    // operations intentionally preserve the cells as-is when importing:
    // dormant/corrupt layouts must not be rewritten without explicit repair.
    [[nodiscard]] bool decode_le_dwords(const std::vector<std::uint8_t>& bytes) noexcept;
    [[nodiscard]] std::vector<std::uint8_t> encode_le_dwords() const;
    [[nodiscard]] bool structurally_valid() const noexcept;

private:
    std::array<std::int32_t, 120> cells_{};
};

enum class InternalState : int {
    EditIdle = 0,
    BrickSelected = 1,
    DrawOnly = 2,
    RetailNoOp3 = 3,
    RetailNoOp4 = 4,
    RetailNoOp5 = 5,
    RetailNoOp6 = 6,
    RetailNoOp7 = 7,
    BeginPlayback = 8,
    Playback = 9,
    ExitToPlayAgain = 10,
};
enum class Toolbar : int {
    Play = 0,
    Stop = 1,
    ClearAll = 2,
    Delete = 3,
};
enum class EditEvent {
    None,
    PaletteSelected,
    PaletteUnselected,
    NotePlaced,
    NoteRemoved,
    NotePickedUp,
    NoteRejected,
    EnterDeleteMode,
    LeaveDeleteMode,
    ClearConfirmation,
    PlayPending,
    StopPlayback,
};
struct EditResult {
    EditEvent event{EditEvent::None};
    int machine{-1};
    bool preview_sound{};
    bool play_sound_before_transition{};
    bool request_yes_no{};
};

// The editor owns the global state fragments 0x512188 / 0x5093D8 /
// 0x513F24 / 0x513F28 / 0x513F30. Audio and cursor COM calls remain host
// callbacks, surfaced as data rather than hidden platform code.
class Editor {
public:
    [[nodiscard]] InternalState state() const noexcept { return state_; }
    [[nodiscard]] int selected_machine() const noexcept { return selected_machine_; }
    [[nodiscard]] bool delete_mode() const noexcept { return delete_mode_; }
    [[nodiscard]] bool play_pending() const noexcept { return play_pending_; }
    [[nodiscard]] const Composition& composition() const noexcept { return composition_; }
    [[nodiscard]] Composition& composition() noexcept { return composition_; }

    [[nodiscard]] EditResult choose_machine(int type) noexcept;
    [[nodiscard]] EditResult click_grid(Cell cell) noexcept;
    [[nodiscard]] EditResult toolbar_click(Toolbar button) noexcept;
    void resolve_clear_confirmation(bool affirmative) noexcept;
    [[nodiscard]] bool advance_play_pending(bool managed_sound_playing) noexcept;
    void stop_playback() noexcept;
    void exit_to_play_again() noexcept { state_ = InternalState::ExitToPlayAgain; }
    void set_composition(Composition value) noexcept { composition_ = value; }

private:
    Composition composition_{};
    InternalState state_{InternalState::EditIdle};
    int selected_machine_{-1};
    bool delete_mode_{};
    bool play_pending_{};
    bool clear_confirmation_pending_{};
};

enum class Conductor : int {
    Bob = 0,
    Wendy = 1,
    FarmerPickles = 2,
};
inline constexpr std::array<int, kConductorCount> kConductorFrameEnd{
    90,85,105, // 50..89, 50..84, 50..104
};
inline constexpr std::array<int, kConductorCount> kVoiceBase{
    510,523,536,
};

struct SampleTrigger {
    int row{};
    int second{};
    int machine_type{};
    int pitch_bank_sample{}; // 0..49, indexes the ten-samples-per-pitch bank
};
struct PlaybackFrame {
    bool within_composition_window{};
    bool new_second{};
    int second{-1};
    int conductor_frame{50};
    std::vector<SampleTrigger> started_samples{};
};

class Playback {
public:
    void begin(Conductor conductor) noexcept;
    [[nodiscard]] PlaybackFrame advance(
        const Composition& composition,
        int elapsed_centiseconds) noexcept;
    [[nodiscard]] Conductor conductor() const noexcept { return conductor_; }
    [[nodiscard]] int animation_frame() const noexcept { return frame_; }
    [[nodiscard]] bool active() const noexcept { return active_; }
    void stop() noexcept { active_ = false; }

private:
    Conductor conductor_{Conductor::Bob};
    int last_elapsed_centiseconds_{999999}; // native state-8 sentinel
    int conductor_tick_{};
    int frame_{50};
    bool active_{};
};

// Recovered from DrawBobsBandScene 0x41F090. A front-end host must emit in
// this order to respect original machine overlaps.
enum class DrawKind {
    Background,
    NoteBrick,
    Machine,
    Conductor,
    Toolbar,
};
struct DrawCommand {
    DrawKind kind{};
    int x{};
    int y{};
    int index{};
    int source_frame{};
    bool color_keyed{};
};
[[nodiscard]] std::vector<DrawCommand> editor_draw_order(
    const Composition& composition,
    Conductor conductor,
    const std::array<int,5>& machine_frames,
    int conductor_frame);

} // namespace btb::bobs_band
