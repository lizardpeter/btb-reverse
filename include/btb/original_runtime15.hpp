#pragma once
#include "btb/original_crt10.hpp"
#include "btb/original_runtime16.hpp"
#include <array>
#include <cstdint>
#include <span>
namespace btb::original_runtime15 {
using U32=std::uint32_t;using I32=std::int32_t;
struct Transition { I32 year{-1},day{},milliseconds{};bool operator==(const Transition&) const=default; };
struct SystemTime { std::uint16_t year{},month{},weekday{},day{},hour{},minute{},second{},milliseconds{}; };
struct TimeState {
    U32 initialized{};I32 timezone{},daylight{1},daylight_bias{-3600};
    bool system_rules{};SystemTime daylight_date{},standard_date{};
    Transition start{},end{};
};
class TimeServices {public:virtual ~TimeServices()=default;virtual void initialize_timezone(TimeState&)=0;};
// 00436B93 / 004369E7 / 004326B8. Calendar years retain original 1970..2038 range.
void compute_transition(TimeState&,bool,bool,I32,I32,I32,I32,I32,I32,I32,I32,I32);
bool is_daylight_time(TimeState&,const std::array<I32,9>&);
I32 calendar_to_time(TimeState&,I32,I32,I32,I32,I32,I32,I32,TimeServices&,std::array<I32,2> stack_prefix);
using ActionRecord=original_runtime16::ActionRecord;
struct SignalState {
    std::span<ActionRecord> actions;
    std::array<U32,4> handlers{}; // signals 2,15,21,22
    U32 exception_pointers{},unhandled_hook{};
    I32 floating_point_code{},floating_first{},floating_count{};
};
struct ExceptionRecord {U32 code{},parameter_count{},first_parameter{};};
class SignalServices {
public:
    virtual ~SignalServices()=default;
    virtual void invoke(U32 handler,I32 signal)=0;
    virtual void invoke_floating(U32 handler,I32 signal,I32 code)=0;
    [[noreturn]] virtual void exit_process(I32)=0;
    virtual I32 unhandled_exception(U32 pointers)=0;
    virtual bool valid_handler(U32)=0;
    virtual I32 invoke_unhandled(U32 handler,U32 pointers)=0;
    [[noreturn]] virtual void terminate_process()=0;
};
// 0043854D / 0043576F / 00433824. Handler identities remain guest DWORDs.
I32 raise_signal(SignalState&,I32,SignalServices&);
I32 exception_to_signal(SignalState&,U32,U32,SignalServices&);
I32 cpp_unhandled_filter(SignalState&,const ExceptionRecord&,U32,SignalServices&);
// 0043669F / 00433888. The existing 00437371 buffer allocator is called directly.
class InputServices {
public:
    virtual ~InputServices()=default;
    virtual void* allocate(std::size_t)=0;
    virtual I32 read_descriptor(U32,std::byte*,U32)=0;
    virtual std::uint8_t descriptor_flags(U32)=0;
};
U32 push_back_byte(U32,original_crt10::Stream&,U32&,InputServices&);
U32 refill_input(original_crt10::Stream&,U32,U32&,InputServices&);
// Calls existing 00431D70 and new 00433888, sharing pointer/count mutations.
U32 read_buffered_byte(original_crt10::Stream&,U32,U32&,InputServices&);
bool verify_8_original_runtime_functions();
}
