#include "btb/bink32_audio_spectrum.hpp"
#include "btb/bink32_packet_decode.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

namespace btb::bink32_recovered {
namespace {
// Bink1's 25 psychoacoustic critical-band boundaries, in Hz.
constexpr std::array<std::uint16_t,25> kCriticalHz{
    100,200,300,400,510,630,770,920,
    1080,1270,1480,1720,2000,2320,2700,3150,
    3700,4400,5300,6400,7700,9500,12000,15500,24500};
constexpr std::array<unsigned,16> kRunLengths{
    2,3,4,5,6,8,9,10,11,12,13,14,15,16,32,64};
constexpr float kQuantScale=0.15289164787221954f;

[[nodiscard]] std::optional<float> read_bink1_float(
    LsbBitReader& reader) noexcept {
    const auto e=reader.read(5);
    const auto v=reader.read(23);
    const auto sign=reader.read(1);
    if(!e||!v||!sign)return std::nullopt;
    const auto magnitude=std::ldexp(static_cast<float>(*v),
                                    static_cast<int>(*e)-23);
    return *sign ? -magnitude : magnitude;
}
}

std::optional<Bink1AudioLayout> plan_bink1_audio_layout(
    const BinkAudioTrackSpec& spec) noexcept {
    if(spec.sample_rate<1000 || spec.channels<1 || spec.channels>2)
        return std::nullopt;
    unsigned bits=spec.sample_rate<22050?9:
                  spec.sample_rate<44100?10:11;
    const bool rdft=spec.transform==BinkAudioTransform::rdft;
    if(rdft&&spec.channels==2)++bits;
    const unsigned length=1U<<bits;
    if(length>4096)return std::nullopt;
    const unsigned effective_rate=spec.sample_rate*(rdft?spec.channels:1U);
    const unsigned half=(effective_rate+1)/2;
    if(!half)return std::nullopt;

    Bink1AudioLayout out{};
    out.transform=spec.transform;
    out.source_sample_rate=spec.sample_rate;
    out.source_channels=spec.channels;
    out.frame_length=static_cast<std::uint16_t>(length);
    out.overlap_length=static_cast<std::uint16_t>(length/16);
    out.transform_channels=rdft?1:spec.channels;
    const float sqr=std::sqrt(static_cast<float>(length));
    out.root=rdft ? 2.0f/(sqr*32768.0f):sqr/32768.0f;
    unsigned count=25;
    for(unsigned i=1;i<=25;++i) {
        if(half<=kCriticalHz[i-1]) {count=i;break;}
    }
    out.band_starts.reserve(count+1);
    out.band_starts.push_back(2);
    for(unsigned i=1;i<count;++i) {
        const auto frequency=kCriticalHz[i-1];
        const unsigned start=((static_cast<unsigned>(frequency)*length)/
                              half)&~1U;
        out.band_starts.push_back(static_cast<std::uint16_t>(
            std::min(start,length)));
    }
    out.band_starts.push_back(static_cast<std::uint16_t>(length));
    if(!std::isfinite(out.root))return std::nullopt;
    return out;
}

std::optional<Bink1AudioSpectrumBlock> decode_bink1_audio_spectrum(
    const BinkAudioBlockPacket& packet,const Bink1AudioLayout& layout,
    std::size_t starting_bit) {
    if(packet.decoded_bytes==0 || packet.compressed_transform.empty() ||
       packet.sample_rate!=layout.source_sample_rate ||
       packet.channels!=layout.source_channels ||
       packet.transform!=layout.transform ||
       layout.frame_length<512 || layout.frame_length>4096 ||
       layout.transform_channels<1 || layout.transform_channels>2 ||
       layout.band_starts.empty() || layout.band_starts.size()>26 ||
       layout.band_starts.front()!=2 ||
       layout.band_starts.back()!=layout.frame_length ||
       !std::isfinite(layout.root) || layout.root<=0)
        return std::nullopt;
    for(std::size_t n=1;n<layout.band_starts.size();++n)
        if(layout.band_starts[n]<layout.band_starts[n-1])
            return std::nullopt;

    LsbBitReader bits(packet.compressed_transform);
    if(!bits.skip(starting_bit))return std::nullopt;
    if(layout.transform==BinkAudioTransform::dct &&
       !bits.skip(2))return std::nullopt;

    Bink1AudioSpectrumBlock result{};
    result.transform_coefficients.reserve(layout.transform_channels);
    for(unsigned channel=0;channel<layout.transform_channels;++channel) {
        std::vector<float> coeff(layout.frame_length,0.0f);
        for(unsigned index=0;index<2;++index) {
            const auto value=read_bink1_float(bits);
            if(!value)return std::nullopt;
            coeff[index]=*value*layout.root;
        }
        std::vector<float> scales;
        scales.reserve(layout.band_starts.size()-1);
        for(std::size_t k=1;k<layout.band_starts.size();++k) {
            const auto q=bits.read(8);
            if(!q)return std::nullopt;
            scales.push_back(
                std::exp(float(std::min(*q,95U))*kQuantScale)*
                layout.root);
        }
        std::size_t current_band=0;
        for(std::size_t i=2;i<layout.frame_length;) {
            const auto long_run=bits.read(1);
            if(!long_run)return std::nullopt;
            unsigned run=8;
            if(*long_run) {
                const auto code=bits.read(4);
                if(!code)return std::nullopt;
                run=8*kRunLengths[*code];
            }
            const auto width=bits.read(4);
            if(!width)return std::nullopt;
            const std::size_t end=std::min<std::size_t>(
                layout.frame_length,i+run);
            for(;i<end;++i) {
                while(current_band+1<scales.size() &&
                      layout.band_starts[current_band+1]<=i)
                    ++current_band;
                if(!*width)continue;
                const auto magnitude=bits.read(*width);
                if(!magnitude)return std::nullopt;
                if(!*magnitude)continue;
                const auto sign=bits.read(1);
                if(!sign)return std::nullopt;
                const float scaled=static_cast<float>(*magnitude)*
                                   scales[current_band];
                coeff[i]=*sign?-scaled:scaled;
            }
        }
        result.transform_coefficients.push_back(std::move(coeff));
    }
    result.end_bit=bits.bit_position();
    return result;
}
} // namespace btb::bink32_recovered
