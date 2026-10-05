#pragma once

#include <cstdint>
#include <optional>

namespace btb::fireworks {

inline constexpr std::int32_t kCertificateNameCenterX = 491;
inline constexpr std::int32_t kCertificateNameY = 135;

inline constexpr std::int32_t kCertificateBadgeX = 471;
inline constexpr std::int32_t kCertificateBadgeY = 156;
inline constexpr std::int32_t kCertificateBadgeCellWidth = 50;
inline constexpr std::int32_t kCertificateBadgeHeight = 45;

inline constexpr std::int32_t kCertificatePrintLeft = 296;
inline constexpr std::int32_t kCertificatePrintTop = 418;
inline constexpr std::int32_t kCertificatePrintRight = 348;
inline constexpr std::int32_t kCertificatePrintBottom = 466;

inline constexpr std::int32_t kCertificatePrintHoverSoundId = 143; // CT_BOB_02
inline constexpr std::int32_t kCertificateRandomVoiceFirst = 144; // CT_BOB_03
inline constexpr std::int32_t kCertificateRandomVoiceCount = 8;
inline constexpr std::int32_t kCertificateVoicePriority = 50;
inline constexpr std::int32_t kCertificateHoverPlaybackFlag = 2;
inline constexpr std::int32_t kCertificateRandomPlaybackFlag = 0;

[[nodiscard]] constexpr std::int32_t certificate_random_voice_sound_id(
    std::int32_t random_mod_8) noexcept {
    return random_mod_8 >= 0 && random_mod_8 < kCertificateRandomVoiceCount
        ? kCertificateRandomVoiceFirst + random_mod_8
        : -1;
}

[[nodiscard]] constexpr bool certificate_print_hit(
    std::int32_t x,
    std::int32_t y) noexcept {

    // Retail uses strict comparisons.
    return x > kCertificatePrintLeft &&
           x < kCertificatePrintRight &&
           y > kCertificatePrintTop &&
           y < kCertificatePrintBottom;
}

enum class CertificateActionKind {
    None,
    PrintCurrentFrame,
    PlayHoverVoice,
    PlayRandomVoice,
};

struct CertificateInteractionState {
    bool print_hover_voice_latched{};
    bool random_voice_latched{};
    std::int32_t last_random_candidate_id{};
};

struct CertificateInteractionInput {
    std::int32_t mouse_x{};
    std::int32_t mouse_y{};
    bool click_active{};
    bool any_managed_sound_playing{};
    std::int32_t candidate_random_mod_8{-1};
    std::int32_t playback_random_mod_8{-1};
};

struct CertificateInteractionOutput {
    CertificateInteractionState state{};
    CertificateActionKind action{CertificateActionKind::None};
    bool stop_all_managed_sounds{};
    std::optional<std::int32_t> sound_id{};
    std::int32_t sound_priority{};
    std::int32_t sound_playback_flag{};
};

// Exact control-flow model of 0x004138A0 after the certificate/name/badge draw.
//
// Important retail quirk: the idle random-voice path calls rand()%8 twice.
// The first result is converted to 144..151 and compared/stored as the
// non-repeat candidate. A second independent rand()%8 chooses the sound that
// is actually played. Therefore the played voice can still repeat.
[[nodiscard]] constexpr CertificateInteractionOutput
update_certificate_interaction(
    CertificateInteractionState state,
    const CertificateInteractionInput& input) noexcept {

    CertificateInteractionOutput output;
    output.state = state;

    const bool over_print =
        certificate_print_hit(input.mouse_x, input.mouse_y);

    if (input.click_active) {
        if (over_print) {
            output.action = CertificateActionKind::PrintCurrentFrame;
            output.stop_all_managed_sounds = true;
        }
        return output;
    }

    if (over_print) {
        if (!output.state.print_hover_voice_latched) {
            output.state.print_hover_voice_latched = true;
            output.action = CertificateActionKind::PlayHoverVoice;
            output.sound_id = kCertificatePrintHoverSoundId;
            output.sound_priority = kCertificateVoicePriority;
            output.sound_playback_flag = kCertificateHoverPlaybackFlag;
        }
        return output;
    }

    output.state.print_hover_voice_latched = false;

    if (input.any_managed_sound_playing ||
        output.state.random_voice_latched) {
        return output;
    }

    const auto candidate =
        certificate_random_voice_sound_id(input.candidate_random_mod_8);
    if (candidate < 0 ||
        candidate == output.state.last_random_candidate_id) {
        return output;
    }

    output.state.random_voice_latched = true;
    output.state.last_random_candidate_id = candidate;

    const auto played =
        certificate_random_voice_sound_id(input.playback_random_mod_8);
    if (played < 0) {
        return output;
    }

    output.action = CertificateActionKind::PlayRandomVoice;
    output.sound_id = played;
    output.sound_priority = kCertificateVoicePriority;
    output.sound_playback_flag = kCertificateRandomPlaybackFlag;
    return output;
}

// DrawFireworksCertificateScreen never writes the Fireworks internal state or
// the outer game-flow state. State 17 remains dormant; certificate exit is
// handled only by shared activity-level exit/back flags outside this function.
inline constexpr bool kCertificateHasDirectExitAction = false;

struct BadgeSourceRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

[[nodiscard]] constexpr BadgeSourceRect certificate_badge_source_rect(
    std::int32_t badge_index) noexcept {

    if (badge_index < 0) {
        return {};
    }

    const auto left = badge_index * kCertificateBadgeCellWidth;
    return {
        left,
        0,
        left + kCertificateBadgeCellWidth,
        kCertificateBadgeHeight,
    };
}

} // namespace btb::fireworks
