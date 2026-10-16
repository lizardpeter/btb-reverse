#include "btb/bink32_entropy_commands.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <utility>

namespace btb::bink32_recovered {
std::optional<ExpandedBlockRun> expand_bink_value_commands(
    LsbBitReader& compressed, const DynamicHuffmanTree& tree,
    std::span<std::uint8_t> destination,std::uint8_t previous) {
    // The retail DLL trusts memory pointers supplied by its own allocator.
    // Native file input is untrusted; do not allocate a pathological plane.
    constexpr std::size_t kMaxBlockValues=1U<<20;
    if(destination.size()>kMaxBlockValues)return std::nullopt;
    auto cursor=compressed;
    std::vector<std::uint8_t> scratch(destination.size());
    constexpr std::array<std::uint8_t,4> run_lengths{4,8,12,32};
    ExpandedBlockRun result{};
    std::size_t pos=0;
    const auto begin=cursor.bit_position();
    while(pos<scratch.size()) {
        // Original 0x30010242 and 0x30010372: 1 = fill remaining
        // destination with immediate four-bit constant.
        const auto immediate=cursor.read(1);
        if(!immediate)return std::nullopt;
        if(*immediate) {
            const auto value=cursor.read(4);
            if(!value)return std::nullopt;
            std::fill(scratch.begin()+static_cast<std::ptrdiff_t>(pos),
                      scratch.end(),static_cast<std::uint8_t>(*value));
            result.filled_remainder=true;
            pos=scratch.size();
            break;
        }
        // Original 0x300102A6..0x300102DC: table-based Huffman read.
        const auto symbol=read_dynamic_huffman_symbol(cursor,tree);
        if(!symbol)return std::nullopt;
        ++result.huffman_symbols;
        const std::uint8_t code=symbol->value;
        if(code<12) {
            scratch[pos++]=code;
            previous=code;
        } else if(code<=15) {
            // Original 0x30010325 indexed 0x30047BDC+code:
            // the four run lengths are 4,8,12,32.
            const auto repeat=run_lengths[code-12];
            if(repeat>scratch.size()-pos)return std::nullopt;
            std::fill_n(scratch.begin()+static_cast<std::ptrdiff_t>(pos),
                        repeat,previous);
            pos+=repeat;
            ++result.repeat_commands;
        } else return std::nullopt;
    }
    result.bits_consumed=cursor.bit_position()-begin;
    std::copy(scratch.begin(),scratch.end(),destination.begin());
    compressed=cursor;
    return result;
}
// x86 30010632..30010651:
// cl = (high << 4)|low; if (cl & 0x80) cl = -(cl & 0x7f);
// else cl &= 0x7f; cl += 0x80, store one unsigned byte.
std::uint8_t combine_bink_signed_nibbles(
    std::uint8_t high,std::uint8_t low) noexcept {
    const auto packed=std::uint8_t(((high&15U)<<4)|(low&15U));
    const auto magnitude=static_cast<unsigned>(packed&0x7fU);
    return static_cast<std::uint8_t>(
        packed&0x80U ? 128U-magnitude : 128U+magnitude);
}
std::optional<std::uint8_t> read_bink_signed_sample(
    LsbBitReader& bits,
    const DynamicHuffmanTree& high_tree,
    const DynamicHuffmanTree& low_tree) noexcept {
    auto probe=bits;
    const auto high=read_dynamic_huffman_symbol(probe,high_tree);
    if(!high)return std::nullopt;
    const auto low=read_dynamic_huffman_symbol(probe,low_tree);
    if(!low)return std::nullopt;
    const auto sample=combine_bink_signed_nibbles(high->value,low->value);
    bits=probe;
    return sample;
}
std::optional<DecodedBinkBlockModes> decode_bink_block_mode_values(
    LsbBitReader& compressed,const DynamicHuffmanTree& tree,
    std::size_t count,std::uint8_t previous_mode) {
    if(!count || count>(1U<<20) || previous_mode>9)
        return std::nullopt;
    auto probe=compressed;
    const auto begin=probe.bit_position();
    // Original Bink read_block_types: ONE branch bit per bundle, not
    // one bit per Huffman symbol. 1 = constant 4-bit value for all t;
    // 0 = Huffman literals with 12..15 repeat previous 4/8/12/32.
    const auto constant=probe.read(1);
    if(!constant)return std::nullopt;
    DecodedBinkBlockModes decoded{};
    decoded.values.resize(count);
    if(*constant) {
        const auto value=probe.read(4);
        if(!value || *value>9)return std::nullopt;
        std::fill(decoded.values.begin(),decoded.values.end(),
                  static_cast<std::uint8_t>(*value));
        decoded.statistics.filled_remainder=true;
    } else {
        constexpr std::array<std::size_t,4> runs{4,8,12,32};
        std::size_t pos=0;
        while(pos<count) {
            const auto symbol=read_dynamic_huffman_symbol(probe,tree);
            if(!symbol)return std::nullopt;
            ++decoded.statistics.huffman_symbols;
            if(symbol->value<12) {
                if(symbol->value>9)return std::nullopt;
                previous_mode=symbol->value;
                decoded.values[pos++]=previous_mode;
            } else {
                const auto n=runs[symbol->value-12];
                if(n>count-pos)return std::nullopt;
                std::fill_n(decoded.values.begin()+
                    static_cast<std::ptrdiff_t>(pos),n,previous_mode);
                pos+=n;
                ++decoded.statistics.repeat_commands;
            }
        }
    }
    decoded.statistics.bits_consumed=probe.bit_position()-begin;
    compressed=probe;
    return decoded;
}

std::optional<BinkBlockTypeBundle> read_bink_block_type_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& type_tree,
    std::size_t plane_width,std::size_t maximum_values) {
    if(!plane_width || plane_width>16384 || !maximum_values ||
       maximum_values>(1U<<20))return std::nullopt;
    // Original 0x3000FC00 derives the count width from 512 + plane
    // width in 8x8 blocks; the first original bundle uses this context.
    const auto context=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(plane_width),4,1,false);
    if(!context.table_bit_width || context.table_bit_width>24)
        return std::nullopt;
    auto probe=bits;
    const auto start=probe.bit_position();
    const auto count=probe.read(context.table_bit_width);
    if(!count || *count>maximum_values)return std::nullopt;
    BinkBlockTypeBundle bundle{};
    bundle.encoded_count=*count;
    if(*count==0) {
        bundle.end_of_bundle=true;
    } else {
        const auto expanded=decode_bink_block_mode_values(
            probe,type_tree,*count,0);
        if(!expanded)return std::nullopt;
        bundle.modes=std::move(expanded->values);
        bundle.commands=expanded->statistics;
    }
    bundle.bits_consumed=probe.bit_position()-start;
    bits=probe;
    return bundle;
}
std::optional<BinkFirstPictureBlockTypes>
decode_first_bink_picture_block_types(
    std::span<const std::uint8_t> video_packet,
    std::size_t plane_width,std::size_t plane_height) {
    if(!plane_width || !plane_height || plane_width>16384 ||
       plane_height>16384 || plane_width>(64U*1024U*1024U)/plane_height)
        return std::nullopt;
    const auto columns=(plane_width+7)/8;
    const auto rows=(plane_height+7)/8;
    if(columns>(1U<<20)/rows)return std::nullopt;
    const auto prologue=parse_alpha_picture_entropy_prologue(video_packet);
    if(!prologue)return std::nullopt;
    auto picture=video_packet.subspan(
        prologue->picture.initial_byte_offset);
    LsbBitReader reader(picture);
    if(!reader.skip(prologue->picture.trees.consumed_bits))
        return std::nullopt;
    const auto modes=read_bink_block_type_bundle(
        reader,prologue->picture.trees.trees[0],
        plane_width,columns*rows);
    if(!modes)return std::nullopt;
    BinkFirstPictureBlockTypes output{};
    output.prologue=std::move(*prologue);
    output.first_bundle=std::move(*modes);
    output.next_picture_bit_offset=
        output.prologue.picture.initial_byte_offset*8+
        reader.bit_position();
    return output;
}

