#include "btb/bink32_playback.hpp"
#include "btb/bink32_recovered.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
using namespace btb::bink32_recovered;
#define CHECK(c) do {if(!(c)){std::fprintf(stderr,"Bink playback regression at %d: %s\n",__LINE__,#c);std::abort();}}while(0)
struct State {std::uint32_t times{},last{},services{},opens{};};
static void track(void* p,std::uint32_t value){
    auto& s=*static_cast<State*>(p);++s.times;s.last=value;
}
static void service(void* p){++static_cast<State*>(p)->services;}
static std::uintptr_t good_open(std::uintptr_t arg,void* p){
    ++static_cast<State*>(p)->opens;return arg+100;
}
static std::uintptr_t failed_open(std::uintptr_t,void* p){
    ++static_cast<State*>(p)->opens;return 0;
}
int main() {
    State s{};
    SoundSystemState sound{};
    CHECK(!set_sound_system(sound,0,1,good_open,&s));
    CHECK(set_sound_system(sound,12,100,good_open,&s));
    CHECK(sound.factory_identity==12&&sound.opened_handle==200&&s.opens==1);
    CHECK(set_sound_system(sound,12,300,failed_open,&s));
    CHECK(sound.opened_handle==200&&s.opens==2);
    sound.locked=true;
    CHECK(!set_sound_system(sound,13,100,good_open,&s));
    CHECK(s.opens==2&&sound.factory_identity==12);
    CHECK(set_sound_system(sound,12,123,good_open,&s));
    CHECK(sound.opened_handle==223&&s.opens==3);
    sound.locked=false;
    CHECK(set_sound_system(sound,13,8,good_open,&s));
    CHECK(sound.factory_identity==13&&sound.opened_handle==108);

    std::array<TrackPauseCallback,2> callbacks{{
        {track,&s},{track,&s}
    }};
    PauseRuntime runtime{callbacks,service,&s};
    PauseTelemetry movie{};
    movie.current_frame=7;
    movie.pause_tick=100;
    movie.interval_tick=120;
    movie.largest_interval=25;movie.largest_frame=4;
    movie.second_interval=12;movie.second_frame=3;
    movie.track_count=2;movie.audio_clock=1;movie.paused=1;
    movie.clock_origin=333;movie.lag_ticks=444;
    CHECK(pause(&movie,0,150,runtime)==0);
    CHECK(movie.accumulated_paused_ticks==50&&movie.pause_tick==0);
    CHECK(movie.interval_tick==0&&movie.largest_interval==30);
    CHECK(movie.largest_frame==7&&movie.second_interval==25);
    CHECK(movie.second_frame==4);
    CHECK(movie.clock_origin==0&&movie.lag_ticks==0);
    CHECK(s.times==2&&s.services==1&&s.last==0);
    CHECK(pause(&movie,1,180,runtime)==1);
    CHECK(movie.paused==1&&s.times==4&&s.services==2&&s.last==1);
    movie.interval_tick=190;movie.current_frame=11;
    CHECK(pause(&movie,1,215,runtime)==1);
    CHECK(movie.second_interval==25&&movie.second_frame==4);
    movie.interval_tick=230;
    CHECK(pause(&movie,0,260,runtime)==0);
    CHECK(movie.second_interval==30&&movie.second_frame==11);
    // Original unsigned 32-bit tick wrap is preserved.
    movie.pause_tick=0xfffffff0U;movie.interval_tick=0;
    CHECK(pause(&movie,0,0x10,runtime)==0);
    CHECK(movie.accumulated_paused_ticks==82);
    CHECK(pause(nullptr,0,100,runtime)==0);

    CHECK(next_frame_number(1,10)==2);
    CHECK(next_frame_number(10,10)==1);
    CHECK(next_frame_number(12,10)==1);
    CHECK(next_frame_number(0,10)==1);
    CHECK(next_frame_number(3,0)==0);
    const auto plan=plan_seek(445,200,1,0,1,15,1,1);
    CHECK(plan.requested_frame==1&&plan.first_decode_frame==1);
    const auto p2=plan_seek(445,200,150,0,1,15,1,101);
    CHECK(p2.requested_frame==150&&p2.first_decode_frame==101);
    const auto p3=plan_seek(445,200,150,0,1,15,1,145);
    CHECK(p3.first_decode_frame==135);
    const auto p4=plan_seek(445,200,999,3,1,15,1,111);
    CHECK(p4.requested_frame==445&&p4.first_decode_frame==445&&p4.use_direct_seek);
    CHECK(plan_seek(100,150,150,0,1,30,1,100).already_at_frame);
    CHECK(plan_seek(0,0,10,0,0,15,1,1).requested_frame==0);
    // Mode 2 of original 300066B0 advances its forward search EACH loop,
    // even after all backward positions are exhausted.
    const std::array<std::uint32_t,8> frames{1,0,0,0,0,1,0,0};
    Movie meta{8,frames};
    CHECK(get_key_frame(&meta,3,2)==6);
    CHECK(get_key_frame(&meta,3,1)==6);
    CHECK(get_key_frame(&meta,3,0)==1);
    std::puts("Original x86 Bink pause/audio/seek regression: PASS");
}
