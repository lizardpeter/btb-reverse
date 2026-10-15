#include "btb/original_tail4.hpp"
#include <stdexcept>
namespace btb::original_tail4 {
namespace {
bool same_minute(const SystemTime& a,const SystemTime& b) {
 return a.year==b.year && a.month==b.month && a.day==b.day
     && a.hour==b.hour && a.minute==b.minute;
}
}
void local_epoch(TimeCache& cache,Clock& io,I32* output,std::array<I32,2> stack_prefix) {
 // Preserve the original API invocation order, including with null output.
 const auto local=io.local_time();
 const auto utc=io.system_time();
 if(!cache.valid || !same_minute(cache.last_utc,utc)) {
  const auto zone=io.timezone_information();
  if(zone.status==0xffffffffU)cache.daylight_status=-1;
  else if(zone.status==2 && zone.daylight_month!=0 && zone.daylight_bias!=0)
   cache.daylight_status=1;
  else cache.daylight_status=0;
  cache.last_utc=utc;
  cache.valid=true;
 }
 const I32 result=original_runtime15::calendar_to_time(
     cache.conversion,local.year,local.month,local.day,local.hour,
     local.minute,local.second,cache.daylight_status,io,stack_prefix);
 if(output)*output=result;
}
U32 probe_stack(U32 requested_bytes,U32 original_esp,U32 return_address,Stack& stack) {
 U32 base=original_esp+4U,remaining=requested_bytes;
 while(remaining>=0x1000U) {
  base-=0x1000U;
  stack.touch(base);
  remaining-=0x1000U;
 }
 const U32 final_esp=base-4U-remaining;
 stack.write32(final_esp,return_address);
 return final_esp;
}
void rtl_unwind_thunk(U32 frame,U32 ip,U32 record,U32 value,Unwind unwind,void* ctx) {
 if(!unwind)throw std::invalid_argument("missing original RtlUnwind import");
 unwind(frame,ip,record,value,ctx);
}
[[noreturn]] void runtime_message_exit(MessageExit exit,void* ctx) {
 if(!exit)throw std::invalid_argument("missing original __amsg_exit provider");
 exit(2,ctx);
 throw std::logic_error("original __amsg_exit returned");
}
}
