#include "btb/bink32_packet_decode.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do { if(!(e)){ std::fprintf(stderr,"Bink decoder packet test line %d: %s\n",__LINE__,#e);std::abort();}}while(0)
static void put(std::vector<std::uint8_t>& b,std::size_t p,std::uint32_t v){
    for(int j=0;j<4;++j)b[p+j]=static_cast<std::uint8_t>(v>>(8*j));
}
int main(int argc,char** argv){
    std::vector<std::uint8_t> bytes(48,0);
    // Audio0 size3; audio1 size0; video 5 bytes.
    put(bytes,0,3);bytes[4]=0x12;bytes[5]=0xab;bytes[6]=0x9f;
    put(bytes,7,0);bytes[11]=0xd6;bytes[12]=0x03;bytes[13]=0xaf;bytes[14]=0xf0;
    ContainerIndex idx;
    idx.header.frame_count=1;
    idx.frame_offsets_flags={0U,16U};
    idx.tracks.resize(2);
    const auto f=split_encoded_frame(idx,bytes,1);
    CHECK(f&&f->audio.size()==2);
    CHECK(f->audio[0].compressed.size()==3&&f->audio[0].compressed[0]==0x12);
    CHECK(f->audio[1].compressed.empty()&&f->video.size()==5);
    CHECK(f->video[0]==0xd6&&f->video[3]==0xf0);
    CHECK(!split_encoded_frame(idx,bytes,0));
    put(bytes,0,0xfffffffeU);
    CHECK(!split_encoded_frame(idx,bytes,1));
    put(bytes,0,3);
    idx.frame_offsets_flags={0U,8U};
    CHECK(!split_encoded_frame(idx,bytes,1));
    idx.frame_offsets_flags={0U,1000U};
    CHECK(!split_encoded_frame(idx,bytes,1));
    const std::array<std::uint8_t,5> data{0xd6,0x03,0xaf,0xf0,0x98};
    LsbBitReader reader(data);
    CHECK(reader.read(0)==0);
    CHECK(reader.peek(3)==6&&reader.bit_position()==0);
    CHECK(reader.read(3)==6);
    CHECK(reader.read(5)==26);
    CHECK(reader.read(8)==3);
    CHECK(reader.read(12)==0x0af);
    CHECK(reader.read(4)==15);
    CHECK(reader.read(8)==0x98);
    CHECK(reader.bits_remaining()==0);
    CHECK(!reader.read(1)&&reader.bit_position()==40);
    CHECK(!reader.peek(33)&&!reader.skip(1));
    LsbBitReader cross(data);
    CHECK(cross.read(32)==0xf0af03d6U);
    CHECK(cross.read(8)==0x98U);
    LsbBitReader align(data);
    CHECK(align.read(5)==22);
    CHECK(align.align(4)&&align.bit_position()==32);
    CHECK(align.read(8)==0x98);
    CHECK(!align.align(4));
    CHECK(!align.align(3));

     // Recovered original 0x3000FC00 entropy-state initialization.
     const auto plane=init_entropy_plane(0xabcU,0x200U,640U,4U,1U,false);
     CHECK(plane.table_bit_width==10&&plane.symbol_bits==4);
     CHECK(plane.prefix_mask==0&&plane.lookup_table_address==0xabcU);
     const auto masked=init_entropy_plane(0xabcU,0x200U,640U,4U,1U,true);
     CHECK(masked.prefix_mask==8);
     const auto single=init_entropy_plane(1,1,0,1,0,true);
     CHECK(single.table_bit_width==0&&single.prefix_mask==1);
     const auto bounded=init_entropy_plane(1,0x100000U,640,4,1,true);
     CHECK(bounded.table_bit_width==15);

    // Optional retail file validation; CI never needs copyrighted BIK assets.
    for(int a=1;a<argc;++a){
        std::ifstream fd(argv[a],std::ios::binary);
        CHECK(fd.good());
        const std::vector<std::uint8_t> movie{
            std::istreambuf_iterator<char>(fd),std::istreambuf_iterator<char>()};
        const auto parsed=parse_container_index(movie);
        CHECK(parsed);
        std::uint64_t audio_bytes=0,video_bytes=0;
        for(std::uint32_t i=1;i<=parsed->header.frame_count;++i) {
            const auto packet=split_encoded_frame(*parsed,movie,i);
            CHECK(packet);
            CHECK(packet->audio.size()==parsed->tracks.size());
            for(const auto& track:packet->audio)
                audio_bytes+=track.compressed.size();
            video_bytes+=packet->video.size();
            LsbBitReader compressed(packet->video);
            CHECK(compressed.peek(8).has_value());
            CHECK(compressed.read(32).has_value());
        }
        std::printf("%s: %u frame packets, audio=%llu bytes, video=%llu bytes\n",
            argv[a],parsed->header.frame_count,
            static_cast<unsigned long long>(audio_bytes),
            static_cast<unsigned long long>(video_bytes));
    }
    std::puts("BinkDoFrame packet routing and LSB bitstream: PASS");
}
