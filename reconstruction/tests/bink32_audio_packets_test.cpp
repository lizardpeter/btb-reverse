#include "btb/bink32_audio_packets.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace btb::bink32_recovered;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"Bink native audio metadata test at line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
int main() {
    ContainerIndex index{};
    CHECK(describe_bink_audio_tracks(index)->empty());
    // Genuine original format sample-rate/flag layout: 0x7000ac44
    // is stereo, DCT transform, prefer 16-bit, sample rate 44100 Hz.
    index.tracks={
        TrackRecord{0x21c00,0x7000ac44,71},
        TrackRecord{4096,(0x0000U<<16)|22050U,72},
        TrackRecord{8192,(0x2000U<<16)|48000U,73}
    };
    const auto fmt=describe_bink_audio_tracks(index);
    CHECK(fmt && fmt->size()==3);
    CHECK((*fmt)[0].track_id==71&&(*fmt)[0].maximum_decoded_bytes==0x21c00);
    CHECK((*fmt)[0].sample_rate==44100&&(*fmt)[0].channels==2);
    CHECK((*fmt)[0].transform==BinkAudioTransform::dct);
    CHECK((*fmt)[0].preferred_int16);
    CHECK((*fmt)[1].sample_rate==22050&&(*fmt)[1].channels==1);
    CHECK((*fmt)[1].transform==BinkAudioTransform::rdft);
    CHECK(!(*fmt)[1].preferred_int16);
    CHECK((*fmt)[2].channels==2 &&
          (*fmt)[2].transform==BinkAudioTransform::rdft);
    const std::array<std::uint8_t,8> packet{
        0x00,0x08,0x00,0x00,0x9a,0xbc,0xde,0xff};
    AudioPacket a{0,packet};
    const std::span<const std::uint8_t> empty{};
    const auto empty_packet=inspect_bink_audio_packet(
        AudioPacket{0,empty},(*fmt)[0]);
    CHECK(empty_packet && empty_packet->decoded_bytes==0 &&
          empty_packet->compressed_transform.empty());
    const auto block=inspect_bink_audio_packet(a,(*fmt)[0]);
    CHECK(block && block->decoded_bytes==2048 &&
          block->compressed_transform.size()==4);
    CHECK(block->compressed_transform[0]==0x9a);
    CHECK(block->track_id==71&&block->sample_rate==44100&&
          block->channels==2&&block->transform==BinkAudioTransform::dct);
    std::array<std::uint8_t,4> silence{0,0,0,0};
    CHECK(inspect_bink_audio_packet(AudioPacket{0,silence},(*fmt)[0]));
    silence[1]=3; // nonzero decoded bytes but no bitstream
    CHECK(!inspect_bink_audio_packet(AudioPacket{0,silence},(*fmt)[0]));
    CHECK(!inspect_bink_audio_packet(
        AudioPacket{0,std::span<const std::uint8_t>(packet).first(3)},(*fmt)[0]));
    auto too_large=packet;
    too_large[0]=0xff;too_large[1]=0xff;too_large[2]=0xff;too_large[3]=0xff;
    CHECK(!inspect_bink_audio_packet(AudioPacket{0,too_large},(*fmt)[0]));
    too_large=packet;too_large[0]=0;too_large[1]=0;
    CHECK(!inspect_bink_audio_packet(AudioPacket{0,too_large},(*fmt)[0]));
    // The rate/capacity must be safe before allocating decoder buffers.
    auto bad=index;
    bad.tracks[0].codec_flags=0;
    CHECK(!describe_bink_audio_tracks(bad));
    bad=index;bad.tracks[1].max_decoded_bytes=0;
    CHECK(!describe_bink_audio_tracks(bad));
    bad=index;bad.tracks[2].codec_flags=(0x7000U<<16)|441U;
    CHECK(!describe_bink_audio_tracks(bad));
    return 0;
}
