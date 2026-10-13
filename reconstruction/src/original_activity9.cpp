#include "btb/original_activity9.hpp"
#include <algorithm>
#include <bit>
namespace btb::original_activity9 {
U32 reset_wave_file(
 btb::dsutil::RetailCWaveFile32& wave,void* mmio,MmioSeek seek,
 MmioTask descend,MmioTask create,MmioTask get_info) noexcept {
 if(wave.m_bIsReadingFromMemory!=0){
  wave.m_pbDataCur=wave.m_pbData;return 0;
 }
 if(!wave.m_hmmio)return 0x800401F0U;
 wave.m_ck.ckid=0x61746164U;
 if(wave.m_dwFlags==1){
  if(!seek||!seek(mmio,static_cast<I32>(
      wave.m_ckRiff.dwDataOffset+4U)))return 0x80004005U;
  if(!descend||!descend(mmio))return 0x80004005U;
  return 0;
 }
 wave.m_ck.cksize=0;
 if(!create||!create(mmio))return 0x80004005U;
 if(!get_info||!get_info(mmio))return 0x80004005U;
 return 0;
}
U32 restore_sound_buffer(
 void* buffer,bool* was_restored,GetStatus status_query,Restore restore,
 SleepMs sleep,std::size_t max_attempts) noexcept {
 if(!buffer)return 0x800401F0U;
 if(was_restored)*was_restored=false;
 if(!status_query)return 0x80004003U;
 U32 status=0;
 const U32 result=status_query(buffer,&status);
 if(std::bit_cast<I32>(result)<0)return result;
 if((status&2U)==0)return 1;
 if(!restore)return 0x80004003U;
 std::size_t attempts=0;
 for(;;){
  const U32 first=restore(buffer);
  if(first==static_cast<U32>(btb::dsutil::kDsErrBufferLost) && sleep)
    sleep(10);
  // Original 00404270 immediately performs a second Restore.
  const U32 second=restore(buffer);
  if(second==0){
   if(was_restored)*was_restored=true;
   return 0;
  }
  if(max_attempts && ++attempts>=max_attempts)return second;
 }
}
U32 sound_play(
 void* sound,bool has_buffers,U32 priority,U32 flags,I32 game_volume,
 ChooseSoundBuffer choose,RestoreSelected restore,FillSound fill,
 ResetSound reset,PlaySound play,SoundVolume volume) noexcept {
 if(!has_buffers)return 0x800401F0U;
 void* buffer=choose?choose(sound):nullptr;
 if(!buffer)return 0x80004005U;
 bool restored=false;
 const U32 hr=restore?restore(buffer,&restored):0x80004003U;
 if(std::bit_cast<I32>(hr)<0)return hr;
 if(restored){
  const U32 filled=fill?fill(sound,buffer):0x80004003U;
  if(std::bit_cast<I32>(filled)<0)return filled;
  if(reset)reset(sound);
 }
 const U32 played=play?play(buffer,0,priority,flags):0x80004003U;
 if(volume)volume(buffer,game_volume);
 return played;
}
I32 hit_test_bobs_band_regions(
 BandHitState& state,std::span<const BandRegion> regions) noexcept {
 const I32 offset=state.mode==1?10:5;
 const I32 px=state.pointer_x+offset,py=state.pointer_y+offset;
 for(std::size_t i=0;i<regions.size();++i){
  const auto& r=regions[i];
  if(r.result!=-1&&r.rect.left<px&&px<r.rect.right&&
     r.rect.top<py&&py<r.rect.bottom){
   state.selected_index=static_cast<I32>(i);
   state.pointer_x=px-offset;state.pointer_y=py-offset;
   return r.result;
  }
 }
 state.pointer_x=px-offset;state.pointer_y=py-offset;
 return -1;
}
void debounce_maze_directions(
 MazeDebounce& samples,I32& horizontal,I32& vertical) noexcept {
 const auto push=[](std::array<I32,3>& history,I32 value){
  history[0]=history[1];history[1]=history[2];history[2]=value;
 };
 push(samples.first,horizontal);
 push(samples.second,vertical);
 if(samples.first[0]!=samples.first[1]||
    samples.first[1]!=samples.first[2]||
    samples.second[0]!=samples.second[1]||
    samples.second[1]!=samples.second[2]){
  horizontal=0;vertical=0;
 }
}
void update_squirrel_conveyor(
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
 }else if(s.phase==2){
  s.position+=step;
  if(s.position>=0){s.position=0;s.phase=0;}
 }
}
void translate_keyboard_key(U32 scancode,std::uint16_t* output,
 GetKeyboardState get_state,MapVirtualKey map,ToAscii to_ascii) noexcept {
 std::array<std::uint8_t,256> keys{};
 if(!get_state || !get_state(keys))return;
 const U32 virtual_key=map?map(scancode,1):0;
 if(to_ascii)to_ascii(virtual_key,scancode,keys,output);
}
bool seek_decode_bink_exact(
 void* movie,void* surface,I32 target,Keyframe keyframe,GotoFrame go,
 MovieFrame frame,DecodeFrame decode,CopyFrame copy,
 DecodeFrame next,std::size_t max_frames) noexcept {
 if(!movie||!keyframe||!go||!frame||!decode||!copy||!next)
  return false;
 go(movie,keyframe(movie,target));
 std::size_t count=0;
 while(frame(movie)!=target){
  if(count++>=max_frames)return false;
  decode(movie);copy(movie,surface);next(movie);
 }
 return true;
}
void draw_park_pond_segments(
 I32 count,I32 stride,I32 height,I32 x,I32 y,SegmentBlit blit) noexcept {
 if(!blit)return;
 for(I32 i=1;i<=count;++i){
  const Rect source{i*stride,0,std::min((i+1)*stride,2053),height};
  blit(x,y,source);
 }
}
} // namespace btb::original_activity9
