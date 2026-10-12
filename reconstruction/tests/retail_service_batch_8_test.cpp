#include "btb/retail_service_batch_8.hpp"
#include <cassert>
#include <algorithm>
#include <array>
using namespace btb::retail_services;
static int count(const Plan& p,Kind kind) {return static_cast<int>(
 std::count_if(p.calls.begin(),p.calls.end(),[&](const Effect& e){return e.op==kind;}));}
static bool has(const Plan& p,Kind kind,int a){
 return std::any_of(p.calls.begin(),p.calls.end(),[&](const Effect& e){return e.op==kind&&e.a==a;});}
int main(){
 // 00407B60
 Inputs inputs{};inputs.registered[0]=1;inputs.registered[1]=2;
 inputs.registered[15]=3;inputs.animation_surface=4;
 inputs.primary_input=5;inputs.keyboard_input=6;
 inputs.cursor_sound=7;inputs.interface_sound=8;
 auto p=release_inputs(inputs);
 assert(count(p,Kind::UnregisterSurface)==16);
 assert(count(p,Kind::ReleaseSurface)==4);
 assert(count(p,Kind::UnacquireInput)==2);
 assert(count(p,Kind::ReleaseInterface)==2);
 assert(count(p,Kind::StopSound)==2);
 assert(p.calls.back().op==Kind::UnloadProfiles);
 assert(inputs.primary_input==0&&inputs.keyboard_input==0);
 assert(inputs.registered[15]==0);
 Inputs empty{};p=release_inputs(empty);
 assert(count(p,Kind::UnregisterSurface)==16&&count(p,Kind::ReleaseSurface)==0);
 // 00406AD0
 Credits credits{};credits.slides={1,0,3};credits.interval_seconds=4;
 p=update_credits(credits,1,false);
 assert(count(p,Kind::BlitSurface)==1&&credits.index==0);
 p=update_credits(credits,8,false);
 assert(credits.index==1&&has(p,Kind::BlitSurface,1));
 p=update_credits(credits,0,true);
 assert(credits.index==2&&has(p,Kind::BlitSurface,2));
 p=update_credits(credits,0,true);
 assert(credits.index==3&&credits.quit_requested&&credits.counter==1);
 assert(count(p,Kind::UnregisterSurface)==3);
 assert(count(p,Kind::ReleaseSurface)==2);
 assert(credits.slides[0]==0&&credits.slides[2]==0);
 // 00408F70
 BinkFrame movie{};movie.current_frame=0;movie.total_frames=2;
 movie.next_enabled=true;
 p=decode_and_blit_bink(movie);
 assert(p.result==0&&movie.current_frame==1&&movie.decoded);
 assert(count(p,Kind::BinkDoFrame)==1&&count(p,Kind::BinkCopyToBuffer)==1);
 assert(count(p,Kind::BinkNextFrame)==1&&count(p,Kind::BinkBlit)==1);
 BinkFrame failed{};failed.acquire_surface_failed=true;
 p=decode_and_blit_bink(failed);
 assert(p.result==1&&count(p,Kind::BinkCopyToBuffer)==0);
 // 004090D0
 GlobalBink global{};global.video.total_frames=5;
 global.audio_present=true;global.audio_duration=200;global.audio_cursor=10;
 global.audio_frame_ended=true;
 p=advance_global_bink(global);
 assert(p.result==0&&global.video.current_frame==1);
 assert(global.audio_cursor==200&&global.audio_completed_count==1);
 global.paused_on_input=true;global.input_left=true;
 p=advance_global_bink(global);
 assert(p.result==1&&global.closed);
 assert(count(p,Kind::CloseBink)==1);
 GlobalBink ended{};ended.video.current_frame=9;
 ended.video.total_frames=9;
 p=advance_global_bink(ended);
 assert(p.result==1&&ended.closed);
 // 004092D0
 SoundManagerBoot boot{};boot.slider_position=120;
 p=initialize_sound_manager(boot,true,0);
 assert(boot.created&&boot.initialized&&boot.volume==840);
 assert(count(p,Kind::CreateSoundManager)==1);
 SoundManagerBoot broken{};
 p=initialize_sound_manager(broken,false,-5);
 assert(!broken.created&&!broken.initialized&&broken.dialog_closed);
 assert(has(p,Kind::EndDialog,3));
 // 00402E10 / 00402CF0 80-slot sound policy/source handling
 ManagedBank bank{};bank.available_ids={3,4,5,6};
 p=play_managed_sound(bank,9,50,1);
 assert(p.result==0&&p.calls.empty());
 p=play_managed_sound(bank,3,50,1);
 assert(p.result==1&&find_sound_slot(bank,3)==0);
 assert(count(p,Kind::AcquireSound)==1&&count(p,Kind::PlaySound)==2);
 p=play_managed_sound(bank,3,50,1);
 assert(p.result==2&&p.calls.empty());
 p=play_managed_sound(bank,4,50,2);
 assert(p.result==0); // priority policy=1 prevents mixing
 bank.slots[0].policy=2;
 p=play_managed_sound(bank,4,50,2);
 assert(p.result==1&&count(p,Kind::StopSound)==1);
 assert(bank.slots[0].state==3);
 p=play_managed_sound(bank,3,50,0);
 assert(p.result==1&&count(p,Kind::PlaySound)==1);
 for(int i=0;i<80;++i){
  bank.slots[static_cast<std::size_t>(i)]={.sound_id=100+i,.state=2,.priority=i,.policy=0,.is_playing=true,.native_created=true};
 }
 bank.available_ids.push_back(77);
 auto acquisition=acquire_sound(bank,77,20,0,true);
 assert(acquisition.slot==-1&&acquisition.effects.calls.empty());
 bank.slots[12].is_playing=false;
 acquisition=acquire_sound(bank,77,20,0,true);
 assert(acquisition.slot==12&&count(acquisition.effects,Kind::ReleaseSound)==1);
 assert(bank.slots[12].sound_id==77);
 // 00405380
 PrinterScale scale{};p=set_printer_scale(scale,3);
 assert(scale.mode==3&&has(p,Kind::SetScaleMode,3));
 p=set_printer_scale(scale,4);
 assert(p.result==-1&&scale.exception_requested&&scale.mode==3);
}
