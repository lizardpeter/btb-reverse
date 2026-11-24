#include "btb/original_fireworks_start2.hpp"
#include <array>
#include <sstream>
#include <stdexcept>
namespace btb::original_fireworks_start2 {
namespace {
constexpr std::array<std::string_view,kMovieCount> kPaths{{
 R"(Data\SubGameFirework\graph\airbombredleft.bik)",
 R"(Data\SubGameFirework\graph\smallgreenleft.bik)",
 R"(Data\SubGameFirework\graph\mediumleftred.bik)",
 R"(Data\SubGameFirework\graph\bigblueleft.bik)",
 R"(Data\SubGameFirework\graph\bigredleft.bik)",
 R"(Data\SubGameFirework\graph\Mediumgreenleft.bik)",
 R"(Data\SubGameFirework\graph\smallblueleft.bik)",
 R"(Data\SubGameFirework\graph\airbombblueleft.bik)",
 R"(Data\SubGameFirework\graph\Candlered.bik)",
 R"(Data\SubGameFirework\graph\Wheelred.bik)",
 R"(Data\SubGameFirework\graph\Wheelgreen.bik)",
 R"(Data\SubGameFirework\graph\Candleblue.bik)",
 R"(Data\SubGameFirework\graph\airbombredright.bik)",
 R"(Data\SubGameFirework\graph\smallgreenright.bik)",
 R"(Data\SubGameFirework\graph\mediumrightred.bik)",
 R"(Data\SubGameFirework\graph\bigblueright.bik)",
 R"(Data\SubGameFirework\graph\bigredright.bik)",
 R"(Data\SubGameFirework\graph\Mediumgreenright.bik)",
 R"(Data\SubGameFirework\graph\smallblueright.bik)",
 R"(Data\SubGameFirework\graph\airbombblueright.bik)",
 R"(Data\SubGameFirework\graph\topmiddle.bik)",
 R"(Data\SubGameFirework\graph\fireworkcrowdloop.bik)",
 R"(Data\SubGameFirework\graph\fireworkcrowdend.bik)",
}};
std::pair<I32,I32> movie_extent(std::size_t i){
 if(i<8)return {0x154,0xdc};
 if(i<12)return {0x8c,0xdc};
 if(i<20)return {0x154,200};
 if(i==20)return {200,0xdc};
 return {0x8c,0x280};
}
U32 bitmap(EditorState& s,EditorIO& io,std::size_t target,U32 slot,std::string_view path,bool keyed){
 if(path.empty())throw std::runtime_error("original Fireworks bitmap name unresolved");
 const U32 handle=io.load_and_register_bitmap(slot,path);
 if(!handle)throw std::runtime_error("original Fireworks bitmap resource failed");
 s.sprites.at(target)={slot,handle,keyed};
 if(keyed)io.set_color_key(slot,handle,0xff00ff);
 return handle;
}
}
std::string_view movie_path(std::size_t index){
 return kPaths.at(index);
}
void load_firework_movie_bank(MovieBank& s,MovieIO& io){
 if(s.movie_count)throw std::logic_error("Fireworks Bink movie bank already loaded");
 s.minimized_bitmap=io.load_bitmap(0x0050a4b4U,
   R"(Data\SubGameFirework\minimised.bmp)");
 s.certificate_bitmap=io.load_bitmap(0x0050a5ccU,
   R"(Data\SubGameFirework\certificate.bmp)");
 if(!s.minimized_bitmap||!s.certificate_bitmap)
  throw std::runtime_error("original Fireworks overlay bitmap unavailable");
 io.color_key(0x0050a4b4U,s.minimized_bitmap,0xff00ff);
 s.crowd_phase=0; // DAT_0050ab6c reset by original 004115C0.
 for(std::size_t i=0;i<kMovieCount;++i){
  auto& slot=s.slots[i];
  // Original reads into a 1,500,000-byte temporary allocation, copies
  // exactly the number of bytes read, then opens Bink from that copy.
  slot.original_bytes=io.open_game_movie_with_cd_fallback(kPaths[i],kMaxOriginalMovieRead);
  if(slot.original_bytes.empty()||slot.original_bytes.size()>kMaxOriginalMovieRead)
   throw std::runtime_error("invalid original Bink movie read length");
  const auto [w,h]=movie_extent(i);
  slot.width=w;slot.height=h;
  slot.bitmap_handle=io.create_video_surface(
      0x0050937cU+static_cast<U32>(i)*4U,w,h,slot.depth,slot.surface_structure_size);
  if(!slot.bitmap_handle)throw std::runtime_error("original Bink DirectDraw surface unavailable");
  slot.movie_handle=io.open_bink_movie_from_memory(i,slot.original_bytes);
  if(!slot.movie_handle)throw std::runtime_error("original Bink movie opening failed");
  s.movie_count=static_cast<I32>(i+1);
 }
}
void initialize_fireworks_activity(EditorState& s,EditorIO& io){
 if(s.selected_player<0||s.selected_player>=5)
  throw std::out_of_range("original Fireworks player outside five profiles");
 io.install_unload_callback();
 io.mark_player_activity_started(static_cast<std::size_t>(s.selected_player));
 s.activity_route=4;
 s.primary_cursor_x=260;s.primary_cursor_y=168;
 s.other_cursor_x=260;s.other_cursor_y=0;
 s.dragging.fill(0);s.hover.fill(0);s.anim_frame.fill(0);
 s.anim_counter.fill(0);s.launch_pending.fill(0);
 s.timed_launch_pending.fill(0);s.frame_status=0;
 io.load_editor_resources();
 s.selection_grid.fill(-1);
 s.saved_grid.fill(-1);
 s.authored.clear();
 const auto data=io.player_save(static_cast<std::size_t>(s.selected_player));
 if(data){
  if(data->size()!=fireworks::kRetailGridSlotCount*4)
   throw std::runtime_error("Fireworks original player save must contain 72 bytes");
  std::string bytes(reinterpret_cast<const char*>(data->data()),data->size());
  std::istringstream file(bytes,std::ios::binary);
  const auto grid=fireworks::read_retail_grid(file);
  if(!s.authored.load_retail_grid(grid))
   throw std::runtime_error("original Fireworks grid has invalid cell values");
  s.saved_grid=grid;
 }
 auto layout_text=io.placement_layout_data();
 auto palette_text=io.palette_layout_data();
 std::istringstream layout(layout_text),palette(palette_text);
 s.layout=fireworks::parse_layout(layout,palette);
 for(std::size_t i=0;i<12;++i)
  bitmap(s,io,i,0x0050a4b8U+static_cast<U32>(i)*4U,io.palette_bitmap_name(i),true);
 for(std::size_t i=0;i<12;++i)
  bitmap(s,io,12+i,0x0050a568U+static_cast<U32>(i)*4U,io.overlay_bitmap_name(i),true);
 bitmap(s,io,24,0x0050ab0cU,R"(Data\SubGameFirework\graph\Bk_01.bmp)",false);
 bitmap(s,io,25,0x0050ab10U,R"(Data\SubGameFirework\graph\BK_01.bmp)",false);
 bitmap(s,io,26,0x00508c0cU,R"(Data\SubGameDYP\delcurs.bmp)",true);
 io.set_global_volume((s.sound_volume-100)*0x2a);
 io.activate_input();
 s.mouse_active=1;
 s.mouse_capture_enabled=true;
}
}
