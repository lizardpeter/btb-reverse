#include "btb/bink32_audio_stream.hpp"
#include "btb/bink32_audio_packets.hpp"
#include "btb/bink32_audio_spectrum.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

namespace btb::bink32_recovered {
std::optional<Bink1AudioStreamDecoder>
Bink1AudioStreamDecoder::open(const BinkAudioTrackSpec& track) noexcept {
    const auto layout=plan_bink1_audio_layout(track);
    if(!layout)return std::nullopt;
    return Bink1AudioStreamDecoder(track,*layout);
}
std::optional<Bink1DecodedAudioFrame>
Bink1AudioStreamDecoder::decode(const AudioPacket& source) {
    const auto inspected=inspect_bink_audio_packet(source,track_);
    if(!inspected)return std::nullopt;
    Bink1DecodedAudioFrame result{};
    result.declared_pcm_bytes=inspected->decoded_bytes;
    const auto& encoded=inspected->compressed_transform;
    if(encoded.empty()) {
        // No audio packet on this video frame. Do not flush/replace the
        // overlap state or manufacture a silence waveform.
        result.samples.resize(layout_.transform_channels);
        return result;
    }
    constexpr std::size_t kMaxAudioBlocksPerMovieFrame=256;
    constexpr std::size_t kMaxOutputSamplesPerChannel=1'048'576;
    const auto bit_count=encoded.size()*std::size_t(8);
    std::size_t cursor=0;
    auto next_overlap=overlap_; // transactional across all blocks
    result.samples.resize(layout_.transform_channels);
    while(cursor<bit_count) {
        if(result.block_count>=kMaxAudioBlocksPerMovieFrame)
            return std::nullopt;
        const auto decoded=decode_bink1_audio_spectrum(
            *inspected,layout_,cursor);
        if(!decoded || decoded->end_bit<=cursor ||
           decoded->end_bit>bit_count ||
           decoded->transform_coefficients.size()!=
               layout_.transform_channels)
            return std::nullopt;
        const auto time=inverse_bink1_audio_spectrum(*decoded,layout_);
        if(!time)return std::nullopt;
        const auto blended=next_overlap.process(*time);
        if(!blended)return std::nullopt;
        for(std::size_t ch=0;ch<blended->size();++ch) {
            auto& out=result.samples[ch];
            if(out.size()>kMaxOutputSamplesPerChannel-
                    (*blended)[ch].size())
                return std::nullopt;
            out.insert(out.end(),(*blended)[ch].begin(),
                       (*blended)[ch].end());
        }
        ++result.block_count;
        // Bitstream container pads each compressed transform block to
        // a 32-bit boundary. The prefix LE32 *declared decoded size* is
        // already stripped by inspect_bink_audio_packet.
        if(decoded->end_bit>std::numeric_limits<std::size_t>::max()-31)
            return std::nullopt;
        const auto next=(decoded->end_bit+31)&~std::size_t(31);
        if(next>bit_count || next<=cursor)return std::nullopt;
        cursor=next;
    }
    if(cursor!=bit_count)return std::nullopt;
    overlap_=std::move(next_overlap);
    return result;
}
} // namespace btb::bink32_recovered
