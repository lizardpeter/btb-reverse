#include "btb/bink32_pixel_reconstruction.hpp"
#include <array>
#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace btb::bink32_recovered {
namespace {
bool has_block(std::size_t length,std::size_t stride) noexcept {
    // Avoid multiplication overflow from malicious movie dimensions.
    return stride>=kBinkBlockSide &&
           length>=kBinkBlockSide &&
           stride<=(length-kBinkBlockSide)/7;
}
}
bool compose_bink_interframe_block8(
    std::span<const std::uint8_t> reference,std::size_t reference_stride,
    std::span<const std::int8_t> signed_residuals,
    std::span<std::uint8_t> output,std::size_t output_stride) noexcept {
    if(signed_residuals.size()!=kBinkBlockPixels ||
       !has_block(reference.size(),reference_stride) ||
       !has_block(output.size(),output_stride))
        return false;
    std::array<std::uint8_t,kBinkBlockPixels> result{};
    for(std::size_t y=0;y<kBinkBlockSide;++y)
        for(std::size_t x=0;x<kBinkBlockSide;++x) {
            const auto index=y*kBinkBlockSide+x;
            const int prediction=reference[y*reference_stride+x];
            const int delta=signed_residuals[index];
            // Original 3001295D et seq: ADD r8, r/m8 (mod 256), not
            // unsigned saturating addition or color space conversion.
            result[index]=static_cast<std::uint8_t>(prediction+delta);
        }
    for(std::size_t y=0;y<kBinkBlockSide;++y)
        std::copy_n(result.begin()+static_cast<std::ptrdiff_t>(y*kBinkBlockSide),
                    kBinkBlockSide,
                    output.begin()+static_cast<std::ptrdiff_t>(y*output_stride));
    return true;
}
bool store_bink_intra_block8(
    std::span<const std::uint8_t> decoded_pixels,
    std::span<std::uint8_t> output,std::size_t output_stride) noexcept {
    if(decoded_pixels.size()!=kBinkBlockPixels ||
       !has_block(output.size(),output_stride))return false;
    // Snapshot enables callers to use the same underlying buffer with
    // overlapping source/destination without changing the expected copy.
    std::array<std::uint8_t,kBinkBlockPixels> snapshot{};
    std::copy(decoded_pixels.begin(),decoded_pixels.end(),snapshot.begin());
    for(std::size_t y=0;y<kBinkBlockSide;++y)
        std::copy_n(snapshot.begin()+static_cast<std::ptrdiff_t>(y*kBinkBlockSide),
                    kBinkBlockSide,
                    output.begin()+static_cast<std::ptrdiff_t>(y*output_stride));
    return true;
}
} // namespace btb::bink32_recovered
