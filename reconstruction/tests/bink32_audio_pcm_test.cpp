#include "btb/bink32_audio_pcm.hpp"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"Bink1 PCM16 test line %d: %s\n",__LINE__,#x);std::abort();}}while(0)

int main(){
    Bink1AudioLayout layout{};
    layout.source_sample_rate=44100;
    layout.source_channels=2;
    layout.transform_channels=2;
    layout.transform=BinkAudioTransform::dct;
    Bink1DecodedAudioFrame source{};
    source.samples={
        {0.0f,0.5f,-1.0f,2.0f,
         0.5f/32768.0f,1.5f/32768.0f},
        {1.0f,-0.5f,-2.0f,0.0f,
         -0.5f/32768.0f,-1.5f/32768.0f}};
    source.declared_pcm_bytes=24;
    const auto pcm=encode_bink1_pcm16(source,layout);
    CHECK(pcm&&pcm->sample_rate==44100&&pcm->channels==2);
    CHECK(pcm->frame_count()==6);
    const std::vector<std::int16_t> expected{
        0,32767,16384,-16384,-32768,-32768,32767,0,
        0,0,2,-2};
    CHECK(pcm->interleaved_samples==expected);
    const auto bytes=serialize_bink1_pcm16_le(pcm->interleaved_samples);
    CHECK(bytes.size()==24);
    const std::array<std::uint8_t,8> start{0,0,255,127,0,64,0,192};
    for(std::size_t i=0;i<start.size();++i)CHECK(bytes[i]==start[i]);
    // Packed RDFT stereo is a SINGLE transform with interleaved source
    // samples, not two independently decoded channels.
    layout.transform=BinkAudioTransform::rdft;
    layout.transform_channels=1;
    source.samples={{0.1f,-0.1f,0.5f,-0.5f}};
    source.declared_pcm_bytes=8;
    const auto rdft=encode_bink1_pcm16(source,layout);
    CHECK(rdft&&rdft->channels==2&&rdft->frame_count()==2);
    const std::vector<std::int16_t> expected_rdft{
        3277,-3277,16384,-16384};
    CHECK(rdft->interleaved_samples==expected_rdft);
    // Reject a partial stereo frame rather than dropping a channel value.
    source.samples={{0.1f,0.2f,0.3f}};
    source.declared_pcm_bytes=6;
    CHECK(!encode_bink1_pcm16(source,layout));
    source.samples={{0.1f,-0.1f}};
    source.declared_pcm_bytes=3;
    CHECK(!encode_bink1_pcm16(source,layout));
    source.declared_pcm_bytes=4;
    CHECK(encode_bink1_pcm16(source,layout));
    source.samples[0][1]=std::numeric_limits<float>::infinity();
    CHECK(!encode_bink1_pcm16(source,layout));
    source.samples[0][1]=std::numeric_limits<float>::quiet_NaN();
    CHECK(!encode_bink1_pcm16(source,layout));

    // DCT stereo channel lengths cannot disagree.
    layout.transform=BinkAudioTransform::dct;
    layout.transform_channels=2;
    source.samples={{0.0f,1.0f},{0.0f}};
    source.declared_pcm_bytes=8;
    CHECK(!encode_bink1_pcm16(source,layout));
    source.samples={{},{}};source.declared_pcm_bytes=0;
    const auto empty=encode_bink1_pcm16(source,layout);
    CHECK(empty&&empty->frame_count()==0&&empty->interleaved_samples.empty());
    // Mono round-to-even and hard saturation from test values.
    layout.source_channels=1;
    layout.transform_channels=1;
    source.samples={{-32766.5f/32768.0f,0.0f,10000.0f}};
    source.declared_pcm_bytes=6;
    const auto mono=encode_bink1_pcm16(source,layout);
    CHECK(mono&&mono->interleaved_samples[0]==-32766);
    CHECK(mono->interleaved_samples[1]==0);
    CHECK(mono->interleaved_samples[2]==32767);
    layout.source_sample_rate=0;
    CHECK(!encode_bink1_pcm16(source,layout));
    return 0;
}