std::optional<BinkSkipLumaFrame>
decode_bink_skip_only_picture_luma(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,
    std::span<const std::uint8_t> previous_luma) {
    // A deliberately narrow retail-decodable subset. Avoid claiming
    // support for other block modes or bundle-value substreams.
    constexpr std::size_t kMaximumPlaneBytes=64U*1024U*1024U;
    if(!width || !height || width>16384 || height>16384 ||
       width%8 || height%8 || width>kMaximumPlaneBytes/height ||
       previous_luma.size()!=width*height)
        return std::nullopt;
    const auto prologue=parse_alpha_picture_entropy_prologue(video_packet);
    if(!prologue)return std::nullopt;
    const auto columns=width/8,rows=height/8;
    LsbBitReader bits(video_packet.subspan(
        prologue->picture.initial_byte_offset));
    if(!bits.skip(prologue->picture.trees.consumed_bits))
        return std::nullopt;
    const auto count_width=[&](std::uint32_t multiplier,
                               std::size_t context_width) {
        return init_entropy_plane(
            0,0x200U,static_cast<std::uint32_t>(context_width),
            4,multiplier,false).table_bit_width;
    };
    // Original first-row read order from 0x3000E400:
    // subordinate modes, color, pattern, motion X/Y,
    // intra/inter DC, then run. Every one must be empty for
    // this skip-only specialization.
    const std::array<std::uint32_t,8> other_widths{
        count_width(1,width/2),count_width(64,width),
        count_width(8,width),count_width(1,width),
        count_width(1,width),count_width(1,width),
        count_width(1,width),count_width(48,width)
    };
    std::size_t remaining=0,decoded=0,bundles=0;
    for(std::size_t row=0;row<rows;++row) {
        if(!remaining) {
            const auto next=read_bink_block_type_bundle(
                bits,prologue->picture.trees.trees[0],width,
                columns*rows-decoded);
            if(!next || next->end_of_bundle || next->modes.empty() ||
               next->modes.size()%columns!=0 ||
               !std::all_of(next->modes.begin(),next->modes.end(),
                            [](std::uint8_t x){return x==0;}))
                return std::nullopt;
            remaining=next->modes.size();
            ++bundles;
        }
        if(row==0) {
            for(const auto width_bits:other_widths) {
                const auto bundle_count=bits.read(width_bits);
                if(!bundle_count || *bundle_count!=0)
                    return std::nullopt;
            }
        }
        if(remaining<columns)return std::nullopt;
        remaining-=columns;
        decoded+=columns;
    }
    if(remaining || decoded!=columns*rows || !bits.align(4))
        return std::nullopt;
    BinkSkipLumaFrame result{};
    result.luma.assign(previous_luma.begin(),previous_luma.end());
    result.block_types_decoded=decoded;
    result.bundles_decoded=bundles;
    result.next_plane_byte_offset=
        prologue->picture.initial_byte_offset+
        bits.bit_position()/8;
    return result;
}


