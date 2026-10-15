#include "btb/original_profiles_fireworks2.hpp"
#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_profiles_fireworks2;
struct ProfilesHost:ProfilesIO{
 std::vector<U32> slots,keyed;std::vector<std::string> file_names;
 bool malformed_info{},missing_font{},missing_progress{},bad_surface{};
 U32 load_registered_surface(U32 slot,std::string_view path) override{
  assert(!path.empty());slots.push_back(slot);
  return bad_surface&&slot==0x0051b3a4U?0U:static_cast<U32>(slots.size()+100);
 }
 void color_key(U32 slot,U32 handle,U32 rgb) override{
  assert(handle>100&&rgb==0xff00ff);keyed.push_back(slot);
 }
 std::optional<std::string> read_game_data(std::string_view path) override{
  file_names.emplace_back(path);
  if(missing_font&&path.find("fontdatablue")!=std::string_view::npos)return std::nullopt;
  if(path.find("ascii")!=std::string_view::npos){
   std::ostringstream out;for(int i=0;i<141;++i)out<<i<<" ";
   return out.str();
  }
  std::ostringstream out;for(int i=0;i<255;++i)out<<i<<" "<<i+1<<" "<<i+2<<" "<<i+3<<" ";
  return out.str();
 }
 std::optional<std::string> read_local_save(std::string_view file) override{
  file_names.emplace_back(file);
  if(file=="playerinfo.txt"){
   if(malformed_info)return "3 0";
   return "2 3 0 -1 0 -1 0 -1 0 -1 17 29 ";
  }
  if(file=="player1.txt"){
   std::ostringstream out;for(int i=0;i<100;++i)out<<i<<" ";
   return out.str();
  }
  if(missing_progress||file!="player2.txt")return std::nullopt;
  std::ostringstream out;for(int i=0;i<100;++i)out<<100+i<<" ";
  return out.str();
 }
 std::string_view button_path(std::size_t index) override{
  assert(index<20);return "Data\\ui\\original-button.bmp";
 }
};
struct FireHost:FireworksEditorIO{
 struct Draw {U32 slot;I32 x,y;bool clipped;};
 std::vector<Draw> draws;std::vector<I32> sound_ids;int backgrounds{},overlays{},placed{};
 void draw_backdrop(U32 slot) override{
  assert(slot==0x0050ab0c);++backgrounds;}
 void blit_keyed(U32 slot,I32 x,I32 y,std::optional<SpriteFrame> clip) override{
  draws.push_back({slot,x,y,clip.has_value()});
  if(clip)assert(clip->width==128&&clip->height==177);
 }
 void draw_overlay(U32 slot,I32 x,I32 y) override{
  assert(slot==0x0050ab08U&&x==37&&y==284);++overlays;
 }
 void place_firework(I32 x,I32 y,I32 type,bool actor) override{
  assert(actor&&x==300&&y==210&&type==4);++placed;
 }
 void play_sound(I32 id,I32 pri,I32 group) override{
  assert(pri==50&&group==2);sound_ids.push_back(id);
 }
 I32 angle_degrees(I32,I32,I32,I32) override{return 90;}
 I32 step_sine(I32 degrees) override{assert(degrees==90);return 6;}
 I32 step_cosine(I32 degrees) override{assert(degrees==90);return 0;}
 I32 distance(I32,I32,I32,I32) override{return 5;}
};
int main(){
 {
  ProfilesHost io;ProfilesState st;
  initialize_player_profiles(st,io);
  assert(st.initialized&&st.selected==-1);
  assert(st.surfaces.size()==27&&io.keyed.size()==27);
  assert(io.slots.front()==0x0051b39cU&&io.slots[6]==0x00519938U);
  assert(io.slots[7]==0x0051b3a8U&&io.slots.back()==0x0051b3f4U);
  assert(st.fonts[0][8][3]==11&&st.fonts[2][254][0]==254);
  assert(st.ascii[140]==140&&st.ascii_count==141);
  assert(st.profiles[0].name_length==2&&st.profiles[0].badge_index==3);
  assert(st.profiles[0].name_glyph_codes[0]==17&&st.profiles[0].name_glyph_codes[1]==29);
  assert(st.profiles[1].badge_index==-1);
  assert(st.progress[0].values[99]==99&&st.progress[1].values[10]==110);
  assert(st.progress[2].values[0]==-1&&st.progress[2].values[99]==0);
  assert(io.file_names.size()==3+1+1+5);
 }
 {
  ProfilesState s{};ProfilesHost h;h.malformed_info=true;
  bool failed=false;
  try{initialize_player_profiles(s,h);}catch(const std::runtime_error&){failed=true;}
  assert(failed&&!s.initialized);
 }
 {
  ProfilesState s{};ProfilesHost h;h.missing_font=true;
  bool failed=false;
  try{initialize_player_profiles(s,h);}catch(const std::runtime_error&){failed=true;}
  assert(failed&&!s.initialized);
 }
 {
  ProfilesState s{};ProfilesHost h;h.bad_surface=true;
  bool failed=false;
  try{initialize_player_profiles(s,h);}catch(const std::runtime_error&){failed=true;}
  assert(failed&&h.slots.size()==2&&h.keyed.size()==1);
 }
 {
  FireHost io;FireworksEditorState s{};
  s.cells.fill(-1);s.cells[0]=2;s.cells[12]=4;s.cells[6]=7;
  for(int i=0;i<18;++i)s.layout[static_cast<std::size_t>(i)]={100+i,200+i,125+i,225+i};
  s.pointer_y=20;
  s.actors[1].phase=10;s.actors[1].pending_type=4;
  s.actors[1].target_x=300;s.actors[1].target_y=210;
  draw_fireworks_editor(s,io);
  assert(io.backgrounds==1&&io.overlays==1&&io.draws.size()==5);
  assert(io.draws[0].slot==0x0050a4b8U+8&&io.draws[0].x==100&&io.draws[0].y==170);
  assert(io.draws[1].slot==0x0050a4b8U+16&&io.draws[1].x==116&&io.draws[1].y==189);
  assert(io.draws[2].clipped&&io.draws[3].clipped);
  assert(io.draws[4].slot==0x0050a4b8U+28&&io.draws[4].y==176);
  assert(s.actors[1].phase==11&&s.animation_counter==1);
  draw_fireworks_editor(s,io);
  assert(io.placed==1&&s.actors[1].phase==0);
  s.actors[0].phase=12;s.actors[0].counter=4;s.actors[0].target_x=300;s.actors[0].target_y=210;
  s.pointer_y=170;
  draw_fireworks_editor(s,io);
  assert(s.actors[0].phase==0);
  assert(s.animation_counter==3);
 }
}
