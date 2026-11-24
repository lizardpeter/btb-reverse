#include "btb/full_game_generic_ui.hpp"

#include <cstdint>
#include <string>

namespace btb::full_game {

bool point_in_original_polygon(
    const front_end::HotArea& area, int x, int y) noexcept {

    const auto& polygon = area.polygon;
    if (polygon.size() < 3) {
        return false;
    }
    bool inside = false;
    for (std::size_t i = 0, j = polygon.size()-1;
         i < polygon.size(); j = i++) {
        const auto& a = polygon[j];
        const auto& b = polygon[i];
        const auto dx = static_cast<std::int64_t>(b.x)-a.x;
        const auto dy = static_cast<std::int64_t>(b.y)-a.y;
        const auto px = static_cast<std::int64_t>(x)-a.x;
        const auto py = static_cast<std::int64_t>(y)-a.y;

        // The original UI region hitboxes use strict interior comparisons:
        // touching a polygon edge is not an actionable selected area.
        const auto cross = dx*py-dy*px;
        if (cross == 0 &&
            x >= (a.x < b.x ? a.x : b.x) &&
            x <= (a.x > b.x ? a.x : b.x) &&
            y >= (a.y < b.y ? a.y : b.y) &&
            y <= (a.y > b.y ? a.y : b.y)) {
            return false;
        }
        if ((a.y > y) != (b.y > y)) {
            const double intersect_x = static_cast<double>(a.x) +
                static_cast<double>(dx) * (y-a.y) /
                static_cast<double>(dy);
            if (static_cast<double>(x) < intersect_x) {
                inside = !inside;
            }
        }
    }
    return inside;
}

std::optional<std::size_t> generic_ui_hit(
    const front_end::HotAreaScreen& screen, int x, int y) noexcept {

    for (std::size_t i=0; i<screen.areas.size(); ++i) {
        if (point_in_original_polygon(screen.areas[i],x,y)) {
            return i;
        }
    }
    return std::nullopt;
}

bool GenericUiScreenDriver::initialize(
    const progress::Record& record,
    progress::FinaleGate& finale_gate,
    std::string& error) {

    if (!catalog_.validate(error)) {
        initialized_ = false;
        return false;
    }
    const auto index = static_cast<int>(screen_);
    if (!front_end::descriptor(index)) {
        error = "generic UI index outside retail 12-screen catalog";
        initialized_ = false;
        return false;
    }
    const auto& table =
        catalog_.replacements[static_cast<std::size_t>(index)].records;
    if (initial_random_draws_.size() != table.size()) {
        error = "generic UI screen requires the original random initial "
                "hover index for each loaded replacement row";
        initialized_ = false;
        return false;
    }

    ui_state_ = {};
    pregame_narration_stage_=pregame_setup_
        ? initial_retail_pregame_narration_stage(*pregame_setup_) : 0;
    if (screen_ == front_end::Screen::ActivitySelect) {
        finale_gate = progress::update_finale_gate_from_progress(
            finale_gate,record);
        ui_state_.fireworks_finale_locked = finale_gate.locked;
    }
    secondary_delay_=0;
    captured_area_.reset();
    replacements_.clear();
    replacements_.reserve(table.size());
    for (std::size_t i=0; i<table.size(); ++i) {
        replacements_.push_back(
            front_end::initialize_replacement_runtime(
                table[i],initial_random_draws_[i]));
    }
    initialized_ = true;
    error.clear();
    return true;
}

void GenericUiScreenDriver::synchronize_finale_gate(
    progress::FinaleGate gate) noexcept {

    if (screen_ == front_end::Screen::ActivitySelect) {
        ui_state_.fireworks_finale_locked = gate.locked;
    }
}

ActivityFrameOutput GenericUiScreenDriver::advance(
    const ActivityFrameInput& input) {

    ActivityFrameOutput result;
    if (!initialized_) {
        return result;
    }
    // Background stays visible while voice-gated clicks are pending.
    // The caller supplies the real 23-entry source index for this state.
    if (backdrop_) {
        result.draws.push_back(*backdrop_);
    }
    if(pregame_setup_) {
        if(const auto voice=advance_retail_pregame_narration(
                *pregame_setup_,pregame_narration_stage_,input.managed_sound_playing)) {
            result.audio.push_back({AudioOperation::StopManagedSounds});
            result.audio.push_back({AudioOperation::ManagedSoundId,{},*voice,90,1,true});
        }
    }
    const auto index = static_cast<std::size_t>(screen_);
    const auto& records = catalog_.replacements[index].records;

    if (ui_state_.transition_pending) {
        const int pending = ui_state_.pending_area_index;
        if (pending < 0 ||
            pending >= static_cast<int>(records.size())) {
            ui_state_.transition_pending = false;
            ui_state_.pending_area_index = -1;
            return result;
        }
        const auto route = front_end::resolve_generic_ui_click(
            ui_state_,records[static_cast<std::size_t>(pending)],
            input.managed_sound_playing);
        switch (route.route) {
        case front_end::DeferredRoute::WaitForManagedSound:
        case front_end::DeferredRoute::None:
            return result;
        case front_end::DeferredRoute::OpenOptions:
            result.open_options_overlay = true;
            break;
        case front_end::DeferredRoute::OpenProgressScreen:
            result.open_progress_screen = true;
            if (route.stop_all_managed_sounds) {
                result.audio.push_back({AudioOperation::StopManagedSounds});
            }
            break;
        case front_end::DeferredRoute::ApplyTargetCode:
            if (route.target_code) {
                const auto value = *route.target_code;
                if (game_flow::valid_state_value(value)) {
                    result.next_outer_state = value;
                } else {
                    result.negative_ui_action = value;
                }
            }
            break;
        }
        return result;
    }

    if(screen_==front_end::Screen::InstructionWithDifficulty)
        append_generic_ui_difficulty_art(result.draws,records,replacements_,difficulty_);
    const auto hit = generic_ui_hit(
        catalog_.hot_areas[index],input.pointer_x,input.pointer_y);
    const auto press_hit=input.menu_press_position
        ? generic_ui_hit(catalog_.hot_areas[index],input.menu_press_position->first,
                         input.menu_press_position->second) : hit;
    const auto pointer=resolve_menu_pointer(captured_area_,hit,press_hit,input);
    const auto hovered=pointer.hover;
    const auto current = hovered
        ? static_cast<int>(*hovered + 1) : 0;
    const auto prior=ui_state_.selected_area_one_based;
    if (current != prior) {
        if (!hovered) {
            static_cast<void>(front_end::leave_generic_ui_area(ui_state_));
        } else {
            const auto i = *hovered;
            auto entered = front_end::enter_generic_ui_area(
                ui_state_,records[i],replacements_[i],
                static_cast<int>(i),
                screen_ == front_end::Screen::ActivitySelect);
            if (entered.hover_sound_id) {
                result.audio.push_back({
                    AudioOperation::ManagedSoundId, {},
                    *entered.hover_sound_id,
                    entered.hover_sound_priority,
                    entered.hover_sound_arbitration_class
                });
            }
        }
    }

    if (hovered && current==prior) {
        static_cast<void>(front_end::update_hover_animation(
            records[*hovered],replacements_[*hovered]));
    }

    append_generic_ui_art(result.draws,records,replacements_,
                          hovered,pointer.depressed,secondary_delay_,
                          current==prior||pointer.depressed);
    if (pointer.activate && hovered) {
        const auto armed = front_end::arm_generic_ui_click(
            ui_state_,records[*hovered],static_cast<int>(*hovered));
        if (armed.stop_all_managed_sounds) {
            result.audio.push_back({AudioOperation::StopManagedSounds});
        }
        if (armed.click_sound_id) {
            result.audio.push_back({
                AudioOperation::ManagedSoundId, {},
                *armed.click_sound_id,
                armed.click_sound_priority,
                armed.click_sound_arbitration_class
            });
        }
    }
    return result;
}

} // namespace btb::full_game
