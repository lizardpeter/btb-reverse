#pragma once
#include "btb/fireworks_certificate.hpp"
#include "btb/full_game_runtime.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace btb::fireworks {

// Exactly the retail 004138A0 certificate draw order: opaque game artwork,
// centered original brown-font glyph sprites, selected original badge.
// This is NOT a synthesized font or a Windows/GDI substitute.
[[nodiscard]] inline bool render_original_certificate(
    const profiles::ProfileInfo& player,
    std::span<const full_game::Rect> brown_font,
    full_game::ActivityFrameOutput& frame,
    std::string& error) {

    if(player.name_length<0 ||
       player.name_length>static_cast<int>(profiles::kMaxNameGlyphs)) {
        error="original certificate has invalid profile name length";
        return false;
    }
    if(player.badge_index>=profiles::kBadgeCount ||
       player.badge_index<-1) {
        error="original certificate profile badge index is invalid";
        return false;
    }
    frame.draws.push_back({
        "Data/SubGameFirework/certificate.bmp",0,0,std::nullopt,false
    });
    int name_width=0;
    for(int i=0;i<player.name_length;++i) {
        const int glyph=player.name_glyph_codes[static_cast<std::size_t>(i)];
        if(glyph<0||static_cast<std::size_t>(glyph)>=brown_font.size()) {
            error="original certificate references an unavailable glyph";
            return false;
        }
        const auto& rect=brown_font[static_cast<std::size_t>(glyph)];
        if(rect.right<=rect.left || rect.bottom<rect.top) {
            error="original certificate source font rectangle is invalid";
            return false;
        }
        name_width+=rect.right-rect.left;
    }
    int left=kCertificateNameCenterX-name_width/2;
    for(int i=0;i<player.name_length;++i) {
        const auto idx=static_cast<std::size_t>(
            player.name_glyph_codes[static_cast<std::size_t>(i)]);
        auto rect=brown_font[idx];
        ++rect.bottom; // retail inclusive bottom from font-data table
        frame.draws.push_back({
            "Data/ui/enter name boxed font.bmp",left,
            kCertificateNameY,rect,true
        });
        left+=rect.right-rect.left;
    }
    if(player.badge_index>=0) {
        const auto src=certificate_badge_source_rect(player.badge_index);
        frame.draws.push_back({
            "Data/ui/fonttools.bmp",kCertificateBadgeX,kCertificateBadgeY,
            full_game::Rect{src.left,src.top,src.right,src.bottom},true
        });
    }
    error.clear();
    return true;
}

// The original print overlay is a separate DD7 bitmap drawn at (292,417)
// when the pointer hovers over the strict (296,418)-(348,466) hit region.
// Native printing itself must be handled by the original shared printer,
// never by treating a click as successful without a printable bitmap.
inline void draw_original_certificate_print_hover(
    full_game::ActivityFrameOutput& frame,bool pressed) {
    frame.draws.push_back({
        pressed ? "Data/SubGameFirework/certprintdep.bmp"
                : "Data/SubGameFirework/certprint.bmp",
        292,417,std::nullopt,true
    });
}

} // namespace btb::fireworks
