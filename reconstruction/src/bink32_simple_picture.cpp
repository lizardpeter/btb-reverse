#include "btb/bink32_simple_picture.hpp"
#include "btb/bink32_run_scan.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace btb::bink32_recovered {
namespace {
template<class T> struct SourcePool {
    std::vector<T> values{};
    std::size_t cursor{};
    std::size_t refills{};
    bool ended{};
    [[nodiscard]] bool needs_refill() const noexcept {
        return !ended && cursor==values.size();
    }
    void refill(std::vector<T> next,bool end) {
        values=std::move(next);
        cursor=0;
        ended=end;
        ++refills;
    }
    [[nodiscard]] std::optional<T> take() noexcept {
        if(cursor==values.size())return std::nullopt;
        return values[cursor++];
    }
};
std::vector<std::int16_t> dc_words(const OriginalBinkDcPlane& plane) {
    std::vector<std::int16_t> words;
    words.reserve(plane.signed_words_le.size()/2);
    for(std::size_t i=0;i+1<plane.signed_words_le.size();i+=2) {
        auto raw=static_cast<std::uint16_t>(
            plane.signed_words_le[i] |
            std::uint16_t(plane.signed_words_le[i+1])<<8);
        words.push_back(std::bit_cast<std::int16_t>(raw));
    }
    return words;
}
} // namespace

