#include "btb/bink32_audio_movie.hpp"
#include "btb/bink32_audio_spectrum.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <string>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do{if(!(e)){std::fprintf(stderr,"Bink movie audio test %d: %s\n",__LINE__,#e);std::abort();}}while(0)

namespace {
struct Bits {
    std::vector<std::uint8_t> bytes{};
    std::size_t at{};
    void put(std::uint32_t v,unsigned n) {
        for(unsigned i=0;i<n;++i) {
            if(bytes.size()<=at/8)bytes.push_back(0);
            if(v&(1U<<i))bytes[at/8]|=std::uint8_t(1U<<(at&7));
            ++at;
        }
    }
    void align32() {while(at%32)put(0,1);}
};
void put32(std::vector<std::uint8_t>& x,std::size_t p,std::uint32_t n) {
    for(unsigned i=0;i<4;++i)x[p+i]=std::uint8_t(n>>(i*8));
}
std::vector<std::uint8_t> encoded_pcm(const Bink1AudioLayout& layout) {
    Bits w;
    w.put(0,2); // DCT source header
    w.put(18,5);w.put(32768,23);w.put(0,1);
    w.put(0,5);w.put(0,23);w.put(0,1);
    for(std::size_t j=1;j<layout.band_starts.size();++j)w.put(0,8);
    for(unsigned j=2;j<layout.frame_length;j+=8) {
        w.put(0,1);w.put(0,4);
    }
    w.align32();
    std::vector<std::uint8_t> bytes(4,0);
    const std::uint32_t decoded_bytes=
        2*(layout.frame_length-layout.overlap_length);
    put32(bytes,0,decoded_bytes);
    bytes.insert(bytes.end(),w.bytes.begin(),w.bytes.end());
    return bytes;
}
std::vector<std::uint8_t> synthetic_movie(
    std::span<const std::uint8_t> compressed,bool bad_frame=false,
    bool partial_final=false,bool partial_first=false) {
    constexpr unsigned frame_count=3,header=44,track_bytes=12;
    constexpr unsigned index_size=4*(frame_count+1);
    std::vector<std::uint8_t> out(header+track_bytes+index_size,0);
    out[0]='S';out[1]='J';out[2]='P';out[3]='i';
    put32(out,8,frame_count);
    put32(out,12,20000);
    put32(out,16,frame_count);
    put32(out,20,640);put32(out,24,480);
    put32(out,28,15);put32(out,32,1);
    put32(out,36,0x100000);put32(out,40,1);
    put32(out,44,65536);
    put32(out,48,(0x5000U<<16)|11025U);put32(out,52,7);
    const auto index_at=header+track_bytes;
    for(unsigned f=0;f<frame_count;++f) {
        const auto offset=static_cast<std::uint32_t>(out.size());
        put32(out,index_at+f*4,offset|(f==0?1U:0U));
        // One track length-prefixed packet, followed by dummy video.
        if(f==1) {
            out.insert(out.end(),8,0); // no audio, 4 video bytes
        } else {
            std::vector<std::uint8_t> payload(
                compressed.begin(),compressed.end());
            if((partial_final && f==2)||(partial_first && f==0)) {
                const auto declared=std::uint32_t(payload[0])|
                    (std::uint32_t(payload[1])<<8)|
                    (std::uint32_t(payload[2])<<16)|
                    (std::uint32_t(payload[3])<<24);
                CHECK(declared>16);
                put32(payload,0,declared-16);
            }
            const auto size=static_cast<std::uint32_t>(payload.size());
            const auto payload_begin=out.size();
            out.resize(out.size()+4);
            put32(out,payload_begin,size);
            out.insert(out.end(),payload.begin(),payload.end());
            if(bad_frame && f==2) {
                // Truncated stream on the final frame must fail atomically.
                out.resize(out.size()-8);
                put32(out,payload_begin,
                      static_cast<std::uint32_t>(payload.size()-8));
            }
            out.insert(out.end(),4,0);
        }
        while(out.size()%4)out.push_back(0);
    }
    put32(out,index_at+frame_count*4,static_cast<std::uint32_t>(out.size()));
    put32(out,4,static_cast<std::uint32_t>(out.size()-8));
    return out;
}
} // namespace

int main() {
    BinkAudioTrackSpec spec{
        7,65536,11025,0x5000,1,BinkAudioTransform::dct,true};
    const auto layout=plan_bink1_audio_layout(spec);
    CHECK(layout);
    const auto block=encoded_pcm(*layout);
    const auto bytes=synthetic_movie(block);
    std::string error;
    const auto movie=decode_bink1_movie_audio(bytes,0,error);
    CHECK(movie&&error.empty());
    CHECK(movie->channels==1&&movie->sample_rate==11025&&movie->track_id==7);
    CHECK(movie->audio_frame_boundaries.size()==4);
    CHECK(movie->audio_frame_boundaries[0]==0);
    CHECK(movie->audio_frame_boundaries[1]==480);
    CHECK(movie->audio_frame_boundaries[2]==480);
    CHECK(movie->audio_frame_boundaries[3]==960);
    CHECK(movie->samples.size()==960&&movie->sample_frames()==960);
    CHECK(movie->samples[0]!=0&&movie->samples[500]!=0);
    CHECK(movie->timestamp_us(1)==
          std::optional<std::uint64_t>(480ULL*1000000ULL/11025ULL));
    CHECK(movie->timestamp_us(2)==movie->timestamp_us(1));
    CHECK(movie->duration_us()==movie->timestamp_us(3));
    CHECK(!movie->timestamp_us(4));
    // The final audio-bearing frame may declare a short tail even though
    // the decoder's final transform still emits its complete sample block.
    const auto partial_bytes=synthetic_movie(block,false,true);
    const auto partial_movie=decode_bink1_movie_audio(partial_bytes,0,error);
    CHECK(partial_movie&&partial_movie->sample_frames()==960);
    CHECK(partial_movie->audio_frame_boundaries[1]==480);
    CHECK(partial_movie->audio_frame_boundaries[3]==960);
    // A similar short declaration before the final non-empty packet is
    // rejected; terminal allowance must not hide a corrupt middle packet.
    const auto partial_middle=synthetic_movie(block,false,false,true);
    CHECK(!decode_bink1_movie_audio(partial_middle,0,error));
    CHECK(error.find("frame 1")!=std::string::npos);
    CHECK(!decode_bink1_movie_audio(bytes,1,error));
    CHECK(!error.empty());
    const auto corrupt=synthetic_movie(block,true);
    CHECK(!decode_bink1_movie_audio(corrupt,0,error));
    CHECK(error.find("frame 3")!=std::string::npos);
    CHECK(!decode_bink1_movie_audio({},0,error));
    return 0;
}
