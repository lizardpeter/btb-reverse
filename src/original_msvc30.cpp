#include "btb/original_msvc30.hpp"
#include <cstring>
#include <limits>
namespace btb::original_msvc30 {
void exit_immediately(U32 code,Exit exit,void* ctx){if(exit)exit(code,0,0,ctx);}
void exit_with_cleanup(U32 code,Exit exit,void* ctx){if(exit)exit(code,1,0,ctx);}
U32 class_category_four(U8 value,Classify fn,void* ctx){
    return fn?fn(value,0,4,ctx):0;
}
void set_precision_control(void* instance,Control fn,void* ctx){
    if(fn)fn(instance,0x10000U,0x30000U,ctx);
}
bool initialize_zero(U32& latch,Initializer fn,void* ctx){
    if(latch!=0)return false;
    if(fn)fn(0,ctx);
    ++latch;
    return true;
}
bool initialize_minus_three(U32& latch,Initializer fn,void* ctx){
    if(latch!=0)return false;
    if(fn)fn(-3,ctx);
    latch=1;
    return true;
}
void fatal_runtime_error(Notice first,Notice second,Exit exit,void* ctx){
    if(first)first(10,ctx);
    if(second)second(22,ctx);
    exit_with_cleanup(3,exit,ctx);
}
U32 read_input_byte(InputCursor& cursor,Refill refill){
    --cursor.remaining;
    if(cursor.remaining>=0&&cursor.current)return *cursor.current++;
    return refill?refill(cursor.ctx):0xffffffffU;
}
U32 predicate_normalized(U32 arg,Predicate fn,void* ctx){
    return fn&&fn(arg,ctx)!=0?1U:0U;
}
namespace {
void* deleting_destructor(void* p,U8 flags,Destructor destruct,
                          Destructor deallocate,void* ctx){
    if(destruct)destruct(p,ctx);
    if((flags&1U)!=0&&deallocate)deallocate(p,ctx);
    return p;
}
void raise_exception(std::array<U32,7>& local,U32 info,
                     Prepare prepare,Throw raise,void* ctx){
    if(prepare)prepare(local.data(),ctx);
    if(raise)raise(local.data(),info,ctx);
}
}
void* destroy_exception(void* p,U8 f,Destructor d,Destructor free,void* c){
    return deleting_destructor(p,f,d,free,c);
}
void* destroy_type_info(void* p,U8 f,Destructor d,Destructor free,void* c){
    return deleting_destructor(p,f,d,free,c);
}
void* destroy_format_exception(void* p,U8 f,Destructor d,Destructor free,void* c){
    return deleting_destructor(p,f,d,free,c);
}
void throw_format(std::array<U32,7>& local,Prepare init,Throw raise,void* ctx){
    raise_exception(local,0x0043c158U,init,raise,ctx);
}
void throw_class_a(std::array<U32,7>& local,Prepare init,Throw raise,void* ctx){
    raise_exception(local,0x0043c0e8U,init,raise,ctx);
}
void throw_class_b(std::array<U32,7>& local,Prepare init,Throw raise,void* ctx){
    raise_exception(local,0x0043c210U,init,raise,ctx);
}
U32 scan_until_class_fails(U32& scanned,InputCursor& stream,Refill refill,
                           ClassifyByte classifier,void* ctx){
    for(;;){
        ++scanned;
        const U32 value=read_input_byte(stream,refill);
        if(!classifier||classifier(value,ctx)==0)return value;
    }
}
void shift_c_string(char* text,I32 distance){
    if(distance!=0)std::memmove(text+distance,text,std::strlen(text)+1);
}
U8 handle_bit40(std::span<const HandleRow> rows,U32 index,U32 maximum){
    if(index>=maximum||index>=rows.size())return 0;
    return rows[index].flags&0x40U;
}
void with_conversion_flag(U32& flag,U32 a,void* b,I32 c,I32 d,
                          Conversion fn,void* ctx){
    flag=1;
    if(fn)fn(a,b,c,d,ctx);
    flag=0;
}
U32 class_eight(U32 value,I32 locale,std::span<const U8> flags,
                MaskClassify complex,void* ctx){
    if(locale>1)return complex?complex(value,8,ctx):0;
    return value<flags.size()?flags[value]&8U:0;
}
namespace {
void format_table(void* object,U32* result,U8* input,U32 table,
                  FormatSetup prepare,FormatFinish finish,void* ctx){
    std::array<U16,6> scratch{};
    if(prepare)prepare(object,scratch,input,ctx);
    if(finish)finish(scratch,result,table,ctx);
}
}
void format_first(void* obj,U32* out,U8* src,FormatSetup p,FormatFinish f,void* c){
    format_table(obj,out,src,0x00449f48U,p,f,c);
}
void format_second(void* obj,U32* out,U8* src,FormatSetup p,FormatFinish f,void* c){
    format_table(obj,out,src,0x00449f60U,p,f,c);
}
U32 merge_control_word(U32 desired,U32 mask,ReadControl read,
                       WriteControl write,void* ctx){
    const U32 before=read?read(ctx):0;
    const U32 after=(before&~mask)|(desired&mask);
    if(write)write(after,ctx);
    return after;
}
U32 hex_fold(U32 value,I32 locale,std::span<const U8> table,
             MaskClassify complex,void* ctx){
    const U32 class4=locale<2
        ? (value<table.size()?table[value]&4U:0U)
        : (complex?complex(value,4,ctx):0U);
    return class4?value:(value&~0x20U)-7U;
}
void install_conversion_functions(ConversionFunctions& target,
                                  const std::array<RuntimeFunction,6>& src){
    for(unsigned i=0;i<6;++i)target.functions[i]=src[i];
}
void write_string_bytes(const char* text,I32 length,I32& total,
                        WriteChar write,void* ctx){
    for(I32 i=0;i<length;++i){
        if(write)write(static_cast<U8>(text[i]),total,ctx);
        if(total==-1)return;
    }
}
void reset_buffered_stream(I32 mode,StreamState& state,
                           void* file,Flush flush,void* ctx){
    if((state.flags&0x10U)==0)return;
    if(flush)flush(file,ctx);
    if(mode==0)return;
    state.flags&=0xeeU;
    state.buffer_size=0;state.cursor=0;state.remaining=0;
}
U32 native_handle_or_error(std::span<const HandleRow> rows,
                           U32 index,U32 maximum,U32& os_error,U32& err){
    if(index<maximum&&index<rows.size()&&(rows[index].flags&1)!=0)
        return rows[index].value;
    os_error=0;err=9;
    return 0xffffffffU;
}
std::optional<bool> compare_modulo(I32 threshold,I32 value,I32 divisor) noexcept{
    if(divisor==0||(value==std::numeric_limits<I32>::min()&&divisor==-1))
        return std::nullopt;
    return threshold<(value%divisor);
}
const Triple* find_triple(std::span<const Triple> rows,I32 key) noexcept{
    for(const auto& r:rows)if(r.key==key)return &r;
    return nullptr;
}
}
