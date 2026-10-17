#include "btb/bink32_playback.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace btb::bink32_recovered {

// Original _BinkSetSoundSystem@8 30003CB0..30003CF7.
// Factory identity and non-null handle are process-global in the retail DLL.
bool set_sound_system(
    SoundSystemState& s,std::uintptr_t id,std::uintptr_t arg,
    SoundOpenCallback open,void* user) {
    if(!id||!open)return false;
    if(s.factory_identity && s.factory_identity!=id && s.locked)
        return false;
    if(!s.factory_identity||s.factory_identity!=id)
        s.factory_identity=id;
    const auto result=open(arg,user);
    if(result)s.opened_handle=result;
    return s.opened_handle!=0;
}

// Native _BinkPause@8 at 30006DB0..30006EA8. All tick subtraction is
// uint32 wrapping arithmetic like x86 SUB. The original function updates
// timing statistics even when the pause flag is already the requested value.
std::uint32_t pause(PauseTelemetry* m,std::uint32_t requested,
                    std::uint32_t now,const PauseRuntime& runtime) {
    if(!m)return 0;
    if(m->pause_tick){
        m->accumulated_paused_ticks+=now-m->pause_tick;
        m->pause_tick=0;
    }
    if(m->interval_tick) {
        const std::uint32_t elapsed=now-m->interval_tick;
        const auto largest=m->largest_interval;
        m->interval_tick=0;
        if(elapsed>largest) {
            m->second_interval=largest;
            m->second_frame=m->largest_frame;
            m->largest_interval=elapsed;
            m->largest_frame=m->current_frame;
        }else if(elapsed>m->second_interval) {
            m->second_interval=elapsed;
            m->second_frame=m->current_frame;
        }
    }
    if(requested==0&&m->paused) {
        m->clock_origin=0;
        m->lag_ticks=0;
    }
    m->paused=requested;
    const auto count=std::min<std::size_t>(m->track_count,runtime.tracks.size());
    for(std::size_t i=0;i<count;++i) {
        if(runtime.tracks[i].invoke)
            runtime.tracks[i].invoke(runtime.tracks[i].user,requested);
    }
    if(m->audio_clock&&m->track_count&&runtime.service_audio)
        runtime.service_audio(runtime.user);
    return m->paused;
}

// Tail of _BinkNextFrame@4 3000668A: wrap last frame to one; otherwise
// increment once. The original also services audio and frame binding; this
// isolated routine does neither.
std::uint32_t next_frame_number(std::uint32_t current,
                                std::uint32_t total) noexcept {
    if(!total)return 0;
    return current>=total?1U:current+1;
}

// Original _BinkGoto@12 300067A0..30006846: seek clamping,
// audio preroll in whole frames (ceil(rate/divisor)), and keyframe choice.
// Codec and audio decoder seek loop (3000687D..30006936) not yet translated.
SeekPreflight plan_seek(
    std::uint32_t frames,std::uint32_t current,std::uint32_t target,
    std::uint32_t flags,std::uint32_t tracks,std::uint32_t rate,
    std::uint32_t divisor,std::uint32_t prior_key) noexcept {
    if(!frames)return {};
    const std::uint32_t requested=std::clamp(target,1U,frames);
    const bool direct=(flags&3U)==3U;
    SeekPreflight plan{requested,requested,direct,current==requested};
    if(plan.already_at_frame||direct)return plan;
    std::uint32_t preroll=requested;
    if((flags&2U)==0&&tracks&&divisor) {
        const std::uint32_t frames_per_second=static_cast<std::uint32_t>(
            (std::uint64_t(rate)+divisor-1U)/divisor);
        preroll=frames_per_second>=requested?1U:requested-frames_per_second;
    }
    const auto key=(flags&1U)?requested:prior_key;
    if(key!=0&&key<preroll)preroll=key;
    plan.first_decode_frame=preroll;
    return plan;
}
} // namespace btb::bink32_recovered
