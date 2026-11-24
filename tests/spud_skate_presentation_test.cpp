#include "btb/spud_skate_presentation.hpp"

#include <cassert>

using namespace btb::spud_skate;

int main() {
    static_assert(kSurfaceBindings.size() == 10);

    constexpr auto* bad =
        surface_binding(SurfaceKind::BadMovie);
    static_assert(bad);
    static_assert(bad->global_address == 0x00514E8C);
    static_assert(bad->filename == "bad.bik");
    static_assert(bad->movie_surface);

    constexpr auto* normal =
        surface_binding(SurfaceKind::NormalMovie);
    static_assert(normal && normal->global_address == 0x00514E90);
    static_assert(normal->filename == "normal.bik");

    constexpr auto* okay =
        surface_binding(SurfaceKind::OkayMovie);
    static_assert(okay && okay->global_address == 0x00514E94);
    static_assert(okay->filename == "ok.bik");

    constexpr auto* good =
        surface_binding(SurfaceKind::GoodMovie);
    static_assert(good && good->global_address == 0x00514E98);
    static_assert(good->filename == "good.bik");

    constexpr auto* end =
        surface_binding(SurfaceKind::EndMovie);
    static_assert(end && end->global_address == 0x00514E9C);
    static_assert(end->filename == "end.bik");

    constexpr auto* background =
        surface_binding(SurfaceKind::Background);
    static_assert(background);
    static_assert(background->global_address == 0x00514EA0);
    static_assert(background->filename ==
        "Data\\SubGameSpudSkate\\minimised.bmp");
    static_assert(!background->movie_surface);

    constexpr auto* digits =
        surface_binding(SurfaceKind::ScoreDigits);
    static_assert(digits);
    static_assert(digits->global_address == 0x005148B4);
    static_assert(digits->filename ==
        "Data\\SubGameSpudSkate\\skatespudscore.bmp");

    constexpr auto* overlay1 =
        surface_binding(SurfaceKind::NormalOverlay);
    constexpr auto* overlay2 =
        surface_binding(SurfaceKind::OkayOverlay);
    constexpr auto* overlay3 =
        surface_binding(SurfaceKind::GoodOverlay);
    static_assert(overlay1 && overlay1->global_address == 0x00514B60);
    static_assert(overlay2 && overlay2->global_address == 0x00514B64);
    static_assert(overlay3 && overlay3->global_address == 0x00514B68);

    static_assert(
        movie_surface_for_quality(StuntQuality::Bad) ==
        SurfaceKind::BadMovie);
    static_assert(
        movie_surface_for_quality(StuntQuality::Normal) ==
        SurfaceKind::NormalMovie);
    static_assert(
        movie_surface_for_quality(StuntQuality::Okay) ==
        SurfaceKind::OkayMovie);
    static_assert(
        movie_surface_for_quality(StuntQuality::Good) ==
        SurfaceKind::GoodMovie);

    static_assert(!overlay_surface_for_quality(StuntQuality::Bad));
    static_assert(
        overlay_surface_for_quality(StuntQuality::Normal) ==
        SurfaceKind::NormalOverlay);
    static_assert(
        overlay_surface_for_quality(StuntQuality::Okay) ==
        SurfaceKind::OkayOverlay);
    static_assert(
        overlay_surface_for_quality(StuntQuality::Good) ==
        SurfaceKind::GoodOverlay);

    static_assert(kMovieDestination.x == 20);
    static_assert(kMovieDestination.y == 20);
    static_assert(kMovieDestination.width == 600);
    static_assert(kMovieDestination.height == 380);

    constexpr auto bad0 =
        synchronized_presentation(StuntQuality::Bad, 0);
    static_assert(bad0.movie_surface == SurfaceKind::BadMovie);
    static_assert(!bad0.quality_overlay);
    static_assert(bad0.score.count == 1);
    static_assert(bad0.score.digits[0].digit == 0);
    static_assert(bad0.score.digits[0].x == 475);
    static_assert(bad0.score.digits[0].y == 415);

    constexpr auto good45 =
        synchronized_presentation(StuntQuality::Good, 45);
    static_assert(good45.movie_surface == SurfaceKind::GoodMovie);
    static_assert(good45.quality_overlay == SurfaceKind::GoodOverlay);
    static_assert(good45.quality_overlay_x == 267);
    static_assert(good45.quality_overlay_y == 415);
    static_assert(good45.score.count == 2);
    static_assert(good45.score.digits[0].digit == 4);
    static_assert(good45.score.digits[0].x == 450);
    static_assert(good45.score.digits[1].digit == 5);
    static_assert(good45.score.digits[1].x == 475);

    constexpr auto result45 = end_movie_presentation(45);
    static_assert(result45.movie_surface == SurfaceKind::EndMovie);
    static_assert(result45.movie_destination.x == 20);
    static_assert(result45.movie_destination.y == 20);
    static_assert(result45.background_present);
    static_assert(!result45.quality_overlay_present);
    static_assert(result45.score.count == 2);

    RuntimeState runtime;
    runtime.phase = PlaybackPhase::SynchronizedRun;
    runtime.quality = StuntQuality::Okay;
    runtime.score = 19;

    auto presentation = presentation_for_runtime(runtime);
    assert(presentation.phase == PlaybackPhase::SynchronizedRun);
    assert(presentation.synchronized);
    assert(!presentation.end_movie);
    assert(presentation.synchronized->movie_surface ==
           SurfaceKind::OkayMovie);
    assert(presentation.synchronized->quality_overlay ==
           SurfaceKind::OkayOverlay);
    assert(presentation.synchronized->score.count == 2);
    assert(presentation.synchronized->score.digits[0].digit == 1);
    assert(presentation.synchronized->score.digits[1].digit == 9);

    runtime.phase = PlaybackPhase::EndMovie;
    presentation = presentation_for_runtime(runtime);
    assert(!presentation.synchronized);
    assert(presentation.end_movie);
    assert(presentation.end_movie->movie_surface ==
           SurfaceKind::EndMovie);
    assert(presentation.end_movie->score.count == 2);

    runtime.phase = PlaybackPhase::Complete;
    presentation = presentation_for_runtime(runtime);
    assert(!presentation.synchronized);
    assert(!presentation.end_movie);

    constexpr auto streams = quality_stream_set(44);
    static_assert(streams.streams[0] == SurfaceKind::BadMovie);
    static_assert(streams.streams[1] == SurfaceKind::NormalMovie);
    static_assert(streams.streams[2] == SurfaceKind::OkayMovie);
    static_assert(streams.streams[3] == SurfaceKind::GoodMovie);
    static_assert(streams.remain_frame_synchronized);
    static_assert(streams.seek_target_after_movie_end == 44);
}
