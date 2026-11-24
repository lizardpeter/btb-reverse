#include "btb/original_cxx12.hpp"
#include "btb/original_crt_batch_32.hpp"
#include "btb/original_string8.hpp"
#include "btb/original_runtime16.hpp"
#include <bit>
#include <stdexcept>
#include <string>
namespace btb::original_cxx12 {
namespace {
constexpr U32 cpp=0xe06d7363,magic=0x19930520,active_record=0x51c420,active_context=0x51c424,translator=0x51c428;
I32 signed32(U32 x){return std::bit_cast<I32>(x);}
U32 byte(Services& io,U32 p){return std::to_integer<U32>(io.bytes(p,1)[0]);}
struct Link {
 State& state;Registration registration;
 explicit Link(State& s):state(s),registration{s.chain,0x435f38,-1,0,0}{s.chain=&registration;}
 ~Link(){state.chain=registration.previous;}
};
bool cpp_record(U32 record,Services& io){return io.read32(record)==cpp && io.read32(record+16)==3 && io.read32(record+20)==magic;}
bool rethrow(U32 record,Services& io){return cpp_record(record,io) && !io.read32(record+28);}
bool probe(U32 address,U32 count,bool writing,Services& io){
 auto callback=[](const void* p,std::size_t n,void* context){auto& pair=*static_cast<std::pair<Services*,bool>*>(context);const auto at=static_cast<U32>(reinterpret_cast<std::uintptr_t>(p));return !(pair.second?pair.first->writable(at,static_cast<U32>(n)):pair.first->readable(at,static_cast<U32>(n)));};
 std::pair<Services*,bool> context{&io,writing};
 if(writing)return original_crt32::writable_pointer_valid(reinterpret_cast<void*>(static_cast<std::uintptr_t>(address)),count,callback,&context);
 return original_crt32::readable_pointer_valid(reinterpret_cast<const void*>(static_cast<std::uintptr_t>(address)),count,callback,&context);
}
U32 adjust(U32 object,U32 type,Services& io){return original_startup11::adjust_pointer(object,{signed32(io.read32(type+8)),signed32(io.read32(type+12)),signed32(io.read32(type+16))},io);}
void copy(U32 to,U32 from,U32 n,Services& io){auto dest=io.bytes(to,n),source=io.bytes(from,n);original_string8::move_bytes(dest.data(),source.data(),n);}
std::string name(U32 at,Services& io){std::string result;for(U32 i=0;i<1048576;++i){auto c=byte(io,at+i);if(!c)return result;result.push_back(static_cast<char>(c));}throw std::out_of_range("unterminated guest RTTI name");}
bool matches(U32 handler,U32 catchable,U32 info,Services& io){
 const U32 first=io.read32(handler+4);if(!first||!byte(io,first+8))return true;
 const U32 second=io.read32(catchable+4);std::string a=name(first+8,io),b=name(second+8,io);
 original_runtime16::Type ta{a.c_str()},tb{b.c_str()};
 return original_runtime16::catch_type_matches({static_cast<std::uint8_t>(byte(io,handler)),&ta},{static_cast<std::uint8_t>(byte(io,catchable)),first==second?&ta:&tb},io.read32(info));
}
}
void copy_catch(State& state,U32 record,U32 frame,U32 handler,U32 catchable,Services& io){
 Link link(state);const U32 type=io.read32(handler+4);
 if(!type||!byte(io,type+8))return;
 const U32 offset=io.read32(handler+8);if(!offset)return;
 link.registration.level=0;const U32 target=frame+12+offset;
 if(!probe(io.read32(record+24),1,false,io)||!probe(target,1,true,io))unexpected(state,io);
 const U32 object=io.read32(record+24);
 if(byte(io,handler)&8){io.write32(target,object);io.write32(target,adjust(object,catchable,io));return;}
 const U32 length=io.read32(catchable+20);
 if(byte(io,catchable)&1){
  copy(target,object,length,io);
  if(length==4){const U32 pointer=io.read32(target);if(pointer)io.write32(target,adjust(pointer,catchable,io));}
  return;
 }
 const U32 constructor=io.read32(catchable+24);
 if(!constructor){copy(target,adjust(object,catchable,io),length,io);return;}
 if(!original_crt32::executable_pointer_valid(reinterpret_cast<const void*>(static_cast<std::uintptr_t>(constructor)),[](const void* p,void* context){return !static_cast<Services*>(context)->executable(static_cast<U32>(reinterpret_cast<std::uintptr_t>(p)));},&io))unexpected(state,io);
 const U32 source=adjust(io.read32(record+24),catchable,io);
 if(byte(io,catchable)&4){const std::array<U32,2> args{source,1};(void)call_object_virtual(target,constructor,args,io);}
 else {const std::array<U32,1> args{source};(void)call_object(target,constructor,args,io);}
}
void finish_catch(State& state,const Cleanup& cleanup,Services& io){
 io.write32(cleanup.frame-4,cleanup.saved_stack);io.write32(active_record,cleanup.previous_record);io.write32(active_context,cleanup.previous_context);
 if(cpp_record(cleanup.record,io)&&!cleanup.rethrow&&cleanup.result){(void)abnormal_termination(state,io);destroy_exception(state,cleanup.record,io);}
}
[[noreturn]] void resume_catch(State& state,U32 target,U32 frame,Services& io){
 if(!state.chain)throw std::invalid_argument("catch continuation requires registration");
 state.chain=state.chain->previous;io.resume(target,io.read32(frame-4),frame+12);
 throw std::logic_error("continuation returned");
}
U32 translate(State& state,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,I32 nesting,U32 dispatch_frame,Services& io){
 (void)dispatcher;Translation registration;registration.previous=state.chain;registration.handler=0x42fddb;registration.level=nesting;registration.frame=frame;registration.function_info=info;registration.dispatch_frame=dispatch_frame;
 const auto stack=io.stack_context();registration.saved_stack=stack[0];registration.saved_base=stack[1];Registration* const previous=state.chain;state.chain=&registration;bool handled=false;
 try {
  try {io.invoke_translation(io.read32(translator),io.read32(record),record,context,registration);}
  catch(const Raised& error){(void)translator_handler(state,error.record,registration,error.context,io);}
 }catch(const ResumeTransfer& transfer){
  if(transfer.target!=registration.return_target || transfer.stack!=registration.saved_stack || transfer.base!=registration.saved_base){state.chain=previous;throw;}
  handled=true;
 }catch(...){state.chain=previous;throw;}
 if(registration.unwound && previous && state.chain)previous->previous=state.chain->previous;
 state.chain=previous;return handled?1:0;
}
U32 dispatch(State& state,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,I32 nesting,U32 dispatch_frame,bool translated,Services& io){
 if(io.read32(info)!=magic)unexpected(state,io);
 if(!(io.read32(record+4)&0x66)){
  if(io.read32(info+12)){
   if(io.read32(record)==cpp && io.read32(record+20)>magic){const U32 target=io.read32(io.read32(record+28)+8);if(target)return io.invoke_dispatch(target,{record,frame,context,dispatcher,info,static_cast<U32>(nesting),dispatch_frame,static_cast<U32>(translated)});}
   search(state,record,frame,context,dispatcher,info,translated,nesting,dispatch_frame,io);
  }
 }else if(io.read32(info+4)&&nesting==0)unwind_cpp(state,frame,dispatcher,info,-1,io);
 return 1;
}
void enter_catch(State& state,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,U32 handler,U32 catchable,U32 try_block,I32 nesting,U32 dispatch_frame,Services& io){
 if(catchable)copy_catch(state,record,frame,handler,catchable,io);
 unwind_to(state,dispatch_frame?dispatch_frame:frame,record,io);
 unwind_cpp(state,frame,dispatcher,info,signed32(io.read32(try_block)),io);
 io.write32(frame+8,io.read32(try_block+4)+1);
 const U32 target=invoke_catch(state,record,frame,context,info,io.read32(handler+12),nesting,0x100,io);
 if(target)resume_catch(state,target,frame,io);
}
void search(State& state,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,bool translated,I32 nesting,U32 dispatch_frame,Services& io){
 const I32 level=signed32(io.read32(frame+8));if(level < -1 || signed32(io.read32(info+4))<=level)unexpected(state,io);
 if(io.read32(record)==cpp){
  if(rethrow(record,io)){
   record=io.read32(active_record);if(!record)return;
   context=io.read32(active_context);if(!probe(record,1,false,io))unexpected(state,io);
   if(rethrow(record,io))unexpected(state,io);
  }
  if(cpp_record(record,io)){
   U32 first{},last{};U32 block=find_try_range(state,info,nesting,level,first,last,io);
   for(;first<last;++first,block+=20){
    if(signed32(io.read32(block))<=level && level<=signed32(io.read32(block+4))){
     U32 handler=io.read32(block+16);bool found=false;
     for(I32 remaining=signed32(io.read32(block+12));remaining>0 && !found;--remaining,handler+=16){
      const U32 list=io.read32(io.read32(record+28)+12);U32 item=list+4;
      for(I32 n=signed32(io.read32(list));n>0;--n,item+=4){const U32 type=io.read32(item);if(matches(handler,type,io.read32(record+28),io)){enter_catch(state,record,frame,context,dispatcher,info,handler,type,block,nesting,dispatch_frame,io);found=true;break;}}
     }
    }
   }
   if(translated)destroy_exception(state,record,io);return;
  }
 }
 if(!translated)foreign_exception(state,record,frame,context,dispatcher,info,level,nesting,dispatch_frame,io);else terminate(state,io);
}
U32 invoke_catch(State& state,U32 record,U32 frame,U32 context,U32 info,U32 target,I32 nesting,U32 status,Services& io){
 Link link(state);Cleanup cleanup{record,frame,io.read32(frame-4),io.read32(active_record),io.read32(active_context),target,false};
 io.write32(active_record,record);io.write32(active_context,context);link.registration.level=1;
 try {cleanup.result=call_catch(state,frame,info,target,nesting,status,io);}
 catch(const Raised& error){
  if(rethrow(error.record,io))cleanup.result=0;else {finish_catch(state,cleanup,io);throw;}
 }catch(...){finish_catch(state,cleanup,io);throw;}
 link.registration.level=-1;finish_catch(state,cleanup,io);return cleanup.result;
}
void foreign_exception(State& state,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,I32 level,I32 nesting,U32 dispatch_frame,Services& io){
 if(io.read32(translator) && translate(state,record,frame,context,dispatcher,info,nesting,dispatch_frame,io))return;
 U32 first{},last{};U32 block=find_try_range(state,info,nesting,level,first,last,io);
 for(;first<last;++first,block+=20){
  if(signed32(io.read32(block))<=level && level<=signed32(io.read32(block+4))){
   const U32 end=io.read32(block+12)*16+io.read32(block+16),type=io.read32(end-12);
   if(!type||!byte(io,type+8))enter_catch(state,record,frame,context,dispatcher,info,end-16,0,block,nesting,dispatch_frame,io);
  }
 }
}
U32 translator_handler(State& state,U32 record,Translation& registration,U32 context,Services& io){
 if(io.read32(record+4)&0x66){registration.unwound=true;return 1;}
 (void)dispatch(state,record,registration.frame,context,0,registration.function_info,registration.level,registration.dispatch_frame,true,io);
 if(!registration.unwound)unwind_to(state,io.registration_address(registration),record,io);
 io.resume(registration.return_target,registration.saved_stack,registration.saved_base);throw std::logic_error("translation continuation returned");
}
U32 frame_handler(State& state,U32 record,U32 frame,U32 context,U32 dispatcher,U32 info,Services& io){return dispatch(state,record,frame,context,dispatcher,info,0,0,false,io);}
U32 catch_handler(State& state,U32 record,U32 registration,U32 context,Services& io){return dispatch(state,record,io.read32(registration+12),context,0,io.read32(registration+8),signed32(io.read32(registration+16)),registration,false,io);}
}
