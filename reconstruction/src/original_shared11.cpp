#include "btb/original_shared11.hpp"
namespace btb::original_shared11 {
void set_cursor_surface(CursorSurfaceState& state,void* surface,
                        GetSurfaceDescription describe) noexcept {
 if(!surface){state.current=state.default_surface;return;}
 state.current=surface;
 SurfaceDescription desc{};
 if(describe)describe(surface,desc);
 state.source={0,0,desc.width,desc.height};
}
void begin_loading_cursor(BeginLoadingState& state,SoundCommand reset,
                          SoundCommand play) noexcept {
 state.source={0,0,100,100};
 state.cursor_surface=state.loading_surface;
 state.loading_latch=1;
 if(state.voice_restart_latch==1){
  if(reset)reset(state.restart_sound);
  if(play)play(state.restart_sound);
 }
}
btb::input::AcquireStateStep direct_input_acquire_step(
 bool primary_present,bool active) noexcept {
 return btb::input::acquire_state_step(primary_present,active);
}
void stop_activity_music(void*& music,SoundCommand stop,
                         DeleteSound destroy) noexcept {
 if(!music)return;
 void* const original=music;
 if(stop)stop(original);
 if(music&&destroy)destroy(music,1);
 music=nullptr;
}
void stop_sound_slot(
 btb::sound::RetailSoundManager32& state,std::size_t slot,void* sound,
 SoundCommand stop,SoundCommand reset) noexcept {
 if(slot>=btb::sound::kManagedSlotCount)return;
 btb::sound::stop_slot_metadata(state,slot);
 if(stop)stop(sound);
 if(reset)reset(sound);
}
void release_sound_slot(
 btb::sound::RetailSoundManager32& state,std::size_t slot,void* sound,
 DeleteSound destroy) noexcept {
 if(slot>=btb::sound::kManagedSlotCount)return;
 if(state.buffer_group_ptr32[slot]&&destroy)destroy(sound,1);
 const auto original_id=state.sound_id_by_slot[slot];
 state.buffer_group_ptr32[slot]=0;
 state.priority[slot]=-1;
 state.slot_state[slot]=0;
 if(state.valid_sound_id(original_id))
  state.slot_by_sound_id[static_cast<std::size_t>(original_id)]=-1;
 state.sound_id_by_slot[slot]=-1;
}
void pause_global_movie(GlobalMovieState& state,MoviePause pause,
                        MovieVolume volume) noexcept {
 if(!state.movie)return;
 if(pause)pause(state.movie,1);
 state.paused=1;
 if(volume)volume(state.movie,0,0);
 state.input_gate=1;
}
void resume_global_movie(GlobalMovieState& state,MoviePause pause,
                         MovieVolumeRestore apply) noexcept {
 if(!state.movie)return;
 if(pause)pause(state.movie,0);
 state.paused=0;
 if(apply)apply();
 state.input_gate=0;
}
void close_global_movie(GlobalMovieState& state,MovieClose close) noexcept {
 if(close)close(state.movie);
 state.movie=nullptr;state.input_gate=1;
}
void destruct_sound_manager(btb::dsutil::RetailCSoundManager32& manager,
                            void* device,ReleaseCom release) noexcept {
 if(manager.m_pDS&&release)release(device);
 if(manager.m_pDS)manager.m_pDS=0;
}
std::uint32_t sound_set_volume(
 const btb::dsutil::RetailCSound32& sound,
 std::span<const SoundVolumeBuffer> buffers,std::int32_t volume) noexcept {
 if(sound.m_dwNumBuffers>buffers.size())
  return static_cast<std::uint32_t>(btb::dsutil::kCoENotInitialized);
 std::uint32_t result=0;
 for(std::size_t i=0;i<sound.m_dwNumBuffers;++i){
  if(!buffers[i].buffer||!buffers[i].set_volume)
   return static_cast<std::uint32_t>(btb::dsutil::kCoENotInitialized);
  result |= buffers[i].set_volume(buffers[i].buffer,volume);
 }
 return result;
}
}