std::optional<BinkSimplePictureLuma> decode_bink_simple_picture_luma(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,
    std::span<const std::uint8_t> previous_luma) {
    constexpr std::size_t kMaxPixels=64U*1024U*1024U;
    constexpr std::size_t kMaxTiles=1U<<20;
    if(!width || !height || width>16384 || height>16384 ||
       (width%8)!=0 || (height%8)!=0 ||
       width>kMaxPixels/height ||
       (!previous_luma.empty() && previous_luma.size()!=width*height))
        return std::nullopt;
    const auto columns=width/8,rows=height/8;
    if(columns>kMaxTiles/rows)return std::nullopt;
    const auto tiles=columns*rows;
    // Each 8x8 pixel block may consume up to 64 color samples.
    const auto maximum_samples=std::min(kMaxPixels,width*height);
    const auto prologue=parse_alpha_picture_entropy_prologue(video_packet);
    if(!prologue)return std::nullopt;
    const auto& trees=prologue->picture.trees.trees;
    LsbBitReader bits(video_packet.subspan(
        prologue->picture.initial_byte_offset));
    if(!bits.skip(prologue->picture.trees.consumed_bits))
        return std::nullopt;

    SourcePool<std::uint8_t> types{},subtypes{},colors{},patterns{},runs{};
    SourcePool<std::int8_t> motion_x{},motion_y{};
    SourcePool<std::int16_t> intra_dc{},inter_dc{};
    std::uint8_t last_color_high=0;
    const auto intra_context=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(width),11,1,false);
    const auto inter_context=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(width),11,1,true);

    std::vector<BinkBlockCommand> commands(tiles);
    BinkSimplePictureLuma result{};

    // Bink 0x3000E400 checks all nine bundle refill sources at EACH
    // row boundary, in strict sequence. Ended sources cannot restart.
    for(std::size_t row=0;row<rows;++row) {
        if(types.needs_refill()) {
            auto b=read_bink_block_type_bundle(bits,trees[0],width,tiles);
            if(!b)return std::nullopt;
            types.refill(std::move(b->modes),b->end_of_bundle);
        }
        if(subtypes.needs_refill()) {
            auto b=read_bink_block_type_bundle(bits,trees[1],width/2,tiles);
            if(!b)return std::nullopt;
            subtypes.refill(std::move(b->modes),b->end_of_bundle);
        }
        if(colors.needs_refill()) {
            // read_bink_color_bundle requires the full tree set,
            // including sixteen adaptive high-nibble contexts.
            auto b=read_bink_color_bundle(
                bits,prologue->picture.trees,width,last_color_high,
                false,maximum_samples);
            if(!b)return std::nullopt;
            last_color_high=b->next_high_nibble;
            colors.refill(std::move(b->values),b->end_of_bundle);
        }
        if(patterns.needs_refill()) {
            auto b=read_bink_pattern_bundle(
                bits,trees[19],width,maximum_samples);
            if(!b)return std::nullopt;
            patterns.refill(std::move(b->values),b->end_of_bundle);
        }
        if(motion_x.needs_refill()) {
            auto b=read_bink_signed_motion_bundle(
                bits,trees[20],width,tiles);
            if(!b)return std::nullopt;
            motion_x.refill(std::move(b->values),b->end_of_bundle);
        }
        if(motion_y.needs_refill()) {
            auto b=read_bink_signed_motion_bundle(
                bits,trees[21],width,tiles);
            if(!b)return std::nullopt;
            motion_y.refill(std::move(b->values),b->end_of_bundle);
        }
        if(intra_dc.needs_refill()) {
            auto b=decode_original_bink_dc_context_packet(
                bits,intra_context,tiles);
            if(!b)return std::nullopt;
            auto v=dc_words(*b);
            intra_dc.refill(std::move(v),b->signed_words_le.empty());
        }
        if(inter_dc.needs_refill()) {
            auto b=decode_original_bink_dc_context_packet(
                bits,inter_context,tiles);
            if(!b)return std::nullopt;
            auto v=dc_words(*b);
            inter_dc.refill(std::move(v),b->signed_words_le.empty());
        }
        if(runs.needs_refill()) {
            auto b=read_bink_run_bundle(bits,trees[22],width,
                                        maximum_samples);
            if(!b)return std::nullopt;
            runs.refill(std::move(b->values),b->end_of_bundle);
        }

        // A scaled 16x16 tile consumes one block type on the top
        // row and a matching marker on the bottom row, but only ONE
        // subtype/pixel payload at its upper-left corner.
        for(std::size_t col=0;col<columns;) {
            const auto encoded=types.take();
            if(!encoded || *encoded>=result.mode_counts.size())
                return std::nullopt;
            ++result.mode_counts[*encoded];
            const auto mode=static_cast<BinkBlockMode>(*encoded);
            auto& cmd=commands[row*columns+col];
            const auto populate_pixels=[&](BinkBlockMode pixels)->bool {
                if(pixels==BinkBlockMode::Fill) {
                    const auto color=colors.take();
                    if(!color)return false;
                    cmd.fill=*color;
                    return true;
                }
                if(pixels==BinkBlockMode::Pattern) {
                    for(auto& color:cmd.pattern_colors) {
                        const auto value=colors.take();
                        if(!value)return false;
                        color=*value;
                    }
                    for(auto& mask:cmd.pattern_rows) {
                        const auto value=patterns.take();
                        if(!value)return false;
                        mask=*value;
                    }
                    return true;
                }
                if(pixels==BinkBlockMode::Raw) {
                    for(auto& value:cmd.samples) {
                        const auto color=colors.take();
                        if(!color)return false;
                        value=*color;
                    }
                    return true;
                }
                if(pixels==BinkBlockMode::Run) {
                    // Original Bink1: a four-bit pattern selects one
                    // of sixteen exact 64-coordinate scan orders.
                    // Run and color bundle consumption is independent
                    // of the selected pixel permutation.
                    const auto scan=bits.read(4);
                    if(!scan)return false;
                    const auto emit=[&](std::size_t index,
                                        std::uint8_t pixel) {
                        cmd.samples[bink_run_scan_pixel(
                            static_cast<std::uint8_t>(*scan),index)]=pixel;
                    };
                    std::size_t written=0;
                    while(written<63) {
                        const auto encoded=runs.take();
                        if(!encoded)return false;
                        const std::size_t n=std::size_t(*encoded)+1;
                        if(n>64-written)return false;
                        const auto repeat=bits.read(1);
                        if(!repeat)return false;
                        if(*repeat) {
                            const auto color=colors.take();
                            if(!color)return false;
                            for(std::size_t i=0;i<n;++i)
                                emit(written+i,*color);
                        } else {
                            for(std::size_t i=0;i<n;++i) {
                                const auto color=colors.take();
                                if(!color)return false;
                                emit(written+i,*color);
                            }
                        }
                        written+=n;
                    }
                    if(written==63) {
                        const auto last=colors.take();
                        if(!last)return false;
                        emit(63,*last);
                    }
                    return written>=63;
                }
                return false;
            };
            if(mode==BinkBlockMode::Scaled) {
                if(col&1U || col+1>=columns)return std::nullopt;
                if(row&1U) {
                    // Original odd-row scaled marker has no new
                    // pixels. Verify it covers the previous anchor.
                    if(commands[(row-1)*columns+col].mode!=
                           BinkBlockMode::Scaled ||
                       cmd.mode!=BinkBlockMode::Covered ||
                       commands[row*columns+col+1].mode!=
                           BinkBlockMode::Covered)
                        return std::nullopt;
                    col+=2;
                    continue;
                }
                if(row+1>=rows)return std::nullopt;
                const auto subtype=subtypes.take();
                if(!subtype)return std::nullopt;
                cmd.mode=BinkBlockMode::Scaled;
                cmd.scaled_submode=static_cast<BinkBlockMode>(*subtype);
                // These three source-backed scaled payloads already
                // have an exact nearest-neighbor 2x pixel compositor.
                if(!populate_pixels(cmd.scaled_submode))
                    return std::nullopt;
                commands[row*columns+col+1].mode=BinkBlockMode::Covered;
                commands[(row+1)*columns+col].mode=BinkBlockMode::Covered;
                commands[(row+1)*columns+col+1].mode=BinkBlockMode::Covered;
                col+=2;
                continue;
            }
            if(cmd.mode==BinkBlockMode::Covered)
                return std::nullopt;
            cmd.mode=mode;
            switch(mode) {
            case BinkBlockMode::Skip:
                break;
            case BinkBlockMode::Motion: {
                const auto mx=motion_x.take();
                const auto my=motion_y.take();
                if(!mx || !my)return std::nullopt;
                cmd.motion_x=*mx;
                cmd.motion_y=*my;
                break;
            }
            case BinkBlockMode::Fill:
            case BinkBlockMode::Pattern:
            case BinkBlockMode::Run:
            case BinkBlockMode::Raw:
                if(!populate_pixels(mode))return std::nullopt;
                break;
            default:
                // DCT and residue modes remain deliberately
                // unsupported until original transform data is wired.
                return std::nullopt;
            }
            ++col;
        }
    }
    if(!bits.align(4))return std::nullopt;
    auto output=compose_bink_mode_plane(width,height,commands,previous_luma);
    if(!output)return std::nullopt;
    result.image=std::move(*output);
    result.source_refills={
        types.refills,subtypes.refills,colors.refills,patterns.refills,
        motion_x.refills,motion_y.refills,intra_dc.refills,
        inter_dc.refills,runs.refills
    };
    result.next_plane_byte_offset=
        prologue->picture.initial_byte_offset+bits.bit_position()/8;
    return result;
}
} // namespace btb::bink32_recovered
