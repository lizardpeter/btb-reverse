#include "btb/bink32_yuva_frame.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <optional>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do {if(!(e)){std::fprintf(stderr,"Bink YUVA frame test %d: %s\n",__LINE__,#e);std::abort();}}while(0)
namespace {
struct Bits {
    std::vector<std::uint8_t> bytes{};
    std::size_t bit_count{};
    void put(unsigned value,unsigned count) {
        for(unsigned i=0;i<count;++i) {
            if(bytes.size()<=bit_count/8)bytes.push_back(0);
            if((value>>i)&1U)
                bytes[bit_count/8]|=
                    static_cast<std::uint8_t>(1U<<(bit_count&7U));
            ++bit_count;
        }
    }
    void align32() {while(bytes.size()%4)bytes.push_back(0);}
};
unsigned width_bits(std::size_t width,unsigned multiplier,
                    unsigned symbol_bits=4,bool prefix_mask=false) {
    const auto ctx=init_entropy_plane(0,0x200U,
        static_cast<std::uint32_t>(width),
        symbol_bits,multiplier,prefix_mask);
    CHECK(ctx.table_bit_width>0 && ctx.table_bit_width<=24);
    return ctx.table_bit_width;
}
Bits make_constant_plane(std::size_t width,std::size_t height,
                         unsigned color,bool skip) {
    CHECK(width%8==0 && height%8==0);
    Bits out;
    for(unsigned n=0;n<23;++n)out.put(0,4); // identity trees
    const unsigned tiles=static_cast<unsigned>((width/8)*(height/8));
    out.put(tiles,width_bits(width,1));
    out.put(1,1);
    out.put(skip?0:6,4); // uniform Skip or Fill type
    out.put(0,width_bits(width/2,1)); // subordinate scaled type
    if(skip) {
        out.put(0,width_bits(width,64)); // no colors
    } else {
        out.put(tiles,width_bits(width,64));
        out.put(1,1); // constant color
        out.put((color>>4)&15U,4); // high Huffman nibble
        out.put(color&15U,4); // low Huffman nibble
    }
    out.put(0,width_bits(width,8)); // pattern
    out.put(0,width_bits(width,1)); // motion X
    out.put(0,width_bits(width,1)); // motion Y
    out.put(0,width_bits(width,1,11,false)); // intra DC
    out.put(0,width_bits(width,1,11,true)); // inter DC
    out.put(0,width_bits(width,48)); // run
    out.align32();
    return out;
}
void append(std::vector<std::uint8_t>& output,const Bits& plane) {
    output.insert(output.end(),plane.bytes.begin(),plane.bytes.end());
}
std::vector<std::uint8_t> make_sjpi_frame(
    std::size_t width,std::size_t height,
    std::array<unsigned,4> colors,bool skip,
    std::array<std::size_t,4>* ends=nullptr) {
    // colors and ends indexed canonical [Y,U,V,A]
    const auto alpha=make_constant_plane(width,height,colors[3],skip);
    const auto y=make_constant_plane(width,height,colors[0],skip);
    const auto v=make_constant_plane(width/2,height/2,colors[2],skip);
    const auto u=make_constant_plane(width/2,height/2,colors[1],skip);
    std::vector<std::uint8_t> packet(4,0);
    append(packet,alpha);
    const auto alpha_bytes=packet.size();
    CHECK(alpha_bytes<=0xFFFF && alpha_bytes%4==0);
    for(unsigned i=0;i<4;++i)
        packet[i]=static_cast<std::uint8_t>(alpha_bytes>>(8*i));
    packet.insert(packet.end(),{0x12,0x34,0x56,0x78});
    append(packet,y);
    const auto y_end=packet.size();
    append(packet,v);
    const auto v_end=packet.size();
    append(packet,u);
    if(ends)*ends={y_end,packet.size(),v_end,alpha_bytes};
    return packet;
}
} // namespace

