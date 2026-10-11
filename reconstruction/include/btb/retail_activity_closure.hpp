#pragma once
// Original Bob Builds a Park, SHA-256
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Additional native-source activity/input/WinMM closures. Callback providers
// must perform real game operations; no synthesized surfaces or audio.

#include "btb/retail_media_game_closure.hpp"
#include "btb/retail_system_closure.hpp"
#include "btb/retail_shared_runtime.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>

namespace btb::retail_activity {
using U32=std::uint32_t;
using I32=std::int32_t;
using Rect=btb::retail_leaf::Rect32;

// 004048F0 CWaveFile_ResetFile. Memory resets current pointer to beginning.
// File mode requires MMIO handle, reads seek to CK_DATA dataOffset+4 and
// mmioDescend(...,MMIO_FINDCHUNK=16); writes call CreateChunk and GetInfo.
using MmioSeek=bool (*)(void*,I32) noexcept;
using MmioTask=bool (*)(void*) noexcept;
[[nodiscard]] inline U32 reset_wave_file(
    btb::dsutil::RetailCWaveFile32& wave,void* mmio,
    MmioSeek seek,MmioTask descend_data,MmioTask create_chunk,
    MmioTask get_info) noexcept {
    if(wave.m_bIsReadingFromMemory!=0) {
        wave.m_pbDataCur=wave.m_pbData;
        return 0;
    }
    if(!wave.m_hmmio)return 0x800401F0U;
    wave.m_ck.ckid=0x61746164U;
    if(wave.m_dwFlags==1) {
        if(!seek||!seek(mmio,static_cast<I32>(wave.m_ckRiff.dwDataOffset+4U)))
            return 0x80004005U;
        if(!descend_data||!descend_data(mmio))return 0x80004005U;
        return 0;
    }
    wave.m_ck.cksize=0;
    if(!create_chunk||!create_chunk(mmio))return 0x80004005U;
    if(!get_info||!get_info(mmio))return 0x80004005U;
    return 0;
}

// 00404270 CSound_RestoreBuffer. Ghidra's register-recovery artifact uses
// unaff_EBX; accept native GetStatus DSBSTATUS_BUFFERLOST bit explicitly
// rather than assuming unknown register contents. Restored outflag true only
// after successful restore. Sleep(10) is requested when DSERR_BUFFERLOST.
using GetStatus=U32 (*)(void*,U32*) noexcept;
using Restore=U32 (*)(void*) noexcept;
using SleepMs=void (*)(U32) noexcept;
[[nodiscard]] inline U32 restore_sound_buffer(
    void* buffer,bool* was_restored,
    GetStatus get_status,Restore restore,SleepMs sleep,
    std::size_t max_attempts=0) noexcept {
    if(!buffer)return 0x800401F0U;
    if(was_restored)*was_restored=false;
    if(!get_status)return 0x80004003U;
    U32 status=0;
    const U32 hr=get_status(buffer,&status);
    if(std::bit_cast<I32>(hr)<0)return hr;
    if((status&2U)==0)return 1;
    if(!restore)return 0x80004003U;
    std::size_t attempts=0;
    for(;;){
        U32 restored=restore(buffer);
        if(restored==static_cast<U32>(btb::dsutil::kDsErrBufferLost) && sleep)
            sleep(10);
        // Ghidra code performs another Restore in the same iteration.
        restored=restore(buffer);
        if(restored==0){
            if(was_restored)*was_restored=true;
            return 0;
        }
        if(max_attempts!=0 && ++attempts>=max_attempts)return restored;
    }
}

// 00404360 CSound_Play. Sequence: GetFreeBuffer, RestoreBuffer, optionally
// fill and CSound_Reset for lost buffers, Play(0,priority,flags), then
// SetVolume(game global), retaining original Play HRESULT as result.
using ChooseSoundBuffer=void* (*)(void*) noexcept;
using RestoreSelected=U32 (*)(void*,bool*) noexcept;
using FillSound=U32 (*)(void*,void*) noexcept;
using ResetSound=void (*)(void*) noexcept;
using PlaySound=U32 (*)(void*,U32,U32,U32) noexcept;
using SoundVolume=void (*)(void*,I32) noexcept;
[[nodiscard]] inline U32 sound_play(
    void* sound,bool has_buffer_array,U32 priority,U32 flags,I32 game_volume,
    ChooseSoundBuffer choose,RestoreSelected restore,
    FillSound fill,ResetSound reset,PlaySound play,
    SoundVolume set_volume) noexcept {
    if(!has_buffer_array)return 0x800401F0U;
    void* buffer=choose?choose(sound):nullptr;
    if(!buffer)return 0x80004005U;
    bool restored=false;
    const U32 hr=restore?restore(buffer,&restored):0x80004003U;
    if(std::bit_cast<I32>(hr)<0)return hr;
    if(restored) {
        const U32 filled=fill?fill(sound,buffer):0x80004003U;
        if(std::bit_cast<I32>(filled)<0)return filled;
        if(reset)reset(sound);
    }
    const U32 result=play?play(buffer,0,priority,flags):0x80004003U;
    if(set_volume)set_volume(buffer,game_volume);
    return result;
}

// 0041FB90 Bobs Band region hit test: pointer offsets 10 in mode1, else5,
// strict rectangles and active flag. Store selection index; preserve the
// quirky original cursor adjustment on a successful hit.
struct BandRegion {
    Rect rect{};
    I32 result{};
};
struct BandHitState {
    I32 pointer_x{};
    I32 pointer_y{};
    I32 mode{};
    I32 selected_index{-1};
};
[[nodiscard]] inline I32 hit_test_bobs_band_regions(
    BandHitState& state,std::span<const BandRegion> regions) noexcept {
    const I32 offset=state.mode==1?10:5;
    const I32 x=state.pointer_x+offset;
    const I32 y=state.pointer_y+offset;
    for(std::size_t i=0;i<regions.size();++i){
        const auto& r=regions[i];
        if(r.result!=-1 && r.rect.left<x && x<r.rect.right &&
           r.rect.top<y && y<r.rect.bottom){
            state.selected_index=static_cast<I32>(i);
            state.pointer_y=y-offset;
            state.pointer_x=x-offset;
            return r.result;
        }
    }
    state.pointer_x=x-offset;
    state.pointer_y=y-offset;
    return -1;
}

// 0041AEB0: two three-sample histories (left/right and up/down). If any
// directional sample changed during the last three frames, clear both
// current inputs. Older samples are shifted before recording new inputs.
struct MazeDebounce {
    std::array<I32,3> first{};
    std::array<I32,3> second{};
};
inline void debounce_maze_directions(
    MazeDebounce& history,I32& first,I32& second) noexcept {
    const auto update=[](std::array<I32,3>& samples,I32 value){
        samples[0]=samples[1];
        samples[1]=samples[2];
        samples[2]=value;
    };
    update(history.first,first);
    update(history.second,second);
    const bool stable_first=history.first[0]==history.first[1]&&
                            history.first[1]==history.first[2];
    const bool stable_second=history.second[0]==history.second[1]&&
                             history.second[1]==history.second[2];
    if(!stable_first||!stable_second){first=0;second=0;}
}

// 00427320 source-defined squirrel conveyor phases, 2-frame animation and
// a separate 50-unit vs 5-unit movement step (window mode dependent).
struct SquirrelConveyor {
    I32 phase{};
    I32 position{};
    I32 animation_countdown{};
    I32 animation_frame{};
    U32 window_counter{};
};
using GenerateConveyorChoices=void (*)() noexcept;
inline void update_squirrel_conveyor(
    SquirrelConveyor& s,bool windowed,GenerateConveyorChoices choices) noexcept {
    I32 step=5;
    if(windowed){
        step=50;
        ++s.window_counter;
        if((s.window_counter&1U)!=0)return;
    }
    if(s.phase==0)return;
    --s.animation_countdown;
    if(s.animation_countdown<1){
        s.animation_countdown=2;
        if(++s.animation_frame>2)s.animation_frame=0;
    }
    if(s.phase==1){
        s.position+=step;
        if(s.position>500){
            s.position=-420;
            if(choices)choices();
            s.phase=2;
        }
    } else if(s.phase==2){
        s.position+=step;
        if(s.position>=0){s.position=0;s.phase=0;}
    }
}

// 00408460: GetKeyboardLayout(0), GetKeyboardState, MapVirtualKeyEx
// translation type 1, then ToAsciiEx with current scan code. If
// GetKeyboardState fails, do not translate/write output.
using GetKeyboardState=bool (*)(std::array<std::uint8_t,256>&) noexcept;
using MapVirtualKey=U32 (*)(U32,U32) noexcept;
using ToAscii=void (*)(U32,U32,const std::array<std::uint8_t,256>&,
                        std::uint16_t*) noexcept;
inline void translate_keyboard_key(
    U32 scan_code,std::uint16_t* output,
    GetKeyboardState get_state,MapVirtualKey map_key,ToAscii translate) noexcept {
    std::array<std::uint8_t,256> keys{};
    if(!get_state||!get_state(keys))return;
    const U32 virtual_key=map_key?map_key(scan_code,1):0;
    if(translate)translate(virtual_key,scan_code,keys,output);
}

// 00424C80: seek via BinkGetKeyFrame(target,0) + BinkGoto(key,0),
// loop DoFrame/CopyToBuffer/NextFrame until FrameNum==target. Original
// unrecognized ESI/EBP registers mean the host supplies real surface.
using Keyframe=I32 (*)(void*,I32) noexcept;
using GotoFrame=void (*)(void*,I32) noexcept;
using MovieFrame=I32 (*)(void*) noexcept;
using DecodeFrame=void (*)(void*) noexcept;
using CopyFrame=void (*)(void*,void*) noexcept;
inline bool seek_decode_bink_exact(
    void* movie,void* surface,I32 target,Keyframe keyframe,
    GotoFrame go,MovieFrame frame,DecodeFrame decode,
    CopyFrame copy,DecodeFrame next,std::size_t max_frames) noexcept {
    if(!movie||!keyframe||!go||!frame||!decode||!copy||!next)return false;
    go(movie,keyframe(movie,target));
    std::size_t steps=0;
    while(frame(movie)!=target){
        if(steps++>=max_frames)return false; // host safety on corrupt movie
        decode(movie);
        copy(movie,surface);
        next(movie);
    }
    return true;
}

// 0040D7D0 Park Designer segmented pond: source chunks begin at
// segment index 1, width=type-specific stride, max right edge 2053.
using SegmentBlit=void (*)(I32,I32,const Rect&) noexcept;
inline void draw_park_pond_segments(
    I32 segment_count,I32 stride,I32 height,I32 dest_x,I32 dest_y,
    SegmentBlit blit) noexcept {
    if(!blit)return;
    for(I32 i=1;i<=segment_count;++i){
        Rect source{i*stride,0,std::min((i+1)*stride,2053),height};
        blit(dest_x,dest_y,source);
    }
}

} // namespace btb::retail_activity
