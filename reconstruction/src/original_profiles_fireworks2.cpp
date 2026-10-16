#include "btb/original_profiles_fireworks2.hpp"
#include <algorithm>
#include <array>
#include <sstream>
#include <stdexcept>
namespace btb::original_profiles_fireworks2 {
namespace {
constexpr std::array<std::pair<U32,std::string_view>,7> kFixedBitmaps{{
 {0x0051b39cU,R"(Data\ui\blue name boxed font.bmp)"},
 {0x0051b3a4U,R"(Data\ui\enter name boxed font.bmp)"},
 {0x0051993cU,R"(Data\ui\certificate boxed font.bmp)"},
 {0x00519958U,R"(Data\ui\tool icons.bmp)"},
 {0x0051b398U,R"(Data\ui\fonttools.bmp)"},
 {0x0051c260U,R"(Data\ui\Hourglass.bmp)"},
 {0x00519938U,R"(Data\SubGameDYP\delcurs.bmp)"}
}};
constexpr std::array<std::string_view,3> kFontFiles{{
 R"(loaddata\fontdatabrown.txt)",R"(loaddata\fontdatablue.txt)",
 R"(loaddata\fontdatacertificate.txt)"
}};
void add_surface(ProfilesState& st,ProfilesIO& io,U32 addr,std::string_view path){
 if(path.empty())throw std::runtime_error("profile surface path missing");
 const U32 handle=io.load_registered_surface(addr,path);
 if(!handle)throw std::runtime_error("profile original DirectDraw bitmap unavailable");
 io.color_key(addr,handle,0xff00ffU);
 st.surfaces.push_back({addr,handle,true});
}
template<std::size_t N>
void read_ints(std::array<I32,N>& output,std::string_view path,ProfilesIO& io){
 const auto text=io.read_game_data(path);
 if(!text)throw std::runtime_error("required original font or ASCII data missing");
 std::istringstream in(*text);
 for(auto& n:output){
  if(!(in>>n)){
   if(in.eof())break;
   throw std::runtime_error("invalid original font / ASCII input");
  }
 }
}
}
void initialize_player_profiles(ProfilesState& st,ProfilesIO& io){
 st=ProfilesState{};
 // Original first seven single bitmap addresses, then 20 sequential button
 // variants at guest addresses 0x51b3a8..0x51b3f4.
 for(const auto& [slot,path]:kFixedBitmaps)add_surface(st,io,slot,path);
 for(std::size_t i=0;i<20;++i)add_surface(
   st,io,0x0051b3a8U+static_cast<U32>(i)*4U,io.button_path(i));
 for(std::size_t family=0;family<3;++family){
  const auto text=io.read_game_data(kFontFiles[family]);
  if(!text)throw std::runtime_error("original font data unavailable");
  std::istringstream in(*text);
  for(auto& glyph:st.fonts[family]){
   if(!(in>>glyph[0]>>glyph[1]>>glyph[2]>>glyph[3])){
    if(in.eof())break;
    throw std::runtime_error("malformed original font glyph row");
   }
  }
 }
 read_ints(st.ascii,R"(loaddata\ascii.txt)",io);
 st.ascii_count=st.ascii.size();
 for(auto& profile:st.profiles){profile.name_length=0;profile.badge_index=-1;}
 if(const auto text=io.read_local_save("playerinfo.txt")){
  std::istringstream in(*text);
  st.profiles=profiles::read_playerinfo(in);
 }
 for(std::size_t i=0;i<profiles::kProfileCount;++i){
  auto& record=st.progress[i];
  record.values.fill(0);
  const auto name="player"+std::to_string(i+1)+".txt";
  if(const auto text=io.read_local_save(name)){
   std::istringstream in(*text);
   record=progress::read_record(in);
  }else record.values[0]=-1;
 }
 st.selected=-1;st.initialized=1;
}
void draw_fireworks_editor(FireworksEditorState& st,FireworksEditorIO& io){
 io.draw_backdrop(0x0050ab0cU);
 // The source loops row 0 and row 2 first; the six middle-row cells are
 // blitted after the two flying actors and their control overlays.
 for(int row:{0,2})
  for(int col=0;col<6;++col){
   const int index=row*6+col,type=st.cells[static_cast<std::size_t>(index)];
   if(type>=0&&type<12){
    const auto& r=st.layout[static_cast<std::size_t>(index)];
    io.blit_keyed(0x0050a4b8U+static_cast<U32>(type)*4U,
           r.left+(row==2?4:0),r.top-30+(row==2?7:0),std::nullopt);
   }
  }
 const int threshold=(st.actors[0].phase==1||st.actors[1].phase==1)?35:160;
 const int control=st.pointer_y<=threshold?1:0;
 auto& actor=st.actors[static_cast<std::size_t>(control)];
 actor.counter=(actor.counter+1)%6;
 if(!actor.counter)actor.frame=(actor.frame+1)%9;
 // The original EDI/x87 output from this branch is not wholly
 // recoverable. Geometry math stays behind the original provider.
 const int cursor_x=std::clamp(st.pointer_x,61,575);
 if(actor.x<cursor_x-64){++actor.x;actor.direction=2;}
 else if(actor.x>cursor_x-64){--actor.x;actor.direction=6;}
 if(actor.phase==10)actor.phase=11;
 else if(actor.phase==11){
  io.place_firework(actor.target_x,actor.target_y,actor.pending_type,true);
  actor.phase=0;
  const auto count=static_cast<int>(std::count_if(st.cells.begin(),st.cells.end(),
                                      [](I32 n){return n>=0;}));
  if(count>17)io.play_sound(control?0x381:0x35c,50,2);
 }else if(actor.phase==12){
  if(++actor.counter>=6){
   actor.counter=0;
   actor.frame=std::max(13,actor.frame+1);
   if(actor.frame>24)actor.frame=13;
   const auto degrees=io.angle_degrees(actor.x,actor.y,actor.target_x,actor.target_y);
   actor.direction=((degrees+22)/45)%8;
   if(actor.direction<0)actor.direction+=8;
   actor.x+=io.step_sine(degrees);
   actor.y+=io.step_cosine(degrees);
   if(io.distance(actor.x,actor.y,actor.target_x,actor.target_y)<16){
    actor.phase=0;actor.direction=4;
   }
  }
 }
 for(int i=1;i>=0;--i){
  const auto& a=st.actors[static_cast<std::size_t>(i)];
  io.blit_keyed(0x0050a494U+static_cast<U32>(i)*4U,a.x,a.y,
                SpriteFrame{0,0,128,177});
 }
 io.draw_overlay(0x0050ab08U,37,284);
 for(int col=0;col<6;++col){
  const int index=6+col,type=st.cells[static_cast<std::size_t>(index)];
  if(type<0||type>=12)continue;
  const auto& r=st.layout[static_cast<std::size_t>(index)];
  io.blit_keyed(0x0050a4b8U+static_cast<U32>(type)*4U,r.left,r.top-30,std::nullopt);
 }
 ++st.animation_counter;
}
}