int main(int argc,char**argv) {
    constexpr std::size_t w=32,h=16;
    constexpr std::array<unsigned,4> colors{62,121,191,244};
    std::array<std::size_t,4> expected_ends{};
    const auto encoded=make_sjpi_frame(w,h,colors,false,&expected_ends);
    const auto decoded=decode_bink_sjpi_yuva_frame(encoded,w,h);
    CHECK(decoded && decoded->width==w && decoded->height==h);
    CHECK(decoded->video_bytes_consumed==encoded.size());
    CHECK(decoded->plane_end_byte_offsets==expected_ends);
    for(unsigned plane=0;plane<4;++plane) {
        const auto expected=(plane==1||plane==2)?w*h/4:w*h;
        CHECK(decoded->planes[plane].size()==expected);
        CHECK(std::all_of(decoded->planes[plane].begin(),
                          decoded->planes[plane].end(),
                          [&](auto value){return value==colors[plane];}));
        CHECK(decoded->mode_counts[plane][6]==expected/64);
    }
    // Byte-for-byte native/reference parity. The actual retail
    // comparator uses an independently decoded raw yuva420p stream;
    // here we test exact plane offsets and first differing pixels.
    std::vector<std::uint8_t> raw_reference;
    for(const auto& pixels:decoded->planes)
        raw_reference.insert(raw_reference.end(),
                             pixels.begin(),pixels.end());
    const auto parity=compare_bink_yuva_reference(
        *decoded,raw_reference);
    CHECK(parity && parity->identical &&
          parity->total_different_bytes==0);
    for(const auto& row:parity->planes)
        CHECK(row.different_bytes==0);
    raw_reference[1]=static_cast<std::uint8_t>(colors[0]+5);
    raw_reference[w*h+3]=static_cast<std::uint8_t>(colors[1]-7);
    const auto diff=compare_bink_yuva_reference(
        *decoded,raw_reference);
    CHECK(diff && !diff->identical &&
          diff->total_different_bytes==2);
    CHECK(diff->planes[0].different_bytes==1 &&
          diff->planes[0].first_different_index==1);
    CHECK(diff->planes[0].first_reference==colors[0]+5 &&
          diff->planes[0].first_native==colors[0] &&
          diff->planes[0].maximum_absolute_difference==5);
    CHECK(diff->planes[1].different_bytes==1 &&
          diff->planes[1].first_different_index==3 &&
          diff->planes[1].maximum_absolute_difference==7);
    CHECK(diff->planes[2].different_bytes==0 &&
          diff->planes[3].different_bytes==0);
    CHECK(diff->planes[0].sum_absolute_differences==5 &&
          diff->planes[1].sum_absolute_differences==7);
    CHECK(!compare_bink_yuva_reference(
        *decoded,std::span<const std::uint8_t>(
            raw_reference).first(raw_reference.size()-1)));
    {
        // The original SJPi can also omit alpha entirely. Its
        // packet begins directly with a 32-bit picture control,
        // rather than an alpha length and alpha plane. Remove the
        // original alpha segment but preserve the picture DWORD.
        const auto remove_alpha=[](
            const std::vector<std::uint8_t>& with_alpha) {
            CHECK(with_alpha.size()>=8);
            const std::size_t start=
                std::size_t(with_alpha[0]) |
                (std::size_t(with_alpha[1])<<8) |
                (std::size_t(with_alpha[2])<<16) |
                (std::size_t(with_alpha[3])<<24);
            CHECK(start>=4 && start+4<with_alpha.size());
            return std::vector<std::uint8_t>(
                with_alpha.begin()+static_cast<std::ptrdiff_t>(start),
                with_alpha.end());
        };
        const auto opaque_packet=remove_alpha(encoded);
        const auto opaque=decode_bink_sjpi_video_frame(
            opaque_packet,w,h,false);
        CHECK(opaque && !opaque->source_has_alpha);
        CHECK(opaque->video_bytes_consumed==opaque_packet.size());
        for(unsigned p=0;p<3;++p)
            CHECK(opaque->planes[p]==decoded->planes[p]);
        CHECK(opaque->planes[3].size()==w*h);
        CHECK(std::all_of(opaque->planes[3].begin(),
                          opaque->planes[3].end(),
                          [](auto alpha){return alpha==255;}));
        CHECK(opaque->plane_end_byte_offsets[3]==0);
        CHECK(std::all_of(opaque->mode_counts[3].begin(),
                          opaque->mode_counts[3].end(),
                          [](auto count){return count==0;}));
        // A second opaque frame can refer to previous Y/U/V, but
        // never to an alpha section that did not exist in source.
        const auto opaque_skip=remove_alpha(
            make_sjpi_frame(w,h,{0,0,0,0},true));
        const auto reused_opaque=decode_bink_sjpi_video_frame(
            opaque_skip,w,h,false,&*opaque);
        CHECK(reused_opaque && reused_opaque->planes==opaque->planes);
        CHECK(!decode_bink_sjpi_video_frame(
            opaque_skip,w,h,false));
        CHECK(!decode_bink_sjpi_video_frame(
            opaque_skip,w,h,false,&*decoded));
        const auto alpha_skip=make_sjpi_frame(
            w,h,{0,0,0,0},true);
        CHECK(!decode_bink_sjpi_video_frame(
            alpha_skip,w,h,true,&*opaque));
    }
    // An interframe encoded entirely with SKIP must reuse all four
    // previous source planes, including swapped V/U chroma order.
    const auto skip=make_sjpi_frame(w,h,{0,0,0,0},true);
    const auto reused=decode_bink_sjpi_yuva_frame(
        skip,w,h,&*decoded);
    CHECK(reused);
    CHECK(reused->planes==decoded->planes);
    for(const auto& stats:reused->mode_counts)
        CHECK(stats[0]>0 && stats[6]==0);
    CHECK(!decode_bink_sjpi_yuva_frame(skip,w,h));
    CHECK(!decode_bink_sjpi_yuva_frame(encoded,w,h-1));
    CHECK(!decode_bink_sjpi_yuva_frame(encoded,w+1,h));
    auto prev_invalid=*decoded;
    prev_invalid.planes[2].pop_back();
    CHECK(!decode_bink_sjpi_yuva_frame(skip,w,h,&prev_invalid));
    // The recorded alpha length must match exactly the plane's
    // 32-bit-aligned end, otherwise the packet is untrustworthy.
    auto corrupt_header=encoded;
    ++corrupt_header[0];
    CHECK(!decode_bink_sjpi_yuva_frame(corrupt_header,w,h));
    corrupt_header=encoded;
    corrupt_header[0]=0;
    CHECK(!decode_bink_sjpi_yuva_frame(corrupt_header,w,h));
    auto truncated=encoded;
    truncated.resize(truncated.size()-5);
    CHECK(!decode_bink_sjpi_yuva_frame(truncated,w,h));
    CHECK(decoded->planes[1]==reused->planes[1]);
    {
        // Exact display geometry from original BTB walkthroughs:
        // 282x179 YUV420 without alpha. The codec allocates rounded
        // 8x8 blocks at 288x184 luma and 144x96 chroma, and keeps
        // the margins across frames even though they are invisible.
        constexpr std::size_t width=282,height=179;
        constexpr std::size_t pw=288,ph=184,cw=144,ch=96;
        const auto make_odd=[](bool skip) {
            std::vector<std::uint8_t> bytes{
                0x12,0x34,0x56,0x78}; // SJPi picture DWORD
            append(bytes,make_constant_plane(pw,ph,77,skip));
            append(bytes,make_constant_plane(cw,ch,193,skip));
            append(bytes,make_constant_plane(cw,ch,41,skip));
            return bytes;
        };
        const auto odd_encoded=make_odd(false);
        const auto odd=decode_bink_sjpi_video_frame(
            odd_encoded,width,height,false);
        CHECK(odd && !odd->source_has_alpha);
        CHECK(odd->video_bytes_consumed==odd_encoded.size());
        CHECK(odd->planes[0].size()==width*height);
        CHECK(odd->planes[1].size()==141*90);
        CHECK(odd->planes[2].size()==141*90);
        CHECK(odd->planes[3].size()==width*height);
        CHECK(odd->codec_planes[0].size()==pw*ph);
        CHECK(odd->codec_planes[1].size()==cw*ch);
        CHECK(odd->codec_planes[2].size()==cw*ch);
        CHECK(odd->codec_plane_widths[0]==pw);
        CHECK(odd->codec_plane_heights[0]==ph);
        CHECK(std::all_of(odd->planes[0].begin(),
                          odd->planes[0].end(),
                          [](auto x){return x==77;}));
        CHECK(std::all_of(odd->planes[1].begin(),
                          odd->planes[1].end(),
                          [](auto x){return x==41;}));
        CHECK(std::all_of(odd->planes[2].begin(),
                          odd->planes[2].end(),
                          [](auto x){return x==193;}));
        CHECK(std::all_of(odd->planes[3].begin(),
                          odd->planes[3].end(),
                          [](auto x){return x==255;}));
        std::vector<std::uint8_t> odd_reference;
        for(const auto& plane:odd->planes)
            odd_reference.insert(odd_reference.end(),
                                 plane.begin(),plane.end());
        const auto parity=compare_bink_yuva_reference(
            *odd,odd_reference);
        CHECK(parity && parity->identical);
        CHECK(parity->planes[0].compared_bytes==width*height);
        CHECK(parity->planes[1].compared_bytes==141*90);
        odd_reference.pop_back();
        CHECK(!compare_bink_yuva_reference(*odd,odd_reference));

        auto prior=*odd;
        // An 8x8 codec margin is not an extra display pixel.
        // Preserve it through an interframe SKIP operation.
        prior.codec_planes[0][pw*ph-1]=159;
        const auto skipped=decode_bink_sjpi_video_frame(
            make_odd(true),width,height,false,&prior);
        CHECK(skipped);
        CHECK(skipped->planes==prior.planes);
        CHECK(skipped->codec_planes[0].back()==159);
        CHECK(!decode_bink_sjpi_video_frame(
            make_odd(true),width,height,false));
        prior.codec_planes[1].pop_back();
        CHECK(!decode_bink_sjpi_video_frame(
            make_odd(true),width,height,false,&prior));
    }
    // Optional unmodified retail movie probe. No movie/DLL is
    // included in the public repository, and a decoder refusal
    // is reported as a refusal rather than disguised pixel parity.
    for(int arg=1;arg<argc;++arg) {
        std::ifstream in(argv[arg],std::ios::binary);
        CHECK(in.good());
        std::vector<std::uint8_t> retail{
            std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>()};
        const auto index=parse_container_index(retail);
        CHECK(index);
        std::optional<BinkNativeYuVaFrame> prior;
        std::size_t decoded_frames=0,first_failure=0;
        for(std::uint32_t i=1;i<=index->header.frame_count;++i) {
            const auto parts=split_encoded_frame(*index,retail,i);
            CHECK(parts);
            const auto next=decode_bink_sjpi_yuva_frame(
                parts->video,index->header.width,index->header.height,
                prior?&*prior:nullptr);
            if(!next) {
                if(!first_failure)first_failure=i;
                prior.reset();
                continue;
            }
            ++decoded_frames;
            prior=std::move(*next);
        }
        std::printf(
          "Retail native SJPi YUVA: %zu/%u complete frames; "
          "first failed frame=%zu; pixel parity unverified\n",
          decoded_frames,index->header.frame_count,first_failure);
    }
    std::puts("Native Bink SJPi full Y/U/V/alpha and frame-history: PASS");
}
