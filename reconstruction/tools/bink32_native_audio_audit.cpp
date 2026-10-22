// Diagnostic-only native Bink1 audio audit. Reads an unmodified original
// BIK, decodes each selected audio track packet into candidate floats,
// reports coverage and optionally exports raw interleaved LE IEEE754 F32.
// No proprietary movie bytes or RAD/FFmpeg dependency are embedded.
// Output is NOT yet certified equal to the 2002 Bink DLL PCM stream.
#include "btb/bink32_audio_stream.hpp"
#include "btb/bink32_audio_pcm.hpp"
#include "btb/bink32_audio_packets.hpp"
#include "btb/bink32_container.hpp"
#include "btb/bink32_packet_decode.hpp"
#include <algorithm>
#include <bit>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <limits>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

using namespace btb::bink32_recovered;
namespace {
[[nodiscard]] bool emit_float(std::ofstream& out,float sample) {
    const auto bits=std::bit_cast<std::uint32_t>(sample);
    const char b[4]{
        static_cast<char>(bits),
        static_cast<char>(bits>>8),
        static_cast<char>(bits>>16),
        static_cast<char>(bits>>24)
    };
    out.write(b,4);
    return bool(out);
}
[[nodiscard]] bool save_samples(
    std::ofstream& out,const Bink1DecodedAudioFrame& decoded,
    const Bink1AudioLayout& layout) {
    const auto& samples=decoded.samples;
    if(samples.size()!=layout.transform_channels)return false;
    if(layout.transform==BinkAudioTransform::rdft) {
        for(float value:samples[0])
            if(!emit_float(out,value))return false;
    } else {
        const auto count=samples.front().size();
        for(const auto& channel:samples)
            if(channel.size()!=count)return false;
        for(std::size_t n=0;n<count;++n)
            for(const auto& channel:samples)
                if(!emit_float(out,channel[n]))return false;
    }
    return true;
}
} // namespace
int main(int argc,char** argv) {
    if(argc!=3 && argc!=5) {
        std::fprintf(stderr,
            "Usage: %s original.bik zero_based_track_index"
            " [--output-f32 candidate_audio.f32 | --output-s16le candidate_audio.pcm]\n"
            "Candidate PCM is experimental and not retail-verified.\n",
            argv[0]);
        return 2;
    }
    const bool s16=argc==5 && std::string_view(argv[3])=="--output-s16le";
    if(argc==5 && !s16 && std::string_view(argv[3])!="--output-f32")return 2;
    unsigned chosen{};
    const std::string_view arg(argv[2]);
    const auto number=std::from_chars(arg.data(),arg.data()+arg.size(),chosen);
    if(number.ec!=std::errc{}||number.ptr!=arg.data()+arg.size()) {
        std::fputs("Track index must be a nonnegative integer.\n",stderr);
        return 2;
    }

    const auto original=std::filesystem::path(argv[1]);
    std::ifstream input(original,std::ios::binary|std::ios::ate);
    if(!input){std::fputs("Cannot open original BIK.\n",stderr);return 1;}
    constexpr std::streamoff max_size=100'000'000;
    const auto size=input.tellg();
    if(size<=0 || size>max_size) {
        std::fputs("BIK outside bounded 100MB input range.\n",stderr);
        return 1;
    }
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    input.seekg(0);
    if(!input.read(reinterpret_cast<char*>(bytes.data()),
                   static_cast<std::streamsize>(bytes.size()))) {
        std::fputs("Unable to read complete original movie.\n",stderr);
        return 1;
    }
    const auto index=parse_container_index(bytes);
    if(!index) {std::fputs("Invalid Bink1 movie/index.\n",stderr);return 1;}
    const auto specs=describe_bink_audio_tracks(*index);
    if(!specs || chosen>=specs->size()) {
        std::fputs("No valid audio track exists at that index.\n",stderr);
        return 1;
    }
    const auto& spec=(*specs)[chosen];
    std::uint32_t last_audio_frame=0;
    for(std::uint32_t frame=1;frame<=index->header.frame_count;++frame) {
        const auto packet=split_encoded_frame(*index,bytes,frame);
        if(!packet || chosen>=packet->audio.size()) {
            std::fprintf(stderr,"Corrupt source audio envelope at frame %u.\\n",frame);
            return 1;
        }
        if(!packet->audio[chosen].compressed.empty())
            last_audio_frame=frame;
    }
    if(!last_audio_frame) {
        std::fputs("Selected track has no non-empty audio packets.\\n",stderr);
        return 1;
    }
    auto decoder=Bink1AudioStreamDecoder::open(spec);
    if(!decoder) {
        std::fputs("Unsupported native Bink1 audio format.\n",stderr);
        return 1;
    }
    std::ofstream pcm;
    if(argc==5) {
        pcm.open(argv[4],std::ios::binary|std::ios::trunc);
        if(!pcm){std::fputs("Unable to open candidate output.\n",stderr);return 1;}
    }
    std::size_t blocks=0,skipped=0;
    std::uint64_t output_values=0;
    double maximum=0.0;
    for(std::uint32_t frame=1;frame<=index->header.frame_count;++frame) {
        const auto encoded=split_encoded_frame(*index,bytes,frame);
        if(!encoded || chosen>=encoded->audio.size()) {
            std::fprintf(stderr,"Corrupt movie frame %u audio envelope.\n",frame);
            return 1;
        }
        const auto result=decoder->decode(encoded->audio[chosen]);
        if(!result) {
            std::fprintf(stderr,
                "Native Bink1 audio refused original frame %u of %u, "
                "track %u. No PCM parity claimed.\n",
                frame,index->header.frame_count,chosen);
            return 1;
        }
        if(!result->block_count)++skipped;
        blocks+=result->block_count;
        if(result->samples.size()!=decoder->layout().transform_channels)
            return 1;
        for(const auto& channel:result->samples) {
            output_values+=channel.size();
            for(float sample:channel) {
                if(!std::isfinite(sample)) {
                    std::fprintf(stderr,"Nonfinite native sample at frame %u\n",frame);
                    return 1;
                }
                maximum=std::max(maximum,std::abs(double(sample)));
            }
        }
        if(argc==5) {
            if(s16) {
                const auto packed=encode_bink1_pcm16(
                    *result,decoder->layout(),frame==last_audio_frame);
                if(!packed) {
                    std::fprintf(stderr,
                        "PCM16 structural mismatch in source frame %u: "
                        "declared bytes=%u\n",
                        frame,result->declared_pcm_bytes);
                    return 1;
                }
                const auto data=serialize_bink1_pcm16_le(
                    packed->interleaved_samples);
                pcm.write(reinterpret_cast<const char*>(data.data()),
                          static_cast<std::streamsize>(data.size()));
                if(!pcm) {
                    std::fprintf(stderr,"PCM16 output failed at frame %u\n",frame);
                    return 1;
                }
            } else if(!save_samples(pcm,*result,decoder->layout())) {
                std::fprintf(stderr,"F32 output failed at frame %u\n",frame);
                return 1;
            }
        }
    }
    std::printf("Native Bink1 audio structural audit: %u/%u packets, "
                "%zu compressed blocks, %zu empty track frames\n",
                index->header.frame_count,index->header.frame_count,
                blocks,skipped);
    std::printf("Track %u: ID=%u, %u Hz, %u channel(s), %s, "
                "%llu candidate float sample values, max abs=%.7g\n",
                chosen,spec.track_id,spec.sample_rate,spec.channels,
                spec.transform==BinkAudioTransform::dct?"DCT":"RDFT",
                static_cast<unsigned long long>(output_values),maximum);
    std::puts("WARNING: structural decode only. These sample values "
              "have NOT been validated against retail PCM or FFmpeg.");
    return 0;
}
