#pragma once
#include "btb/full_game_park_designer.hpp"
#include "btb/full_game_park_scene.hpp"
#include "btb/park_designer_editor_control_slice.hpp"
#include <functional>
#include <span>

namespace btb::full_game {
// Production provider. All bitmap commands refer to original assets, not
// stand-in artwork. Selection state is transient; the retail 30512-byte
// persistent layout remains unchanged.
class RetailParkDesignerNativeService final : public ParkDesignerNativeService {
public:
    using PrintProfile=std::function<bool(profiles::ProfileInfo&,std::span<const Rect>&,std::string&)>;
    explicit RetailParkDesignerNativeService(retail::OriginalRetailRandom& rng,
        PrintProfile print_profile={}):rng_(rng),print_profile_(std::move(print_profile)) {}
    void begin_session() noexcept override;
    void cancel_drag(park_designer::SaveData&) noexcept override;
    bool native_intro() const noexcept override {return true;}
    bool normal_cursor() const noexcept override {return !selection_&&controls_.cursor_mode==0;}
    std::optional<std::pair<int,int>> take_queued_voice() noexcept override;
    bool render(park_designer::SaveData&,const park_designer::BoundAreas&,ActivityFrameOutput&,std::string&) override;
    bool update_editor(park_designer::SaveData&,const park_designer::BoundAreas&,const ActivityFrameInput&,ActivityFrameOutput&,std::string&) override;
    bool emit_sound(const park_designer::LoopSound&,ActivityFrameOutput&,std::string&) override;
private:
    struct Selection {
        park_designer::EditorMode mode{};
        int page{},variant{};
        std::optional<int> slot{};
    };
    retail::OriginalRetailRandom& rng_;
    PrintProfile print_profile_{};
    bool compose_print_label(ActivityFrameOutput&,std::string&);
    RetailParkScenePlan scene_{};
    park_designer::EditorControlState controls_{};
    std::optional<Selection> selection_{};
    std::optional<std::pair<int,int>> queued_{};
    int hover_{-1},view_hover_{-1};
    bool backing_started_{},back_hover_{};
    void voice(ActivityFrameOutput&,const ActivityFrameInput&,int,int);
};
}
