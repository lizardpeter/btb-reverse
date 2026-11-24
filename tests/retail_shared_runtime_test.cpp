#include "btb/retail_shared_runtime.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

using namespace btb::retail_shared;

namespace {
std::vector<int> events;
void desc(void*,SurfaceDescription& d) noexcept {
    events.push_back(1);
    assert(d.size==0x7c && d.flags==6);
    d.width=53; d.height=71;
}
void stop(void*) noexcept {events.push_back(2);}
void reset(void*) noexcept {events.push_back(3);}
void play(void*) noexcept {events.push_back(4);}
void destroy(void*,std::int32_t flag) noexcept {assert(flag==1);events.push_back(5);}
void pause(void*,std::int32_t mode) noexcept {events.push_back(mode==1?6:7);}
void volume(void*,std::int32_t a,std::int32_t b) noexcept {
    assert(a==0 && b==0);events.push_back(8);
}
void apply_volume() noexcept {events.push_back(9);}
void close(void*) noexcept {events.push_back(10);}
void release(void*) noexcept {events.push_back(11);}
std::uint32_t volume_a(void*,std::int32_t value) noexcept {
    assert(value==-1100);events.push_back(12);return 0x80004005U;
}
std::uint32_t volume_b(void*,std::int32_t value) noexcept {
    assert(value==-1100);events.push_back(13);return 0x100U;
}
bool order(std::initializer_list<int> values) {return events==std::vector<int>(values);}
}

