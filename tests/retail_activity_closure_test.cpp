#include "btb/retail_activity_closure.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <vector>

using namespace btb::retail_activity;
namespace {
std::vector<int> trace;
int restore_calls=0;
int pos=0;
void* obj=reinterpret_cast<void*>(0x11);
bool mmio_seek(void*,I32 position) noexcept {trace.push_back(1);return position==44;}
bool descend(void*) noexcept {trace.push_back(2);return true;}
bool create(void*) noexcept {trace.push_back(3);return true;}
bool info(void*) noexcept {trace.push_back(4);return true;}
U32 status(void*,U32* bits) noexcept {*bits=2;trace.push_back(5);return 0;}
U32 restore(void*) noexcept {trace.push_back(6);return (++restore_calls)==1?0x88780096U:0;}
void sleep(U32 ms) noexcept {trace.push_back(7);assert(ms==10);}
void* choose(void*) noexcept {trace.push_back(8);return obj;}
U32 restored(void*,bool* out) noexcept {*out=true;trace.push_back(9);return 0;}
U32 fill(void*,void*) noexcept {trace.push_back(10);return 0;}
void reset(void*) noexcept {trace.push_back(11);}
U32 play(void*,U32 a,U32 b,U32 c) noexcept {
    trace.push_back(12);assert(a==0&&b==17&&c==1);return 0x80004005U;
}
void volume(void*,I32 volume) noexcept {trace.push_back(13);assert(volume==-420);}
void choices() noexcept {trace.push_back(14);}
bool keyboard(std::array<std::uint8_t,256>& keys) noexcept {
    trace.push_back(15);keys[2]=77;return true;
}
bool keyboard_fail(std::array<std::uint8_t,256>&) noexcept {
    trace.push_back(16);return false;
}
U32 map(U32 scan,U32 type) noexcept {
    trace.push_back(17);assert(scan==55&&type==1);return 44;
}
void to_ascii(U32 key,U32 scan,const std::array<std::uint8_t,256>& keys,
              std::uint16_t* out) noexcept {
    trace.push_back(18);assert(key==44&&scan==55&&keys[2]==77);*out=65;
}
I32 keyframe(void*,I32 target) noexcept {trace.push_back(19);return target-2;}
void seek(void*,I32 frame) noexcept {trace.push_back(20);pos=frame;}
I32 frame(void*) noexcept {return pos;}
void decode(void*) noexcept {trace.push_back(21);}
void copy(void*,void*) noexcept {trace.push_back(22);}
void next(void*) noexcept {trace.push_back(23);++pos;}
std::vector<Rect> sources;
void blit(I32 x,I32 y,const Rect& r) noexcept {
    assert(x==10&&y==20);sources.push_back(r);
}
bool order(std::initializer_list<int> seq) {return trace==std::vector<int>(seq);}
}

