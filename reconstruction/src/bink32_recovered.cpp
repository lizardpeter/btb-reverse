#include "btb/bink32_recovered.hpp"
#include <algorithm>
namespace btb::bink32_recovered {

namespace {
std::uint32_t le32(std::span<const std::uint8_t> bytes,std::size_t off) noexcept {
    return std::uint32_t(bytes[off]) | (std::uint32_t(bytes[off+1])<<8)
         | (std::uint32_t(bytes[off+2])<<16) | (std::uint32_t(bytes[off+3])<<24);
}
}
// Source: original BinkOpen code 30003E77..30003EB6 compares four DWORD
// literals. Notably 30003E90 compares with 0x69504A53 (ASCII "SJPi").
Magic classify_magic(std::uint32_t m) noexcept {
    switch(m) {
    case 0x664b4942U:return Magic::BIKf;
    case 0x674b4942U:return Magic::BIKg;
    case 0x684b4942U:return Magic::BIKh;
    case 0x69504a53U:return Magic::SJPi;
    default:return Magic::Invalid;
    }
}
// Read only fields directly verified against retail BinkOpen's 44-byte
// header read; this is NOT the complete open/decode procedure.
// The null frame-count branch is at 30003EB6; fields at 0x14/18/1C/20
// are confirmed by the actual retail BIKs supplied for this project.
std::optional<Header> parse_header_prefix(
    std::span<const std::uint8_t> bytes) noexcept {
    if(bytes.size()<36)return std::nullopt;
    const auto magic=classify_magic(le32(bytes,0));
    if(magic==Magic::Invalid)return std::nullopt;
    Header out{magic,le32(bytes,8),le32(bytes,20),
               le32(bytes,24),le32(bytes,28),le32(bytes,32)};
    if(out.frame_count==0)return std::nullopt;
    return out;
}

// Original _RADSetMemory@8 at 30001000. Two global pointer writes.
void rad_set_memory(Globals& g,std::uintptr_t alloc,std::uintptr_t release) noexcept {
    g.allocation_callback=alloc;g.free_callback=release;
}
// Original _BinkBufferSetResolution@12 at 30001880. Three global writes.
void buffer_set_resolution(Globals& g,std::uint32_t w,
                           std::uint32_t h,std::uint32_t bpp) noexcept {
    g.buffer_width=w;g.buffer_height=h;g.buffer_bpp=bpp;
}
// _BinkBufferGetError@0 at 30003910: address of global buffer.
const char* buffer_get_error(const Globals& g) noexcept {return g.buffer_error.c_str();}

// _BinkSetError@4 at 30003C80: copies the original NUL-terminated string.
// Use an owning std::string rather than reproducing the original unsafe strcpy.
void set_error(Globals& g,const char* message) {g.error=message?message:"";}
// _BinkGetError@0 at 30003CA0.
const char* get_error(const Globals& g) noexcept {return g.error.c_str();}
// _BinkSetFrameRate@8 at 30003D00.
void set_frame_rate(Globals& g,std::uint32_t rate,std::uint32_t divisor) noexcept {
    g.frame_rate=rate;g.frame_rate_divisor=divisor;
}
// _BinkSetIOSize@4 at 30003D20.
void set_io_size(Globals& g,std::uint32_t bytes) noexcept {g.io_buffer_size=bytes;}
// _BinkSetIO@4 at 30003D30.
void set_io(Globals& g,std::uintptr_t callback) noexcept {g.io_callback=callback;}
// _BinkSetSimulate@4 at 30003D40.
void set_simulate(Globals& g,std::uint32_t amount) noexcept {g.simulation=amount;}
// _BinkSetSoundTrack@8 at 30003D50: unsigned clamp to eight; copy
// exactly count track IDs and preserve the remaining existing slots.
void set_sound_track(Globals& g,std::uint32_t count,std::span<const std::uint32_t> ids) {
    const auto n=std::min<std::uint32_t>(count,8);
    if(ids.size()<n) {g.sound_track_count=0;return;} // avoid unsafe retail read
    g.sound_track_count=n;
    std::copy_n(ids.begin(),n,g.sound_track_ids.begin());
}

// _BinkGetTrackType@8 at 30007AE0: [HBINK + 0x264][index].
std::uint32_t get_track_type(const Movie* m,std::size_t i) noexcept {
    return m&&i<m->track_types.size()?m->track_types[i]:0;
}
// _BinkGetTrackMaxSize@8 at 30007B00: [HBINK + 0x260][index].
std::uint32_t get_track_max_size(const Movie* m,std::size_t i) noexcept {
    return m&&i<m->track_max_sizes.size()?m->track_max_sizes[i]:0;
}
// _BinkGetTrackID@8 at 30007B20: [HBINK + 0x268][index].
std::uint32_t get_track_id(const Movie* m,std::size_t i) noexcept {
    return m&&i<m->track_ids.size()?m->track_ids[i]:0;
}
// _BinkSetVideoOnOff@8 at 30007DA0: [HBINK + 0x2A4] = value.
void set_video_on_off(Movie* m,std::uint32_t value) noexcept {
    if(m)m->video_on_off=value;
}

// _BinkGetKeyFrame@12 at 300066B0..30006783 (nontrivial reverse walk).
// Control flow recovered from branch addresses 300066CD/DE/E9/F0/
// 30006723/2B/58/7F. Bink frame numbers are one-based.
std::uint32_t get_key_frame(const Movie* m,std::uint32_t frame,
                            std::uint32_t mode) noexcept {
    if(!m||frame==0)return 0;
    const auto flags=m->frame_offset_flags;
    const auto key=[&](std::uint32_t i) noexcept -> bool {
        return i<flags.size()&&(flags[i]&1U)!=0;
    };
    // Unless bit 7 is set, a frame already flagged as key is returned.
    if((mode&0x80U)==0&&key(frame-1))return frame;
    switch(mode&0x7fU) {
    case 0: {
        // Scan backward from (frame-2) through index 1.
        if(frame<=2)return frame-1;
        for(std::uint32_t i=frame-2;i>=1;--i) {
            if(key(i))return i+1;
            if(i==1)break;
        }
        return 1;
    }
    case 1: {
        // Search forward from index frame for next key.
        for(std::uint32_t i=frame;i<m->frame_count&&i<flags.size();++i)
            if(key(i))return i+1;
        return 0;
    }
    case 2: {
        // Interleaved backward/forward probes as in original 300066F0.
        if(frame>=2) {
            for(std::int64_t i=static_cast<std::int64_t>(frame)-2;i>=0;--i) {
                if(key(static_cast<std::uint32_t>(i)))
                    return static_cast<std::uint32_t>(i)+1;
                if(frame<m->frame_count&&key(frame))return frame+1;
            }
        }
        if(frame<m->frame_count&&key(frame))return frame+1;
        return 0;
    }
    default:return 0;
    }
}
} // namespace btb::bink32_recovered
