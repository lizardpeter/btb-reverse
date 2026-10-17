#include "btb/full_game_fireworks.hpp"
#include "btb/fireworks_certificate_native.hpp"

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
        pre_show_movie_open_ = false;
        movie_bank_open_ = false;
        show_runtime_.reset();
        show_started_centiseconds_ = 0;
        certificate_state_={};
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
        if(!open_pre_show_movie_ || !cancel_pre_show_movie_) {
            output.fatal_error=
                "original Fireworks pre-show Bink open/close provider is unavailable";
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
        pre_show_movie_open_=true;
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
        // The original global Bink service has already BinkClosed on
        // true EOS. No extra activity-owned close should affect a later movie.
        pre_show_movie_open_=false;
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
    if(state == InternalState::ShowSetup) {
        if(!open_movie_bank_ || !close_movie_bank_) {
            output.fatal_error=
                "original Fireworks 23-movie bank provider unavailable";
            return output;
        }
        std::string error;
        if(!open_movie_bank_(original_data_dir_,error)) {
            output.fatal_error=error.empty()
                ? "original Fireworks 23-movie bank cannot load" : error;
            return output;
        }
        movie_bank_open_=true;
        show_runtime_.reset();
        show_started_centiseconds_=input.elapsed_centiseconds;
        editor_.internal_state=InternalState::ShowPlayback;
        return output;
    }
    if(state == InternalState::ShowPlayback) {
        if(!movie_bank_open_ || !advance_show_frame_) {
            output.fatal_error=
                "Fireworks show lacks the original decoder/compositor";
            return output;
        }
        fireworks::ShowFrameInput physical{};
        const auto elapsed=static_cast<std::int64_t>(
            input.elapsed_centiseconds)-
            static_cast<std::int64_t>(show_started_centiseconds_);
        physical.elapsed_ticks=static_cast<std::int32_t>(
            elapsed<0?0:elapsed>2147483647LL?2147483647LL:elapsed);
        physical.any_managed_sound_playing=input.managed_sound_playing;
        // Physical per-ID status from recovered IsSoundIdPlaying(349),
        // including the real DirectSound slot GetStatus observations.
        physical.crowd_end_sound_playing=
            input.fireworks_crowd_end_sound_playing;
        if(original_random_ && show_runtime_.crowd_phase()<3 &&
           !input.managed_sound_playing) {
            physical.random_chance_roll_0_to_9=
                original_random_->next_rand()%10;
            if(physical.random_chance_roll_0_to_9==0)
                physical.random_variant_roll_0_to_24=
                    original_random_->next_rand()%25;
        }
        fireworks::ShowFrameOutput decisions{};
        std::string error;
        if(!advance_show_frame_(
                sequence_,show_runtime_,physical,decisions,error)) {
            output.fatal_error=error.empty()
                ? "original Fireworks Bink show frame could not decode"
                : error;
            return output;
        }
        if(decisions.stop_all_before_sound)
            output.audio.push_back({AudioOperation::StopManagedSounds});
        if(decisions.sound_id) {
            append_sound(output,*decisions.sound_id,
                         decisions.sound_priority,
                         decisions.sound_arbitration_class);
        }
        if(decisions.stop_all_for_certificate)
            output.audio.push_back({AudioOperation::StopManagedSounds});
        if(decisions.enter_certificate)
            editor_.internal_state=InternalState::Certificate;
        // The original authored-show background is NOT the editor's
        // Bk_01e.bmp. Its verified registered DD7 surface is 0x0050AB10.
        // Render this opaque 640x480 retail bitmap before the separately
        // queued 23-Bink effect/crowd overlays.
        output.draws.push_back({
            "Data\\SubGameFirework\\graph\\BK_01v.bmp",
            0,0,std::nullopt,false
        });
        // Render_ordered clears the normal DD7 backbuffer; the native
        // game device then blits exactly the queued decoded effect movies
        // in original crowd/active/topmiddle source order.
        return output;
    }
    if(state == InternalState::Certificate) {
        if(!certificate_profile_) {
            output.fatal_error=
                "original selected-profile glyph/badge service unavailable";
            return output;
        }
        profiles::ProfileInfo profile{};
        std::span<const Rect> font{};
        std::string error;
        if(!certificate_profile_(player_index_,profile,font,error)) {
            output.fatal_error=error.empty()
                ? "original certificate profile metadata unavailable" : error;
            return output;
        }
        if(!fireworks::render_original_certificate(
                profile,font,output,error)) {
            output.fatal_error=error;
            return output;
        }
        fireworks::CertificateInteractionInput buttons{};
        buttons.mouse_x=input.pointer_x;
        buttons.mouse_y=input.pointer_y;
        buttons.click_active=input.click_pulse;
        buttons.any_managed_sound_playing=input.managed_sound_playing;
        // Source requests the CRT random stream only after the real
        // voice-busy gate, the hover hit and its one-shot latch.
        if(!buttons.click_active &&
           !fireworks::certificate_print_hit(
               buttons.mouse_x,buttons.mouse_y) &&
           !buttons.any_managed_sound_playing &&
           !certificate_state_.random_voice_latched &&
           original_random_) {
            buttons.candidate_random_mod_8=original_random_->next_rand()%8;
            // The executable draws a second independent roll only when
            // the first candidate differs from the stored candidate.
            if(fireworks::certificate_random_voice_sound_id(
                   buttons.candidate_random_mod_8)!=
               certificate_state_.last_random_candidate_id)
                buttons.playback_random_mod_8=original_random_->next_rand()%8;
        }
        auto decision=fireworks::update_certificate_interaction(
            certificate_state_,buttons);
        certificate_state_=decision.state;
        if(fireworks::certificate_print_hit(
               input.pointer_x,input.pointer_y) &&
           !input.click_pulse)
            fireworks::draw_original_certificate_print_hover(output,false);
        if(decision.sound_id)
            append_sound(output,*decision.sound_id,
                decision.sound_priority,decision.sound_arbitration_class);
        if(decision.stop_all_managed_sounds)
            output.audio.push_back({AudioOperation::StopManagedSounds});
        if(decision.action==
           fireworks::CertificateActionKind::PrintCurrentFrame) {
            // Original ExportAndPrintGameImage is a separate native
            // printer provider. Refuse the click until it is connected:
            // this path must never claim the certificate was printed.
            fireworks::draw_original_certificate_print_hover(output,true);
            output.fatal_error=
                "original Fireworks print bitmap/printer adapter unavailable";
        }
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
    // Leaving during original state-15 playback must release its real
    // x86 Bink handle. Otherwise the shared movie service would keep
    // decoding over the next native menu after Fireworks was unloaded.
    if (pre_show_movie_open_) {
        if (!cancel_pre_show_movie_) {
            error="cannot release active original Fireworks Bink decoder";
            return false;
        }
        cancel_pre_show_movie_();
        pre_show_movie_open_=false;
    }
    if(movie_bank_open_) {
        if(!close_movie_bank_) {
            error="cannot release original Fireworks 23-movie bank";
            return false;
        }
        close_movie_bank_();
        movie_bank_open_=false;
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
    pre_show_movie_open_=false;
    movie_bank_open_=false;
    show_runtime_.reset();
    show_started_centiseconds_=0;
    certificate_state_={};
    error.clear();
    return true;
}

} // namespace btb::full_game
