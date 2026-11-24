#include "btb/bink32_block_modes.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::bink32_recovered {
namespace {
constexpr std::size_t kTile=8;
constexpr std::size_t kMaxPlaneBytes=64U*1024U*1024U;
constexpr std::size_t kMaxTiles=1U<<20;

[[nodiscard]] bool uses_reference(BinkBlockMode mode) noexcept {
    return mode==BinkBlockMode::Skip || mode==BinkBlockMode::Motion ||
           mode==BinkBlockMode::Residue || mode==BinkBlockMode::Inter;
}
[[nodiscard]] bool allowed_scaled_submode(BinkBlockMode mode) noexcept {
    return mode==BinkBlockMode::Run || mode==BinkBlockMode::Intra ||
           mode==BinkBlockMode::Fill || mode==BinkBlockMode::Pattern ||
           mode==BinkBlockMode::Raw;
}
[[nodiscard]] std::uint8_t sample_at(const BinkBlockCommand& c,
                                      BinkBlockMode mode,
                                      std::size_t x,std::size_t y) noexcept {
    if(mode==BinkBlockMode::Fill) return c.fill;
    if(mode==BinkBlockMode::Pattern)
        return c.pattern_colors[(c.pattern_rows[y]>>x)&1U];
    return c.samples[y*kTile+x];
}
} // namespace

std::optional<BinkModePlane> compose_bink_mode_plane(
    std::size_t width,std::size_t height,
    std::span<const BinkBlockCommand> commands,
    std::span<const std::uint8_t> previous) {
    if(!width || !height || width>16384 || height>16384 ||
       width>kMaxPlaneBytes/height)
        return std::nullopt;
    const std::size_t columns=(width+7)/kTile;
    const std::size_t rows=(height+7)/kTile;
    if(columns>kMaxTiles/rows || commands.size()!=columns*rows)
        return std::nullopt;
    if(!previous.empty() && previous.size()!=width*height)
        return std::nullopt;

    BinkModePlane frame{};
    frame.width=width;
    frame.height=height;
    frame.pixels.resize(width*height);
    // A malformed Scaled anchor cannot overwrite standalone neighbors.
    std::vector<std::uint8_t> covered(commands.size(),0);
    for(std::size_t ty=0;ty<rows;++ty) {
        for(std::size_t tx=0;tx<columns;++tx) {
            const std::size_t index=ty*columns+tx;
            const auto& cmd=commands[index];
            if(covered[index]) {
                if(cmd.mode!=BinkBlockMode::Covered)return std::nullopt;
                continue;
            }
            if(cmd.mode==BinkBlockMode::Covered)return std::nullopt;
            const std::size_t x0=tx*kTile;
            const std::size_t y0=ty*kTile;
            if(cmd.mode==BinkBlockMode::Scaled) {
                if((tx&1U) || (ty&1U) || tx+1>=columns ||
                   ty+1>=rows || !allowed_scaled_submode(cmd.scaled_submode))
                    return std::nullopt;
                const std::size_t right=index+1;
                const std::size_t below=index+columns;
                const std::size_t diagonal=below+1;
                if(commands[right].mode!=BinkBlockMode::Covered ||
                   commands[below].mode!=BinkBlockMode::Covered ||
                   commands[diagonal].mode!=BinkBlockMode::Covered)
                    return std::nullopt;
                covered[right]=covered[below]=covered[diagonal]=1;
                for(std::size_t dy=0;dy<16 && y0+dy<height;++dy)
                    for(std::size_t dx=0;dx<16 && x0+dx<width;++dx)
                        frame.pixels[(y0+dy)*width+x0+dx]=
                            sample_at(cmd,cmd.scaled_submode,dx/2,dy/2);
                continue;
            }
            if(static_cast<unsigned>(cmd.mode)>9U)return std::nullopt;
            if(uses_reference(cmd.mode) && previous.size()!=width*height)
                return std::nullopt;
            for(std::size_t dy=0;dy<kTile && y0+dy<height;++dy) {
                for(std::size_t dx=0;dx<kTile && x0+dx<width;++dx) {
                    const auto x=x0+dx;
                    const auto y=y0+dy;
                    std::uint8_t pixel{};
                    if(uses_reference(cmd.mode)) {
                        const auto srcx=static_cast<std::int64_t>(x)+
                            (cmd.mode==BinkBlockMode::Skip?0:cmd.motion_x);
                        const auto srcy=static_cast<std::int64_t>(y)+
                            (cmd.mode==BinkBlockMode::Skip?0:cmd.motion_y);
                        if(srcx<0 || srcy<0 ||
                           srcx>=static_cast<std::int64_t>(width) ||
                           srcy>=static_cast<std::int64_t>(height))
                            return std::nullopt;
                        pixel=previous[static_cast<std::size_t>(srcy)*width+
                                       static_cast<std::size_t>(srcx)];
                        if(cmd.mode==BinkBlockMode::Residue ||
                           cmd.mode==BinkBlockMode::Inter) {
                            // Original x86 8-bit addition wraps modulo 256.
                            pixel=static_cast<std::uint8_t>(
                                static_cast<int>(pixel)+
                                static_cast<int>(cmd.residuals[dy*kTile+dx]));
                        }
                    } else {
                        // Run and Intra require upstream decoded 8x8 pixels.
                        pixel=sample_at(cmd,cmd.mode,dx,dy);
                    }
                    frame.pixels[y*width+x]=pixel;
                }
            }
        }
    }
    return frame;
}
} // namespace btb::bink32_recovered
