#include "btb/bink32_audio_master_clock.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
using btb::bink32_recovered::Bink1AudioMasterClock;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"audio clock test line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
int main(){
    Bink1AudioMasterClock audio(48000,96000);
    CHECK(audio.observe(1'000'000,0,false)==0);
    CHECK(audio.observe(1'010'000,480,false)==10000);
    CHECK(audio.observe(1'020'000,960,false)==20000);
    CHECK(audio.observe(1'021'000,900,false)==20000);
    CHECK(!audio.observe(1'019'000,990,false));
    CHECK(audio.observe(1'022'000,0,true)==20000);
    CHECK(audio.observe(1'032'000,0,true)==30000);
    CHECK(audio.observe(1'042'000,0,true)==40000);
    audio.rewind();
    CHECK(audio.observe(6'000'000,100,false)==0);
    CHECK(audio.observe(6'010'000,580,false)==10000);
    Bink1AudioMasterClock zero_rate(0,20);
    CHECK(!zero_rate.observe(1,1,false));
    Bink1AudioMasterClock zero_samples(44100,0);
    CHECK(!zero_samples.observe(1,1,false));
    Bink1AudioMasterClock cursor(44100,44100);
    CHECK(cursor.observe(0,44100,false)==0);
    CHECK(cursor.observe(10000,44220,true)==0);
    CHECK(cursor.observe(11000,0,true)==1000);
    return 0;
}