std::optional<BinkSignedMotionBundle> read_bink_signed_motion_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& tree,
    std::size_t plane_width,std::size_t maximum_values) {
    if(!plane_width || plane_width>16384 || !maximum_values ||
       maximum_values>(1U<<20))return std::nullopt;
    const auto count_width=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(plane_width),4,1,false)
        .table_bit_width;
    if(!count_width || count_width>24)return std::nullopt;
    auto probe=bits;
    const auto start=probe.bit_position();
    const auto count=probe.read(count_width);
    if(!count || *count>maximum_values)return std::nullopt;
    BinkSignedMotionBundle out{};
    out.encoded_count=*count;
    if(!*count) {
        out.end_of_bundle=true;
    } else {
        const auto uniform=probe.read(1);
        if(!uniform)return std::nullopt;
        out.constant_bundle=*uniform!=0;
        out.values.resize(*count);
        if(*uniform) {
            const auto magnitude=probe.read(4);
            if(!magnitude)return std::nullopt;
            int signed_value=static_cast<int>(*magnitude);
            if(signed_value) {
                const auto negative=probe.read(1);
                if(!negative)return std::nullopt;
                if(*negative)signed_value=-signed_value;
            }
            std::fill(out.values.begin(),out.values.end(),
                      static_cast<std::int8_t>(signed_value));
        } else {
            for(auto& value:out.values) {
                const auto magnitude=read_dynamic_huffman_symbol(probe,tree);
                if(!magnitude)return std::nullopt;
                int signed_value=magnitude->value;
                if(signed_value) {
                    const auto negative=probe.read(1);
                    if(!negative)return std::nullopt;
                    if(*negative)signed_value=-signed_value;
                }
                value=static_cast<std::int8_t>(signed_value);
            }
        }
    }
    out.bits_consumed=probe.bit_position()-start;
    bits=probe;
    return out;
}
std::optional<BinkPatternBundle> read_bink_pattern_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& tree,
    std::size_t plane_width,std::size_t maximum_values) {
    if(!plane_width || plane_width>16384 || !maximum_values ||
       maximum_values>(1U<<20))return std::nullopt;
    const auto count_width=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(plane_width),4,8,false)
        .table_bit_width;
    if(!count_width || count_width>24)return std::nullopt;
    auto probe=bits;
    const auto start=probe.bit_position();
    const auto count=probe.read(count_width);
    if(!count || *count>maximum_values)return std::nullopt;
    BinkPatternBundle out{};
    out.encoded_count=*count;
    if(!*count) {
        out.end_of_bundle=true;
    } else {
        out.values.resize(*count);
        for(auto& value:out.values) {
            const auto low=read_dynamic_huffman_symbol(probe,tree);
            if(!low)return std::nullopt;
            const auto high=read_dynamic_huffman_symbol(probe,tree);
            if(!high)return std::nullopt;
            value=static_cast<std::uint8_t>(low->value |
                                             (high->value<<4));
        }
    }
    out.bits_consumed=probe.bit_position()-start;
    bits=probe;
    return out;
}


