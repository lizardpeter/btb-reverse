#include "btb/bink32_audio_pcm.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
namespace {
[[nodiscard]] std::int16_t to_s16(float value) noexcept {
    const double scaled=double(value)*32768.0;
    if(scaled>=32767.0)return 32767;
    if(scaled<=-32768.0)return -32768;
    const double below=std::floor(scaled);
    const auto integer=static_cast<std::int32_t>(below);
    const double remainder=scaled-below;
    const bool carry=remainder>0.5 ||
                     (remainder==0.5 && (integer&1)!=0);
    return static_cast<std::int16_t>(integer+static_cast<int>(carry));
}
}

std::optional<Bink1Pcm16Block> encode_bink1_pcm16(
    const Bink1DecodedAudioFrame& decoded,
    const Bink1AudioLayout& layout) {
    if(!layout.source_sample_rate || layout.source_channels<1 ||
       layout.source_channels>2 ||
       layout.transform_channels<1 ||
       layout.transform_channels>2 ||
       decoded.samples.size()!=layout.transform_channels)
        return std::nullopt;
    const std::size_t channels=layout.source_channels;
    const std::size_t transforms=layout.transform_channels;
    if(layout.transform==BinkAudioTransform::rdft) {
        if(transforms!=1)return std::nullopt;
    } else if(transforms!=channels)return std::nullopt;

    const std::size_t count=decoded.samples.front().size();
    constexpr std::size_t kMaxValues=4'194'304;
    if(count>kMaxValues/channels)return std::nullopt;
    if(layout.transform==BinkAudioTransform::rdft &&
       count%channels!=0)return std::nullopt;
    const std::size_t total=count*transforms;
    if(total>kMaxValues || total>std::numeric_limits<std::uint32_t>::max()/2)
        return std::nullopt;
    if(decoded.declared_pcm_bytes!=total*2)return std::nullopt;
    for(const auto& part:decoded.samples)
        if(part.size()!=count ||
           !std::all_of(part.begin(),part.end(),
                        [](float f){return std::isfinite(f);}))
            return std::nullopt;
    Bink1Pcm16Block pcm{
        layout.source_sample_rate,
        layout.source_channels,
        std::vector<std::int16_t>(total)};
    if(layout.transform==BinkAudioTransform::rdft) {
        for(std::size_t i=0;i<count;++i)
            pcm.interleaved_samples[i]=to_s16(decoded.samples[0][i]);
    } else {
        for(std::size_t i=0;i<count;++i)
            for(std::size_t channel=0;channel<channels;++channel)
                pcm.interleaved_samples[i*channels+channel]=
                    to_s16(decoded.samples[channel][i]);
    }
    return pcm;
}

std::vector<std::uint8_t> serialize_bink1_pcm16_le(
    std::span<const std::int16_t> interleaved) {
    std::vector<std::uint8_t> data;
    data.reserve(interleaved.size()*2);
    for(const auto value:interleaved) {
        const auto bits=static_cast<std::uint16_t>(value);
        data.push_back(static_cast<std::uint8_t>(bits));
        data.push_back(static_cast<std::uint8_t>(bits>>8));
    }
    return data;
}
} // namespace btb::bink32_recovered