int main() {
    // 004048F0 both file-backed read and memory-backed reset.
    btb::dsutil::RetailCWaveFile32 wave{};
    wave.m_bIsReadingFromMemory=1;wave.m_pbData=77;wave.m_pbDataCur=90;
    trace.clear();
    assert(reset_wave_file(wave,obj,mmio_seek,descend,create,info)==0);
    assert(wave.m_pbDataCur==77&&trace.empty());
    wave.m_bIsReadingFromMemory=0;wave.m_hmmio=0;
    assert(reset_wave_file(wave,obj,mmio_seek,descend,create,info)==0x800401F0U);
    wave.m_hmmio=99;wave.m_dwFlags=1;
    wave.m_ckRiff.dwDataOffset=40;
    trace.clear();
    assert(reset_wave_file(wave,obj,mmio_seek,descend,create,info)==0);
    assert(wave.m_ck.ckid==0x61746164U&&order({1,2}));
    wave.m_dwFlags=2;trace.clear();
    assert(reset_wave_file(wave,obj,mmio_seek,descend,create,info)==0);
    assert(order({3,4})&&wave.m_ck.cksize==0);

    // 00404270: restored bit + two Restore calls + one Sleep if first
    // attempt reported DSERR_BUFFERLOST. Explicit outflag set on success.
    trace.clear();restore_calls=0;
    bool was_restored=false;
    assert(restore_sound_buffer(obj,&was_restored,status,restore,sleep,5)==0);
    assert(was_restored);
    assert(order({5,6,7,6}));
    assert(restore_sound_buffer(nullptr,&was_restored,status,restore,sleep)==
           0x800401F0U);

    // 00404360: GetFreeBuffer / Restore / Fill / rewind / Play /
    // SetVolume. The result is Play HRESULT, not volume HRESULT.
    trace.clear();
    assert(sound_play(obj,true,17,1,-420,choose,restored,fill,reset,
                      play,volume)==0x80004005U);
    assert(order({8,9,10,11,12,13}));
    trace.clear();
    assert(sound_play(obj,false,17,1,-420,choose,restored,fill,reset,
                      play,volume)==0x800401F0U);
    assert(trace.empty());

    // 0041FB90 Bobs Band strict hit rectangle and source selection; offset5
    // unless active mode1 offset10. Restore cursor to un-offset position.
    BandHitState band{10,20,0,-1};
    const std::array<BandRegion,2> regions{{
        {{11,21,21,31},900},
        {{50,50,80,80},-1}
    }};
    assert(hit_test_bobs_band_regions(band,regions)==900);
    assert(band.selected_index==0&&band.pointer_x==10&&band.pointer_y==20);
    band={10,20,1,-1};
    assert(hit_test_bobs_band_regions(band,regions)==900);
    band={0,0,0,-1};
    assert(hit_test_bobs_band_regions(band,regions)==-1);
    assert(band.pointer_x==0&&band.pointer_y==0);

    // 0041AEB0 a direction pulse needs three matching buffered samples;
    // transition clears both signals, not just whichever changed.
    MazeDebounce debounce{};
    I32 left=1,up=1;
    debounce_maze_directions(debounce,left,up);
    assert(left==0&&up==0);
    left=1;up=1;
    debounce_maze_directions(debounce,left,up);
    assert(left==0&&up==0);
    left=1;up=1;
    debounce_maze_directions(debounce,left,up);
    assert(left==1&&up==1);
    left=0;up=1;
    debounce_maze_directions(debounce,left,up);
    assert(left==0&&up==0);

    // 00427320 in windowed mode only even ticks advance and use step50;
    // nonwindowed uses step5. Source phase1 >500 resets to -420.
    SquirrelConveyor squirrel{1,480,1,0,0};
    trace.clear();
    update_squirrel_conveyor(squirrel,true,choices);
    assert(squirrel.position==480&&squirrel.phase==1);
    update_squirrel_conveyor(squirrel,true,choices);
    assert(squirrel.position==-420&&squirrel.phase==2);
    assert(squirrel.animation_frame==1&&order({14}));
    squirrel.position=-2;squirrel.phase=2;
    update_squirrel_conveyor(squirrel,false,choices);
    assert(squirrel.position==0&&squirrel.phase==0);

    // 00408460 KeyboardState failure stops before MapVirtualKeyEx.
    trace.clear();std::uint16_t ascii=0;
    translate_keyboard_key(55,&ascii,keyboard_fail,map,to_ascii);
    assert(ascii==0&&order({16}));
    trace.clear();
    translate_keyboard_key(55,&ascii,keyboard,map,to_ascii);
    assert(ascii==65&&order({15,17,18}));

    // 00424C80 seeks to previous keyframe and consumes Bink frames
    // until target, using actual callback frames rather than guessed data.
    trace.clear();pos=0;
    assert(seek_decode_bink_exact(obj,obj,10,keyframe,seek,frame,decode,
                                  copy,next,10));
    assert(pos==10&&order({19,20,21,22,23,21,22,23}));
    trace.clear();pos=0;
    assert(!seek_decode_bink_exact(obj,obj,10,keyframe,seek,frame,decode,
                                  copy,next,1));

    // 0040D7D0 source segments begin at 1, not 0. Last right is clamped.
    sources.clear();
    draw_park_pond_segments(3,800,90,10,20,blit);
    assert(sources.size()==3);
    assert(sources[0].left==800&&sources[0].right==1600);
    assert(sources[1].left==1600&&sources[1].right==2053);
    assert(sources[2].left==2400&&sources[2].right==2053);
    assert(sources[2].bottom==90);
}
