#include "btb/original_park_init.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <vector>
using namespace btb::original_park_init;
struct LoadEvent {U32 slot{},symbol{},offset{},handle{};int kind{},group{},ordinal{};};
struct KeyEvent {U32 slot{},handle{},color{};};
struct Host final:IO {
 I32 registered{},restored{},stopped{},palette_rects{};
 bool saved_active{},missing_asset{};
 std::vector<LoadEvent> calls{};
 std::vector<KeyEvent> keys{};
 void register_original_unload() override{++registered;}
 U32 load_original_bitmap(U32 slot,U32 va) override{
  assert(slot!=0&&va!=0);
  const U32 handle=1000U+static_cast<U32>(calls.size());
  calls.push_back({slot,va,0,handle,0,0,0});
  return missing_asset?0:handle;
 }
 U32 load_indexed_original_bitmap(U32 slot,U32 va,U32 offset) override{
  assert((va==0x00441344U||va==0x004415c4U||va==0x00440bc4U));
  assert(offset%0x80U==0);
  const U32 handle=1000U+static_cast<U32>(calls.size());
  calls.push_back({slot,va,offset,handle,1,0,0});
  return handle;
 }
 U32 load_original_toolbar_bitmap(U32 slot,I32 group,I32 ordinal) override{
  assert(group>=0&&group<3&&ordinal>=0);
  const U32 handle=1000U+static_cast<U32>(calls.size());
  calls.push_back({slot,0,0,handle,2,group,ordinal});
  return handle;
 }
 void set_magenta_key(U32 slot,U32 handle,U32 rgb) override{
  assert(slot!=0&&handle>=1000&&rgb==0xff00ffU);
  keys.push_back({slot,handle,rgb});
 }
 void load_original_park_data(btb::park_designer::SaveData& save,
                              btb::park_designer::BoundAreas& bounds) override{
  ++restored;
  assert(save.state(btb::park_designer::TrailingStateIndex::NextDecorateRecord)==300);
  assert(save.state(btb::park_designer::TrailingStateIndex::DecoratePaletteMaxPage)==3);
  assert(bounds[0][0][0].vertices.empty());
  if(saved_active){
   using btb::park_designer::TrailingStateIndex;
   save.state(TrailingStateIndex::NextBandstandRecord)=203;
   save.state(TrailingStateIndex::Season)=1;
   save.state(TrailingStateIndex::SelectedObjectRecord)=42;
  }
 }
 U32 original_palette_surface(I32 mode,I32 cat,I32 variant,
                               const std::vector<Surface>& originals) override{
  assert(mode>=0&&mode<3&&cat>=0&&cat<4&&variant>=0&&variant<5);
  const U32 slot=mode==0?(cat==0?0x0050926cU:0x00509258U):
                 mode==1?(cat==0?0x00509288U:0x0050928cU):
                          (cat==0?0x00509278U:0x0050927cU);
  for(const auto& a:originals)if(a.guest_slot==slot)return a.native_handle;
  assert(false);return 0;
 }
  std::array<I32,4> original_palette_source_rect(I32 mode,I32 cat,I32 variant,U32 sprite) override{
  assert(mode>=0&&mode<3&&cat>=0&&cat<4&&variant>=0&&variant<5);
  assert(sprite>=1000);
  ++palette_rects;
  return {variant*16,cat*32,variant*16+16,cat*32+32};
 }
 void stop_all_managed_sounds() override{++stopped;}
};
int main(){
 {
  Host io;State s;
  initialize_park_designer(s,io);
  assert(s.initialized&&s.register_unload&&s.sound_stopped&&s.source_data_loaded);
  assert(io.registered==1&&io.restored==1&&io.stopped==1);
  assert(io.calls.size()==82&&s.surfaces.size()==82);
  assert(io.palette_rects==60&&s.palette.size()==60);
  assert(io.keys.size()==78);
  assert(s.surfaces[0].guest_slot==0x00507a00U);
  assert(s.surfaces[0].source_string_va==0x004425fcU);
  assert(s.surfaces[0].keyed);
  assert(s.surfaces[2].guest_slot==0x00509250U&&!s.surfaces[2].keyed);
  assert(io.calls[38].slot==0x00507b3cU&&io.calls[38].offset==0);
  assert(io.calls[39].slot==0x004fca94U&&io.calls[39].offset==0);
  assert(io.calls[46].slot==0x00507b4cU&&io.calls[46].offset==0x200U);
  assert(io.calls[57].slot==0x00507a14U&&io.calls[57].kind==2);
  assert(io.calls[60].slot==0x00504190U&&io.calls[60].group==1);
  assert(io.calls[70].slot==0x00508ad4U&&io.calls[70].group==2);
  const auto first=s.palette[0];
  const auto last=s.palette[59];
  assert(first.mode==0&&first.category==0&&first.variant==0);
  assert(first.left==0&&first.top==0&&first.right==34&&first.bottom==61);
  assert(last.mode==2&&last.category==3&&last.variant==4);
  assert(last.left==136&&last.top==183&&last.right==170&&last.bottom==244);
  assert(last.source_left==64&&last.source_top==96);
  assert(s.save.state(btb::park_designer::TrailingStateIndex::NextBandstandRecord)==200);
  assert(s.save.objects[0].object_code==-1&&s.save.objects[399].left==-1);
  assert(s.scene_music_flag==1&&s.game_mode==4);
 }
 {
  Host io;State s;io.saved_active=true;
  initialize_park_designer(s,io);
  assert(s.scene_music_flag==0);
  assert(s.selected_object==42&&s.current_season==1);
 }
 {
  Host io;State s;io.missing_asset=true;
  bool threw=false;
  try{initialize_park_designer(s,io);}catch(const std::runtime_error&){threw=true;}
  assert(threw&&!s.initialized&&io.restored==0);
 }
}
