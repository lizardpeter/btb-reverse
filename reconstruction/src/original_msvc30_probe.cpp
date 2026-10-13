#include "btb/original_msvc30.hpp"
#include <array>
#include <cstring>
namespace btb::original_msvc30 {
namespace {
struct Events {I32 seen[12]{};I32 count{};U32 last{};};
void exit_hook(U32 code,U32 cleanup,U32 reserved,void* p){
    auto& e=*static_cast<Events*>(p);
    e.seen[e.count++]=static_cast<I32>(code);
    e.seen[e.count++]=static_cast<I32>(cleanup);e.last=reserved;
}
void notice(U32 code,void* p){
    auto& e=*static_cast<Events*>(p);e.seen[e.count++]=static_cast<I32>(code);
}
U32 classify(U32 c,U32 mask,U32 kind,void*) {
    return c==3&&mask==0&&kind==4?11U:0U;
}
void control(void*,U32 desired,U32 mask,void* p){
    auto& e=*static_cast<Events*>(p);e.last=desired;
    e.seen[e.count++]=static_cast<I32>(mask);
}
void initialize(I32 arg,void* p){
    auto& e=*static_cast<Events*>(p);e.seen[e.count++]=arg;
}
U32 refill(void* p){++static_cast<Events*>(p)->count;return 0xffffffffU;}
I32 predicate(U32 code,void*){return code==3?7:0;}
void destruct(void*,void* p){
    auto& e=*static_cast<Events*>(p);e.seen[e.count++]=1;
}
void deallocate(void*,void* p){
    auto& e=*static_cast<Events*>(p);e.seen[e.count++]=2;
}
void prepare(void* obj,void* p){
    auto& e=*static_cast<Events*>(p);
    e.seen[e.count++]=3;static_cast<U32*>(obj)[0]=22;
}
void thrower(void* obj,U32 table,void* p){
    auto& e=*static_cast<Events*>(p);
    e.seen[e.count++]=4;e.last=table;
    e.seen[e.count++]=static_cast<I32>(static_cast<U32*>(obj)[0]);
}
U32 is_letter(U32 v,void*){return v=='A'?1:0;}
void converter(U32 a,void*,I32,I32,void* p){static_cast<Events*>(p)->last=a;}
U32 multibyte(U32 code,U32 mask,void* p){
    static_cast<Events*>(p)->last=mask;
    return code==5?mask:0;
}
void format_setup(void*,std::array<U16,6>& s,U8*,void* p){
    s[0]=19;++static_cast<Events*>(p)->count;
}
void format_finish(std::array<U16,6>& s,U32* target,U32 table,void* p){
    *target=table+s[0];++static_cast<Events*>(p)->count;
}
U32 read_control(void*){return 0xabcd1234U;}
void write_control(U32 value,void* p){static_cast<Events*>(p)->last=value;}
void global_func(){}
void write_char(U32 c,I32& total,void* p){
    static_cast<Events*>(p)->last=c;++total;
}
void flush(void*,void* p){++static_cast<Events*>(p)->count;}
}
bool verify_30_original_msvc_functions(){
    Events e{};
    exit_immediately(5,exit_hook,&e);
    if(e.count!=2||e.seen[0]!=5||e.seen[1]!=0)return false;
    e={};exit_with_cleanup(6,exit_hook,&e);
    if(e.seen[1]!=1)return false;
    if(class_category_four(3,classify,&e)!=11)return false;
    e={};set_precision_control(nullptr,control,&e);
    if(e.last!=0x10000U||e.seen[0]!=0x30000)return false;
    U32 latch=0;e={};
    if(!initialize_zero(latch,initialize,&e)||latch!=1||e.seen[0]!=0||
       initialize_zero(latch,initialize,&e))return false;
    latch=0;e={};
    if(!initialize_minus_three(latch,initialize,&e)||
       e.seen[0]!=-3||latch!=1)return false;
    e={};fatal_runtime_error(notice,notice,exit_hook,&e);
    if(e.count!=4||e.seen[0]!=10||e.seen[1]!=22||
       e.seen[2]!=3||e.seen[3]!=1)return false;
    const U8 bytes[]{'A','B','C'};
    e={};InputCursor input{bytes,2,&e};
    if(read_input_byte(input,refill)!='A'||
       read_input_byte(input,refill)!='B'||
       read_input_byte(input,refill)!=0xffffffffU||e.count!=1)return false;
    if(predicate_normalized(3,predicate,&e)!=1||
       predicate_normalized(4,predicate,&e)!=0)return false;
    I32 obj=7;e={};
    if(destroy_exception(&obj,1,destruct,deallocate,&e)!=&obj ||
       e.count!=2||e.seen[0]!=1||e.seen[1]!=2)return false;
    e={};
    if(destroy_type_info(&obj,0,destruct,deallocate,&e)!=&obj||
       e.count!=1)return false;
    e={};
    if(destroy_format_exception(&obj,1,destruct,deallocate,&e)!=&obj||
       e.count!=2)return false;
    std::array<U32,7> local{};
    e={};throw_format(local,prepare,thrower,&e);
    if(e.count!=3||e.last!=0x0043c158U)return false;
    e={};throw_class_a(local,prepare,thrower,&e);
    if(e.count!=3||e.last!=0x0043c0e8U)return false;
    e={};throw_class_b(local,prepare,thrower,&e);
    if(e.count!=3||e.last!=0x0043c210U)return false;
    e={};const U8 chars[]{'A','B'};
    InputCursor stream{chars,2,&e};
    U32 scanned=0;
    if(scan_until_class_fails(scanned,stream,refill,is_letter,&e)!='B' ||
       scanned!=2)return false;
    char shifted[24]="ABCD";
    shift_c_string(shifted,2);
    if(std::strcmp(shifted+2,"ABCD")!=0)return false;
    const std::array<HandleRow,2> handles{{{4,0x41,{}},{8,0,{}}}};
    if(handle_bit40(handles,0,2)!=0x40||
       handle_bit40(handles,1,2)!=0||
       handle_bit40(handles,3,2)!=0)return false;
    U32 flag=0;e={};
    with_conversion_flag(flag,83,nullptr,1,2,converter,&e);
    if(flag!=0||e.last!=83)return false;
    const std::array<U8,6> flags{0,8,0,4,0,8};
    e={};
    if(class_eight(1,1,flags,multibyte,&e)!=8||
       class_eight(5,2,flags,multibyte,&e)!=8||e.last!=8)return false;
    e={};U8 fmt_input[2]{};U32 formatted=0;
    format_first(nullptr,&formatted,fmt_input,format_setup,format_finish,&e);
    if(e.count!=2||formatted!=0x00449f48U+19)return false;
    e={};format_second(nullptr,&formatted,fmt_input,format_setup,format_finish,&e);
    if(e.count!=2||formatted!=0x00449f60U+19)return false;
    e={};
    constexpr U32 merged=(0xabcd1234U&~0x00ff00ffU)|
                          (0x12345678U&0x00ff00ffU);
    if(merge_control_word(0x12345678U,0x00ff00ffU,read_control,
                          write_control,&e)!=merged||e.last!=merged)
        return false;
    if(hex_fold('A',1,flags,multibyte,&e)!=(('A'&~0x20U)-7U))
        return false;
    const std::array<RuntimeFunction,6> funcs{
        global_func,global_func,global_func,global_func,global_func,global_func};
    ConversionFunctions installed{};
    install_conversion_functions(installed,funcs);
    if(installed.functions!=funcs)return false;
    e={};I32 total=0;
    write_string_bytes("Hello",5,total,write_char,&e);
    if(total!=5||e.last!='o')return false;
    StreamState st{11,22,33,0x1fU,44};e={};
    reset_buffered_stream(1,st,nullptr,flush,&e);
    if(e.count!=1||st.flags!=(0x1fU&0xeeU)||
       st.cursor!=0||st.remaining!=0||st.buffer_size!=0)return false;
    U32 os_error=22,error=22;
    if(native_handle_or_error(handles,0,2,os_error,error)!=4)return false;
    if(native_handle_or_error(handles,1,2,os_error,error)!=0xffffffffU||
       os_error!=0||error!=9)return false;
    if(compare_modulo(1,9,4)!=std::optional<bool>{false} ||
       compare_modulo(0,9,4)!=std::optional<bool>{true} ||
       compare_modulo(1,9,0).has_value())return false;
    const std::array<Triple,2> triples{{{7,8,9},{10,11,12}}};
    return find_triple(triples,10)==&triples[1]&&
           find_triple(triples,99)==nullptr;
}
}
