#include "btb/original_fireworks_start2.hpp"
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_fireworks_start2;
struct MovieMock:MovieIO{
 struct Surface {U32 at{};I32 w{},h{},depth{},size{};};
 std::vector<std::string> names,events;
 std::vector<Surface> surfaces;
 std::vector<std::size_t> decoded;
 std::size_t requested_max{},bad_index{kMovieCount};
 int loaded_bitmaps{},keyed{};
 U32 load_bitmap(U32 addr,std::string_view path) override {
  assert(addr==0x0050a4b4U||addr==0x0050a5ccU);
  assert(path.ends_with(".bmp"));++loaded_bitmaps;
  events.push_back("bitmap");return 100+loaded_bitmaps;
 }
 void color_key(U32 slot,U32 handle,U32 rgb) override {
  assert(slot==0x0050a4b4U&&handle==101&&rgb==0xff00ff);++keyed;
 }
 std::vector<std::uint8_t> open_game_movie_with_cd_fallback(std::string_view path,std::size_t limit) override{
  assert(limit==1'500'000);
  assert(path==movie_path(names.size()));
  requested_max=limit;
  names.emplace_back(path);events.push_back("read");
  if(names.size()-1==bad_index)return {};
  return {0x42,0x49,0x4e,0x4b,static_cast<std::uint8_t>(names.size()-1)};
 }
 U32 create_video_surface(U32 at,I32 w,I32 h,I32 depth,I32 size) override {
  assert(depth==7&&size==124);
  surfaces.push_back({at,w,h,depth,size});
  events.push_back("surface");return 1000+static_cast<U32>(surfaces.size());
 }
 U32 open_bink_movie_from_memory(std::size_t i,const std::vector<std::uint8_t>& bytes) override{
  assert(bytes.size()==5&&bytes[0]==0x42&&bytes[4]==i);
  decoded.push_back(i);events.push_back("open");return 2000+static_cast<U32>(i);
 }
};
struct EditorMock:EditorIO{
 int installs{},marks{},editor_resources{},enabled{},keyed{},volume{},bitmap_count{};
 std::size_t player{};
 bool corrupt{},no_save{};
 std::vector<U32> slots;
 void install_unload_callback() override {++installs;}
 void mark_player_activity_started(std::size_t p) override {++marks;player=p;}
 void load_editor_resources() override {++editor_resources;}
 std::optional<std::vector<std::uint8_t>> player_save(std::size_t p) override {
  assert(p==player);
  if(no_save)return std::nullopt;
  // Original 18 little-endian int32s; slot zero is type 2.
  std::vector<std::uint8_t> bytes(72,0xff);
  bytes[0]=2;bytes[1]=bytes[2]=bytes[3]=0;
  if(corrupt)bytes.pop_back();
  return bytes;
 }
 std::string placement_layout_data() override {return rectangles(18);}
 std::string palette_layout_data() override {return rectangles(12);}
 static std::string rectangles(int count){
  std::ostringstream s;
  for(int i=0;i<count;++i)
   s<<20+i*2<<" "<<30+i*2<<" "<<0<<" "<<0<<" "
    <<60+i*2<<" "<<90+i*2<<" "<<0<<" "<<0<<"\n";
  return s.str();
 }
 std::string_view palette_bitmap_name(std::size_t i) override{
  static constexpr const char* names[12]={"a.bmp","b.bmp","c.bmp","d.bmp",
   "e.bmp","f.bmp","g.bmp","h.bmp","i.bmp","j.bmp","k.bmp","l.bmp"};
  assert(i<12);return names[i];
 }
 std::string_view overlay_bitmap_name(std::size_t i) override{
  assert(i<12);return "retail-overlay.bmp";
 }
 U32 load_and_register_bitmap(U32 at,std::string_view path) override{
  assert(!path.empty());slots.push_back(at);return 300+static_cast<U32>(++bitmap_count);
 }
 void set_color_key(U32 slot,U32 bmp,U32 rgb) override {
  assert(rgb==0xff00ff&&bmp>300);
  assert(std::find(slots.begin(),slots.end(),slot)!=slots.end());
  ++keyed;
 }
 void set_global_volume(I32 v) override{volume=v;}
 void activate_input() override{++enabled;}
};
int main(){
 {
  MovieBank state{};MovieMock io;
  load_firework_movie_bank(state,io);
  assert(state.movie_count==23&&state.crowd_phase==0);
  assert(io.loaded_bitmaps==2&&io.keyed==1);
  assert(io.names.size()==23&&io.surfaces.size()==23&&io.decoded.size()==23);
  assert(movie_path(20).ends_with("topmiddle.bik"));
  assert(movie_path(21).ends_with("fireworkcrowdloop.bik"));
  assert(movie_path(22).ends_with("fireworkcrowdend.bik"));
  assert(state.slots[0].width==340&&state.slots[0].height==220);
  assert(state.slots[8].width==140&&state.slots[8].height==220);
  assert(state.slots[12].width==340&&state.slots[12].height==200);
  assert(state.slots[20].width==200&&state.slots[20].height==220);
  assert(state.slots[21].width==140&&state.slots[21].height==640);
  assert(state.slots[22].width==140&&state.slots[22].height==640);
  for(std::size_t i=0;i<23;++i){
   assert(io.surfaces[i].at==0x0050937cU+i*4);
   assert(state.slots[i].bitmap_handle==1001+i);
   assert(state.slots[i].movie_handle==2000+i);
   assert(state.slots[i].original_bytes.size()==5);
   assert(io.events[i*3+2]=="read"||io.events[i*3+2]=="surface"||io.events[i*3+2]=="open");
  }
  bool repeated=false;
  try{load_firework_movie_bank(state,io);}catch(const std::logic_error&){repeated=true;}
  assert(repeated);
  MovieBank fail{};MovieMock missing;missing.bad_index=5;
  bool missing_fails=false;
  try{load_firework_movie_bank(fail,missing);}
  catch(const std::runtime_error&){missing_fails=true;}
  assert(missing_fails&&fail.movie_count==5&&missing.decoded.size()==5);
 }
 {
  EditorState s{};s.selected_player=2;s.sound_volume=80;
  EditorMock io;
  initialize_fireworks_activity(s,io);
  assert(io.installs==1&&io.marks==1&&io.player==2);
  assert(io.editor_resources==1&&io.enabled==1);
  assert(io.volume==(80-100)*42);
  assert(io.bitmap_count==27&&io.keyed==25);
  assert(s.primary_cursor_x==260&&s.primary_cursor_y==168);
  assert(s.activity_route==4&&s.mouse_active==1&&s.mouse_capture_enabled);
  assert(s.authored.at(0,0)==btb::fireworks::FireworkType::MediumRed);
  assert(s.authored.occupied_count()==1&&s.saved_grid[0]==2);
  assert(s.saved_grid[17]==-1&&s.selection_grid[119]==-1);
  assert(s.layout.placement_runtime[0].left==30);
  assert(s.layout.placement_runtime[0].top==10);
  assert(s.sprites[0].original_slot==0x0050a4b8U);
  assert(s.sprites[12].original_slot==0x0050a568U);
  assert(s.sprites[24].original_slot==0x0050ab0cU);
  assert(s.sprites[25].original_slot==0x0050ab10U);
  assert(s.sprites[26].original_slot==0x00508c0cU);
  EditorState new_user{};new_user.selected_player=0;
  EditorMock empty;empty.no_save=true;
  initialize_fireworks_activity(new_user,empty);
  assert(new_user.authored.occupied_count()==0&&new_user.saved_grid[0]==-1);
  EditorState invalid{};invalid.selected_player=7;EditorMock skipped;
  bool invalid_index=false;
  try{initialize_fireworks_activity(invalid,skipped);}
  catch(const std::out_of_range&){invalid_index=true;}
  assert(invalid_index&&skipped.installs==0);
  EditorState corrupt{};corrupt.selected_player=3;EditorMock bad;bad.corrupt=true;
  bool rejected=false;
  try{initialize_fireworks_activity(corrupt,bad);}
  catch(const std::runtime_error&){rejected=true;}
  assert(rejected&&bad.bitmap_count==0&&bad.enabled==0);
 }
}
