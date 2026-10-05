#include "btb/fireworks_certificate.hpp"

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
}
