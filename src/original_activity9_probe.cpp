#include "btb/original_activity9.hpp"
#include <array>
namespace btb::original_activity9 {
namespace {
struct Probe{
 int seeks{},descends{},creates{},infos{},status{},restores{},slept{},
     chosen{},filled{},reset{},played{},volumes{},choices{},
     keyboard{},mapped{},ascii{},frames{},goto_calls{},decoded{},
     copied{},next{},blits{};
 U32 status_bits{2},last_play_priority{},last_play_flags{};
 I32 current_frame{},last_keyframe{},max_right{};
};
Probe g{};
void* const kBuf=reinterpret_cast<void*>(0x1000);
bool seek(void*,I32 at) noexcept {++g.seeks;return at==128;}
bool descend(void*) noexcept {++g.descends;return true;}
bool create(void*) noexcept {++g.creates;return true;}
bool info(void*) noexcept {++g.infos;return true;}
U32 status(void*,U32* value) noexcept {
 ++g.status;*value=g.status_bits;return 0;
}
U32 restore(void*) noexcept {
 ++g.restores;
 return g.restores%2==1
    ?static_cast<U32>(btb::dsutil::kDsErrBufferLost):0U;
}
void sleep(U32 time) noexcept {if(time==10)++g.slept;}
void* choose(void*) noexcept {++g.chosen;return kBuf;}
U32 restored(void*,bool* flag) noexcept {*flag=true;return 0;}
U32 fill(void*,void*) noexcept {++g.filled;return 0;}
void reset(void*) noexcept {++g.reset;}
U32 play(void*,U32 offset,U32 priority,U32 flags) noexcept {
 if(offset==0){++g.played;g.last_play_priority=priority;
   g.last_play_flags=flags;}
 return 0;
}
void volume(void*,I32 amount) noexcept {if(amount==17)++g.volumes;}
void choices() noexcept {++g.choices;}
bool keys(std::array<std::uint8_t,256>& state) noexcept {
 ++g.keyboard;state[30]=1;return true;
}
U32 map(U32 scan,U32 mode) noexcept {
 if(scan==30&&mode==1)++g.mapped;
 return 65;
}
void ascii(U32 key,U32 scan,const std::array<std::uint8_t,256>& keys,
           std::uint16_t* result) noexcept {
 if(key==65&&scan==30&&keys[30]){
  ++g.ascii;*result=static_cast<std::uint16_t>('A');
 }
}
I32 keyframe(void*,I32 frame) noexcept {return frame-2;}
void goto_frame(void*,I32 frame) noexcept {
 ++g.goto_calls;g.last_keyframe=frame;g.current_frame=frame;
}
I32 frame(void*) noexcept {return g.current_frame;}
void decode(void*) noexcept {++g.decoded;}
void copy(void*,void*) noexcept {++g.copied;}
void next(void*) noexcept {++g.next;++g.current_frame;}
void blit(I32 x,I32 y,const Rect& source) noexcept {
 if(x==10&&y==20&&source.right<=2053){
  ++g.blits;g.max_right=source.right;
 }
}
}
bool verify_9_original_activity_functions() noexcept {
 g={};
 // 004048F0: reset from memory, file-read MMIO, file-write MMIO.
 btb::dsutil::RetailCWaveFile32 wave{};
 wave.m_bIsReadingFromMemory=1;
 wave.m_pbData=0x1234;
 if(original_activity9::reset_wave_file(
     wave,kBuf,seek,descend,create,info)!=0 ||
    wave.m_pbDataCur!=0x1234)return false;
 wave.m_bIsReadingFromMemory=0;wave.m_hmmio=1;
 wave.m_dwFlags=1;wave.m_ckRiff.dwDataOffset=124;
 if(original_activity9::reset_wave_file(
     wave,kBuf,seek,descend,create,info)!=0 ||
    g.seeks!=1||g.descends!=1||
    wave.m_ck.ckid!=0x61746164U)return false;
 wave.m_dwFlags=0;
 if(original_activity9::reset_wave_file(
     wave,kBuf,seek,descend,create,info)!=0 ||
    g.creates!=1||g.infos!=1)return false;
 // 00404270 exact lost restore cycle sleeps after DSERR_BUFFERLOST.
 bool recovered=false;
 if(original_activity9::restore_sound_buffer(
     kBuf,&recovered,status,restore,sleep,2)!=0||
    !recovered||g.status!=1||g.restores!=2||g.slept!=1)return false;
 g.status_bits=0;recovered=true;
 if(original_activity9::restore_sound_buffer(
     kBuf,&recovered,status,restore,sleep,2)!=1||
    recovered||g.restores!=2)return false;
 // 00404360 CSound_Play explicit choose, fill and reset, then volume.
 if(original_activity9::sound_play(kBuf,true,6,8,17,choose,
     restored,fill,reset,play,volume)!=0||
    g.chosen!=1||g.filled!=1||g.reset!=1||g.played!=1||
    g.volumes!=1||g.last_play_priority!=6||
    g.last_play_flags!=8)return false;
 // 0041FB90 Bob's Band hit uses strict bounds with source pointer offset.
 const std::array<BandRegion,2> regions{
   BandRegion{Rect{0,0,40,40},7},BandRegion{Rect{40,40,80,80},9}
 };
 BandHitState hit{20,20,0,-1};
 if(original_activity9::hit_test_bobs_band_regions(hit,regions)!=7||
    hit.selected_index!=0||hit.pointer_x!=20||hit.pointer_y!=20)
    return false;
 hit.pointer_x=35;hit.pointer_y=35;hit.mode=1;
 if(original_activity9::hit_test_bobs_band_regions(hit,regions)!=9||
    hit.selected_index!=1)return false;
 // 0041AEB0 maze input debounces until all 3 consecutive samples agree.
 MazeDebounce history{};
 I32 horizontal=1,vertical=0;
 original_activity9::debounce_maze_directions(
     history,horizontal,vertical);
 if(horizontal!=0)return false;
 horizontal=1;
 original_activity9::debounce_maze_directions(
     history,horizontal,vertical);
 if(horizontal!=0)return false;
 horizontal=1;
 original_activity9::debounce_maze_directions(
     history,horizontal,vertical);
 if(horizontal!=1)return false;
 // 00427320 Squirrel advances 50 every other windowed frame or 5 native.
 SquirrelConveyor conveyor{};conveyor.phase=1;conveyor.position=498;
 original_activity9::update_squirrel_conveyor(conveyor,false,choices);
 if(conveyor.phase!=2||conveyor.position!=-420||g.choices!=1)
    return false;
 conveyor.phase=2;conveyor.position=-50;conveyor.window_counter=0;
 original_activity9::update_squirrel_conveyor(conveyor,true,choices);
 if(conveyor.position!=-50)return false;
 original_activity9::update_squirrel_conveyor(conveyor,true,choices);
 if(conveyor.position!=0||conveyor.phase!=0)return false;
 // 00408460 original MapVirtualKeyEx type1 -> ToAsciiEx.
 std::uint16_t out=0;
 original_activity9::translate_keyboard_key(
     30,&out,keys,map,ascii);
 if(out!='A'||g.keyboard!=1||g.mapped!=1||g.ascii!=1)return false;
 // 00424C80 Bink exact keyframe seek executes only remaining delta frames.
 g.current_frame=0;
 if(!original_activity9::seek_decode_bink_exact(
     kBuf,kBuf,5,keyframe,goto_frame,frame,decode,copy,next,8)||
    g.last_keyframe!=3||g.current_frame!=5||
    g.decoded!=2||g.copied!=2||g.next!=2)return false;
 // 0040D7D0 source pond segments from index 1, max right 2053.
 original_activity9::draw_park_pond_segments(
     3,700,120,10,20,blit);
 if(g.blits!=3||g.max_right!=2053)return false;
 return true;
}
}
