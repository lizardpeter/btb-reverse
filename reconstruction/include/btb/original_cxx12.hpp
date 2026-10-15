#pragma once
#include "btb/original_unwind13.hpp"
#include <array>
#include <cstddef>
#include <span>
namespace btb::original_cxx12 {
using namespace original_unwind13;
struct Raised {U32 record{},context{};};
struct ResumeTransfer {U32 target{},stack{},base{};};
struct Translation:Registration {
 U32 dispatch_frame{},return_target{0x42fdad},saved_stack{},saved_base{};
 bool unwound{};
};
class Services:public original_unwind13::Services {
public:
 virtual std::span<std::byte> bytes(U32 address,U32 count)=0;
 virtual bool readable(U32 address,U32 count)=0;
 virtual bool writable(U32 address,U32 count)=0;
 virtual bool executable(U32 address)=0;
 virtual U32 invoke_dispatch(U32 target,const std::array<U32,8>& arguments)=0;
 virtual void invoke_translation(U32 target,U32 code,U32 record,U32 context,Translation&)=0;
 virtual std::array<U32,2> stack_context()=0; // emulated caller ESP/EBP
 virtual U32 registration_address(const Registration&)=0;
 // Source execution adapters transfer through ResumeTransfer. A real OS bridge
 // must supply compatible stack/frame dispatch before native PE32 admission.
 [[noreturn]] virtual void resume(U32 target,U32 stack,U32 base)=0;
};
struct Cleanup {
 U32 record{},frame{},saved_stack{},previous_record{},previous_context{},result{};
 bool rethrow{};
};
void copy_catch(State&,U32 record,U32 frame,U32 handler,U32 catchable,Services&); //00432371
void finish_catch(State&,const Cleanup&,Services&); //00432301
[[noreturn]] void resume_catch(State&,U32 target,U32 frame,Services&); //0042FBE7
U32 translate(State&,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,I32 nesting,U32 dispatch_frame,Services&); //0042FD27
U32 dispatch(State&,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,I32 nesting,U32 dispatch_frame,bool translated,Services&); //00431DC5
void enter_catch(State&,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,U32 handler,U32 catchable,U32 try_block,I32 nesting,U32 dispatch_frame,Services&); //004321C0
void search(State&,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,bool translated,I32 nesting,U32 dispatch_frame,Services&); //00431E60
U32 invoke_catch(State&,U32 record,U32 frame,U32 context,U32 info,U32 target,I32 nesting,U32 status,Services&); //0043223B
void foreign_exception(State&,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,I32 level,I32 nesting,U32 dispatch_frame,Services&); //00432007
U32 translator_handler(State&,U32 record,Translation&,U32 context,Services&); //0042FDDB
U32 frame_handler(State&,U32 record,U32 frame,U32 context,U32 dispatcher,U32 function_info,Services&); //0042FC78
U32 catch_handler(State&,U32 record,U32 registration,U32 context,Services&); //0042FD02
}
