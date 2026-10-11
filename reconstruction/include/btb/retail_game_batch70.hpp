#pragma once
// Source-backed game-system control flow, original BTB PE32. Explicit inputs
// replace original global addresses; callbacks execute real game subsystems.
// These functions are host-neutral behavior models, not original x86 ABI code.
#include "btb/retail_leaf_runtime.hpp"
#include "btb/retail_shared_runtime.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
namespace btb::retail_game70 {
using I32=std::int32_t;
using U32=std::uint32_t;
using Rect=btb::retail_leaf::Rect32;

// 00402640 FullscreenSystemKeyHook. Only active for HC_ACTION==0 and
// WM_KEYDOWN/UP, WM_SYSKEYDOWN/UP. Suppress alt+return/F4/tab, alt+ESC,
// and ctrl+alt+ESC/Delete. Otherwise delegate to CallNextHookEx.
[[nodiscard]] constexpr bool suppress_fullscreen_system_key(
    I32 hook_code,U32 message,I32 virtual_key,bool alt,bool ctrl) noexcept {
    if(hook_code!=0 || !(message==0x100||message==0x101||
                         message==0x104||message==0x105))return false;
    if(alt&&(virtual_key==13||virtual_key==0x73||virtual_key==9||
             virtual_key==0x1b))return true;
    if(ctrl&&virtual_key==0x1b)return true;
    if(alt&&ctrl&&virtual_key==0x2e)return true;
    return false;
}

// 00403830. Registered DirectDraw surfaces: only existing surface pointers
// are released/reloaded; reload guard flag is reset on all exits.
struct RegisteredBitmap {
    void* surface{};
    std::string_view filename{};
    bool color_keyed{};
};
using SurfaceRelease=void (*)(void*) noexcept;
using SurfaceLoad=void* (*)(std::string_view) noexcept;
using KeySurface=void (*)(void*,U32) noexcept;
inline void reload_registered_bitmap_surfaces(
    std::span<RegisteredBitmap> records,bool& reloading,
    SurfaceRelease release,SurfaceLoad load,KeySurface key) noexcept {
    reloading=true;
    for(auto& r:records) {
        if(!r.surface)continue;
        if(release)release(r.surface);
        r.surface=load?load(r.filename):nullptr;
        if(r.color_keyed&&key)key(r.surface,0xff00ff);
    }
    reloading=false;
}

// 00407920. Returns 1 after eleven distinct global data-table loaders,
// in fixed original sequence, including the video and play-again tables.
using LoadTable=void (*)(I32) noexcept;
[[nodiscard]] inline I32 load_global_data_tables(LoadTable invoke) noexcept {
    for(I32 i=0;i<11;++i)if(invoke)invoke(i);
    return 1;
}

// 00409CD0. Inclusive hover button rect 299..346 x 422..464.
// Voice 0x8f on first hover; click invokes StopAll, redraw, Print.
struct DinoPrintState {I32 x{},y{};bool click{},alternate{},hover_latched{};};
enum class DinoPrintEvent { DrawBase, DrawHighlight, VoiceHover, StopAll, DrawPrintSurface, Print };
using DinoPrintSink=void (*)(DinoPrintEvent,bool) noexcept;
inline void update_dino_print_button(DinoPrintState& s,DinoPrintSink sink) noexcept {
    const bool over=s.x>=299&&s.x<=346&&s.y>=422&&s.y<=464;
    if(sink)sink(DinoPrintEvent::DrawBase,false);
    if(!s.click) {
        if(!over) {s.hover_latched=false;return;}
        if(sink)sink(DinoPrintEvent::DrawHighlight,s.alternate);
        if(!s.hover_latched) {
            if(sink)sink(DinoPrintEvent::VoiceHover,false);
            s.hover_latched=true;
        }
    } else if(over) {
        if(sink) {
            sink(DinoPrintEvent::StopAll,false);
            sink(DinoPrintEvent::DrawPrintSurface,false);
            sink(DinoPrintEvent::Print,false);
        }
    }
}

// 0040D870. Selected pond segment source rect; positive segment index
// is offset by the title-defined leading segment count. Clip right at 2053.
[[nodiscard]] constexpr Rect pond_segment_source(
    I32 segment,I32 leading_segments,I32 stride,I32 height) noexcept {
    const I32 effective=segment>0 ? segment+leading_segments : segment;
    const I32 left=stride*effective;
    const I32 raw_right=stride*(effective+1);
    return {left,0,raw_right>2053?2053:raw_right,height};
}

// 0040E3E0. Drop marks an object active if it is not one of the eligible
// nondeleted records [201,299]. Then clear drag state and reset cursor.
struct ParkDrag {
    I32 mode{},selected{};
    std::span<I32> visible_flags{};
    std::span<const I32> valid_slots{};
};
inline void finish_park_designer_drag(ParkDrag& s,void (*clear_cursor)()) noexcept {
    if(s.mode==2 && s.selected>=0 &&
       static_cast<std::size_t>(s.selected)<s.visible_flags.size()) {
        const auto index=static_cast<std::size_t>(s.selected);
        const bool within=s.selected>=201&&s.selected<=299;
        if(!within || index>=s.valid_slots.size() || s.valid_slots[index]!=1)
            s.visible_flags[index]=1;
    }
    s.mode=0;
    if(clear_cursor)clear_cursor();
}

// 0041F4B0. 24-wide Grand Opening layout head=type, tail marker=10.
// Plays original effect and seeds per-pair parity state (id%2+1) if zero.
using GrandSound=void (*)(I32) noexcept;
[[nodiscard]] inline bool place_grand_opening_event(
    std::span<I32> grid,std::span<const I32> lengths,
    std::span<I32> pair_modes,I32 row,I32 col,I32 type,
    GrandSound play) noexcept {
    if(type<0||type>=static_cast<I32>(lengths.size()) ||
       type/2>=static_cast<I32>(pair_modes.size()))return false;
    if(!btb::retail_shared::can_place_grand_opening(
          grid,row,col,lengths[static_cast<std::size_t>(type)]))return false;
    const auto start=static_cast<std::size_t>(row*24+col);
    grid[start]=type;
    for(I32 i=1;i<lengths[static_cast<std::size_t>(type)];++i)
        grid[start+static_cast<std::size_t>(i)]=10;
    if(play)play(type-row*10);
    auto& pair=pair_modes[static_cast<std::size_t>(type/2)];
    if(pair==0)pair=type%2+1;
    return true;
}

// 00422760. Rare random Spud voice: once per rand()%3000==0; do not
// overlap 0x2D2..0x2D5. Avoid immediately repeating last chosen ID.
using Random15=U32 (*)() noexcept;
using VoiceActive=bool (*)(I32) noexcept;
using PlayVoice=I32 (*)(I32,U32,I32) noexcept;
[[nodiscard]] inline I32 maybe_play_random_spud_voice(
    I32 cooldown,I32 last_voice,Random15 random,VoiceActive playing,
    PlayVoice play) noexcept {
    if(cooldown>=1 || !random)return cooldown;
    U32 draw=random();
    if(draw%3000U!=0U)return static_cast<I32>(draw/3000U);
    for(I32 id=0x2d2;id<0x2d6;++id)if(playing&&playing(id))return 1;
    I32 chosen=0x2d2+static_cast<I32>(random()%4U);
    // Pure source uses retry until !=last; caller's generator must advance.
    for(int retry=0;chosen==last_voice&&retry<128;++retry)
        chosen=0x2d2+static_cast<I32>(random()%4U);
    if(chosen==last_voice)return 0; // bound malformed generator
    return play?play(chosen,50,1):0;
}

// 00427270. Step x/y toward target by 2; snap if signed distance<4,
// including overshoot. Return true only once both have converged.
[[nodiscard]] constexpr bool step_squirrel_actor(
    I32& x,I32& y,I32 target_x,I32 target_y) noexcept {
    if(x<target_x)x+=2;else if(x>target_x)x-=2;
    if(y<target_y)y+=2;else if(y>target_y)y-=2;
    const auto abs32=[](I32 a,I32 b) {
        return btb::retail_leaf::x86_signed_abs_diff(a,b);
    };
    if(abs32(x,target_x)<4)x=target_x;
    if(abs32(y,target_y)<4)y=target_y;
    return abs32(x,target_x)<4 && abs32(y,target_y)<4;
}

// 00428120. In-source walkthrough hover triggers voice if no managed
// voice plays, then decode movie and restart when frame>=total.
using WalkDecode=void (*)() noexcept;
inline void update_walkthrough_movie(
    I32 cursor_x,I32 cursor_y,bool any_managed_sound,
    I32 help_id,PlayVoice voice,WalkDecode decode,
    U32 frame,U32 frames,WalkDecode restart) noexcept {
    if(cursor_x>49&&cursor_x<365&&cursor_y>140&&cursor_y<350 &&
       !any_managed_sound&&voice)
        (void)voice(help_id,90,1);
    if(decode)decode();
    if(frames<=frame && restart)restart();
}

// 00428E30. Persistent UI: if play-again flag, blit first two layers once;
// otherwise Blt(waiting) retries DDERR_WASSTILLDRAWING until success/error.
using GenericBlit=U32 (*)(I32,bool) noexcept;
[[nodiscard]] inline I32 draw_generic_ui(
    bool play_again,GenericBlit blit,std::size_t max_attempts=0) noexcept {
    if(!blit)return 1;
    if(play_again) {(void)blit(0,false);(void)blit(1,true);return 1;}
    std::size_t attempts=0;
    for(;;){
        const auto hr=blit(2,false);
        if(hr!=static_cast<U32>(btb::display::kDdErrWasStillDrawing) ||
           (max_attempts && ++attempts>=max_attempts))return 1;
    }
}

// 00407770. Source video-sequence list contains at most 10 entries;
// read until original fscanf signals EOF (-1) then close the file.
using NextI32=bool (*)(I32*) noexcept;
[[nodiscard]] inline std::size_t load_video_sequence(
    std::span<I32> dest,NextI32 next) noexcept {
    std::size_t n=0;
    for(;n<dest.size()&&n<10;++n){
        I32 value=0;
        if(!next||!next(&value))break;
        dest[n]=value;
    }
    return n;
}

// 00415CB0. Read x/y pairs until EOF, append sentinel (-1,-1).
using NextPair=bool (*)(I32&,I32&) noexcept;
[[nodiscard]] inline std::size_t load_herding_coordinate_pairs(
    std::span<std::array<I32,2>> output,NextPair next) noexcept {
    if(output.empty())return 0;
    std::size_t count=0;
    for(;count+1<output.size();++count){
        I32 x=0,y=0;
        if(!next||!next(x,y))break;
        output[count]={x,y};
    }
    output[count]={-1,-1};
    return count;
}

// 00407390 / 00407430. The original does one fscanf of the indicated
// global options / play-again record and closes. Input is a true stream
// callback, with no invented defaults if records are missing.
using ReadOneRecord=bool (*)(std::span<I32>) noexcept;
[[nodiscard]] inline bool load_options_config(std::span<I32> values,
                                               ReadOneRecord read) noexcept {
    return read && read(values);
}
[[nodiscard]] inline bool load_play_again_overlay(std::span<I32> values,
                                                    ReadOneRecord read) noexcept {
    return read && read(values);
}

} // namespace btb::retail_game70
