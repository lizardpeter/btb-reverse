#include "btb/fireworks_certificate.hpp"
#include "btb/fireworks_certificate_native.hpp"
#include <array>
#include <string>

#include <cassert>

using namespace btb::fireworks;

int main() {
    static_assert(kCertificateNameCenterX == 491);
    static_assert(kCertificateNameY == 135);
    static_assert(kCertificateBadgeX == 471);
    static_assert(kCertificateBadgeY == 156);
    static_assert(kCertificatePrintHoverSoundId == 143);
    static_assert(certificate_random_voice_sound_id(0) == 144);
    static_assert(certificate_random_voice_sound_id(7) == 151);
    static_assert(certificate_random_voice_sound_id(8) == -1);
    static_assert(!kCertificateHasDirectExitAction);

    constexpr CertificateInteractionState cert_initial{};

    constexpr CertificateInteractionInput hover_input{
        320, 440, false, false, -1, -1};
    constexpr auto hover =
        update_certificate_interaction(cert_initial, hover_input);
    static_assert(hover.action == CertificateActionKind::PlayHoverVoice);
    static_assert(hover.state.print_hover_voice_latched);
    static_assert(hover.sound_id && *hover.sound_id == 143);
    static_assert(hover.sound_priority == 50);
    static_assert(hover.sound_arbitration_class == 2);

    constexpr auto hover_again =
        update_certificate_interaction(hover.state, hover_input);
    static_assert(hover_again.action == CertificateActionKind::None);

    constexpr CertificateInteractionInput print_input{
        320, 440, true, false, -1, -1};
    constexpr auto print =
        update_certificate_interaction(hover.state, print_input);
    static_assert(print.action == CertificateActionKind::PrintCurrentFrame);
    static_assert(print.stop_all_managed_sounds);

    // Moving outside clears the hover latch and starts the one-shot random
    // certificate voice when no other managed sound is playing.
    constexpr CertificateInteractionInput random_input{
        100, 100, false, false, 0, 7};
    constexpr auto random_voice =
        update_certificate_interaction(hover.state, random_input);
    static_assert(!random_voice.state.print_hover_voice_latched);
    static_assert(random_voice.state.random_voice_latched);
    static_assert(random_voice.state.last_random_candidate_id == 144);
    static_assert(random_voice.action == CertificateActionKind::PlayRandomVoice);
    static_assert(random_voice.sound_id && *random_voice.sound_id == 151);
    static_assert(random_voice.sound_arbitration_class == 0);

    // Retail's stored non-repeat candidate and played voice are separate
    // random rolls, so this intentionally stores 144 while playing 151.
    static_assert(
        random_voice.state.last_random_candidate_id !=
        *random_voice.sound_id);

    constexpr CertificateInteractionState prior_candidate{
        false, false, 144};
    constexpr auto repeat_candidate_blocked =
        update_certificate_interaction(prior_candidate, random_input);
    static_assert(
        repeat_candidate_blocked.action == CertificateActionKind::None);
    static_assert(!repeat_candidate_blocked.state.random_voice_latched);

    constexpr CertificateInteractionInput sound_busy_input{
        100, 100, false, true, 1, 1};
    constexpr auto sound_busy =
        update_certificate_interaction(cert_initial, sound_busy_input);
    static_assert(sound_busy.action == CertificateActionKind::None);

    static_assert(!certificate_print_hit(296, 419));
    static_assert(certificate_print_hit(297, 419));
    static_assert(certificate_print_hit(347, 465));
    static_assert(!certificate_print_hit(348, 465));
    static_assert(!certificate_print_hit(347, 466));

    constexpr auto badge0 = certificate_badge_source_rect(0);
    static_assert(badge0.left == 0);
    static_assert(badge0.right == 50);
    static_assert(badge0.bottom == 45);

    constexpr auto badge4 = certificate_badge_source_rect(4);
    static_assert(badge4.left == 200);
    static_assert(badge4.right == 250);

    // Physical certificate draws original sprites, not host vector text.
    btb::profiles::ProfileInfo profile{};
    profile.name_length=2;
    profile.badge_index=4;
    profile.name_glyph_codes[0]=0;
    profile.name_glyph_codes[1]=1;
    std::array<btb::full_game::Rect,191> original_brown{};
    original_brown[0]={0,0,10,18};
    original_brown[1]={20,0,40,18};
    btb::full_game::ActivityFrameOutput frame{};
    std::string error;
    assert(render_original_certificate(
        profile,original_brown,frame,error));
    assert(frame.draws.size()==4);
    assert(frame.draws[0].source_asset==
           "Data/SubGameFirework/certificate.bmp");
    assert(frame.draws[0].x==0 && frame.draws[0].y==0);
    assert(frame.draws[1].source_asset==
           "Data/ui/enter name boxed font.bmp");
    assert(frame.draws[1].x==476 && frame.draws[1].y==135);
    assert(frame.draws[1].source_rectangle->bottom==19);
    assert(frame.draws[2].x==486);
    assert(frame.draws[2].source_rectangle->left==20);
    assert(frame.draws[3].source_asset=="Data/ui/fonttools.bmp");
    assert(frame.draws[3].x==471 && frame.draws[3].y==156);
    assert(frame.draws[3].source_rectangle->left==200);
    assert(frame.draws[3].source_rectangle->right==250);
    assert(frame.draws[3].source_rectangle->bottom==45);
    draw_original_certificate_print_hover(frame,false);
    assert(frame.draws.back().source_asset==
           "Data/SubGameFirework/certprint.bmp");
    assert(frame.draws.back().x==292 && frame.draws.back().y==417);
    draw_original_certificate_print_hover(frame,true);
    assert(frame.draws.back().source_asset==
           "Data/SubGameFirework/certprintdep.bmp");

    profile.name_glyph_codes[1]=200;
    frame={};
    assert(!render_original_certificate(
        profile,original_brown,frame,error));
    assert(error.find("glyph")!=std::string::npos);
    profile.name_glyph_codes[1]=1;
    profile.badge_index=6;
    frame={};
    assert(!render_original_certificate(
        profile,original_brown,frame,error));
    assert(error.find("badge")!=std::string::npos);
}
