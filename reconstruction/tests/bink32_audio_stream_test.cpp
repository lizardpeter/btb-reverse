#include "btb/bink32_audio_stream.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do {if(!(e)){std::fprintf(stderr,"Bink1 audio stream test %d: %s\n",__LINE__,#e);std::abort();}}while(0)
namespace {
struct BitWriter {
    std::vector<std::uint8_t> bytes{};
    std::size_t count{};
    void put(std::uint32_t value,unsigned width){
        for(unsigned i=0;i<width;++i){
            if(bytes.size()<=count/8)bytes.push_back(0);
            if((value>>i)&1U)bytes[count/8]|=std::uint8_t(1U<<(count&7));
            ++count;
        }
    }
    void align32(){while(count%32)put(0,1);}
    void add_silent_transform(const Bink1AudioLayout& layout,
                              std::uint32_t dc_mantissa=0) {
        if(layout.transform==BinkAudioTransform::dct)put(0,2);
        for(unsigned channel=0;channel<layout.transform_channels;++channel) {
            put(dc_mantissa?23:0,5);
            put(dc_mantissa,23);
            put(0,1);
            put(0,5);put(0,23);put(0,1);
            for(std::size_t i=1;i<layout.band_starts.size();++i)
                put(0,8);
            for(std::size_t i=2;i<layout.frame_length;i+=8){
                put(0,1);put(0,4);
            }
        }
        align32();
    }
};
std::vector<std::uint8_t> as_audio(const std::vector<std::uint8_t>& encoded,
                                   std::uint32_t decoded_bytes) {
    std::vector<std::uint8_t> packet(4,0);
    for(unsigned j=0;j<4;++j)
        packet[j]=std::uint8_t(decoded_bytes>>(8*j));
    packet.insert(packet.end(),encoded.begin(),encoded.end());
    return packet;
}
} // namespace
int main() {
    const BinkAudioTrackSpec meta{
        71,65536,11025,0x1000,1,BinkAudioTransform::dct,true};
    auto session=Bink1AudioStreamDecoder::open(meta);
    CHECK(session&&session->layout().frame_length==512);
    const auto layout=session->layout();
    const auto ready=layout.frame_length-layout.overlap_length;
    CHECK(ready==480);
    BitWriter encoded;
    encoded.add_silent_transform(layout);
    const auto single_packet=as_audio(encoded.bytes,ready*2);
    const AudioPacket single{0,single_packet};
    auto frame=session->decode(single);
    CHECK(frame&&frame->block_count==1&&frame->declared_pcm_bytes==960);
    CHECK(frame->samples.size()==1&&frame->samples[0].size()==480);
    CHECK(std::all_of(frame->samples[0].begin(),frame->samples[0].end(),
                      [](float x){return x==0;}));
    const AudioPacket no_audio{0,{}};
    frame=session->decode(no_audio);
    CHECK(frame && frame->block_count==0 && frame->samples[0].empty());

    BitWriter two;
    two.add_silent_transform(layout,1);
    two.add_silent_transform(layout,0);
    const auto double_packet=as_audio(two.bytes,ready*4);
    frame=session->decode(AudioPacket{0,double_packet});
    CHECK(frame&&frame->block_count==2);
    CHECK(frame->samples.size()==1&&frame->samples[0].size()==ready*2);
    CHECK(frame->samples[0][100]!=0); // after the first overlap region

    // A multi-block packet fails atomically if block 2 is truncated.
    BitWriter invalid;
    invalid.add_silent_transform(layout,1);
    invalid.put(0xffffffffU,32); // 32 bits cannot form a full spectrum
    const auto broken=as_audio(invalid.bytes,ready*4);
    auto baseline=Bink1AudioStreamDecoder::open(meta);
    CHECK(baseline);
    CHECK(baseline->decode(AudioPacket{0,double_packet}));
    CHECK(!session->decode(AudioPacket{0,broken}));
    const auto after_error=session->decode(single);
    const auto expected=baseline->decode(single);
    CHECK(after_error&&expected);
    CHECK(after_error->samples==expected->samples);

    session->rewind();
    auto reset=Bink1AudioStreamDecoder::open(meta);
    CHECK(reset);
    const auto restarted=session->decode(single);
    const auto fresh=reset->decode(single);
    CHECK(restarted&&fresh&&restarted->samples==fresh->samples);

    // Reconstruct RDFT interleaved stereo as one packed time-domain
    // stream, instead of fabricating independently decoded channels.
    auto stereo=meta;
    stereo.sample_rate=44100;
    stereo.channels=2;
    stereo.transform=BinkAudioTransform::rdft;
    auto rdft=Bink1AudioStreamDecoder::open(stereo);
    CHECK(rdft&&rdft->layout().frame_length==4096);
    CHECK(rdft->layout().transform_channels==1);
    BitWriter stereo_bits;
    stereo_bits.add_silent_transform(rdft->layout());
    const auto st=as_audio(stereo_bits.bytes,7680);
    const auto interleaved=rdft->decode(AudioPacket{0,st});
    CHECK(interleaved&&interleaved->block_count==1);
    CHECK(interleaved->samples.size()==1);
    CHECK(interleaved->samples[0].size()==3840);
    return 0;
}
