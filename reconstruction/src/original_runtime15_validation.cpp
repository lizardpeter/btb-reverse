#include "btb/original_runtime15.hpp"
#include <stdexcept>
namespace btb::original_runtime15 {
bool verify_8_original_runtime_functions() {
    struct Time final:TimeServices {void initialize_timezone(TimeState&) override{}} time;
    TimeState s;s.daylight=0;
    if(calendar_to_time(s,1970,1,1,0,0,0,0,time,{0,0})!=0 || s.initialized!=1)return false;
    compute_transition(s,true,true,102,4,1,0,0,2,0,0,0);
    if(s.start.day!=96 || s.start.milliseconds!=7200000)return false;
    std::array<I32,9> tm{};tm[5]=102;tm[7]=180;s.daylight=1;
    if(!is_daylight_time(s,tm))return false;
    struct Signals final:SignalServices {
        I32 called{};void invoke(U32,I32 sig) override{called=sig;}
        void invoke_floating(U32,I32 sig,I32) override{called=sig;}
        [[noreturn]] void exit_process(I32) override{throw std::logic_error("unexpected exit");}
        I32 unhandled_exception(U32) override{return 42;}
        bool valid_handler(U32) override{return true;}
        I32 invoke_unhandled(U32,U32) override{return 23;}
        [[noreturn]] void terminate_process() override{throw std::logic_error("unexpected terminate");}
    } signals;
    std::array<ActionRecord,3> actions{ActionRecord{0xc000001d,4,100},ActionRecord{0xc000008e,8,100},ActionRecord{0xc0000005,11,100}};
    SignalState state;state.actions=actions;state.exception_pointers=22;state.unhandled_hook=100;
    if(raise_signal(state,4,signals)!=0 || signals.called!=4 || state.exception_pointers!=22)return false;
    if(exception_to_signal(state,0xc0000005,33,signals)!=-1 || signals.called!=11 || state.exception_pointers!=22)return false;
    if(cpp_unhandled_filter(state,{},33,signals)!=23)return false;
    struct Input final:InputServices {
        std::array<std::byte,4096> bytes{};
        void* allocate(std::size_t) override{return bytes.data();}
        I32 read_descriptor(U32,std::byte* out,U32) override{out[0]=std::byte{'a'};out[1]=std::byte{'b'};return 2;}
        std::uint8_t descriptor_flags(U32) override{return 0x82;}
    } input;
    original_crt10::Stream stream;stream.flags=1;U32 allocated=0;
    if(refill_input(stream,0,allocated,input)!='a' || allocated!=1 || stream.count!=1 || !(stream.flags&0x2000))return false;
    if(read_buffered_byte(stream,0,allocated,input)!='b' || stream.count!=0)return false;
    return push_back_byte('z',stream,allocated,input)=='z' && read_buffered_byte(stream,0,allocated,input)=='z';
}
}
