#include "btb/original_tail4.hpp"
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace btb::original_tail4;
struct TestClock:Clock {
 SystemTime loc{2026,1,0,4,15,45,30,0};
 SystemTime utc{2026,1,0,4,20,45,30,0};
 ZoneInfo zone{1,3,60};
 unsigned local_calls{},utc_calls{},zone_calls{},init_calls{};
 SystemTime local_time() override {++local_calls;return loc;}
 SystemTime system_time() override {++utc_calls;return utc;}
 ZoneInfo timezone_information() override {++zone_calls;return zone;}
 void initialize_timezone(btb::original_runtime15::TimeState& time) override {
  ++init_calls;time.timezone=0;time.daylight=0;
 }
};
struct TestStack:Stack {
 std::vector<U32> touched;std::vector<std::pair<U32,U32>> written;
 void touch(U32 p) override {touched.push_back(p);}
 void write32(U32 p,U32 v) override {written.emplace_back(p,v);}
};
struct ExitRaised {U32 code;};
int main() {
 // Clock refresh is keyed on UTC year, month, day, hour and minute, not seconds.
 TestClock clock;TimeCache state;I32 result=0,again=0;
 local_epoch(state,clock,&result);
 assert(clock.local_calls==1 && clock.utc_calls==1 && clock.zone_calls==1);
 assert(clock.init_calls==1 && state.daylight_status==0 && state.valid);
 assert(state.last_utc.second==30);
 clock.utc.second=59;
 local_epoch(state,clock,&again);
 assert(result==again && clock.zone_calls==1 && clock.local_calls==2);
 clock.utc.minute=46;clock.zone={2,3,60};
 local_epoch(state,clock,nullptr);
 assert(clock.zone_calls==2 && state.daylight_status==1 && state.last_utc.minute==46);
 clock.utc.day=5;clock.zone={2,0,60};
 local_epoch(state,clock,nullptr);
 assert(clock.zone_calls==3 && state.daylight_status==0);
 clock.utc.hour=21;clock.zone={0xffffffffU,3,60};
 local_epoch(state,clock,nullptr);
 assert(clock.zone_calls==4 && state.daylight_status==-1);
 // Compare the caller's result to the already recovered 004326B8 dependency.
 btb::original_runtime15::TimeState independent;
 TestClock comparison;
 I32 reference=btb::original_runtime15::calendar_to_time(
   independent,comparison.loc.year,comparison.loc.month,comparison.loc.day,
   comparison.loc.hour,comparison.loc.minute,comparison.loc.second,0,comparison,{});
 assert(result==reference);
 // Zero, exact and non-page-aligned stack sizes preserve original return word.
 for(const U32 bytes:{0U,1U,4095U,4096U,4097U,8192U,8193U}) {
  TestStack stack;const U32 esp=0x90000U;
  assert(probe_stack(bytes,esp,0x12345678U,stack)==esp-bytes);
  assert(stack.written.size()==1);
  assert((stack.written[0]==std::pair<U32,U32>{esp-bytes,0x12345678U}));
  assert(stack.touched.size()==bytes/4096U);
  for(U32 i=0;i<stack.touched.size();++i)
   assert(stack.touched[i]==esp+4U-(i+1U)*4096U);
 }
 struct Trace {U32 frame{},ip{},record{},value{};unsigned count{};} trace;
 const auto unwind=+[](U32 a,U32 b,U32 c,U32 d,void* ctx){
  auto& t=*static_cast<Trace*>(ctx);t.frame=a;t.ip=b;t.record=c;t.value=d;++t.count;
 };
 rtl_unwind_thunk(1,2,3,4,unwind,&trace);
 assert(trace.frame==1 && trace.ip==2 && trace.record==3
        && trace.value==4 && trace.count==1);
 bool invalid=false;
 try {rtl_unwind_thunk(1,2,3,4,nullptr,nullptr);}
 catch(const std::invalid_argument&) {invalid=true;}
 assert(invalid);
 auto fatal=+[](U32 code,void*){throw ExitRaised{code};};
 try {runtime_message_exit(fatal,nullptr);assert(false);}
 catch(const ExitRaised& e){assert(e.code==2);}
 return 0;
}
