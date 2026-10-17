#include "btb/bink32_container.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string_view>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(c) do {if(!(c)){std::fprintf(stderr,"BIK parser line %d: %s\n",__LINE__,#c);std::abort();}}while(0)
static void put(std::vector<std::uint8_t>& b,std::size_t p,std::uint32_t n) {
    for(int i=0;i<4;++i)b[p+i]=static_cast<std::uint8_t>(n>>(i*8));
}
static std::vector<std::uint8_t> fixture(std::uint32_t tracks){
    constexpr std::uint32_t frames=3;
    const std::size_t index=44+tracks*12;
    const std::uint32_t first=static_cast<std::uint32_t>(index+16);
    std::vector<std::uint8_t> b(first+32,0);
    put(b,0,0x69504a53);put(b,4,static_cast<std::uint32_t>(b.size()-8));
    put(b,8,frames);put(b,12,12);put(b,16,frames);
    put(b,20,282);put(b,24,179);put(b,28,15);put(b,32,1);
    put(b,36,tracks?0x100000U:0);put(b,40,tracks);
    for(std::uint32_t i=0;i<tracks;++i) {
        put(b,44+i*12,0x21c00);
        put(b,48+i*12,0x7000ac44);
        put(b,52+i*12,i+71);
    }
    put(b,index,first|1);
    put(b,index+4,first+12);
    put(b,index+8,first+20);
    put(b,index+12,static_cast<std::uint32_t>(b.size()));
    return b;
}
int main(int argc,char** argv) {
    CHECK(!parse_container_index({}));
    // A truncated but plausible 44-byte file with 64 declared tracks
    // previously underflowed size_t in (size - index_start), then read OOB.
    std::vector<std::uint8_t> truncated(44,0);
    put(truncated,0,0x69504a53U);
    put(truncated,4,36);
    put(truncated,8,1); put(truncated,20,282); put(truncated,24,179);
    put(truncated,28,15); put(truncated,32,1); put(truncated,40,64);
    CHECK(!parse_container_index(truncated));
    for(unsigned n: {0U,1U}) {
        auto b=fixture(n);
        const auto p=parse_container_index(b);
        CHECK(p.has_value()&&p->header.magic==Magic::SJPi);
        CHECK(p->header.frame_count==3&&p->header.width==282);
        CHECK(p->header.height==179&&p->header.frame_rate==15);
        CHECK(p->tracks.size()==n&&p->frame_offsets_flags.size()==4);
        CHECK(p->declared_file_size_minus_8==b.size()-8);
        const auto one=p->frame(1),two=p->frame(2),three=p->frame(3);
        CHECK(one&&two&&three&&one->key_frame&&!two->key_frame);
        CHECK(one->compressed_bytes==12&&two->compressed_bytes==8);
        CHECK(three->compressed_bytes==12);
        CHECK(!p->frame(0)&&!p->frame(4));
        if(n)CHECK(p->tracks[0].track_id==71);
        // Invalid footer, nonmonotonic frame indexes, bad length,
        // misaligned flags and short data all fail closed.
        put(b,4,111);
        CHECK(!parse_container_index(b));
        put(b,4,static_cast<std::uint32_t>(b.size()-8));
        const auto start=44+n*12;
        put(b,start+8,4);
        CHECK(!parse_container_index(b));
        put(b,start+8,static_cast<std::uint32_t>(b.size()-12));
        put(b,start+4,(static_cast<std::uint32_t>(start+16))|2U);
        CHECK(!parse_container_index(b));
    }
    // Optionally validate the real supplied assets in development.
    // CI does not require or download copyrighted video files.
    for(int i=1;i<argc;++i) {
        std::ifstream file(argv[i],std::ios::binary);
        CHECK(file.good());
        const std::vector<std::uint8_t> b{
            std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
        const auto p=parse_container_index(b);
        CHECK(p);
        CHECK(p->frame(1)&&p->frame(p->header.frame_count));
        std::printf("%s: %u frames, %u x %u, %zu audio tracks\n",argv[i],
                    p->header.frame_count,p->header.width,p->header.height,
                    p->tracks.size());
    }
    std::puts("Original SJPi Bink frame-index parsing: PASS");
}
