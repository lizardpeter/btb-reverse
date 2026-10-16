#include "btb/full_game_runtime.hpp"
#include <array>

namespace btb::full_game {
GameFrame GameRoot::advance_confirmation_overlay(const ActivityFrameInput& input,GameFrame frame) {
    if (!initialized_activity_) {
        frame.kind=FrameKind::FrontEndFailed;
        frame.error="original Yes/No overlay has no registered underlay";
        return frame;
    }
    auto& modal=*confirmation_modal_;
    auto& driver=drivers_[static_cast<std::size_t>(*initialized_activity_)];
    frame.activity=*initialized_activity_;
    frame.effects=driver->render_modal(input);
    if (frame.effects.fatal_error) {
        frame.kind=FrameKind::ActivityFailed;frame.error=*frame.effects.fatal_error;return frame;
    }
    constexpr std::array<const char*,4> backgrounds{
        "Data/ui/deletegame.bmp","Data/ui/Deleteallobjects.bmp",
        "Data/ui/Deletebricks.bmp","Data/ui/DeleteFireworks.bmp"};
    if (modal.context<0 || modal.context>=static_cast<int>(backgrounds.size())) {
        frame.kind=FrameKind::ActivityFailed;frame.error="original confirmation context outside 0..3";
        return frame;
    }
    modal.x=input.pointer_x;modal.y=input.pointer_y;
    modal.down=input.pointer_button_held;modal.press=input.click_pulse;
    const auto plan=shared_game_batch_9::update_yes_no(modal);
    constexpr std::array<const char*,4> buttons{
        "Data/ui/playagain/payessel.bmp","Data/ui/playagain/panosel.bmp",
        "Data/ui/playagain/payesdep.bmp","Data/ui/playagain/panodep.bmp"};
    for (const auto& operation:plan.effects) {
        if (operation.kind==shared_game_batch_9::Kind::DrawOverlay)
            frame.effects.draws.push_back({backgrounds[static_cast<std::size_t>(modal.context)],0,0,{},true});
        if (operation.kind==shared_game_batch_9::Kind::DrawSelection) {
            const int selected=operation.address;
            if (selected>=0 && selected<4)
                frame.effects.draws.push_back({buttons[static_cast<std::size_t>(selected)],
                    selected%2?334:220,260,{},true});
        }
    }
    if (modal.result>=0) {
        confirmation_result_=modal.result!=0;
        globals_.dispatcher.generic_yes_no_active=false;
        confirmation_modal_.reset();
        frame.clear_input_pulse=true;
    }
    frame.kind=FrameKind::Intercept;
    return frame;
}
} // namespace btb::full_game
