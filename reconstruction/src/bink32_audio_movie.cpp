#include "btb/bink32_audio_movie.hpp"
#include "btb/bink32_audio_packets.hpp"
#include "btb/bink32_audio_stream.hpp"
#include "btb/bink32_container.hpp"
#include "btb/bink32_packet_decode.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace btb::bink32_recovered {
namespace {
constexpr std::size_t kMaxRawMovieBytes=100'000'000;
constexpr std::size_t kMaxPcmBytes=64U*1024U*1024U;
[[nodiscard]] std::optional<std::uint64_t> clock_us(
    std::uint64_t frames,std::uint32_t rate) noexcept {
    if(rate==0 || frames/rate>
        std::numeric_limits<std::uint64_t>::max()/1'000'000ULL)
        return std::nullopt;
    return (frames/rate)*1'000'000ULL+
           ((frames%rate)*1'000'000ULL)/rate;
}
}

std::optional<std::uint64_t>
Bink1MoviePcm16::duration_us() const noexcept {
    return clock_us(sample_frames(),sample_rate);
}
std::optional<std::uint64_t>
Bink1MoviePcm16::timestamp_us(std::uint32_t end_frame) const noexcept {
    if(end_frame>=audio_frame_boundaries.size())return std::nullopt;
    return clock_us(audio_frame_boundaries[end_frame],sample_rate);
}

std::optional<Bink1MoviePcm16>
decode_bink1_movie_audio(std::span<const std::uint8_t> original_bik,
                         std::size_t track_index,std::string& error) {
    if(original_bik.empty()||original_bik.size()>kMaxRawMovieBytes) {
        error="original BIK exceeds bounded input size";
        return std::nullopt;
    }
    const auto index=parse_container_index(original_bik);
    if(!index || index->header.magic!=Magic::SJPi ||
       index->header.frame_count==0) {
        error="not a complete supported SJPi Bink1 movie";
        return std::nullopt;
    }
    const auto tracks=describe_bink_audio_tracks(*index);
    if(!tracks || track_index>=tracks->size()) {
        error="requested original Bink audio track is unavailable";
        return std::nullopt;
    }
    const auto& selected=(*tracks)[track_index];
    auto decoder=Bink1AudioStreamDecoder::open(selected);
    if(!decoder) {
        error="Bink1 audio transform and rate are not supported";
        return std::nullopt;
    }
    if(std::size_t(index->header.frame_count)+1>kMaxRawMovieBytes) {
        error="unreasonably large original video frame count";
        return std::nullopt;
    }
    // Locate the last non-empty packet before decoding. Only that packet
    // may declare a partial final transform block; a short declaration in
    // the middle of a movie is malformed and must not be silently accepted.
    std::uint32_t last_audio_frame=0;
    for(std::uint32_t frame=1;frame<=index->header.frame_count;++frame) {
        const auto packet=split_encoded_frame(*index,original_bik,frame);
        if(!packet || track_index>=packet->audio.size()) {
            error="invalid source audio packet at video frame "+
                  std::to_string(frame);
            return std::nullopt;
        }
        if(!packet->audio[track_index].compressed.empty())
            last_audio_frame=frame;
    }
    if(!last_audio_frame) {
        error="selected original Bink track contains no audio packets";
        return std::nullopt;
    }
    Bink1MoviePcm16 result{
        selected.sample_rate,selected.channels,selected.track_id};
    result.audio_frame_boundaries.reserve(
        std::size_t(index->header.frame_count)+1);
    result.audio_frame_boundaries.push_back(0);
    for(std::uint32_t frame=1;frame<=index->header.frame_count;++frame) {
        const auto packet=split_encoded_frame(*index,original_bik,frame);
        if(!packet || track_index>=packet->audio.size()) {
            error="invalid source audio packet at video frame "+
                  std::to_string(frame);
            return std::nullopt;
        }
        const auto decoded=decoder->decode(packet->audio[track_index]);
        if(!decoded) {
            error="native Bink1 audio decoding refused video frame "+
                  std::to_string(frame);
            return std::nullopt;
        }
        const auto pcm=encode_bink1_pcm16(
            *decoded,decoder->layout(),frame==last_audio_frame);
        if(!pcm || pcm->channels!=result.channels ||
           pcm->sample_rate!=result.sample_rate) {
            error="native Bink1 PCM16 envelope mismatch at video frame "+
                  std::to_string(frame);
            return std::nullopt;
        }
        if(pcm->interleaved_samples.size() >
           kMaxPcmBytes/2-result.samples.size()) {
            error="source Bink1 audio exceeds bounded 64 MiB PCM budget";
            return std::nullopt;
        }
        result.samples.insert(result.samples.end(),
            pcm->interleaved_samples.begin(),
            pcm->interleaved_samples.end());
        if(result.samples.size()%result.channels!=0) {
            error="source Bink1 packet cut an interleaved sample frame";
            return std::nullopt;
        }
        result.audio_frame_boundaries.push_back(result.sample_frames());
    }
    if(!result.duration_us()) {
        error="source Bink1 audio duration overflow";
        return std::nullopt;
    }
    error.clear();
    return result;
}
} // namespace btb::bink32_recovered
