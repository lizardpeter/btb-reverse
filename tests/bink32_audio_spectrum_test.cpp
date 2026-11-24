#include "btb/bink32_audio_spectrum.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <vector>

using namespace btb::bink32_recovered;
#define CHECK(e) do {if(!(e)){std::fprintf(stderr,"Bink1 spectrum test line %d: %s\n",__LINE__,#e);std::abort();}} while(0)
namespace {
struct Bits {
    std::vector<std::uint8_t> data{};
    std::size_t n{};
    void put(std::uint32_t value,unsigned width) {
        for(unsigned j=0;j<width;++j) {
            if(n/8>=data.size())data.push_back(0);
            if((value>>j)&1U)
                data[n/8]|=static_cast<std::uint8_t>(1U<<(n&7));
            ++n;
        }
    }
};
Bits packet_for(const Bink1AudioLayout& layout,bool first_nonzero) {
    Bits w;
    if(layout.transform==BinkAudioTransform::dct)w.put(0,2);
    for(unsigned ch=0;ch<layout.transform_channels;++ch) {
        // Original SJPi signed floats: power(5), mantissa(23), sign(1).
        for(unsigned j=0;j<2;++j) {
            w.put(0,5);w.put(0,23);w.put(0,1);
        }
        // Exactly one 8-bit quantization selector for every audio band.
        for(std::size_t i=1;i<layout.band_starts.size();++i)
            w.put(0,8);
        for(std::size_t pos=2;pos<layout.frame_length;) {
            w.put(0,1); // eight spectral coefficients
            const bool nonzero=first_nonzero&&ch==0&&pos==2;
            w.put(nonzero?3:0,4); // compressed magnitude width
            const auto count=std::min<std::size_t>(8,layout.frame_length-pos);
            if(nonzero)for(std::size_t j=0;j<count;++j) {
                w.put(j==0?1:0,3);
                if(j==0)w.put(1,1); // explicit negative sign
            }
            pos+=count;
        }
    }
    return w;
}
}
int main() {
    auto source=BinkAudioTrackSpec{
        71,0x21c00,44100,0x7000,2,BinkAudioTransform::dct,true};
    const auto plan=plan_bink1_audio_layout(source);
    CHECK(plan && plan->frame_length==2048&&plan->overlap_length==128);
    CHECK(plan->transform_channels==2&&plan->band_starts.front()==2);
    CHECK(plan->band_starts.back()==2048);
    CHECK(plan->band_starts.size()==26);
    CHECK(std::isfinite(plan->root)&&plan->root>0);
    const auto encoded=packet_for(*plan,true);
    BinkAudioBlockPacket packet{
        8192,encoded.data,source.track_id,source.sample_rate,
        source.channels,source.transform};
    const auto spectral=decode_bink1_audio_spectrum(packet,*plan);
    CHECK(spectral&&spectral->end_bit==encoded.n);
    CHECK(spectral->transform_coefficients.size()==2);
    CHECK(spectral->transform_coefficients[0].size()==2048);
    CHECK(spectral->transform_coefficients[0][0]==0.0f);
    CHECK(spectral->transform_coefficients[0][1]==0.0f);
    CHECK(std::abs(spectral->transform_coefficients[0][2]+plan->root)<1e-8f);
    for(std::size_t i=3;i<2048;++i)
        CHECK(spectral->transform_coefficients[0][i]==0.0f);
    CHECK(std::all_of(spectral->transform_coefficients[1].begin(),
                      spectral->transform_coefficients[1].end(),
                      [](float x){return x==0.0f;}));
    // Bink1 low and middle rate variants select smaller transforms.
    auto mono=source;
    mono.channels=1;
    mono.sample_rate=11025;
    mono.transform=BinkAudioTransform::rdft;
    const auto low=plan_bink1_audio_layout(mono);
    CHECK(low&&low->frame_length==512&&low->overlap_length==32);
    CHECK(low->transform_channels==1);
    auto mid=mono;mid.sample_rate=32000;
    const auto middle=plan_bink1_audio_layout(mid);
    CHECK(middle&&middle->frame_length==1024);
    mono=source;mono.transform=BinkAudioTransform::rdft;
    const auto rdft=plan_bink1_audio_layout(mono);
    CHECK(rdft&&rdft->frame_length==4096&&rdft->transform_channels==1);
    const auto rdft_data=packet_for(*rdft,false);
    BinkAudioBlockPacket rp{
        8192,rdft_data.data,mono.track_id,mono.sample_rate,
        mono.channels,mono.transform};
    const auto rs=decode_bink1_audio_spectrum(rp,*rdft);
    CHECK(rs&&rs->transform_coefficients.size()==1&&
          rs->transform_coefficients[0].size()==4096);
    CHECK(rs->end_bit==rdft_data.n);
    // An empty block NEVER constitutes a successfully decoded spectrum.
    auto empty=packet;
    empty.decoded_bytes=0;
    CHECK(!decode_bink1_audio_spectrum(empty,*plan));
    auto mismatched=packet;
    mismatched.sample_rate=22050;
    CHECK(!decode_bink1_audio_spectrum(mismatched,*plan));
    CHECK(!decode_bink1_audio_spectrum(packet,*plan,
                                     encoded.data.size()*8+1));
    for(std::size_t remove=1;remove<=2;++remove) {
        auto short_bits=encoded.data;
        short_bits.resize(short_bits.size()-remove);
        auto truncated=packet;
        truncated.compressed_transform=short_bits;
        CHECK(!decode_bink1_audio_spectrum(truncated,*plan));
    }
    // A sparse zero frame should not produce noise or NaN coefficients.
    const auto quiet=packet_for(*plan,false);
    auto quiet_packet=packet;
    quiet_packet.compressed_transform=quiet.data;
    const auto zeros=decode_bink1_audio_spectrum(quiet_packet,*plan);
    CHECK(zeros);
    for(const auto& channel:zeros->transform_coefficients)
        for(float sample:channel)CHECK(sample==0.0f);
    // A full 29-bit signed-float DC value reconstructs a real floating
    // spectral bin, not an invented time-domain PCM sample.
    auto dc=packet_for(*low,false);
    CHECK(dc.n>29);
    // bit 2 is inside the first 5-bit exponent for RDFT, so exponent=0.
    // mantissa 1 occupies the first bit immediately after exponent.
    dc.data[0]|=static_cast<std::uint8_t>(1U<<5);
    BinkAudioBlockPacket dp{
        4096,dc.data,mono.track_id,low->source_sample_rate,
        low->source_channels,low->transform};
    const auto bins=decode_bink1_audio_spectrum(dp,*low);
    CHECK(bins);
    CHECK(std::abs(bins->transform_coefficients[0][0]-
                   std::ldexp(1.0f,-23)*low->root)<1e-12f);
    return 0;
}
