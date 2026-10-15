#pragma once
// Source-level reconstruction of four PE32 CRT / Win32 boundary procedures.
// The wrappers are not original x86 ABI replacements. No retail parity claim.
#include "btb/original_runtime15.hpp"
#include <array>
#include <cstdint>
namespace btb::original_tail4 {
using U32=std::uint32_t;
using I32=std::int32_t;
using SystemTime=original_runtime15::SystemTime;
struct ZoneInfo { U32 status{};std::uint16_t daylight_month{};I32 daylight_bias{}; };
class Clock:public original_runtime15::TimeServices {
public:
 virtual SystemTime local_time()=0;
 virtual SystemTime system_time()=0;
 virtual ZoneInfo timezone_information()=0;
};
struct TimeCache {
 bool valid{};
 SystemTime last_utc{};
 I32 daylight_status{-1};
 original_runtime15::TimeState conversion{};
};
// 0042FFE2: GetLocalTime -> GetSystemTime -> once-per-UTC-minute TZ refresh
// -> recovered 004326B8 calendar_to_time. Original stray stack words explicit.
void local_epoch(TimeCache&,Clock&,I32* output,std::array<I32,2> stack_prefix={});
// 00430F00: guest model of x86 __chkstk page touch and return-address move.
// This cannot substitute for compiler-provided __chkstk on an actual x86 stack.
class Stack {
public:
 virtual ~Stack()=default;
 virtual void touch(U32 guest_address)=0;
 virtual void write32(U32 guest_address,U32 value)=0;
};
U32 probe_stack(U32 requested_bytes,U32 original_esp,U32 return_address,Stack&);
// 0043A220: PE32 import thunk. The provider must be a real OS/guest unwind.
using Unwind=void(*)(U32,U32,U32,U32,void*);
void rtl_unwind_thunk(U32 target_frame,U32 target_ip,U32 record,U32 value,Unwind,void*);
// 00437BF1: original wrapper of __amsg_exit(2), which must not return.
using MessageExit=void(*)(U32,void*);
[[noreturn]] void runtime_message_exit(MessageExit,void*);
}