int main() {
    // 00407AF0: cursor fallback preserves rect; nonnull surface reads size.
    events.clear();
    CursorSurfaceState cursor{};
    cursor.default_surface=reinterpret_cast<void*>(0x01);
    cursor.source={5,6,7,8};
    set_cursor_surface(cursor,nullptr,desc);
    assert(cursor.current==cursor.default_surface);
    assert(cursor.source.left==5 && events.empty());
    set_cursor_surface(cursor,reinterpret_cast<void*>(0x02),desc);
    assert(cursor.current==reinterpret_cast<void*>(0x02));
    assert(cursor.source.left==0&&cursor.source.top==0);
    assert(cursor.source.right==53&&cursor.source.bottom==71);
    assert(order({1}));

    // 00407960, 00407E10: loading cursor and DirectInput policy.
    events.clear();
    BeginLoadingState loading{};
    loading.loading_surface=reinterpret_cast<void*>(0x44);
    loading.restart_sound=reinterpret_cast<void*>(0x55);
    loading.voice_restart_latch=1;
    begin_loading_cursor(loading,reset,play);
    assert(loading.cursor_surface==loading.loading_surface);
    assert(loading.loading_latch==1);
    assert(loading.source.right==100&&loading.source.bottom==100);
    assert(order({3,4}));
    events.clear();loading.voice_restart_latch=0;
    begin_loading_cursor(loading,reset,play);
    assert(events.empty());
    constexpr auto acquire=direct_input_acquire_step(true,true);
    static_assert(acquire.acquire_primary&&acquire.acquire_secondary);
    static_assert(direct_input_acquire_step(false,true).return_value==1);
    static_assert(direct_input_acquire_step(true,false).unacquire_primary);

    // 004280A0: Stop -> delete -> clear even when playback status irrelevant.
    events.clear();
    void* music=reinterpret_cast<void*>(0x123);
    stop_activity_music(music,stop,destroy);
    assert(music==nullptr&&order({2,5}));
    events.clear();stop_activity_music(music,stop,destroy);
    assert(events.empty());

    // 00402CC0/00402BC0: native 80-slot managed sound metadata.
    btb::sound::RetailSoundManager32 manager{};
    btb::sound::initialize_retail_metadata(manager);
    manager.buffer_group_ptr32[4]=0x321;
    manager.sound_id_by_slot[4]=510;
    manager.slot_by_sound_id[510]=4;
    manager.priority[4]=53;
    manager.slot_state[4]=2;
    events.clear();
    stop_sound_slot(manager,4,reinterpret_cast<void*>(0x321),stop,reset);
    assert(manager.slot_state[4]==3&&order({2,3}));
    events.clear();
    release_sound_slot(manager,4,reinterpret_cast<void*>(0x321),destroy);
    assert(order({5}));
    assert(manager.buffer_group_ptr32[4]==0);
    assert(manager.priority[4]==-1&&manager.slot_state[4]==0);
    assert(manager.sound_id_by_slot[4]==-1&&manager.slot_by_sound_id[510]==-1);

    // 00410FD0/00411080: even crossing the 6-column boundary is allowed
    // by retail's flat pointer scan, unlike a rectangular placement check.
    std::array<std::int32_t,24> fireworks{};
    fireworks.fill(-1);
    const std::array<std::int32_t,3> lengths{1,2,3};
    assert(can_place_firework(fireworks,0,5,2));
    assert(!can_place_firework(fireworks,3,5,2));
    fireworks[5]=1;fireworks[6]=1;
    assert(!can_place_firework(fireworks,0,5,2));
    assert(remove_firework(fireworks,lengths,0,5)==1);
    assert(fireworks[5]==-1&&fireworks[6]==-1);
    assert(remove_firework(fireworks,lengths,0,5)==-1);
    assert(can_place_firework(fireworks,0,0,0));

    // 0041F3A0: 24-column event test DOES reject row wrap.
    std::array<std::int32_t,48> band{};
    band.fill(-1);
    assert(can_place_grand_opening(band,0,23,1));
    assert(!can_place_grand_opening(band,0,23,2));
    assert(can_place_grand_opening(band,1,0,24));
    band[3]=12;
    assert(!can_place_grand_opening(band,0,2,3));
    assert(can_place_grand_opening(band,0,2,0));

    // 0040D420 strict hit boxes terminate at first -1 sentinel.
    constexpr std::array<Rect32,3> toolbars{
        Rect32{10,10,20,20},Rect32{30,30,40,40},Rect32{-1,0,0,0}
    };
    static_assert(hit_test_park_toolbar(toolbars,11,11)==0);
    static_assert(hit_test_park_toolbar(toolbars,10,11)==-1);
    static_assert(hit_test_park_toolbar(toolbars,35,35)==1);
    static_assert(hit_test_park_toolbar(toolbars,40,40)==-1);

    // 0040DC90 selectors (retail memory has 100 records of stride 0x4c).
    std::array<PondRecord,4> pond{PondRecord{7},PondRecord{25},PondRecord{100},PondRecord{101}};
    PondGlobals pg{3,4};
    clear_pond_primary_by_selector(pond,1,pg);
    assert(pond[0].primary_type==-1 && pg.value_005079fc==-1);
    assert(pond[1].primary_type==25&&pg.value_00507a28==3);
    clear_pond_primary_by_selector(pond,2,pg);
    assert(pond[1].primary_type==-1);
    assert(pond[2].primary_type==100&&pond[3].primary_type==101);
    clear_pond_primary_by_selector(pond,0,pg);
    assert(pond[2].primary_type==-1&&pg.value_00507a28==-1);

    // 0040DCE0 selectors: zero removes categories 0/1/2; one removes only 2.
    std::array<BandstandRecord,4> group{
        BandstandRecord{7,0},BandstandRecord{8,1},
        BandstandRecord{9,2},BandstandRecord{10,3}
    };
    clear_bandstand_by_category(group,1);
    assert(group[0].active_object==7);
    assert(group[1].active_object==8);
    assert(group[2].active_object==-1);
    clear_bandstand_by_category(group,0);
    assert(group[0].active_object==-1&&group[1].active_object==-1);
    assert(group[3].active_object==10);

    // 0040D780 reproduces original non-strict tie result.
    static_assert(compare_park_sort_layer(-1,-1,99,100)==0);
    static_assert(compare_park_sort_layer(-1,0,99,100)==-1);
    static_assert(compare_park_sort_layer(0,-1,99,100)==1);
    static_assert(compare_park_sort_layer(0,0,4,4)==-1);
    static_assert(compare_park_sort_layer(0,0,5,4)==1);

    // 00416310 depth comparator: types 15/16 always first even on ties.
    const HerdingSortEntry special{15,0,0,0};
    const HerdingSortEntry ordinary{3,2,3,5};
    const HerdingSortEntry deep{3,2,40,5};
    assert(compare_herding_depth(special,special)==-1);
    assert(compare_herding_depth(special,ordinary)==-1);
    assert(compare_herding_depth(ordinary,special)==1);
    assert(compare_herding_depth(deep,ordinary)==1);
    assert(compare_herding_depth(ordinary,deep)==-1);
    assert(compare_herding_depth(ordinary,ordinary)==-1);

    // 00425980: frame atlas uses floor divide and remainder by 9.
    static_assert(squirrel_frame_rect(0).left==0);
    static_assert(squirrel_frame_rect(1).left==356);
    static_assert(squirrel_frame_rect(9).left==89);
    static_assert(squirrel_frame_rect(10).left==445);
    static_assert(squirrel_frame_rect(10).right==534);
    static_assert(squirrel_frame_rect(10).bottom==133);

    // 00409260 / 004092A0 / 00408F50 preserve Bink call ordering.
    events.clear();
    GlobalMovieState movie{};
    pause_global_movie(movie,pause,volume);
    assert(events.empty());
    movie.movie=reinterpret_cast<void*>(0xABC);
    pause_global_movie(movie,pause,volume);
    assert(movie.paused==1&&movie.input_gate==1);
    assert(order({6,8}));
    events.clear();
    resume_global_movie(movie,pause,apply_volume);
    assert(movie.paused==0&&movie.input_gate==0);
    assert(order({7,9}));
    events.clear();close_global_movie(movie,close);
    assert(order({10}) && movie.movie==nullptr && movie.input_gate==1);
    events.clear();close_global_movie(movie,close);
    assert(order({10})); // Ghidra unconditionally calls BinkClose

    // 00402FC0: untouched HWND/rect survives this original constructor.
    btb::display::RetailDisplayManager32 display{};
    display.hwnd32=0xBEEF;
    display.windowed=1;
    display.direct_draw7_ptr32=55;
    construct_display_manager(display);
    assert(display.vtable_ptr32==btb::display::kDisplayManagerVtable32);
    assert(display.direct_draw7_ptr32==0&&display.primary_surface_ptr32==0);
    assert(display.render_surface_ptr32==0);
    assert(display.unused_legacy_surface_ptr32==0);
    assert(display.hwnd32==0xBEEF&&display.windowed==1);

    // 00404510 CWaveFile ctor zeros only four specifically observed fields.
    btb::dsutil::RetailCWaveFile32 wave{};
    wave.m_pwfx=1;wave.m_hmmio=2;wave.m_dwSize=3;
    wave.m_bIsReadingFromMemory=1;wave.m_dwFlags=0xBEEF;
    construct_wave_file(wave);
    assert(wave.m_pwfx==0&&wave.m_hmmio==0&&wave.m_dwSize==0);
    assert(wave.m_bIsReadingFromMemory==0);
    assert(wave.m_dwFlags==0xBEEF);

    // 00404DD0 printer constructor recovers exact 0x124 object.
    btb::printing::RetailBitmapPrinter32 printer{};
    printer.scale_mode=-9;
    printer.document_name[0]='Z';
    construct_bitmap_printer(printer);
    assert(sizeof(printer)==0x124);
    assert(printer.scale_mode==3 && printer.document_name[0]=='\0');
    assert(printer.vtable_ptr32==btb::printing::kBitmapPrinterVtable32);

    // 00403BB0/00403BC0 sound manager COM pointer lifetime.
    btb::dsutil::RetailCSoundManager32 sm{};
    sm.m_pDS=12;
    construct_sound_manager(sm);
    assert(sm.m_pDS==0);
    events.clear();
    destruct_sound_manager(sm,reinterpret_cast<void*>(0x33),release);
    assert(events.empty());
    sm.m_pDS=12;
    destruct_sound_manager(sm,reinterpret_cast<void*>(0x33),release);
    assert(sm.m_pDS==0 && order({11}));

    // 004043F0 SetVolume: visit all buffers, OR HRESULTs.
    btb::dsutil::RetailCSound32 snd{};
    snd.m_dwNumBuffers=2;
    std::array<SoundVolumeBuffer,2> vbs{
        SoundVolumeBuffer{reinterpret_cast<void*>(0x1),volume_a},
        SoundVolumeBuffer{reinterpret_cast<void*>(0x2),volume_b}
    };
    events.clear();
    assert(sound_set_volume(snd,vbs,-1100)==(0x80004005U|0x100U));
    assert(order({12,13}));
    snd.m_dwNumBuffers=0;events.clear();
    assert(sound_set_volume(snd,vbs,-1100)==0&&events.empty());
}
