#include "btb/original_runtime15.hpp"
#include <bit>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace btb::original_runtime15;
U32 word(std::istream& in){U32 v{};in>>std::hex>>v;return v;}
I32 sint(std::istream& in){return std::bit_cast<I32>(word(in));}
Transition transition(std::istream& in){return {sint(in),sint(in),sint(in)};}
SystemTime system_time(std::istream& in){SystemTime t;for(auto* p:{&t.year,&t.month,&t.weekday,&t.day,&t.hour,&t.minute,&t.second,&t.milliseconds})*p=static_cast<std::uint16_t>(word(in));return t;}
struct Time final:TimeServices {int calls{};void initialize_timezone(TimeState&) override{++calls;}};
struct Signals final:SignalServices {
    SignalState& state;std::vector<U32> trace;explicit Signals(SignalState& s):state(s){}
    void invoke(U32,I32 sig) override{trace.insert(trace.end(),{1,static_cast<U32>(sig),0,state.exception_pointers,static_cast<U32>(state.floating_point_code)});}
    void invoke_floating(U32,I32 sig,I32 code) override{trace.insert(trace.end(),{1,static_cast<U32>(sig),static_cast<U32>(code),state.exception_pointers,static_cast<U32>(state.floating_point_code)});}
    [[noreturn]] void exit_process(I32 code) override{throw code;}
    I32 unhandled_exception(U32) override{return 42;}
    bool valid_handler(U32) override{return valid;}
    bool valid{true};
    I32 invoke_unhandled(U32,U32 ptr) override{trace.insert(trace.end(),{2,ptr,0,state.exception_pointers,static_cast<U32>(state.floating_point_code)});return 23;}
    [[noreturn]] void terminate_process() override{throw I32{99};}
};
struct Input final:InputServices {
    std::array<std::byte,4096> buffer{};bool okay{};I32 count{};std::uint8_t flags{};int reads{};
    void* allocate(std::size_t n) override{assert(n==4096);return okay?buffer.data():nullptr;}
    I32 read_descriptor(U32 fd,std::byte* out,U32 capacity) override{
        assert(fd==3);++reads;
        assert(count<=0 || static_cast<U32>(count)<=capacity);
        for(I32 i=0;i<count;++i)out[i]=std::byte{static_cast<std::uint8_t>('A'+i)};
        return count;
    }
    std::uint8_t descriptor_flags(U32 fd) override{assert(fd==3);return flags;}
};
int main(int argc,char** argv) {
    if(argc!=2 || !verify_8_original_runtime_functions())return 1;
    std::ifstream file(argv[1]);if(!file)return 2;std::string line,kind;unsigned checked=0;
    while(std::getline(file,line)) {
        std::istringstream in(line);in>>kind;bool okay=false;
        if(kind=="TRANS") {
            TimeState state;state.daylight_bias=sint(in);std::array<I32,11> args{};for(auto& v:args)v=sint(in);
            const auto start=transition(in),end=transition(in);
            compute_transition(state,args[0]!=0,args[1]!=0,args[2],args[3],args[4],args[5],args[6],args[7],args[8],args[9],args[10]);
            okay=state.start==start && state.end==end;
        } else if(kind=="DST") {
            TimeState state;state.daylight_bias=sint(in);state.daylight=sint(in);state.system_rules=word(in)!=0;
            state.daylight_date=system_time(in);state.standard_date=system_time(in);
            std::array<I32,9> tm{};for(auto& v:tm)v=sint(in);const bool expected=word(in)!=0;
            const auto start=transition(in),end=transition(in);
            okay=is_daylight_time(state,tm)==expected && state.start==start && state.end==end;
        } else if(kind=="CAL") {
            TimeState state;state.daylight_bias=sint(in);state.daylight=sint(in);state.timezone=sint(in);state.initialized=1;
            std::array<I32,2> prefix{sint(in),sint(in)};std::array<I32,7> args{};for(auto& v:args)v=sint(in);
            const I32 expected=sint(in);const auto start=transition(in),end=transition(in);Time service;
            const auto result=calendar_to_time(state,args[0],args[1],args[2],args[3],args[4],args[5],args[6],service,prefix);
            okay=result==expected && state.start==start && state.end==end && service.calls==0;
        } else if(kind=="IO") {
            const U32 mode=word(in);Input service;service.okay=word(in)!=0;service.count=sint(in);service.flags=static_cast<std::uint8_t>(word(in));
            const U32 value=word(in);const bool base=word(in)!=0;const auto offset=word(in);
            btb::original_crt10::Stream state;state.count=word(in);state.flags=word(in);state.capacity=word(in);
            for(unsigned i=0;i<4;++i)service.buffer[i]=std::byte{static_cast<std::uint8_t>('A'+i)};
            if(base){state.base=service.buffer.data();state.pointer=state.base+offset;}
            const U32 expected=word(in),alloc=word(in),ptr=word(in),count=word(in),flags=word(in),cap=word(in);
            const U32 b0=word(in),b1=word(in);U32 allocations=17;
            const U32 result=mode?refill_input(state,3,allocations,service):push_back_byte(value,state,allocations,service);
            const auto pointer=state.base?static_cast<U32>(state.pointer-state.base):0;
            okay=result==expected && allocations==alloc && pointer==ptr && state.count==count && state.flags==flags && state.capacity==cap &&
                (state.base?std::to_integer<U32>(state.base[0]):0)==b0 && (state.base?std::to_integer<U32>(state.base[1]):0)==b1;
        } else if(kind=="SIGNAL") {
            const U32 mode=word(in);const I32 sig=sint(in);const U32 choice=word(in),code=word(in);
            SignalState state;state.floating_first=sint(in);state.floating_count=1;state.floating_point_code=sint(in);
            state.exception_pointers=word(in);const U32 ptr=word(in),expected=word(in),saved=word(in);const I32 fpe=sint(in);
            std::array<ActionRecord,4> actions{ActionRecord{0xc000001d,4,choice},ActionRecord{0xc000008e,8,choice},ActionRecord{0xc0000005,11,choice},ActionRecord{0xc0000090,8,choice}};
            state.actions=actions;state.handlers.fill(choice);std::array<ActionRecord,4> after{};
            for(auto& a:after){a.code=word(in);a.signal=sint(in);a.action=word(in);}
            std::array<U32,4> handlers{};for(auto& v:handlers)v=word(in);
            const unsigned traces=word(in);std::vector<U32> trace;for(unsigned i=0;i<traces*5;++i)trace.push_back(word(in));
            Signals service(state);const U32 result=static_cast<U32>(mode?exception_to_signal(state,code,ptr,service):raise_signal(state,sig,service));
            okay=result==expected && state.exception_pointers==saved && state.floating_point_code==fpe && state.handlers==handlers && service.trace==trace;
            for(unsigned i=0;i<4;++i)okay=okay && actions[i].code==after[i].code && actions[i].signal==after[i].signal && actions[i].action==after[i].action;
        }
        if(!in || !okay){std::cerr<<"Mismatch at runtime vector "<<checked<<": "<<line<<'\n';return 3;}
        ++checked;
    }
    if(checked!=7500)return 4;
    // Required unhandled/exit providers and exception signature gates.
    std::array<ActionRecord,1> actions{ActionRecord{7,4,0}};SignalState state;state.actions=actions;Signals services(state);
    assert(exception_to_signal(state,99,8,services)==42);
    bool exited=false;try{raise_signal(state,4,services);}catch(I32 code){exited=code==3;}assert(exited);
    state.unhandled_hook=100;assert(cpp_unhandled_filter(state,{},8,services)==23);
    services.valid=false;assert(cpp_unhandled_filter(state,{},8,services)==0);
    bool terminated=false;try{cpp_unhandled_filter(state,{0xe06d7363,3,0x19930520},8,services);}catch(I32 code){terminated=code==99;}assert(terminated);
    TimeState time;time.daylight=0;Time tz;
    assert(calendar_to_time(time,1970,1,1,0,0,0,0,tz,{0,0})==0);
    assert(calendar_to_time(time,1970,1,2,0,0,0,0,tz,{0,0})==86400 && tz.calls==1);
    // Northern boundaries, then southern wrap-around with explicit cached transitions.
    time.daylight=1;time.start={102,100,7200000};time.end={102,300,3600000};std::array<I32,9> tm{};tm[5]=102;tm[7]=100;tm[2]=1;tm[1]=59;tm[0]=59;
    assert(!is_daylight_time(time,tm));tm[2]=2;tm[1]=0;tm[0]=0;assert(is_daylight_time(time,tm));tm[7]=300;tm[2]=1;assert(!is_daylight_time(time,tm));
    time.start={102,300,7200000};time.end={102,100,3600000};tm[7]=50;assert(is_daylight_time(time,tm));tm[7]=200;assert(!is_daylight_time(time,tm));
    // Existing input-byte caller now refills and shares the same stream state.
    Input input;input.okay=true;input.count=2;btb::original_crt10::Stream stream;stream.flags=1;U32 allocations=0;
    assert(read_buffered_byte(stream,3,allocations,input)=='A');assert(read_buffered_byte(stream,3,allocations,input)=='B');assert(input.reads==1);
    assert(push_back_byte('Z',stream,allocations,input)=='Z');assert(read_buffered_byte(stream,3,allocations,input)=='Z');
    btb::original_crt10::Stream rejected;assert(read_buffered_byte(rejected,3,allocations,input)==0xffffffff && rejected.count==0xffffffff);
    std::cout<<checked<<" original x86 runtime cases and connected boundary tests passed\n";
}
