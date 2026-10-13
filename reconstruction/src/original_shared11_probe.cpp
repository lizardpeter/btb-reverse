#include "btb/original_shared11.hpp"
#include <array>
namespace btb::original_shared11 {
namespace {
std::array<int,48> events{};std::size_t used{};
void event(int n){if(used<events.size())events[used++]=n;}
bool sequence(std::initializer_list<int> values){
 if(used!=values.size())return false;
 std::size_t i=0;
 for(int value:values)if(events[i++]!=value)return false;
 return true;
}
void clear(){used=0;}
void desc(void*,SurfaceDescription& d) noexcept {
 event(1);d.width=53;d.height=71;
}
void stop(void*) noexcept {event(2);}
void reset(void*) noexcept {event(3);}
void play(void*) noexcept {event(4);}
void destroy(void*,std::int32_t flag) noexcept {if(flag==1)event(5);}
void pause(void*,std::int32_t mode) noexcept {event(mode==1?6:7);}
void zero_volume(void*,std::int32_t a,std::int32_t b) noexcept {
 if(a==0&&b==0)event(8);
}
void restore_volume() noexcept {event(9);}
void close(void*) noexcept {event(10);}
void release(void*) noexcept {event(11);}
std::uint32_t volume_a(void*,std::int32_t v) noexcept {
 if(v==-1100)event(12);return 0x80004005U;
}
std::uint32_t volume_b(void*,std::int32_t v) noexcept {
 if(v==-1100)event(13);return 0x100U;
}
}
bool verify_11_shared_native_functions() noexcept {
 clear();
 CursorSurfaceState cursor{};
 cursor.default_surface=reinterpret_cast<void*>(0x1);
 cursor.source={5,6,7,8};
 original_shared11::set_cursor_surface(cursor,nullptr,desc);
 if(cursor.current!=cursor.default_surface||
    cursor.source.left!=5||used!=0)return false;
 original_shared11::set_cursor_surface(
     cursor,reinterpret_cast<void*>(0x2),desc);
 if(cursor.source.right!=53||cursor.source.bottom!=71||
    !sequence({1}))return false;
 clear();
 BeginLoadingState load{};
 load.loading_surface=reinterpret_cast<void*>(0x44);
 load.restart_sound=reinterpret_cast<void*>(0x55);
 load.voice_restart_latch=1;
 original_shared11::begin_loading_cursor(load,reset,play);
 if(load.cursor_surface!=load.loading_surface||
    load.source.right!=100||load.loading_latch!=1||
    !sequence({3,4}))return false;
 // DirectInput: absent->return 1; active->acquire, inactive->unacquire.
 const auto missing=original_shared11::direct_input_acquire_step(false,true);
 const auto active=original_shared11::direct_input_acquire_step(true,true);
 const auto inactive=original_shared11::direct_input_acquire_step(true,false);
 if(missing.return_value!=1||!active.acquire_primary||
    !active.acquire_secondary||inactive.return_value!=0||
    !inactive.unacquire_primary||!inactive.unacquire_secondary)
    return false;
 clear();
 void* music=reinterpret_cast<void*>(0x123);
 original_shared11::stop_activity_music(music,stop,destroy);
 if(music||!sequence({2,5}))return false;
 clear();original_shared11::stop_activity_music(music,stop,destroy);
 if(used)return false;
 btb::sound::RetailSoundManager32 manager{};
 btb::sound::initialize_retail_metadata(manager);
 manager.buffer_group_ptr32[4]=0x321;
 manager.sound_id_by_slot[4]=510;
 manager.slot_by_sound_id[510]=4;
 manager.priority[4]=53;manager.slot_state[4]=2;
 original_shared11::stop_sound_slot(
     manager,4,reinterpret_cast<void*>(0x321),stop,reset);
 if(manager.slot_state[4]!=3||!sequence({2,3}))return false;
 clear();
 original_shared11::release_sound_slot(
     manager,4,reinterpret_cast<void*>(0x321),destroy);
 if(!sequence({5})||manager.buffer_group_ptr32[4]!=0||
    manager.priority[4]!=-1||manager.slot_state[4]!=0||
    manager.sound_id_by_slot[4]!=-1||
    manager.slot_by_sound_id[510]!=-1)return false;
 clear();
 GlobalMovieState movie{};
 original_shared11::pause_global_movie(movie,pause,zero_volume);
 if(used)return false;
 movie.movie=reinterpret_cast<void*>(0xABC);
 original_shared11::pause_global_movie(movie,pause,zero_volume);
 if(movie.paused!=1||movie.input_gate!=1||!sequence({6,8}))
    return false;
 clear();original_shared11::resume_global_movie(
     movie,pause,restore_volume);
 if(movie.paused!=0||movie.input_gate!=0||!sequence({7,9}))
    return false;
 clear();original_shared11::close_global_movie(movie,close);
 if(movie.movie||movie.input_gate!=1||!sequence({10}))return false;
 clear();original_shared11::close_global_movie(movie,close);
 if(!sequence({10}))return false;
 clear();
 btb::dsutil::RetailCSoundManager32 ds{};
 ds.m_pDS=1;
 original_shared11::destruct_sound_manager(
     ds,reinterpret_cast<void*>(0xABC),release);
 if(ds.m_pDS!=0||!sequence({11}))return false;
 clear();
 btb::dsutil::RetailCSound32 sound{};
 sound.m_dwNumBuffers=2;
 const std::array<SoundVolumeBuffer,2> buffers{{
     {reinterpret_cast<void*>(0x10),volume_a},
     {reinterpret_cast<void*>(0x11),volume_b}
 }};
 const auto result=original_shared11::sound_set_volume(
     sound,buffers,-1100);
 if(result!=(0x80004005U|0x100U)||!sequence({12,13}))
    return false;
 sound.m_dwNumBuffers=3;
 if(original_shared11::sound_set_volume(sound,buffers,-1100)!=
    static_cast<std::uint32_t>(btb::dsutil::kCoENotInitialized))
    return false;
 return true;
}
}
