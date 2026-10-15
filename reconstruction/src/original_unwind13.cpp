#include "btb/original_unwind13.hpp"
#include "btb/original_tail4.hpp"
#include "btb/original_crt9.hpp"
#include "btb/original_msvc30.hpp"
#include <bit>
#include <stdexcept>
namespace btb::original_unwind13 {
namespace {
I32 signed32(U32 value) { return std::bit_cast<I32>(value); }
struct Link {
    State& state;
    Registration registration;
    Link(State& s,U32 handler,I32 level,U32 frame):state(s),registration{s.chain,handler,level,frame} {
        state.chain=&registration;
    }
    ~Link() { state.chain=registration.previous; }
};
U32 setting_frame(U32 target,U32 frame,U32 status,Services& io) {
    U32 result{};
    original_crt9::call_setting_frame(signed32(status),
        [&](I32 code){io.notify(target,frame+12,static_cast<U32>(code));},
        [&]{result=io.invoke_frame(target,frame+12);},
        [&](I32 code){io.notify(result,frame+12,static_cast<U32>(code));});
    return result;
}
void call_original_rtl_unwind(U32 frame,U32 ip,U32 record,U32 value,Services& io) {
    original_tail4::rtl_unwind_thunk(frame,ip,record,value,
        +[](U32 f,U32 instruction,U32 r,U32 v,void* ctx){
            static_cast<Services*>(ctx)->rtl_unwind(f,instruction,r,v);
        },&io);
}
void abort_runtime(Services& io) {
    original_msvc30::fatal_runtime_error(
        [](U32 code,void* p){static_cast<Services*>(p)->report_error(code);},
        [](U32 code,void* p){static_cast<Services*>(p)->raise_signal(code);},
        [](U32 code,U32 quick,U32 returning,void* p){static_cast<Services*>(p)->exit_process(code,quick,returning);},&io);
    throw std::logic_error("nonreturning runtime exit returned");
}
}
void throw_exception(U32 object,U32 info,Services& io) {
    // DAT_0043B5E0: C++ exception code, noncontinuable, three DWORD parameters.
    const std::array<U32,3> arguments{0x19930520U,object,info};
    io.raise_exception(0xe06d7363U,1,arguments);
}
U32 call_object(U32 object,U32 target,std::span<const U32> args,Services& io) {
    return io.invoke_this(target,object,args);
}
U32 call_object_virtual(U32 object,U32 target,std::span<const U32> args,Services& io) {
    return io.invoke_this(target,object,args);
}
void unwind_to(State& state,U32 frame,U32 record,Services& io) {
    Registration* const saved=state.chain;
    if(!saved)throw std::invalid_argument("original unwind requires a registration");
    call_original_rtl_unwind(frame,0x0042fc51,record,0,io);
    io.write32(record+4,io.read32(record+4)&~2U);
    saved->previous=state.chain;
    state.chain=saved;
}
U32 call_catch(State& state,U32 frame,U32 info,U32 target,I32 nesting,U32 status,Services& io) {
    Link link(state,0x0042fd02,signed32(static_cast<U32>(nesting)+1),frame);
    link.registration.function_info=info;
    return setting_frame(target,frame,status,io);
}
U32 find_try_range(State& state,U32 info,I32 nesting,I32 level,U32& first,U32& last,Services& io) {
    const U32 count=io.read32(info+12),table=io.read32(info+16);
    U32 index=count,upper=count,previous=count;
    while(nesting>=0) {
        if(index==0xffffffffU)unexpected(state,io);
        --index;
        const U32 at=table+index*20;
        if((signed32(io.read32(at+4))<level && level<=signed32(io.read32(at+8))) || index==0xffffffffU) {
            --nesting;upper=previous;previous=index;
        }
    }
    first=index+1;last=upper;
    if(count<upper || upper<first)unexpected(state,io);
    return table+first*20;
}
void global_unwind(U32 frame,Services& io) { call_original_rtl_unwind(frame,0x0042fee4,0,0,io); }
void local_unwind(State& state,U32 frame,I32 stop,U32 frame_base,Services& io) {
    Link link(state,0x0042feec,-2,frame);
    for(;;) {
        const U32 table=io.read32(frame+8);
        const I32 level=signed32(io.read32(frame+12));
        if(level==-1 || level==stop)return;
        const U32 entry=table+static_cast<U32>(level)*12;
        const U32 parent=io.read32(entry);
        link.registration.level=signed32(parent);
        io.write32(frame+12,parent);
        if(!io.read32(entry+4)) {
            const U32 target=io.read32(entry+8);
            // 0042FFA2 obtains the notification value from [EBP+8],
            // even though its caller pushes 0x101 (consumed by RET 4).
            io.notify(target,frame_base,io.read32(frame_base+8));
            (void)io.invoke_frame(target,frame_base);
        }
    }
}
bool abnormal_termination(const State& state,Services& io) {
    return state.chain && state.chain->handler==0x0042feec &&
        static_cast<U32>(state.chain->level)==io.read32(state.chain->frame+12);
}
void unwind_cpp(State& state,U32 frame,U32 context,U32 info,I32 stop,Services& io) {
    (void)context;
    Link link(state,0x00435f38,-1,frame);
    I32 level=signed32(io.read32(frame+8));
    while(level!=stop) {
        if(level<0 || signed32(io.read32(info+4))<=level)unexpected(state,io);
        const U32 entry=io.read32(info+8)+static_cast<U32>(level)*8;
        const U32 target=io.read32(entry+4);
        if(target) {
            link.registration.level=0;
            try {(void)setting_frame(target,frame,0x103,io);}
            catch(const GuestException& error) {
                if(error.code==0xe06d7363U)terminate(state,io);
                throw;
            }
            link.registration.level=-1;
        }
        // The next-state field is read AFTER the callback (it can mutate metadata).
        level=signed32(io.read32(io.read32(info+8)+static_cast<U32>(level)*8));
    }
    io.write32(frame+8,static_cast<U32>(level));
}
void destroy_exception(State& state,U32 record,Services& io) {
    Link link(state,0x00435f38,-1,record);
    if(record) {
        const U32 target=io.read32(io.read32(record+28)+4);
        if(target) {
            link.registration.level=0;
            (void)call_object(io.read32(record+24),target,{},io);
        }
    }
}
[[noreturn]] void terminate(State& state,Services& io) {
    Link link(state,0x00435f38,-1,0);
    if(state.terminate_handler) {
        link.registration.level=1;
        try { io.invoke_handler(state.terminate_handler); } catch(...) {}
    }
    link.registration.level=-1;
    abort_runtime(io);
    throw std::logic_error("terminate returned");
}
[[noreturn]] void unexpected(State& state,Services& io) {
    Link link(state,0x00435f38,-1,0);
    if(state.unexpected_handler) {
        link.registration.level=1;
        try { io.invoke_handler(state.unexpected_handler); } catch(...) {}
    }
    link.registration.level=-1;
    terminate(state,io);
}
}
