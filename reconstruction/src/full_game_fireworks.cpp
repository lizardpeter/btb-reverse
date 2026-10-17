#include "btb/full_game_fireworks.hpp"

#include <array>
#include <cstdint>
#include <exception>
#include <fstream>
#include <string_view>
#include <system_error>

namespace btb::full_game {
namespace {

constexpr std::string_view kGraph = "Data\\SubGameFirework\\graph\\";
constexpr std::array<std::string_view, 12> kTypeBitmaps{{
    "red_airbom_01.bmp", "small_green_01.bmp", "medium_red_01.bmp",
    "blue_large_01.bmp", "red_large_01.bmp", "medium_green_01.bmp",
    "small_blue_01.bmp", "blue_airbom_01.bmp", "red_candel_01.bmp",
    "red_spiner_01.bmp", "green_spiner_01.bmp", "blue_candel_01.bmp"
}};

[[nodiscard]] bool strict_hit(
    const fireworks::Recti& rect, int x, int y) noexcept {
    return x > rect.left && x < rect.right &&
           y > rect.top && y < rect.bottom;
}

[[nodiscard]] std::uint32_t nonnegative_roll(int value) noexcept {
    return static_cast<std::uint32_t>(value) & 0x7fffffffU;
}

void append_sound(ActivityFrameOutput& output, int id,
                  int priority = 50, int kind = 1) {
    if (id >= 0) {
        output.audio.push_back({
            AudioOperation::ManagedSoundId, {}, id, priority, kind
        });
    }
}

} // namespace

bool FireworksDriver::initialize(int player_index, std::string& error) {
    if (initialized()) {
        error = "Fireworks already initialized; unload before reentry";
        return false;
    }
    if (player_index < 0 ||
        player_index >= static_cast<int>(progress::kPlayerCount)) {
        error = "Fireworks player index outside five retail profiles";
        return false;
    }

    std::ifstream placements(
        original_data_dir_ / "graph" / "fireworks.txt");
    std::ifstream palette(
        original_data_dir_ / "graph" / "fireworkboxes.txt");
    if (!placements || !palette) {
        error = "cannot open original Fireworks graph rectangle tables";
        return false;
    }

    try {
        auto candidate_layout = fireworks::parse_layout(placements, palette);
        fireworks::Sequence candidate_sequence;
        const auto name = fireworks::retail_grid_filename(
            static_cast<std::size_t>(player_index));
        const auto file = save_dir_ / std::string(name);

        // The first run has no player-specific file. Any existing file
        // must be the authentic 72-byte little-endian signed-int layout.
        std::error_code ec;
        const bool exists = std::filesystem::exists(file, ec);
        if (ec) {
            error = "cannot inspect Fireworks profile save: " + ec.message();
            return false;
        }
        if (exists) {
            std::ifstream input(file, std::ios::binary);
            if (!input) {
                error = "cannot open existing Fireworks profile layout";
                return false;
            }
            if (!candidate_sequence.load_retail_grid(
                    fireworks::read_retail_grid(input))) {
                error = "existing Fireworks layout has invalid cell types";
                return false;
            }
        }

        layout_ = std::move(candidate_layout);
        sequence_ = std::move(candidate_sequence);
        editor_ = fireworks::EditorRuntimeState{};
        player_index_ = player_index;
        awaiting_delete_confirmation_ = false;
        error.clear();
        return true;
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }
}

Draw FireworksDriver::convert_draw(
    const fireworks::EditorDrawCommand& command) const {
    Draw result;
    result.x = command.x;
    result.y = command.y;
    result.color_keyed = command.color_keyed;

    switch (command.kind) {
    case fireworks::EditorRenderKind::Background:
        result.source_asset = std::string(kGraph) + "Bk_01e.bmp";
        break;
    case fireworks::EditorRenderKind::Firework:
        if (command.firework_type) {
            const auto id = static_cast<int>(*command.firework_type);
            if (fireworks::original_firework_type_valid(id)) {
                result.source_asset =
                    std::string(kGraph) +
                    std::string(kTypeBitmaps[static_cast<std::size_t>(id)]);
            }
        }
        break;
    case fireworks::EditorRenderKind::Actor:
        if (command.actor) {
            result.source_asset = std::string(kGraph) +
                (*command.actor == fireworks::PlacementActorChannel::Bob
                 ? "bobsprite1_8bit.bmp" : "wendysprite8_bit.bmp");
        }
        if (command.source_rect) {
            const auto& rect = *command.source_rect;
            result.source_rectangle =
                Rect{rect.left, rect.top, rect.right, rect.bottom};
        }
        break;
    case fireworks::EditorRenderKind::MiddleOverlay:
        result.source_asset = std::string(kGraph) + "middle.bmp";
        break;
    case fireworks::EditorRenderKind::BottomOverlay:
        result.source_asset = std::string(kGraph) + "bottom.bmp";
        break;
    }
    return result;
}

std::optional<Draw> FireworksDriver::control_overlay(
    const fireworks::EditorControlOutput& control) {
    if (!control.surface) return std::nullopt;
    std::string_view bitmap;
    switch (*control.surface) {
    case fireworks::EditorControlSurface::DeleteAllHover:
        bitmap = "fireworkdeleteallred.bmp"; break;
    case fireworks::EditorControlSurface::DeleteAllPressed:
        bitmap = "fireworkdeletealldep.bmp"; break;
    case fireworks::EditorControlSurface::DeleteHover:
        bitmap = "fireworkdeletered.bmp"; break;
    case fireworks::EditorControlSurface::DeletePressed:
        bitmap = "fireworkdeletedep.bmp"; break;
    case fireworks::EditorControlSurface::PlayHover:
        bitmap = "fireworkplayred.bmp"; break;
    case fireworks::EditorControlSurface::PlayPressed:
        bitmap = "fireworkplaydep.bmp"; break;
    }
    return Draw{
        std::string(kGraph) + std::string(bitmap),
        control.surface_x, control.surface_y,
        std::nullopt, true
    };
}

ActivityFrameOutput FireworksDriver::advance(
    const ActivityFrameInput& input) {
    ActivityFrameOutput output;
    if (!layout_) {
        output.fatal_error = "Fireworks editor advanced without initialization";
        return output;
    }

    using fireworks::InternalState;
    auto state = editor_.internal_state;
    const auto draw_original_editor = [&]() {
        const auto frame = fireworks::run_retail_editor_frame(
            editor_,sequence_,*layout_,input.pointer_x,input.pointer_y);
        output.draws.reserve(frame.draw_commands.size()+1);
        for(const auto& draw:frame.draw_commands)
            output.draws.push_back(convert_draw(draw));
        for(const auto& tick:frame.placement_ticks)
            if(tick.sound_id)append_sound(output,*tick.sound_id);
    };

    // Retail state 14 waits until the just-requested managed Play voice
    // has started. Only then does it open the original disk Bink and
    // advance to state 15; no elapsed-time or synthetic EOS shortcut.
    if(state == InternalState::PreShowMovieSetup) {
        if(!fireworks::should_open_pre_show_movie(
                input.managed_sound_playing)) {
            draw_original_editor();
            return output;
        }
        if(!open_pre_show_movie_) {
            output.fatal_error=
                "original Fireworks pre-show Bink provider is unavailable";
            return output;
        }
        const auto movie=original_data_dir_.parent_path()/
            "movies"/"fireworkcomplete.bik";
        std::string error;
        if(!open_pre_show_movie_(movie,error)) {
            output.fatal_error=error.empty()
                ? "original fireworkcomplete.bik could not open" : error;
            return output;
        }
        editor_.internal_state=InternalState::PreShowMoviePlayback;
        // The native renderer clears the now-empty editor surface as retail
        // does before drawing the original Bink frames on later ticks.
        return output;
    }
    if(state == InternalState::PreShowMoviePlayback) {
        if(!input.original_global_movie_finished) {
            // The real Bink device already copied the image into its
            // DirectDraw target before the game-state dispatch.
            output.retain_original_global_movie_pixels=true;
            return output;
        }
        const auto finish=fireworks::pre_show_movie_completion_action();
        editor_.internal_state=finish.next_state;
        if(finish.stop_all_managed_sounds)
            output.audio.push_back({AudioOperation::StopManagedSounds});
        output.audio.push_back({
            AudioOperation::ManagedSoundId,{},finish.sound_id,
            finish.sound_priority,finish.arbitration_class,
            finish.mark_sound_slot_input_interruptible
        });
        // State 8 must subsequently load the authentic 23-movie bank. This
        // transition alone does not pretend the authored show is complete.
        return output;
    }
    if(state != InternalState::Editor &&
       state != InternalState::DeleteSelected &&
       state != InternalState::EditorDragRelease) {
        output.fatal_error =
            "Fireworks show/preview/certificate requires original "
            "23-movie Bink bank; refusing synthetic completion";
        return output;
    }

    if (awaiting_delete_confirmation_ && input.confirmation_resolved) {
        if (input.confirmation_yes) {
            (void)fireworks::apply_delete_all_confirmation(
                editor_, sequence_, true);
        }
        awaiting_delete_confirmation_ = false;
    }

    const auto control = fireworks::update_editor_controls(
        editor_,
        {input.pointer_x, input.pointer_y, input.click_pulse,
         false, input.click_pulse});
    if (control.sound_id) {
        append_sound(output, *control.sound_id, 90, 1);
    }
    if (control.open_yes_no_confirmation) {
        awaiting_delete_confirmation_ = true;
        output.open_yes_no_confirmation = true;
        output.yes_no_context = control.confirmation_context;
    }

    // The 33-entry hit table is ordered: 18 grid cells, 12 palette
    // entries, then the three controls processed above.
    if (input.click_pulse &&
        (editor_.internal_state == InternalState::Editor ||
         editor_.internal_state == InternalState::DeleteSelected)) {
        for (std::size_t i = 0; i < 30; ++i) {
            const auto& region = layout_->interactive_regions[i];
            if (!strict_hit(region.rect, input.pointer_x, input.pointer_y)) {
                continue;
            }
            if (i < fireworks::kTimelineSlots) {
                const auto row = i / fireworks::kTimelineColumns;
                const auto column = i % fireworks::kTimelineColumns;
                if (editor_.internal_state == InternalState::DeleteSelected) {
                    (void)sequence_.remove(row, column);
                } else {
                    (void)fireworks::begin_placement_region_action(
                        sequence_, row, column);
                    const auto result = fireworks::complete_placement_release(
                        editor_, sequence_, row, column,
                        static_cast<int>(nonnegative_roll(input.random_value) % 4));
                    if (result.sound_id) append_sound(output, *result.sound_id);
                }
            } else if (editor_.internal_state == InternalState::Editor) {
                const auto palette = fireworks::process_palette_action(
                    editor_, static_cast<fireworks::FireworkType>(i - 18),
                    static_cast<int>(nonnegative_roll(input.random_value) % 5));
                if (palette.begin.sound_id) {
                    append_sound(output, *palette.begin.sound_id);
                }
            }
            break;
        }
    }

    draw_original_editor();
    if (const auto overlay = control_overlay(control)) {
        output.draws.push_back(*overlay);
    }
    return output;
}

bool FireworksDriver::unload(std::string& error) {
    if (!layout_) {
        error.clear();
        return true;
    }

    std::error_code ec;
    std::filesystem::create_directories(save_dir_, ec);
    if (ec) {
        error = "cannot create Fireworks save directory: " + ec.message();
        return false;
    }

    const auto name = fireworks::retail_grid_filename(
        static_cast<std::size_t>(player_index_));
    std::ofstream output(save_dir_ / std::string(name),
                         std::ios::binary | std::ios::trunc);
    if (!output) {
        error = "cannot open Fireworks save layout for writing";
        return false;
    }
    try {
        fireworks::write_retail_grid(output, sequence_.retail_grid());
        output.flush();
        if (!output) {
            error = "cannot flush 72-byte Fireworks layout";
            return false;
        }
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }

    layout_.reset();
    sequence_.clear();
    editor_ = fireworks::EditorRuntimeState{};
    player_index_ = -1;
    awaiting_delete_confirmation_ = false;
    error.clear();
    return true;
}

} // namespace btb::full_game
