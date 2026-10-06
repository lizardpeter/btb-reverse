#pragma once

#include "btb/spud_skate_runtime.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

namespace btb::spud_skate {

enum class SurfaceKind {
    BadMovie,
    NormalMovie,
    OkayMovie,
    GoodMovie,
    EndMovie,
    Background,
    ScoreDigits,
    NormalOverlay,
    OkayOverlay,
    GoodOverlay,
};

struct SurfaceBinding {
    SurfaceKind kind{};
    std::uint32_t global_address{};
    std::string_view filename{};
    bool movie_surface{};
};

inline constexpr std::array<SurfaceBinding,10> kSurfaceBindings{{
    {SurfaceKind::BadMovie,     0x00514E8C, "bad.bik", true},
    {SurfaceKind::NormalMovie,  0x00514E90, "normal.bik", true},
    {SurfaceKind::OkayMovie,    0x00514E94, "ok.bik", true},
    {SurfaceKind::GoodMovie,    0x00514E98, "good.bik", true},
    {SurfaceKind::EndMovie,     0x00514E9C, "end.bik", true},
    {SurfaceKind::Background,   0x00514EA0,
     "Data\\SubGameSpudSkate\\minimised.bmp", false},
    {SurfaceKind::ScoreDigits,  0x005148B4,
     "Data\\SubGameSpudSkate\\skatespudscore.bmp", false},
    {SurfaceKind::NormalOverlay,0x00514B60,
     "Data\\SubGameSpudSkate\\1.bmp", false},
    {SurfaceKind::OkayOverlay,  0x00514B64,
     "Data\\SubGameSpudSkate\\2.bmp", false},
    {SurfaceKind::GoodOverlay,  0x00514B68,
     "Data\\SubGameSpudSkate\\3.bmp", false},
}};

[[nodiscard]] constexpr const SurfaceBinding*
surface_binding(SurfaceKind kind) noexcept {
    for (const auto& binding : kSurfaceBindings) {
        if (binding.kind == kind) {
            return &binding;
        }
    }
    return nullptr;
}

[[nodiscard]] constexpr SurfaceKind movie_surface_for_quality(
    StuntQuality quality) noexcept {
    switch (quality) {
    case StuntQuality::Bad:    return SurfaceKind::BadMovie;
    case StuntQuality::Normal: return SurfaceKind::NormalMovie;
    case StuntQuality::Okay:   return SurfaceKind::OkayMovie;
    case StuntQuality::Good:   return SurfaceKind::GoodMovie;
    }
    return SurfaceKind::BadMovie;
}

[[nodiscard]] constexpr std::optional<SurfaceKind>
overlay_surface_for_quality(StuntQuality quality) noexcept {
    switch (quality) {
    case StuntQuality::Bad:
        return std::nullopt;
    case StuntQuality::Normal:
        return SurfaceKind::NormalOverlay;
    case StuntQuality::Okay:
        return SurfaceKind::OkayOverlay;
    case StuntQuality::Good:
        return SurfaceKind::GoodOverlay;
    }
    return std::nullopt;
}

struct DestinationRect {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t width{};
    std::int32_t height{};
};

inline constexpr DestinationRect kMovieDestination{
    kMovieScreenX,
    kMovieScreenY,
    kMovieWidth,
    kMovieHeight,
};

inline constexpr DestinationRect kQualityOverlayDestination{
    kQualityOverlayX,
    kQualityOverlayY,
    0,
    0,
};

struct SynchronizedPresentation {
    SurfaceKind movie_surface{SurfaceKind::BadMovie};
    DestinationRect movie_destination{kMovieDestination};

    std::optional<SurfaceKind> quality_overlay{};
    std::int32_t quality_overlay_x{kQualityOverlayX};
    std::int32_t quality_overlay_y{kQualityOverlayY};

    ScoreRenderPlan score{};
    bool background_present{true};
};

[[nodiscard]] constexpr SynchronizedPresentation
synchronized_presentation(
    StuntQuality quality,
    std::int32_t score) noexcept {
    return {
        movie_surface_for_quality(quality),
        kMovieDestination,
        overlay_surface_for_quality(quality),
        kQualityOverlayX,
        kQualityOverlayY,
        score_render_plan(score),
        true,
    };
}

struct EndMoviePresentation {
    SurfaceKind movie_surface{SurfaceKind::EndMovie};
    DestinationRect movie_destination{kMovieDestination};
    bool background_present{true};
    bool quality_overlay_present{false};
    ScoreRenderPlan score{};
};

[[nodiscard]] constexpr EndMoviePresentation
end_movie_presentation(std::int32_t score) noexcept {
    return {
        SurfaceKind::EndMovie,
        kMovieDestination,
        true,
        false,
        score_render_plan(score),
    };
}

struct PlaybackPresentation {
    PlaybackPhase phase{PlaybackPhase::SynchronizedRun};
    std::optional<SynchronizedPresentation> synchronized{};
    std::optional<EndMoviePresentation> end_movie{};
};

[[nodiscard]] constexpr PlaybackPresentation presentation_for_runtime(
    const RuntimeState& runtime) noexcept {

    if (runtime.phase == PlaybackPhase::SynchronizedRun) {
        return {
            runtime.phase,
            synchronized_presentation(runtime.quality, runtime.score),
            std::nullopt,
        };
    }

    if (runtime.phase == PlaybackPhase::EndMovie) {
        return {
            runtime.phase,
            std::nullopt,
            end_movie_presentation(runtime.score),
        };
    }

    return {
        runtime.phase,
        std::nullopt,
        std::nullopt,
    };
}

struct QualityStreamSet {
    std::array<SurfaceKind,4> streams{{
        SurfaceKind::BadMovie,
        SurfaceKind::NormalMovie,
        SurfaceKind::OkayMovie,
        SurfaceKind::GoodMovie,
    }};
    bool remain_frame_synchronized{true};
    std::int32_t seek_target_after_movie_end{44};
};

[[nodiscard]] constexpr QualityStreamSet
quality_stream_set(std::int32_t loop_start_frame) noexcept {
    QualityStreamSet set;
    set.seek_target_after_movie_end = loop_start_frame;
    return set;
}

} // namespace btb::spud_skate