std::optional<BinkRunBundle> read_bink_run_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& tree,
    std::size_t plane_width,std::size_t maximum_values) {
    if(!plane_width || plane_width>16384 || !maximum_values ||
       maximum_values>(1U<<20))return std::nullopt;
    const auto count_width=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(plane_width),4,48,false)
        .table_bit_width;
    if(!count_width || count_width>24)return std::nullopt;
    auto probe=bits;
    const auto start=probe.bit_position();
    const auto count=probe.read(count_width);
    if(!count || *count>maximum_values)return std::nullopt;
    BinkRunBundle out{};
    out.encoded_count=*count;
    if(!*count) {
        out.end_of_bundle=true;
    } else {
        const auto uniform=probe.read(1);
        if(!uniform)return std::nullopt;
        out.constant_bundle=*uniform!=0;
        out.values.resize(*count);
        if(*uniform) {
            const auto v=probe.read(4);
            if(!v)return std::nullopt;
            std::fill(out.values.begin(),out.values.end(),
                      static_cast<std::uint8_t>(*v));
        } else {
            for(auto& value:out.values) {
                const auto v=read_dynamic_huffman_symbol(probe,tree);
                if(!v)return std::nullopt;
                value=v->value;
            }
        }
    }
    out.bits_consumed=probe.bit_position()-start;
    bits=probe;
    return out;
}
std::optional<BinkColorBundle> read_bink_color_bundle(
    LsbBitReader& bits,const BlockEntropyTrees& trees,
    std::size_t plane_width,std::uint8_t previous_high_nibble,
    bool early_signed_color_format,std::size_t maximum_values) {
    if(!plane_width || plane_width>16384 || !maximum_values ||
       maximum_values>(1U<<20) || previous_high_nibble>=16)
        return std::nullopt;
    const auto count_width=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(plane_width),4,64,false)
        .table_bit_width;
    if(!count_width || count_width>24)return std::nullopt;
    auto probe=bits;
    const auto start=probe.bit_position();
    const auto count=probe.read(count_width);
    if(!count || *count>maximum_values)return std::nullopt;
    BinkColorBundle out{};
    out.encoded_count=*count;
    auto last_high=previous_high_nibble;
    if(!*count) {
        out.end_of_bundle=true;
    } else {
        const auto uniform=probe.read(1);
        if(!uniform)return std::nullopt;
        out.constant_bundle=*uniform!=0;
        const auto read_one=[&]() -> std::optional<std::uint8_t> {
            // The original 23 contexts consist of 2 leading type
            // trees, 16 adaptive color-high trees and 5 trailing
            // low-color/pattern/motionX/motionY/run trees.
            const auto high=read_dynamic_huffman_symbol(
                probe,trees.trees[2+last_high]);
            if(!high)return std::nullopt;
            const auto low=read_dynamic_huffman_symbol(
                probe,trees.trees[18]);
            if(!low)return std::nullopt;
            last_high=high->value;
            return early_signed_color_format
                ? combine_bink_signed_nibbles(last_high,low->value)
                : static_cast<std::uint8_t>((last_high<<4)|low->value);
        };
        out.values.resize(*count);
        if(*uniform) {
            const auto value=read_one();
            if(!value)return std::nullopt;
            std::fill(out.values.begin(),out.values.end(),*value);
        } else {
            for(auto& value:out.values) {
                const auto next=read_one();
                if(!next)return std::nullopt;
                value=*next;
            }
        }
    }
    out.next_high_nibble=last_high;
    out.bits_consumed=probe.bit_position()-start;
    bits=probe;
    return out;
}


