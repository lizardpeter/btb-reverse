#include "btb/full_game_front_end.hpp"

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace btb::full_game {

GenericUiCatalog GenericUiCatalog::load(
    const std::filesystem::path& directory) {

    std::ifstream counts_file(
        directory / "NumUiHotArea.txt");
    std::ifstream areas_file(
        directory / "uiHotArea.txt");
    std::ifstream replacement_file(
        directory / "uiHotAreaReplace.txt");
    if (!counts_file || !areas_file || !replacement_file) {
        throw std::runtime_error(
            "missing original loaddata UI tables "
            "(NumUiHotArea, uiHotArea, uiHotAreaReplace)");
    }

    GenericUiCatalog data;
    data.counts = front_end::parse_hot_area_counts(counts_file);
    data.hot_areas = front_end::parse_hot_areas(areas_file);
    data.replacements =
        front_end::parse_replacement_table(replacement_file);

    std::string error;
    if (!data.validate(error)) {
        throw std::runtime_error(error);
    }
    return data;
}

bool GenericUiCatalog::validate(std::string& error) const {
    if (counts.size() != front_end::kScreenCount ||
        hot_areas.size() != front_end::kScreenCount ||
        replacements.size() != front_end::kScreenCount) {
        error = "original front-end UI tables do not contain 12 screens";
        return false;
    }
    for (std::size_t i = 0; i < front_end::kScreenCount; ++i) {
        if (counts[i] != front_end::kRetailHotAreaCounts[i] ||
            counts[i] < 0 ||
            hot_areas[i].areas.size() !=
                static_cast<std::size_t>(counts[i]) ||
            replacements[i].records.size() !=
                static_cast<std::size_t>(counts[i])) {
            error = "front-end UI table counts disagree at screen " +
                    std::to_string(i);
            return false;
        }
    }
    error.clear();
    return true;
}

std::optional<std::size_t> activity_select_hit(
    const front_end::HotAreaScreen& screen,
    int mouse_x, int mouse_y) noexcept {

    // The ten shipped Activity Select hot areas are four-corner rectangular
    // polygons. Use strict inequalities, matching native hover/click region
    // tests. Never silently flatten an arbitrary future polygon into a box.
    for (std::size_t i = 0; i < screen.areas.size(); ++i) {
        const auto& pts = screen.areas[i].polygon;
        if (pts.size() != 4) {
            continue;
        }
        int left = pts[0].x, right = pts[0].x;
        int top = pts[0].y, bottom = pts[0].y;
        for (const auto& p : pts) {
            left = std::min(left,p.x);
            right = std::max(right,p.x);
            top = std::min(top,p.y);
            bottom = std::max(bottom,p.y);
        }
        if (left >= right || top >= bottom) {
            continue;
        }
        bool rectangular = true;
        for (const auto& p : pts) {
            if ((p.x != left && p.x != right) ||
                (p.y != top && p.y != bottom)) {
                rectangular = false;
                break;
            }
        }
        if (rectangular && mouse_x > left && mouse_x < right &&
            mouse_y > top && mouse_y < bottom) {
            return i;
        }
    }
    return std::nullopt;
}

bool ActivitySelectDriver::initialize(
    const progress::Record& record,
    progress::FinaleGate& finale_gate,
    std::string& error) {

    if (!catalog_.validate(error)) {
        initialized_ = false;
        return false;
    }
    constexpr std::size_t kSelectIndex = 1;
    const auto& records = catalog_.replacements[kSelectIndex].records;
    if (records.size() != 10 ||
        initial_random_draws_.size() != records.size()) {
        error = "Activity Select requires one supplied native random "
                "draw per ten UI replacement records";
        initialized_ = false;
        return false;
    }

    finale_gate = progress::update_finale_gate_from_progress(
        finale_gate, record);
    ui_state_ = {};
    ui_state_.fireworks_finale_locked = finale_gate.locked;
    replacement_runtime_.clear();
    replacement_runtime_.reserve(records.size());
    for (std::size_t i = 0; i < records.size(); ++i) {
        replacement_runtime_.push_back(
            front_end::initialize_replacement_runtime(
                records[i], initial_random_draws_[i]));
    }
    initialized_ = true;
    error.clear();
    return true;
}

void ActivitySelectDriver::synchronize_finale_gate(
    progress::FinaleGate gate) noexcept {
    ui_state_.fireworks_finale_locked = gate.locked;
}

ActivityFrameOutput ActivitySelectDriver::advance(
    const ActivityFrameInput& input) {

    ActivityFrameOutput out;
    if (!initialized_) {
        return out;
    }
    const auto& records = screen().records;

    // The native generic UI updater checks its armed click before initiating
    // further interaction. Audio completion gates the actual transition.
    if (ui_state_.transition_pending) {
        const int pending = ui_state_.pending_area_index;
        if (pending < 0 ||
            pending >= static_cast<int>(records.size())) {
            ui_state_.transition_pending = false;
            ui_state_.pending_area_index = -1;
            return out;
        }

        const auto resolution = front_end::resolve_generic_ui_click(
            ui_state_, records[static_cast<std::size_t>(pending)],
            input.managed_sound_playing);
        switch (resolution.route) {
        case front_end::DeferredRoute::WaitForManagedSound:
        case front_end::DeferredRoute::None:
            return out;
        case front_end::DeferredRoute::OpenOptions:
            out.open_options_overlay = true;
            break;
        case front_end::DeferredRoute::OpenProgressScreen:
            out.open_progress_screen = true;
            if (resolution.stop_all_managed_sounds) {
                out.audio.push_back({
                    AudioOperation::StopManagedSounds
                });
            }
            break;
        case front_end::DeferredRoute::ApplyTargetCode:
            if (resolution.target_code) {
                const auto target = *resolution.target_code;
                if (game_flow::valid_state_value(target)) {
                    out.next_outer_state = target;
                } else {
                    // Back and Help are negative UI actions, not negative
                    // indexes into the 68-entry outer-state jump table.
                    out.negative_ui_action = target;
                }
            }
            break;
        }
        return out;
    }

    const auto hovered = activity_select_hit(
        catalog_.hot_areas[1], input.pointer_x, input.pointer_y);
    const auto prior = ui_state_.selected_area_one_based;
    const auto current = hovered
        ? static_cast<int>(*hovered + 1) : 0;

    if (current != prior) {
        if (current == 0) {
            static_cast<void>(
                front_end::leave_generic_ui_area(ui_state_));
        } else {
            const auto i = *hovered;
            auto step = front_end::enter_generic_ui_area(
                ui_state_, records[i], replacement_runtime_[i],
                static_cast<int>(i), true);
            if (step.hover_sound_id) {
                out.audio.push_back({
                    AudioOperation::ManagedSoundId, {},
                    *step.hover_sound_id,
                    step.hover_sound_priority,
                    step.hover_sound_arbitration_class
                });
            }
        }
    }

    if (hovered) {
        const auto i = *hovered;
        static_cast<void>(front_end::update_hover_animation(
            records[i], replacement_runtime_[i]));
    }

    if (input.click_pulse && hovered) {
        const auto i = *hovered;
        const auto armed = front_end::arm_generic_ui_click(
            ui_state_, records[i], static_cast<int>(i));
        if (armed.stop_all_managed_sounds) {
            out.audio.push_back({AudioOperation::StopManagedSounds});
        }
        if (armed.click_sound_id) {
            out.audio.push_back({
                AudioOperation::ManagedSoundId, {},
                *armed.click_sound_id,
                armed.click_sound_priority,
                armed.click_sound_arbitration_class
            });
        }
    }
    return out;
}

} // namespace btb::full_game
