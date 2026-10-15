#include "btb/original_closure8.hpp"
#include <algorithm>
#include <bit>
#include <limits>
#include <sstream>
#include <stdexcept>
namespace btb::original_closure8 {
namespace {
void release_slot(SurfaceState& s,SurfaceIO& io,U32 at){
 io.unregister(at);
 const U32 surface=s.slot(at);
 if(surface){io.release(surface);s.slot(at)=0;}
}
constexpr std::array<U32,6> profile_fixed{
 0x51b39c,0x51993c,0x51b3a4,0x519958,0x51b398,0x519938};
constexpr std::array<U32,23> park_fixed{
 0x509250,0x509254,0x509258,0x509264,0x509268,0x50925c,
 0x509260,0x50926c,0x509270,0x509274,0x5092b4,0x5092b0,
 0x508ad0,0x5092b8,0x5092bc,0x5092c0,0x5092c4,0x5092c8,
 0x5092cc,0x5092d0,0x5092d4,0x5092d8,0x5092dc};
constexpr std::array<U32,14> park_tail{
 0x507a00,0x4fca90,0x4fcaa8,0x507b54,0x508c0c,
 0x509278,0x50927c,0x509280,0x509284,0x509288,
 0x50928c,0x509290,0x509294,0x509298};
U32 mask_shift(U32 mask){
 if(!mask)throw std::invalid_argument("zero source pixel channel mask");
 return std::countr_zero(mask);
}
std::uint8_t unpack(U32 pixel,U32 mask){
 const U32 shift=mask_shift(mask),count=std::popcount(mask);
 if(count>8)throw std::invalid_argument("pixel mask wider than 8 bits");
 return static_cast<std::uint8_t>(((pixel>>shift)<<((8-count)&31U))&255U);
}
void put16(std::vector<std::uint8_t>& out,std::size_t at,U32 value){
 out.at(at)=static_cast<std::uint8_t>(value);
 out.at(at+1)=static_cast<std::uint8_t>(value>>8);
}
void put32(std::vector<std::uint8_t>& out,std::size_t at,U32 value){
 for(unsigned i=0;i<4;++i)out.at(at+i)=static_cast<std::uint8_t>(value>>(8*i));
}
}
void save_and_unload_profiles(ProfileState& s,ProfileIO& io){
 // Original five fixed globals, then 20 button slots, then cursor bitmap.
 for(std::size_t i=0;i<5;++i)release_slot(s.surfaces,io,profile_fixed[i]);
 for(U32 at=0x51b3a8;at<0x51b3f8;at+=4)release_slot(s.surfaces,io,at);
 release_slot(s.surfaces,io,profile_fixed[5]);
 // Preserve the source order: metadata first, then player1..player5.
 std::ostringstream out;btb::profiles::write_playerinfo(out,s.metadata);
 (void)io.write_file("playerinfo.txt",out.str());
 for(std::size_t p=0;p<s.records.size();++p){
  std::ostringstream record;btb::progress::write_record(record,s.records[p]);
  const std::string filename="player"+std::to_string(p+1)+".txt";
  (void)io.write_file(filename,record.str());
 }
}
void unload_park(ParkState& s,ParkIO& io){
 io.save_park();
 s.activity_active=0;
 for(U32& object:s.active_objects){
  if(object){io.destroy_park_object(object,1);object=0;}
 }
 s.object_table.fill(0);
 s.scene_active=0;
 for(const U32 at:park_fixed)release_slot(s.surfaces,io,at);
 for(U32 at=0x507a14;at<0x507a20;at+=4)release_slot(s.surfaces,io,at);
 for(U32 at=0x504190;at<0x5041b8;at+=4)release_slot(s.surfaces,io,at);
 for(U32 at=0x507b18;at<0x507b3c;at+=4)release_slot(s.surfaces,io,at);
 release_slot(s.surfaces,io,park_tail[0]);
 for(U32 i=0;i<5;++i){
  release_slot(s.surfaces,io,0x507b3c+i*4);
  release_slot(s.surfaces,io,0x4fca94+i*4);
 }
 for(std::size_t i=1;i<park_tail.size();++i)
  release_slot(s.surfaces,io,park_tail[i]);
 for(U32 at=0x508ad4;at<0x508b04;at+=4)release_slot(s.surfaces,io,at);
 io.stop_music();
}
void load_quit_confirmation(QuitData& state,std::istream& input,
                           std::string_view sentinel){
 // Original fscanf has 8 decimal fields and 3 strings. Missing input is
 // unsafe in retail (null FILE*); source port rejects it explicitly.
 for(auto& number:state.metadata)
  if(!(input>>number))throw std::runtime_error("short quitsure numeric header");
 for(auto& label:state.names)
  if(!(input>>label))throw std::runtime_error("short quitsure text header");
 // Ghidra resolves four separately allocated 0x300-offset string columns;
 // each of the six 0x80-byte rows compares one field to the same sentinel.
 for(auto& row:state.labels)
  for(auto& field:row)
   if(field==sentinel)field.clear();
}
void update_progress(ProgressState& s,const btb::progress::Record& p,ProgressIO& io){
 if(s.intro_voice_pending==1&&!io.any_sound_playing()){
  s.intro_voice_pending=0;
  io.play_sound(0x3ba,0x32,2);
  io.mark_last_sound_interruptible();
 }
 io.draw_background();
 io.update_help_button();io.update_help_hover();
 for(std::size_t slot=50;slot<=64;++slot){
  if(io.hover_slot(slot)&&s.hover_previous!=static_cast<I32>(slot)){
   I32 voice=-1;
   if(slot==50||slot==51||slot==55){
    voice=p.values[slot]>0?static_cast<I32>(0x3bb+(io.random_bit()&1)):0x3bd;
   }else if(slot>=52&&slot<=54){
    voice=btb::progress::bobs_band_feedback_sound(p,io.random_bit()&1);
   }else if(slot>=56&&slot<=58){
    voice=btb::progress::dino_feedback_sound(p,io.random_bit()&1);
   }else if(slot==59||slot==60){
    voice=btb::progress::spud_pair_feedback_sound(p,io.random_bit()&1);
   }else if(slot==61||slot==62){
    voice=btb::progress::adventure_pair_feedback_sound(p,io.random_bit()&1);
   }
   if(voice>=0){io.play_sound(static_cast<U32>(voice),0x32,2);
                 s.hover_previous=static_cast<I32>(slot);}
  }
  if(p.values[slot]>0){
   const auto xy=btb::progress::progress_star_position(slot);
   io.draw_star(slot,xy.x,xy.y);
  }
 }
 io.draw_back_button(s.back_down,s.back_hover);
 if(s.back_hover&&s.back_clicked)s.quit_requested=true;
}
[[noreturn]] void abort_removed_disc(RemovedDiscState& s,RemovedDiscIO& io){
 s.shutting_down=true;
 if(s.display_present){io.destroy_display();s.display_present=false;}
 io.show_error("CD Removed",0x10);
 s.quit_posted=true;io.post_quit(0);
 io.release_input();io.shutdown_sound();
 if(s.sound_manager_present){io.release_sound_manager();s.sound_manager_present=false;}
 if(s.hook_installed){io.unhook_input();s.hook_installed=false;}
 s.hardware_active=0;
 io.exit_process(0);
 throw std::logic_error("retail exit unexpectedly returned");
}
void check_disc_volume(DiscState& s,DiscScanIO& io){
 io.read_error_message_lines(s.error_message);
 for(char letter='a';letter<='z';++letter){
  const auto info=io.drive(letter);
  if(info.optical&&info.volume_label==s.expected_volume){
   s.chosen_drive=static_cast<I32>(letter-'a');break;
  }
 }
 if(s.chosen_drive==-1){io.show_missing_disc();io.exit_process(1);}
}
std::vector<std::uint8_t> export_print_bmp(const PixelCapture& s){
 if(!s.width||!s.height||s.width>16384||s.height>16384)
  throw std::invalid_argument("invalid original print surface dimensions");
 // Original source indexes 'height' pixels across each of 'width' rows.
 // The 16-bit source stride and odd 44-byte info header are retained.
 if(s.pitch_bytes<s.height*2U||
    std::size_t(s.pitch_bytes)*s.width>s.pixels.size())
  throw std::out_of_range("print surface scan would read outside lock");
 const std::uint64_t n=std::uint64_t(s.width)*s.height;
 if(n>std::numeric_limits<U32>::max()/3U)
  throw std::overflow_error("print surface too large");
 std::vector<std::uint8_t> out(14+44+static_cast<std::size_t>(n)*3U,0);
 out[0]='B';out[1]='M';
 // Peculiarity of original 00409460: (pixels+18)*3 in file header,
 // while BMP pixel offset is 0x36 and 0x2c info bytes are emitted.
 put32(out,2,static_cast<U32>((n+18)*3U));
 put32(out,10,0x36);
 put32(out,14,0x28);put32(out,18,s.width);put32(out,22,s.height);
 put16(out,26,1);put16(out,28,24);
 put32(out,34,static_cast<U32>(n*3U));
 const auto r=s.masks[0],g=s.masks[1],b=s.masks[2];
 for(U32 row=0;row<s.width;++row){
  const U32 from=s.width-1-row;
  for(U32 col=0;col<s.height;++col){
   const std::size_t at=std::size_t(from)*s.pitch_bytes+col*2U;
   const U32 pixel=U32(s.pixels[at])|(U32(s.pixels[at+1])<<8);
   const std::size_t out_at=58+(std::size_t(row)*s.height+col)*3U;
   out[out_at]=unpack(pixel,g);
   out[out_at+1]=unpack(pixel,r);
   out[out_at+2]=unpack(pixel,b);
  }
 }
 return out;
}
U32 export_and_print(U32 handle,PrintIO& io){
 const auto bmp=export_print_bmp(io.capture_back_buffer());
 io.write_bitmap("PrintMe.bmp",bmp);
 io.load_print_bitmap();
 if(io.create_compatible_dc()){
  const auto table=io.get_color_table();
  if(!table.empty())io.create_palette(table);
 }
 io.initialize_print_dialog(handle);
 if(io.print_dialog()){
  io.printer_construct();
  try{
   io.printer_set_image_and_palette();
   io.printer_set_scale(2);
   io.printer_set_document("Bob the Builder - Bob Builds a Park");
   io.printer_print();
   io.release_dialog_allocations();
   io.printer_destruct();
  }catch(...){
   io.printer_destruct();
   throw;
  }
 }
 return 1;
}
}