std::optional<BinkPictureFirstBundleSet>
decode_first_bink_picture_bundle_set(
    std::span<const std::uint8_t> video_packet,
    std::size_t plane_width,std::size_t plane_height) {
    constexpr std::size_t kMaxRaster=64U*1024U*1024U;
    if(!plane_width || !plane_height || plane_width>16384 ||
       plane_height>16384 || plane_width>kMaxRaster/plane_height)
        return std::nullopt;
    const auto columns=(plane_width+7)/8;
    const auto rows=(plane_height+7)/8;
    if(columns>(1U<<20)/rows)return std::nullopt;
    const auto max_values=std::min(kMaxRaster,
        plane_width*plane_height);
    const auto max_bundle=std::min<std::size_t>(1U<<20,max_values);
    const auto prologue=parse_alpha_picture_entropy_prologue(video_packet);
    if(!prologue)return std::nullopt;
    LsbBitReader reader(video_packet.subspan(
        prologue->picture.initial_byte_offset));
    if(!reader.skip(prologue->picture.trees.consumed_bits))
        return std::nullopt;
    const auto& trees=prologue->picture.trees;
    const auto types=read_bink_block_type_bundle(
        reader,trees.trees[0],plane_width,columns*rows);
    if(!types)return std::nullopt;
    // The subtype count width depends on floor(aligned_width/16)
    // rather than the full 8x8 tile count.
    const auto subtype_width=std::max<std::size_t>(
        1,(columns/2)*8);
    const auto subtypes=read_bink_block_type_bundle(
        reader,trees.trees[1],subtype_width,columns*rows);
    if(!subtypes)return std::nullopt;
    const auto colors=read_bink_color_bundle(
        reader,trees,plane_width,0,false,max_bundle);
    if(!colors)return std::nullopt;
    const auto patterns=read_bink_pattern_bundle(
        reader,trees.trees[19],plane_width,max_bundle);
    if(!patterns)return std::nullopt;
    const auto motion_x=read_bink_signed_motion_bundle(
        reader,trees.trees[20],plane_width,max_bundle);
    if(!motion_x)return std::nullopt;
    const auto motion_y=read_bink_signed_motion_bundle(
        reader,trees.trees[21],plane_width,max_bundle);
    if(!motion_y)return std::nullopt;
    const auto dc_context=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(plane_width),
        11,1,true);
    const auto intra_dc=decode_original_bink_dc_context_packet(
        reader,dc_context,max_bundle);
    if(!intra_dc)return std::nullopt;
    const auto inter_dc=decode_original_bink_dc_context_packet(
        reader,dc_context,max_bundle);
    if(!inter_dc)return std::nullopt;
    const auto runs=read_bink_run_bundle(
        reader,trees.trees[22],plane_width,max_bundle);
    if(!runs)return std::nullopt;
    BinkPictureFirstBundleSet out{};
    out.prologue=std::move(*prologue);
    out.types=std::move(*types);
    out.scaled_subtypes=std::move(*subtypes);
    out.colors=std::move(*colors);
    out.patterns=std::move(*patterns);
    out.motion_x=std::move(*motion_x);
    out.motion_y=std::move(*motion_y);
    out.intra_dc=std::move(*intra_dc);
    out.inter_dc=std::move(*inter_dc);
    out.runs=std::move(*runs);
    out.next_picture_bit_offset=
        out.prologue.picture.initial_byte_offset*8+
        reader.bit_position();
    return out;
}

} // namespace btb::bink32_recovered
