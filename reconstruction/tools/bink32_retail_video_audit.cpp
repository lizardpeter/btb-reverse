// Native Bink1/SJPi retail video decoder and exact YUVA420P audit.
// Standalone, no FFmpeg/RAD DLL dependency. The optional reference
// comes from a separately verified decoder, never from the native
// output itself. This tool does not alter supplied retail media.
#include "btb/bink32_yuva_frame.hpp"
#include "btb/bink32_container.hpp"
#include "btb/bink32_packet_decode.hpp"
#include <algorithm>
#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

using namespace btb::bink32_recovered;
namespace {
void usage() {
    std::cerr <<
      "Usage: btb_bink32_retail_video_audit movie.bik "
      "[--reference-yuva reference.yuva] "
      "[--output-yuva native.yuva] "
      "[--max-frames N]\n"
      "Reference format: consecutive raw yuva420p frames, "
      "each in planar Y,U,V,A order.\n";
}
bool read_reference_frame(std::ifstream& source,
                          std::vector<std::uint8_t>& frame) {
    source.read(reinterpret_cast<char*>(frame.data()),
                static_cast<std::streamsize>(frame.size()));
    return source.gcount()==static_cast<std::streamsize>(frame.size());
}
bool write_frame(std::ofstream& target,
                 const BinkNativeYuVaFrame& frame) {
    for(const auto& plane:frame.planes) {
        target.write(reinterpret_cast<const char*>(plane.data()),
                     static_cast<std::streamsize>(plane.size()));
        if(!target)return false;
    }
    return true;
}
constexpr std::array<const char*,4> kPlaneNames{"Y","U","V","A"};

std::string first_failed_sjpi_stage(
    std::span<const std::uint8_t> video,
    std::size_t w,std::size_t h,
    const BinkNativeYuVaFrame* last) {
    const auto outer=parse_alpha_picture_entropy_prologue(video);
    if(!outer)return "alpha/picture header";
    const auto prev=[&](std::size_t index)
        -> std::span<const std::uint8_t> {
        return last?std::span<const std::uint8_t>(
            last->planes[index]):std::span<const std::uint8_t>{};
    };
    BinkPlaneDecodeDiagnostic diagnostic{};
    const auto details=[&]() {
        return " row="+std::to_string(diagnostic.row)+
               ",col="+std::to_string(diagnostic.column)+
               ",mode="+std::to_string(unsigned(diagnostic.mode))+
               ",packet_bit="+std::to_string(diagnostic.bit_offset)+
               ",source_line="+std::to_string(diagnostic.source_line);
    };
    const auto alpha=decode_bink_picture_plane_at(
        video,4,w,h,prev(3),&diagnostic);
    if(!alpha)return "alpha plane entropy/block decode"+details();
    if(alpha->next_plane_byte_offset!=outer->alpha_section_bytes)
        return "alpha section length mismatch (decoded end="+
            std::to_string(alpha->next_plane_byte_offset)+
            ",declared end="+
            std::to_string(outer->alpha_section_bytes)+")";
    const auto y=decode_bink_picture_plane_at(
        video,outer->picture.initial_byte_offset,w,h,prev(0),&diagnostic);
    if(!y)return "Y plane entropy/block decode"+details();
    const auto v=decode_bink_picture_plane_at(
        video,y->next_plane_byte_offset,w/2,h/2,prev(2),&diagnostic);
    if(!v)return "V chroma plane entropy/block decode (start="+
        std::to_string(y->next_plane_byte_offset)+")"+details();
    const auto u=decode_bink_picture_plane_at(
        video,v->next_plane_byte_offset,w/2,h/2,prev(1),&diagnostic);
    if(!u)return "U chroma plane entropy/block decode (start="+
        std::to_string(v->next_plane_byte_offset)+")"+details();
    return "post-plane validation (all 4 plane probes succeeded)";
}

} // namespace
int main(int argc,char**argv) {
    if(argc<2) {usage();return 1;}
    std::string movie_path=argv[1];
    std::string reference_path,output_path;
    std::optional<std::uint32_t> max_frames;
    for(int i=2;i<argc;++i) {
        const std::string_view flag=argv[i];
        if(i+1>=argc){usage();return 1;}
        const std::string_view value=argv[++i];
        if(flag=="--reference-yuva") {
            reference_path=value;
        } else if(flag=="--output-yuva") {
            output_path=value;
        } else if(flag=="--max-frames") {
            std::uint32_t parsed=0;
            const auto [end,error]=std::from_chars(
                value.data(),value.data()+value.size(),parsed);
            if(error!=std::errc{} ||
               end!=value.data()+value.size() || parsed==0) {
                usage();return 1;
            }
            max_frames=parsed;
        } else {
            usage();return 1;
        }
    }
    // Never permit an output path to alias the supplied original
    // BIK or reference. The retail inputs are strictly read-only.
    if(!output_path.empty()) {
        const auto normalized=[](const std::string& name) {
            std::error_code ec;
            const auto p=std::filesystem::weakly_canonical(name,ec);
            return ec
                ?std::filesystem::absolute(name).lexically_normal()
                :p;
        };
        if(normalized(output_path)==normalized(movie_path) ||
           (!reference_path.empty() &&
            normalized(output_path)==normalized(reference_path))) {
            std::cerr<<"Output YUVA must not overwrite a BIK or reference\\n";
            return 1;
        }
    }
    std::ifstream source(movie_path,std::ios::binary);
    if(!source) {
        std::cerr<<"Cannot read original BIK: "<<movie_path<<"\n";
        return 1;
    }
    std::vector<std::uint8_t> data{
        std::istreambuf_iterator<char>(source),
        std::istreambuf_iterator<char>()};
    const auto index=parse_container_index(data);
    if(!index) {
        std::cerr<<"BIK container validation failed\n";
        return 1;
    }
    if((index->video_flags&0x00100000U)==0 ||
       (index->header.width%16)!=0 ||
       (index->header.height%16)!=0) {
        std::cerr<<"Only SJPi with alpha and aligned YUV420 "
                    "dimensions is supported by this audit\n";
        return 1;
    }
    const auto count=max_frames
        ? std::min(*max_frames,index->header.frame_count)
        : index->header.frame_count;
    const auto w=index->header.width,h=index->header.height;
    const auto bytes_per_frame=(std::size_t(w)*h*5)/2;
    std::ifstream reference;
    if(!reference_path.empty()) {
        reference.open(reference_path,std::ios::binary);
        if(!reference) {
            std::cerr<<"Cannot read reference YUVA: "<<reference_path<<"\n";
            return 1;
        }
    }
    std::ofstream output;
    if(!output_path.empty()) {
        output.open(output_path,std::ios::binary|std::ios::trunc);
        if(!output) {
            std::cerr<<"Cannot create native YUVA: "<<output_path<<"\n";
            return 1;
        }
    }
    std::optional<BinkNativeYuVaFrame> previous;
    std::vector<std::uint8_t> reference_pixels(bytes_per_frame);
    std::size_t decoded_count=0,mismatched_frames=0;
    std::array<std::uint64_t,4> mismatch_totals{};
    std::cout<<"BIK "<<w<<"x"<<h<<" frames="<<index->header.frame_count
             <<" tracks="<<index->tracks.size()
             <<" checking="<<count<<"\n";
    for(std::uint32_t number=1;number<=count;++number) {
        const auto encoded=split_encoded_frame(*index,data,number);
        if(!encoded) {
            std::cerr<<"Frame "<<number<<": malformed audio/video packet\n";
            return 2;
        }
        auto frame=decode_bink_sjpi_yuva_frame(
            encoded->video,w,h,previous?&*previous:nullptr);
        if(!frame) {
            std::cerr<<"Frame "<<number
                     <<": native SJPi YUVA decode refused at "
                     <<first_failed_sjpi_stage(
                           encoded->video,w,h,
                           previous?&*previous:nullptr)
                     <<". Previous successfully decoded frames="
                     <<decoded_count<<". No pixel parity is claimed.\n";
            return 2;
        }
        if(output.is_open() && !write_frame(output,*frame)) {
            std::cerr<<"Cannot write native frame "<<number<<"\n";
            return 1;
        }
        if(reference.is_open()) {
            if(!read_reference_frame(reference,reference_pixels)) {
                std::cerr<<"Reference raw stream is truncated at frame "
                         <<number<<"\n";
                return 1;
            }
            const auto parity=compare_bink_yuva_reference(
                *frame,reference_pixels);
            if(!parity) {
                std::cerr<<"Invalid native/reference geometry at frame "
                         <<number<<"\n";
                return 1;
            }
            if(!parity->identical) {
                ++mismatched_frames;
                std::cout<<"FRAME "<<number
                         <<" DIFF total="<<parity->total_different_bytes;
                for(std::size_t p=0;p<4;++p) {
                    const auto& s=parity->planes[p];
                    mismatch_totals[p]+=s.different_bytes;
                    if(!s.different_bytes)continue;
                    const auto pw=(p==1||p==2)?w/2:w;
                    std::cout<<" "<<kPlaneNames[p]
                        <<":different="<<s.different_bytes
                        <<",first_xy="<<(s.first_different_index%pw)
                        <<","<<(s.first_different_index/pw)
                        <<",native="<<unsigned(s.first_native)
                        <<",reference="<<unsigned(s.first_reference)
                        <<",max_delta="
                        <<unsigned(s.maximum_absolute_difference);
                }
                std::cout<<"\n";
            }
        }
        previous=std::move(frame);
        ++decoded_count;
        if(number==1||number==count||number%25==0)
            std::cout<<"Decoded "<<number<<"/"<<count<<" frames\n";
    }
    if(reference.is_open() && count==index->header.frame_count &&
       reference.peek()!=std::char_traits<char>::eof()) {
        std::cerr<<"Reference has trailing data beyond the declared frames\n";
        return 1;
    }
    std::cout<<"SUMMARY decoded="<<decoded_count
             <<"/"<<count<<" frames, "
             <<mismatched_frames<<" reference-mismatched frames";
    if(reference.is_open())for(std::size_t p=0;p<4;++p)
        std::cout<<" "<<kPlaneNames[p]<<"_mismatches="
                 <<mismatch_totals[p];
    std::cout<<"\n";
    return mismatched_frames ? 3:0;
}
