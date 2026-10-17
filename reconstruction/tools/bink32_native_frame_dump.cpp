// Decode one genuine original SJPi movie frame to a portable RGB PPM.
// Independent of RAD DLL, FFmpeg, Win32 and DirectDraw.
#include "btb/bink32_native_video_stream.hpp"
#include "btb/bink32_surface_convert.hpp"
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <limits>
#include <string_view>
#include <vector>
using namespace btb::bink32_recovered;
int main(int argc,char** argv) {
    if(argc!=4) {
        std::fprintf(stderr,"Usage: %s input.bik one_based_frame output.ppm\n",argv[0]);
        return 2;
    }
    std::uint32_t number{};
    const std::string_view digits{argv[2]};
    const auto parsed=std::from_chars(digits.data(),digits.data()+digits.size(),number);
    if(parsed.ec!=std::errc{} || parsed.ptr!=digits.data()+digits.size() || !number) {
        std::fprintf(stderr,"Frame number must be a positive integer\n");
        return 2;
    }
    std::ifstream in(argv[1],std::ios::binary|std::ios::ate);
    if(!in) {std::perror("Cannot open source BIK");return 1;}
    const auto count=in.tellg();
    if(count<=0||count>100'000'000) {
        std::fprintf(stderr,"BIK must be between 1 and 100,000,000 bytes\n");
        return 1;
    }
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(count));
    in.seekg(0);
    if(!in.read(reinterpret_cast<char*>(bytes.data()),
                static_cast<std::streamsize>(bytes.size()))) {
        std::fprintf(stderr,"BIK read failed\n");return 1;
    }
    auto stream=BinkNativeVideoStream::open(std::move(bytes));
    if(!stream||number>stream->frame_count()||!stream->seek(number)) {
        std::fprintf(stderr,"Native Bink frame validation/decode failed\n");
        return 1;
    }
    const auto* f=stream->current_frame();
    if(!f || !f->height || f->width>SIZE_MAX/4/f->height) return 1;
    std::vector<std::uint8_t> packed(f->width*f->height*4,0);
    const BinkPackedSurface target{packed,f->width*4,f->width,f->height,4,
                                   0x00ff0000,0x0000ff00,0x000000ff};
    if(blit_bink_yuva_to_surface(*f,target)!=BinkSurfaceResult::ok) {
        std::fprintf(stderr,"Native Bink RGB conversion failed\n");return 1;
    }
    std::ofstream out(argv[3],std::ios::binary|std::ios::trunc);
    if(!out) {std::perror("Cannot open output PPM");return 1;}
    out<<"P6\n"<<f->width<<" "<<f->height<<"\n255\n";
    for(std::size_t i=0;i<packed.size();i+=4) {
        const char rgb[3]={static_cast<char>(packed[i+2]),
                           static_cast<char>(packed[i+1]),
                           static_cast<char>(packed[i])};
        out.write(rgb,3);
    }
    if(!out) {std::fprintf(stderr,"PPM write failed\n");return 1;}
    std::printf("Native frame %u/%u, %zux%zu, %zu audio tracks -> %s\n",
                number,stream->frame_count(),f->width,f->height,
                stream->audio_track_count(),argv[3]);
    // A still frame is not a proof of original color-conversion parity.
    return 0;
}
