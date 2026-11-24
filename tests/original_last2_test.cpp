#include "btb/original_last2.hpp"
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>
using namespace btb::original_last2;
struct ParkHost:ParkEditorIO {
 int draws{},toolbars{},hits{},palettes{},arrow_draws{},cursor_calls{},
     view_calls{},delete_calls{},place_calls{};
 I32 control{-1},last_mode{-1},last_page{-1},last_offset{},last_voice{};
 bool previous{},next{};
 std::vector<I32> voices{};
 void draw_original_scene(const btb::park_designer::SaveData&) override{++draws;}
 void draw_original_toolbar(I32 m,I32 p) override{++toolbars;last_mode=m;last_page=p;}
 I32 original_toolbar_hit_test(I32,I32) override{++hits;return control;}
 void set_original_cursor(U32 slot) override{assert(slot==0x00507a00U);++cursor_calls;}
 void play_original_editor_voice(I32 id,I32 priority) override{
  assert(priority==0);last_voice=id;voices.push_back(id);
 }
 void process_original_palette_and_placement(ParkEditor& s) override{
  ++place_calls;assert(s.selected_palette>=0&&s.selected_palette<5);
 }
 void process_original_delete(ParkEditor& s) override{
  ++delete_calls;assert(s.delete_requested==1);
 }
 void process_original_view_controls(ParkEditor&) override{++view_calls;}
 void draw_original_palette(I32 mode,I32 page,I32 offset) override{
  ++palettes;assert(mode>=0&&mode<=3);assert(page>=0);last_offset=offset;
 }
 void draw_original_page_arrow(bool prev,bool nxt) override{
  ++arrow_draws;previous=prev;next=nxt;
 }
};
struct BandHost:BandInitIO {
 I32 crt{},data_reads{},unload{},volume{},volume_calls{},slots{},keys{},
     live_registered{},sequence_reads{};
 std::vector<Surface> loaded{};
 void initialize_original_crt() override{++crt;}
 btb::grand_opening::MachineData load_original_machine_data() override{
  ++data_reads;btb::grand_opening::MachineData m;
  for(I32 i=0;i<10;++i){
   m.machine_positions[static_cast<std::size_t>(i)]={i*20, i*30};
   m.machine_sizes[static_cast<std::size_t>(i)]={150,200};
  }
  return m;
 }
 void register_original_band_unload() override{++unload;}
 U32 load_original_band_bitmap(U32 slot,I32 group,I32 index,U32 retail_va) override{
  assert(slot!=0&&group>=0&&group<=6&&index>=0);
  if(group==0)assert(retail_va==0x00442500U);
  const U32 handle=1000U+static_cast<U32>(slots++);
  loaded.push_back({slot,retail_va,handle,false});
  return handle;
 }
 void key_original_bitmap(U32 slot,U32 handle,U32 rgb) override{
  assert(slot!=0&&handle>=1000&&rgb==0x00ff00ffU);++keys;
 }
 std::array<I32,120> load_original_music_sequence(I32 conductor,I32 player) override{
  ++sequence_reads;assert(conductor==2&&player==1);
  std::array<I32,120> seq{};seq.fill(-1);
  seq[0]=2;seq[119]=7;
  return seq;
 }
 I32 original_machine_grid_x() override{return 10;}
 I32 original_machine_grid_y() override{return 20;}
 I32 original_machine_grid_dx() override{return 30;}
 I32 original_machine_grid_dy() override{return 40;}
 U32 original_machine_editor_sprite(I32 i) override{
  assert(i>=0&&i<10);return static_cast<U32>(100+i);
 }
 U32 original_machine_sequence_sprite(I32 i) override{
  assert(i>=0&&i<10);return static_cast<U32>(200+i);
 }
 void register_original_active_machine(BandEditorSlot& x,I32 index) override{
  assert(x.machine_id==index&&x.timer==100&&x.surface!=0);++live_registered;
 }
 void apply_original_global_volume(I32 attenuation) override{++volume_calls;volume=attenuation;}
};
int main(){
 {
  btb::park_designer::SaveData saved{};
  using Index=btb::park_designer::TrailingStateIndex;
  saved.state(Index::ActiveEditorMode)=0;
  saved.state(Index::PondPalettePage)=2;saved.state(Index::PondPaletteMaxPage)=4;
  saved.state(Index::BandstandPalettePage)=0;
  saved.state(Index::BandstandPaletteMaxPage)=2;
  saved.state(Index::PondSelectedPrimary)=-1;
  saved.state(Index::BandstandSelectedRecord)=-1;
  ParkEditor state;state.save=&saved;ParkHost io;
  state.click=1;io.control=5;
  update_park_editor_0040e440(state,io);
  assert(saved.state(Index::PondPalettePage)==1);
  assert(!io.previous&&io.next&&state.scroll_offset==56);
  state.click=0;update_park_editor_0040e440(state,io);
  state.click=1;io.control=6;update_park_editor_0040e440(state,io);
  assert(saved.state(Index::PondPalettePage)==2);
  assert(state.scroll_offset==-56&&io.previous);
  state.click=0;update_park_editor_0040e440(state,io);
  state.click=1;io.control=8;update_park_editor_0040e440(state,io);
  assert(saved.state(Index::ActiveEditorMode)==1);
  assert(io.cursor_calls==1&&state.last_voice==0xc9&&io.last_voice==0xc9);
  state.click=0;update_park_editor_0040e440(state,io);
  state.click=1;io.control=2;update_park_editor_0040e440(state,io);
  assert(io.place_calls==1&&state.selected_palette==2);
  state.click=0;update_park_editor_0040e440(state,io);
  state.click=1;io.control=11;update_park_editor_0040e440(state,io);
  assert(io.delete_calls==1);
  state.click=0;update_park_editor_0040e440(state,io);
  state.click=1;io.control=10;update_park_editor_0040e440(state,io);
  assert(saved.state(Index::ActiveEditorMode)==3);
  state.click=0;update_park_editor_0040e440(state,io);
  assert(io.view_calls>=1&&io.draws==io.toolbars);
  assert(io.palettes==io.draws&&io.arrow_draws==io.draws);
 }
 {
  ParkHost io;ParkEditor state{};bool threw=false;
  try{update_park_editor_0040e440(state,io);}
  catch(const std::invalid_argument&){threw=true;}
  assert(threw);
 }
 {
  BandHost io;BandInit state{};
  state.conductor=2;state.player_profile=1;state.global_volume=80;
  initialize_bobs_band_0041db20(state,io);
  assert(state.initialized&&state.unload_registered&&state.sequence_loaded);
  assert(state.original_grid_total==120);
  assert(io.crt==1&&io.data_reads==1&&io.unload==1&&io.sequence_reads==1);
  assert(state.resources.size()==42&&io.slots==42&&io.keys==33);
  assert(state.resources[0].slot==0x00508c0cU&&state.resources[0].path_va==0x00442500U);
  assert(state.resources[1].slot==0x0051276cU&&!state.resources[1].keyed);
  assert(state.resources[9].slot==0x00512718U&&state.resources[9].keyed);
  assert(state.resources[19].slot==0x005125a8U&&state.resources[19].keyed);
  assert(state.resources[20].slot==0x00513f1cU);
  assert(state.resources[21].slot==0x00513f20U&&state.resources[21].keyed);
  assert(state.resources.back().slot==0x00512744U+36U);
  assert(io.live_registered==10);
  assert(state.editor_slots[0].sprite_x==100&&state.editor_slots[9].sprite_y==209);
  assert(state.grid[0].machine_code==2&&state.grid[0].x==10&&state.grid[0].y==20);
  assert(state.grid[23].x==700&&state.grid[24].y==60);
  assert(state.grid[119].machine_code==7&&state.grid[119].x==700&&state.grid[119].y==180);
  assert(io.volume_calls==1&&io.volume==-840);
  assert(state.active_music_id==2*13+0x1fe&&state.active_game_music_flag==1);
 }
 {
  BandHost io;BandInit state{};state.conductor=4;
  bool threw=false;try{initialize_bobs_band_0041db20(state,io);}
  catch(const std::out_of_range&){threw=true;}
  assert(threw&&io.slots==0);
 }
}
