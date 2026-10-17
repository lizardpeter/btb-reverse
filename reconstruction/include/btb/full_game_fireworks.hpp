#pragma once

#include "btb/fireworks_editor.hpp"
#include "btb/fireworks_layout.hpp"
#include "btb/fireworks_sequence.hpp"
#include "btb/full_game_runtime.hpp"

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <utility>

namespace btb::full_game {

// Source-backed Fireworks editor adapter for the retail 68-state coordinator.
// It uses the original 18-cell int32 save representation, source rectangle
// tables and the recovered Bob/Wendy animation and control decisions.
//
// The original pre-show transition movie uses the shared native Bink provider.
// The separate 23-movie Fireworks bank, previews, show and certificate are
// still unhosted and remain fail-closed rather than artificially completed.
class FireworksDriver final : public ActivityDriver {
public:
    // Source pre-show 14->15 uses the shared original BinkOpen with disk
    // movie flags. Production Win32 supplies the actual x86 decoder; tests
    // may supply a frame-source fixture without bypassing this boundary.
    using OpenPreShowMovie = std::function<bool(
        const std::filesystem::path&,std::string&)>;

    FireworksDriver(std::filesystem::path original_data_dir,
                    std::filesystem::path save_dir,
                    OpenPreShowMovie open_pre_show_movie = {})
        : original_data_dir_(std::move(original_data_dir)),
          save_dir_(std::move(save_dir)),
          open_pre_show_movie_(std::move(open_pre_show_movie)) {}

    [[nodiscard]] bool initialize(
        int player_index, std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    [[nodiscard]] bool initialized() const noexcept {
        return layout_.has_value();
    }
    [[nodiscard]] const fireworks::Sequence& sequence() const noexcept {
        return sequence_;
    }
    [[nodiscard]] const fireworks::EditorRuntimeState& editor() const noexcept {
        return editor_;
    }

private:
    [[nodiscard]] Draw convert_draw(
        const fireworks::EditorDrawCommand& source) const;
    [[nodiscard]] static std::optional<Draw> control_overlay(
        const fireworks::EditorControlOutput& source);

    std::filesystem::path original_data_dir_{};
    std::filesystem::path save_dir_{};
    std::optional<fireworks::LayoutData> layout_{};
    fireworks::Sequence sequence_{};
    fireworks::EditorRuntimeState editor_{};
    int player_index_{-1};
    bool awaiting_delete_confirmation_{};
    OpenPreShowMovie open_pre_show_movie_{};
};

} // namespace btb::full_game
