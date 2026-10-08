#pragma once

#include "btb/bobs_band.hpp"
#include "btb/bobs_band_animation.hpp"
#include "btb/bobs_band_data.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace btb::bobs_band {

struct AudioRequest {
    enum class Kind {
        StartBacking,
        StopBacking,
        PlayMachineSample,
        StopManagedSounds,
        PlayManagedVoice,
    };
    Kind kind{};
    std::string filename{};
    int managed_sound_id{-1};
    int priority{};
    int arbitration_class{};
};

struct ProgressWrite {
    int slot{-1}; // retail progress indices: 52,53,54 (Bob/Wendy/Pickles)
    int value{1};
    int shared_completion_code{6};
};
struct BandFrame {
    std::vector<AudioRequest> audio{};
    std::vector<DrawCommand> layers{};
    std::optional<ProgressWrite> progress{};
    bool open_clear_confirmation{};
    bool save_and_exit_to_play_again{};
    int new_outer_game_flow_state{-1};
    int play_again_context{-1};
};

// One input update per RunBobsBandActivity frame. Native COM rendering,
// high-resolution counter reads, audio and file persistence remain adapters.
// The original code updates the five machine animation channels each frame;
// conductor animation uses separate edit and performance rules.
struct FrameInput {
    int mouse_x{};
    int mouse_y{};
    bool click_pulse{};
    bool managed_voice_playing{};
    bool backing_track_playing{true};
    bool clear_confirmation_resolved{};
    bool clear_confirmation_yes{};
    int elapsed_centiseconds{};
    int frame_delta{1}; // original global at 0x00446FDC
    int random_value{};
};

class Activity {
public:
    explicit Activity(MachineData data,
                      Conductor conductor = Conductor::Bob,
                      int player_index = 0);

    [[nodiscard]] const Editor& editor() const noexcept { return editor_; }
    [[nodiscard]] Editor& editor() noexcept { return editor_; }
    [[nodiscard]] const MachineAnimations& machines() const noexcept {
        return machines_;
    }
    [[nodiscard]] const Playback& playback() const noexcept {
        return playback_;
    }
    [[nodiscard]] Conductor conductor() const noexcept { return conductor_; }
    [[nodiscard]] int player_index() const noexcept { return player_index_; }
    [[nodiscard]] const MachineData& machine_data() const noexcept {
        return data_;
    }

    [[nodiscard]] BandFrame advance(FrameInput input);
    void leave_to_play_again() noexcept;
    [[nodiscard]] std::string active_sequence_filename() const;
    void restore_saved_composition(Composition source) noexcept {
        editor_.set_composition(source);
    }

private:
    [[nodiscard]] EditResult click(int x, int y);
    void emit_edit_result(EditResult change, BandFrame& frame, int random_value);
    [[nodiscard]] static std::optional<Toolbar> toolbar_hit(
        int x, int y) noexcept;

    MachineData data_{};
    Conductor conductor_{Conductor::Bob};
    int player_index_{};
    Editor editor_{};
    MachineAnimations machines_{};
    ConductorEditorAnimation editor_conductor_{};
    Playback playback_{};
    bool clear_confirmation_outstanding_{};
    bool emit_exit_once_{};
};

inline constexpr std::array<HitRectangle, 4> kToolbarBounds{{
    {324,416,378,472}, // Play
    {260,416,314,472}, // Stop
    {103,416,157,472}, // Clear All
    {481,416,535,472}, // Delete
}};

// Retail state-10 completion calls PreparePlayAgainTransition, saves the
// current grid/profile, unloads audio/surfaces, switches outer state to 0x3C
// and sets saved context 0x2E.
inline constexpr int kBandPlayAgainOuterState = 0x3C;
inline constexpr int kBandPlayAgainContext = 0x2E;

} // namespace btb::bobs_band
