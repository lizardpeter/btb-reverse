#pragma once
#include "btb/original_startup11.hpp"
#include <span>
namespace btb::original_unwind13 {
using U32=std::uint32_t;
using I32=std::int32_t;
// Explicit native representation of the original thread's FS:[0] chain.
// This API does not install a PE32 frame into the host operating system.
struct Registration {
    Registration* previous{};
    U32 handler{};
    I32 level{-1};
    U32 frame{};
    U32 function_info{};
};
struct State {
    Registration* chain{};
    U32 terminate_handler{},unexpected_handler{};
};
struct GuestException {U32 code;};
class Services:public original_startup11::GuestMemory {
public:
    virtual void write32(U32 address,U32 value)=0;
    virtual U32 invoke_this(U32 target,U32 object,std::span<const U32> arguments)=0;
    virtual U32 invoke_frame(U32 target,U32 frame_base)=0;
    virtual void notify(U32 target,U32 frame_base,U32 code)=0;
    virtual void invoke_handler(U32 target)=0;
    virtual void rtl_unwind(U32 frame,U32 instruction,U32 record,U32 value)=0;
    virtual void raise_exception(U32 code,U32 flags,std::span<const U32> parameters)=0;
    virtual void report_error(U32 code)=0;
    virtual void raise_signal(U32 signal)=0;
    [[noreturn]] virtual void exit_process(U32 code,U32 quick,U32 return_to_caller)=0;
};
void throw_exception(U32 object,U32 throw_info,Services&); //00430495
U32 call_object(U32 object,U32 target,std::span<const U32>,Services&); //0042FC1B
U32 call_object_virtual(U32 object,U32 target,std::span<const U32>,Services&); //0042FC22
void unwind_to(State&,U32 frame,U32 record,Services&); //0042FC29
U32 call_catch(State&,U32 frame,U32 function_info,U32 target,I32 nesting,U32 status,Services&); //0042FCAE
U32 find_try_range(State&,U32 function_info,I32 nesting,I32 level,U32& first,U32& last,Services&); //0042FE50
void global_unwind(U32 frame,Services&); //0042FECC
void local_unwind(State&,U32 frame,I32 stop,U32 frame_base,Services&); //0042FF0E
bool abnormal_termination(const State&,Services&); //0042FF76
void unwind_cpp(State&,U32 frame,U32 context,U32 function_info,I32 stop,Services&); //0043210C
void destroy_exception(State&,U32 record,Services&); //00432535
[[noreturn]] void terminate(State&,Services&); //0043260C
[[noreturn]] void unexpected(State&,Services&); //00432662
}
