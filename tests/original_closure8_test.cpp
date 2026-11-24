#include "btb/original_closure8.hpp"
#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_closure8;
struct Event {std::string op;U32 arg{};};
struct ProfileMock:ProfileIO{
 std::vector<Event> events;std::map<std::string,std::string> files;
 void unregister(U32 p) override {events.push_back({"unregister",p});}
 void release(U32 p) override {events.push_back({"release",p});}
 bool write_file(std::string_view p,std::string_view c) override{
  files[std::string(p)]=std::string(c);events.push_back({"write",0});return true;}
};
struct ParkMock:ParkIO {
 std::vector<Event> events;
 void unregister(U32 p) override {events.push_back({"unregister",p});}
 void release(U32 p) override {events.push_back({"release",p});}
 void save_park() override {events.push_back({"save",0});}
 void destroy_park_object(U32 o,U32 flag) override {assert(flag==1);events.push_back({"delete",o});}
 void stop_music() override {events.push_back({"stop",0});}
};
struct ProgressMock:ProgressIO{
 bool playing{};std::size_t hovered{52};unsigned intros{},checks{},stars{},help{};
 std::vector<U32> sounds;std::vector<std::pair<std::size_t,std::pair<I32,I32>>> stars_at;
 bool any_sound_playing() override {++checks;return playing;}
 void play_sound(U32 id,U32 priority,U32 arb) override{
  assert(priority==50 && arb==2);sounds.push_back(id);}
 void mark_last_sound_interruptible() override {++intros;}
 void draw_background() override {}
 void update_help_button() override {++help;}
 void update_help_hover() override {++help;}
 bool hover_slot(std::size_t slot) override {return slot==hovered;}
 U32 random_bit() override {return 0;}
 void draw_star(std::size_t slot,I32 x,I32 y) override{
  ++stars;stars_at.push_back({slot,{x,y}});}
 void draw_back_button(bool down,bool hover) override {
  if(down)assert(hover);
 }
};
struct Exited {U32 code;};
struct AbortMock:RemovedDiscIO{
 std::vector<Event> events;
 void destroy_display() override{events.push_back({"display"});}
 void show_error(std::string_view msg,U32 flags) override{
  assert(msg=="CD Removed"&&flags==16);events.push_back({"error"});}
 void post_quit(I32 code) override{assert(!code);events.push_back({"quit"});}
 void release_input() override{events.push_back({"input"});}
 void shutdown_sound() override{events.push_back({"sound"});}
 void release_sound_manager() override{events.push_back({"manager"});}
 void unhook_input() override{events.push_back({"unhook"});}
 [[noreturn]] void exit_process(U32 code) override{events.push_back({"exit",code});throw Exited{code};}
};
struct DiscMock:DiscScanIO{
 unsigned checked{},errors{},exit_calls{};std::array<std::string,6> lines{"first","second","third","fourth","fifth","sixth"};
 void read_error_message_lines(std::array<std::string,6>& out) override{out=lines;}
 Drive drive(char letter) override{++checked;
  if(letter=='c')return {true,"OTHER"};
  if(letter=='g')return {true,"RET_CD"};
  return {false,"RET_CD"};}
 void show_missing_disc() override {++errors;}
 [[noreturn]] void exit_process(U32 code) override{++exit_calls;throw Exited{code};}
};
struct PrintMock:PrintIO{
 unsigned captures{},written{},printed{},constructed{},destroyed{},dialogs{},palettes{};
 std::vector<std::uint8_t> bmp;
 PixelCapture capture_back_buffer() override {++captures;return {2,2,4,{0xf800,0x7e0,0x1f},{0,0xf8,0,0,0xe0,0x07,0,0}};}
 void write_bitmap(std::string_view name,std::span<const std::uint8_t> data) override{
  assert(name=="PrintMe.bmp");++written;bmp.assign(data.begin(),data.end());}
 void load_print_bitmap() override {}
 bool create_compatible_dc() override {return true;}
 std::vector<Bgra> get_color_table() override{return {{1,2,3,0},{4,5,6,0}};}
 void create_palette(std::span<const Bgra> p) override{assert(p.size()==2);++palettes;}
 void initialize_print_dialog(U32 h) override{assert(h==0x42);++dialogs;}
 bool print_dialog() override{return print;}
 void printer_construct() override{++constructed;}
 void printer_set_image_and_palette() override{}
 void printer_set_scale(U32 scale) override{assert(scale==2);}
 void printer_set_document(std::string_view name) override{assert(name.find("Bob")!=std::string_view::npos);}
 void printer_print() override{++printed;}
 void release_dialog_allocations() override{}
 void printer_destruct() override{++destroyed;}
 bool print{true};
};
static U32 load32(const std::vector<std::uint8_t>& b,std::size_t offset){
 return U32(b.at(offset))|(U32(b.at(offset+1))<<8)|
  (U32(b.at(offset+2))<<16)|(U32(b.at(offset+3))<<24);
}
int main(){
 {
  ProfileState s;ProfileMock io;
  s.metadata[0].name_length=2;s.metadata[0].badge_index=3;
  s.metadata[0].name_glyph_codes[0]=6;s.metadata[0].name_glyph_codes[1]=7;
  s.records[0].values[50]=42;
  s.surfaces.slot(0x51b39c)=101;s.surfaces.slot(0x51b3a8)=102;
  s.surfaces.slot(0x519938)=103;
  save_and_unload_profiles(s,io);
  assert(io.events.size()==(5+20+1+3+6));
  assert(io.events[0].op=="unregister"&&io.events[0].arg==0x51b39c);
  assert(io.events[1].op=="release"&&io.events[1].arg==101);
  assert(s.surfaces.slot(0x51b39c)==0&&s.surfaces.slot(0x51b3a8)==0);
  assert(io.files.size()==6);
  std::istringstream in(io.files.at("playerinfo.txt"));
  auto p=btb::profiles::read_playerinfo(in);
  assert(p[0].name_length==2&&p[0].name_glyph_codes[1]==7);
  std::istringstream record(io.files.at("player1.txt"));
  auto rec=btb::progress::read_record(record);
  assert(rec.values[50]==42);
 }
 {
  ParkState s;ParkMock io;
  s.active_objects[0]=11;s.active_objects[3]=12;
  s.object_table[100]=10;
  s.surfaces.slot(0x509250)=55;s.surfaces.slot(0x508b00)=56;
  unload_park(s,io);
  assert(io.events.front().op=="save"&&io.events.back().op=="stop");
  assert(io.events[1].op=="delete"&&io.events[1].arg==11);
  assert(io.events[2].op=="delete"&&io.events[2].arg==12);
  assert(s.active_objects[0]==0&&s.active_objects[3]==0);
  assert(s.object_table[100]==0&&!s.activity_active&&!s.scene_active);
  assert(s.surfaces.slot(0x509250)==0&&s.surfaces.slot(0x508b00)==0);
  const auto find=[&](U32 slot){return std::find_if(io.events.begin(),io.events.end(),[&](const Event&e){return e.op=="unregister"&&e.arg==slot;});};
  assert(find(0x509250)!=io.events.end()&&find(0x508b00)!=io.events.end());
 }
 {
  QuitData q;
  q.labels[0]={"NONE","valid","NONE","foo"};q.labels[5]={"NONE","bar","NONE","NONE"};
  std::istringstream in("1 2 3 4 5 6 7 8 yes no maybe");
  load_quit_confirmation(q,in,"NONE");
  assert(q.metadata[0]==1&&q.metadata[7]==8&&q.names[1]=="no");
  assert(q.labels[0][0].empty()&&q.labels[0][1]=="valid");
  assert(q.labels[5][0].empty()&&q.labels[5][3].empty());
  bool short_input=false;
  try{std::istringstream bad("1 2");load_quit_confirmation(q,bad,"NONE");}
  catch(const std::runtime_error&){short_input=true;}
  assert(short_input);
 }
 {
  ProgressState s;ProgressMock io;
  btb::progress::Record progress{};
  s.intro_voice_pending=1;progress.values[52]=1;progress.values[56]=1;
  update_progress(s,progress,io);
  assert(s.intro_voice_pending==0&&io.intros==1);
  assert(io.sounds.size()==2&&io.sounds[0]==954&&io.sounds[1]==958);
  assert(io.help==2&&io.stars==2);
  assert(io.stars_at[0].first==52);
  s.back_hover=true;s.back_down=true;s.back_clicked=true;
  update_progress(s,progress,io);
  assert(s.quit_requested && io.sounds.size()==2);
 }
 {
  RemovedDiscState state;AbortMock io;
  state.display_present=true;state.sound_manager_present=true;state.hook_installed=true;
  try{abort_removed_disc(state,io);assert(false);}catch(const Exited& e){assert(!e.code);}
  assert(io.events.size()==8&&io.events[0].op=="display"&&io.events[1].op=="error");
  assert(io.events.back().op=="exit"&&state.hardware_active==0&&state.quit_posted);
  assert(!state.display_present&&!state.sound_manager_present&&!state.hook_installed);
 }
 {
  DiscState state;DiscMock io;state.expected_volume="RET_CD";
  check_disc_volume(state,io);
  assert(state.chosen_drive==6&&io.checked==7&&state.error_message[0]=="first");
  state.expected_volume="MISSING";state.chosen_drive=-1;io.checked=0;
  try{check_disc_volume(state,io);assert(false);}catch(const Exited&e){assert(e.code==1);}
  assert(io.checked==26&&io.errors==1&&io.exit_calls==1);
 }
 {
  PrintMock io;
  assert(export_and_print(0x42,io)==1);
  assert(io.captures==1&&io.written==1&&io.dialogs==1&&io.palettes==1);
  assert(io.constructed==1&&io.destroyed==1&&io.printed==1);
  assert(io.bmp[0]=='B'&&io.bmp[1]=='M'&&load32(io.bmp,2)==66);
  assert(load32(io.bmp,10)==54&&load32(io.bmp,14)==40);
  assert(io.bmp.size()==70);
  PrintMock no_dialog;no_dialog.print=false;
  assert(export_and_print(0x42,no_dialog)==1);
  assert(no_dialog.printed==0&&no_dialog.constructed==0);
  bool invalid=false;
  try{auto capture=io.capture_back_buffer();capture.masks[0]=0;
      (void)export_print_bmp(capture);}
  catch(const std::invalid_argument&){invalid=true;}
  assert(invalid);
 }
}
