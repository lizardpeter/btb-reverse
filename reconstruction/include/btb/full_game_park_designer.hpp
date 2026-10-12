#pragma once
// Source-backed 0x00410D10 Park Designer outer activity driver.
// Entered from the original 68-state GameRoot dispatcher. Assets are still
// supplied by a native scene/editor service, never synthesized here.
#include "btb/full_game_runtime.hpp"
#include "btb/park_designer_activity_loop.hpp"
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace btb::full_game {

class ParkDesignerNativeService {
public:
    virtual ~ParkDesignerNativeService() = default;
    // A service must render genuine retail bitmap surfaces, via its own
    // original resource mapping, and populate source-neutral host Draws.
    virtual bool render(
        const park_designer::SaveData&,
        const park_designer::BoundAreas&,
        ActivityFrameOutput&,std::string&) = 0;
    // This callback is 0x0040E440; its full branch closure remains pending.
    virtual bool update_editor(
        park_designer::SaveData&,const park_designer::BoundAreas&,
        const ActivityFrameInput&,ActivityFrameOutput&,std::string&) = 0;
    // Map original managed ID/music index/voice context to host audio.
    virtual bool emit_sound(
        const park_designer::LoopSound&,
        ActivityFrameOutput&,std::string&) = 0;
};

class ParkDesignerDriver final : public ActivityDriver {
public:
    ParkDesignerDriver(std::filesystem::path original_data_dir,
        std::filesystem::path save_dir,
        std::shared_ptr<ParkDesignerNativeService> service = {})
        : original_data_dir_(std::move(original_data_dir)),
          save_dir_(std::move(save_dir)),service_(std::move(service)) {}

    [[nodiscard]] bool initialize(int player_index,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    void request_exit(bool shutdown = false) noexcept {
        state_.exit_requested=!shutdown;
        state_.shutdown_requested=shutdown;
    }
    void start_original_music_intro() noexcept {
        state_.intro_music_pending=true;
        state_.intro_managed_voice_pending=true;
    }
    void queue_original_voice(int id,int context) noexcept {
        state_.queued_voice=id;state_.queued_voice_context=context;
    }
    [[nodiscard]] bool initialized() const noexcept {return bounds_.has_value();}
    [[nodiscard]] const park_designer::SaveData& save() const noexcept {return state_.save;}
    [[nodiscard]] park_designer::SaveData& mutable_save() noexcept {return state_.save;}
    [[nodiscard]] const park_designer::LoopState& loop() const noexcept {return state_;}

private:
    std::filesystem::path original_data_dir_{};
    std::filesystem::path save_dir_{};
    std::shared_ptr<ParkDesignerNativeService> service_{};
    std::optional<park_designer::BoundAreas> bounds_{};
    park_designer::LoopState state_{};
    int player_index_{-1};
    bool awaiting_delete_all_confirmation_{};
    [[nodiscard]] std::filesystem::path profile_filename() const;
};

} // namespace btb::full_game
