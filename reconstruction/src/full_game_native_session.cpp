#include "btb/full_game_native_session.hpp"

#include <utility>

namespace btb::full_game {

bool NativeGameSession::fail(
    NativeSessionFrame& frame,
    PhysicalPresentationStage stage,
    std::string message) {

    failed_ = true;
    frame.failure = PhysicalPresentationError{stage,std::move(message)};
    return false;
}

NativeSessionFrame NativeGameSession::tick(
    ActivityFrameInput input,
    bool primary_input_pulse,
    bool secondary_input_pulse) {

    NativeSessionFrame frame{};
    if (failed_) {
        frame.failure = PhysicalPresentationError{
            PhysicalPresentationStage::GameAdvance,
            "native session was previously interrupted by provider failure"};
        return frame;
    }

    PhysicalFrameObservation physical{};
    std::string error;
    if (!device_.observe(physical,error)) {
        fail(frame,PhysicalPresentationStage::ObserveDevice,error);
        return frame;
    }

    input.managed_sound_playing = physical.any_managed_voice_playing;
    // The real Bink device owns end-of-stream / input-to-skip signaling.
    // Never artificially complete 13/14 just to make the game advance.
    root_.globals().dispatcher.global_movie_finished =
        physical.global_movie_finished;

    auto game = root_.advance(input);
    frame.prepared.game = game;

    if (game.kind == FrameKind::FrontEndFailed ||
        game.kind == FrameKind::ActivityFailed ||
        game.kind == FrameKind::InvalidState) {
        fail(frame,PhysicalPresentationStage::GameAdvance,
             game.error.empty() ? "original game state dispatch failed"
                                : game.error);
        return frame;
    }

    const auto resolve_movie = [&](const std::string& original)
        -> std::optional<std::filesystem::path> {
        auto resolved=effects_.assets().resolve(original);
        if (!resolved.found()) {
            error="retail movie/help asset could not be resolved: "+original+
                " ("+resolved.error+")";
            return std::nullopt;
        }
        return resolved.absolute_path;
    };

    if (game.original_walkthrough_index) {
        const auto* entry=walkthroughs_.entry(
            *game.original_walkthrough_index);
        if (!entry) {
            fail(frame,PhysicalPresentationStage::MovieResolve,
                 "required original binkwalk.txt index unavailable");
            return frame;
        }
        auto movie=resolve_movie(entry->movie);
        auto help=resolve_movie(entry->spoken_help);
        if (!movie || !help) {
            fail(frame,PhysicalPresentationStage::MovieResolve,error);
            return frame;
        }
        if (walkthrough_open_ &&
            !device_.close_walkthrough_movie(error)) {
            fail(frame,PhysicalPresentationStage::MovieClose,error);
            return frame;
        }
        if (walkthrough_open_) {
            frame.walkthrough_closed = true;
            walkthrough_open_ = false;
        }
        if (!device_.open_walkthrough_movie(*movie,*help,error)) {
            fail(frame,PhysicalPresentationStage::MovieOpen,error);
            return frame;
        }
        walkthrough_open_ = true;
        frame.walkthrough_opened = true;
    }

    if (game.original_global_intro_movie_index) {
        const auto* title=startups_.movie(
            *game.original_global_intro_movie_index);
        if (!title) {
            fail(frame,PhysicalPresentationStage::MovieResolve,
                 "required original startupmovie.txt index unavailable");
            return frame;
        }
        auto file=resolve_movie(*title);
        if (!file) {
            fail(frame,PhysicalPresentationStage::MovieResolve,error);
            return frame;
        }
        if (!device_.open_global_movie(*file,error)) {
            fail(frame,PhysicalPresentationStage::MovieOpen,error);
            return frame;
        }
        frame.global_movie_opened = true;
    }

    // The retail instruction walkthrough is torn down when leaving its
    // pregame state for Back or Start. Difficulty/Help actions remain.
    if (walkthrough_open_ &&
        game.pregame_action &&
        game.pregame_action->next_state) {
        if (!device_.close_walkthrough_movie(error)) {
            fail(frame,PhysicalPresentationStage::MovieClose,error);
            return frame;
        }
        walkthrough_open_ = false;
        frame.walkthrough_closed = true;
    }

    frame.prepared = effects_.prepare(
        std::move(game),physical.managed,
        primary_input_pulse,secondary_input_pulse);

    // A missing original asset is not permission to render synthetic
    // replacement artwork. Fail the whole physical frame before submission.
    if (!frame.prepared.resources.ready_for_submission() ||
        frame.prepared.missing_catalog_ids ||
        frame.prepared.audio_policy.invalid_requests) {
        fail(frame,PhysicalPresentationStage::AssetResolve,
             "original frame requires missing bitmap, WAV or sound ID");
        return frame;
    }

    for (const auto& command : frame.prepared.resources.sound) {
        if (!device_.apply_sound(command,error)) {
            fail(frame,PhysicalPresentationStage::SubmitAudio,error);
            return frame;
        }
    }

    // Global Bink modes 13/14 decode directly into the common display
    // surface during device.observe(). If the original dispatcher is
    // intercepted waiting for that movie, do not clear the Bink pixels
    // with a synthetic empty gameplay/UI draw list before present.
    const auto mode=root_.globals().dispatcher.generic_screen_mode;
    const bool global_movie_frame =
        frame.prepared.game.kind == FrameKind::Intercept &&
        (mode == 13 || mode == 14);
    if (!global_movie_frame &&
        !device_.draw_ordered(frame.prepared.resources.draws,error)) {
        fail(frame,PhysicalPresentationStage::SubmitDraw,error);
        return frame;
    }
    if (!device_.present(error)) {
        fail(frame,PhysicalPresentationStage::Present,error);
        return frame;
    }

    frame.submitted = true;
    return frame;
}

} // namespace btb::full_game
