#include "btb/original_init2.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace btb::original_init2;
struct SpriteEvent{U32 slot{},va{},handle{};};
struct KeyEvent{U32 slot{},handle{},rgb{};};
struct SquirrelHost final:SquirrelIO{
 U32 rand_next{};int unload{},choices{},stopped{},data_reads{},offset_calls{};
 std::vector<SpriteEvent> loaded{};
 std::vector<KeyEvent> keyed{};
 btb::squirrel::Data load_original_data() override{
  ++data_reads;btb::squirrel::Data d{};
  d.animation_step_delay=7;d.initial_horizontal_alignment_offset=70;
  return d;
 }
 void register_activity_unload() override{++unload;}
 U32 next_original_rand() override{return rand_next++;}
 I32 original_run_x_base() override{return 300;}
 I32 original_run_y_base() override{return 480;}
 I32 original_run_y_offset(I32 profile) override{
  ++offset_calls;assert(profile>=0&&profile<3);return profile*16;
 }
 U32 load_registered_bitmap(U32 dest,U32 va) override{
  assert(dest!=0&&va!=0);
  const U32 handle=100+static_cast<U32>(loaded.size());
  loaded.push_back({dest,va,handle});return handle;
 }
 void key_surface(U32 dest,U32 handle,U32 rgb) override{
  assert(rgb==0x00ff00ffU);keyed.push_back({dest,handle,rgb});
 }
 void generate_original_conveyor_choices() override{++choices;}
 void stop_activity_music() override{++stopped;}
};
struct DinoHost final:DinoIO{
 int unload{},level_reads{},volume_calls{},volume{},dimension_queries{};
 I32 selected_level{-1};
 std::vector<SpriteEvent> loaded{};std::vector<KeyEvent> keyed{};
 btb::dino::LevelData load_original_level(I32 idx) override{
  ++level_reads;selected_level=idx;
  btb::dino::LevelData d;
  d.target_positions={{10,20},{30,40},{50,60}};
  d.start_positions={{100,200},{300,400},{500,600}};
  d.extra_positions={{777,999},{888,1111}};
  d.piece_permutation={2,0,1};
  return d;
 }
 void register_activity_unload() override{++unload;}
 U32 load_registered_bitmap(U32 dest,U32 va) override{
  const U32 handle=500+static_cast<U32>(loaded.size());
  loaded.push_back({dest,va,handle});return handle;
 }
 U32 load_original_background_bitmap(U32 dest,I32 level_index) override{
  assert(dest==0x004fc9e8U&&level_index==5);
  return load_registered_bitmap(dest,0x0043e15cU);
 }
 U32 load_original_piece_bitmap(U32 dest,I32 index) override{
  assert(dest==0x004fc9ecU+static_cast<U32>(4*index));
  return load_registered_bitmap(dest,0x0043f860U);
 }
 std::array<I32,2> query_original_surface_size(U32 handle) override{
  ++dimension_queries;assert(handle>=505);
  return {100,125};
 }
 void key_surface(U32 dest,U32 handle,U32 rgb) override{
  assert(rgb==0x00ff00ffU);keyed.push_back({dest,handle,rgb});
 }
 void apply_global_game_volume(I32 attenuation) override{
  ++volume_calls;volume=attenuation;
 }
};
int main(){
 {
  SquirrelHost io;SquirrelInit s;s.profile=1;
  initialize_squirrel(s,io);
  assert(s.initialized&&s.unload_installed==1&&io.unload==1);
  assert(io.data_reads==1&&io.rand_next==12&&io.offset_calls==1);
  assert(s.random_cells[0]==0&&s.random_cells[1]==1&&s.random_cells[2]==2);
  assert(s.random_cells[4]==s.random_cells[3]&&s.random_cells[8]==s.random_cells[7]);
  assert(s.run_x==230&&s.run_y==480);
  assert(s.current_mode==4&&s.cursor_x==620&&s.cursor_y==480);
  assert(s.sprites.size()==16&&io.loaded.size()==16&&io.keyed.size()==15);
  assert(s.sprites[0].guest_address==0x0051509cU);
  assert(s.sprites[0].resource_string_va==0x00446ca0U);
  assert(!s.sprites[0].keyed);
  assert(s.sprites.back().guest_address==0x005150c8U);
  assert(s.music_start_pending==1&&io.choices==1&&io.stopped==1);
  for(const auto x:s.invalid_slots)assert(x==-1);
  for(const auto x:s.invalid_more)assert(x==-1);
  for(const auto x:s.selection_overlay)assert(x==0);
 }
 {
  SquirrelHost io;SquirrelInit s;s.profile=3;
  bool threw=false;
  try{initialize_squirrel(s,io);}catch(const std::out_of_range&){threw=true;}
  assert(threw&&io.loaded.empty());
 }
 {
  DinoHost io;DinoInit s;
  s.species=1;s.profile=2;s.difficulty=2;s.global_volume=75;
  initialize_dino(s,io);
  assert(s.initialized&&s.level_index==5&&io.selected_level==5);
  assert(s.cursor_tolerance==20&&s.piece_count==3&&s.pieces.size()==3);
  assert(s.pieces[0].source_piece_index==2);
  assert(s.pieces[0].target_x==50&&s.pieces[0].target_y==60);
  assert(s.pieces[0].initial_x==100&&s.pieces[0].initial_y==200);
  assert(s.pieces[1].source_piece_index==0);
  assert(s.pieces[1].target_x==10&&s.pieces[1].target_y==20);
  assert(s.pieces[2].source_piece_index==1);
  assert(s.pieces[2].target_x==30&&s.pieces[2].target_y==40);
  assert(s.pieces[2].width==100&&s.pieces[2].height==125);
  assert(s.first_anchor_x==777&&s.first_anchor_y==999);
  assert(s.current_anchor_x==777&&s.anchor_y==999.f);
  assert(s.music_pending==1&&s.animation_mode==4&&s.movement_alpha==0.5f);
  assert(io.loaded.size()==9&&s.sprites.size()==9);
  assert(io.keyed.size()==8&&io.dimension_queries==3);
  assert(io.unload==1&&io.level_reads==1);
  assert(io.volume_calls==1&&io.volume==-1050);
  assert(s.sprites[5].guest_address==0x004fc9e8U&&!s.sprites[5].keyed);
  assert(s.sprites[6].guest_address==0x004fc9ecU&&s.sprites[6].keyed);
 }
 {
  DinoHost io;DinoInit s;s.species=0;s.profile=2;s.difficulty=2;
  initialize_dino(s,io);
  assert(s.level_index==2&&s.cursor_tolerance==9);
 }
}
