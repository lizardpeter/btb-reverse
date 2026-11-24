#include "btb/original_crt_batch_32.hpp"
#include "btb/original_float6.hpp"
#include <bit>
#include <cstring>
#include <limits>
namespace btb::original_crt32 {
char* copy_cstr(char* dst,const char* src) noexcept {
    char* head=dst;
    for(;;){const auto ch=*src++;*dst++=ch;if(ch=='\0')return head;}
}
void zero_words(Words96& v) noexcept {v[0]=0;v[1]=0;v[2]=0;}
void copy_words(Words96& dst,const Words96& src) noexcept {
    // Original is three sequential dword copies.
    for(unsigned i=0;i<3;++i)dst[i]=src[i];
}
bool are_words_zero(const Words96& v) noexcept {
    for(auto x:v)if(x!=0)return false;
    return true;
}
U32 add32_with_carry(U32 a,U32 b,U32& out) noexcept {
    out=a+b;
    return out<a||out<b?1U:0U;
}
void shift_left_one(Words96& w) noexcept {
    const auto low=w[0],mid=w[1];
    w[0]=low<<1U;
    w[1]=(mid<<1U)|(low>>31U);
    w[2]=(w[2]<<1U)|(mid>>31U);
}
void shift_right_one(Words96& w) noexcept {
    const auto mid=w[1];
    w[1]=(mid>>1U)|(w[2]<<31U);
    w[2]>>=1U;
    w[0]=(w[0]>>1U)|(mid<<31U);
}
bool higher_parts_zero(const Words96& w,unsigned index) noexcept {
    if(index>=96U)return false; // safe host input check
    const auto limb=index/32U,bit=31U-(index%32U);
    const auto mask=bit==0?0U:((U32{1}<<bit)-1U);
    if((w[limb]&mask)!=0)return false;
    for(auto i=limb+1;i<3;++i)if(w[i]!=0)return false;
    return true;
}
bool increment_masked(Words96& w,unsigned index) noexcept {
    if(index>=96U)return false; // safe host input check
    const auto limb=index/32U;
    U32 sum{};
    U32 carry=add32_with_carry(
        w[limb],U32{1}<<(31U-index%32U),sum);
    w[limb]=sum;
    for(int i=static_cast<int>(limb)-1;i>=0&&carry!=0;--i){
        carry=add32_with_carry(w[static_cast<std::size_t>(i)],1,sum);
        w[static_cast<std::size_t>(i)]=sum;
    }
    return carry!=0;
}
void add_words(Words96& lhs,const Words96& rhs) noexcept {
    U32 carry=0;
    for(unsigned i=0;i<3;++i){
        const U64 sum=static_cast<U64>(lhs[i])+rhs[i]+carry;
        lhs[i]=static_cast<U32>(sum);
        carry=static_cast<U32>(sum>>32U);
    }
}
U64 msvc_allmul(U32 alo,U32 ahi,U32 blo,U32 bhi) noexcept {
    return (((static_cast<U64>(ahi)<<32U)|alo)*
           ((static_cast<U64>(bhi)<<32U)|blo));
}
U64 msvc_allshl(U32 lo,U32 hi,unsigned shift) noexcept {
    if(shift>=64U)return 0;
    return ((static_cast<U64>(hi)<<32U)|lo)<<shift;
}
void* msvc_memset(void* dst,int byte,std::size_t count) noexcept {
    auto* out=static_cast<U8*>(dst);
    for(std::size_t i=0;i<count;++i)out[i]=static_cast<U8>(byte);
    return dst;
}
std::size_t bounded_length(const char* str,std::size_t maximum) noexcept {
    std::size_t i=0;
    while(i<maximum&&str[i]!='\0')++i;
    return i;
}
char* duplicate_cstr(const char* str,Alloc allocate,void* ctx) noexcept {
    if(str==nullptr||allocate==nullptr)return nullptr;
    const auto size=std::strlen(str);
    if(size==std::numeric_limits<std::size_t>::max())return nullptr;
    auto* dst=static_cast<char*>(allocate(size+1U,ctx));
    return dst==nullptr?nullptr:copy_cstr(dst,str);
}
U32 map_runtime_code(U32 input) noexcept {
    switch(input){
    case 0x3a4:return 0x411;
    case 0x3a8:return 0x804;
    case 0x3b5:return 0x412;
    case 0x3b6:return 0x404;
    default:return 0;
    }
}
bool executable_pointer_valid(
    const void* p,BadCodeProbe probe,void* ctx) noexcept {
    return probe!=nullptr&&!probe(p,ctx);
}
bool readable_pointer_valid(
    const void* p,std::size_t size,BadRangeProbe probe,void* ctx) noexcept {
    return probe!=nullptr&&!probe(p,size,ctx);
}
bool writable_pointer_valid(
    void* p,std::size_t size,BadRangeProbe probe,void* ctx) noexcept {
    return probe!=nullptr&&!probe(p,size,ctx);
}
bool char_class_matches(
    U8 code,U32 mask,U8 category,std::span<const U8> flags,
    std::span<const U16> classmask) noexcept {
    if(static_cast<std::size_t>(code)+1U>=flags.size() ||
       code>=classmask.size())return false;
    return (flags[static_cast<std::size_t>(code)+1U]&category)!=0 ||
           (mask!=0&&(static_cast<U32>(classmask[code])&mask)!=0);
}
const RegionRow* find_allocation_region(
    U32 target,std::span<const RegionRow> rows) noexcept {
    for(const auto& row:rows){
        // Original unsigned subtraction is intentionally wraparound.
        if(target-row[3]<0x100000U)return &row;
    }
    return nullptr;
}
void run_initializers(std::span<const Initializer> functions){
    for(const auto fn:functions)if(fn)fn();
}
void forward_if_not_eof(U32 code,InputCallback call,void* ctx) {
    if(code!=0xffffffffU&&call)call(code,ctx);
}
void write_stream_byte(
    U32 code,WriteCursor& cur,OverflowWrite overflow,void* ctx){
    --cur.remaining;
    U32 result=code&0xffU;
    if(cur.remaining<0 || cur.offset>=cur.buffer.size())
        result=overflow?overflow(code,ctx):0xffffffffU;
    else cur.buffer[cur.offset++]=static_cast<U8>(code);
    if(result==0xffffffffU){cur.total=-1;return;}
    ++cur.total;
}
void repeat_stream_byte(
    U32 code,I32 count,WriteCursor& cur,OverflowWrite overflow,void* ctx){
    for(I32 i=0;i<count;++i){
        write_stream_byte(code,cur,overflow,ctx);
        if(cur.total==-1)return;
    }
}
void unwind_frame(const std::array<U32,8>& frame,LocalUnwind unwind,void* ctx){
    if(unwind)unwind(frame[6],frame[7],ctx);
}
LegacyVptrObject* construct_object_a(
    LegacyVptrObject& obj,ObjectInitializer initialize,void* ctx){
    if(initialize)initialize(ctx);
    obj.vtable=0x0043bb54U;
    return &obj;
}
LegacyVptrObject* construct_object_b(
    LegacyVptrObject& obj,ObjectInitializer initialize,void* ctx){
    if(initialize)initialize(ctx);
    obj.vtable=0x0043bb84U;
    return &obj;
}
void decode_first(const U16* input,U32* output){
    original_float6::Extended12 value;
    for(unsigned i=0;i<6;++i)value[i]=input[i];
    (void)original_float6::convert_extended(value,std::span<U32>{output,2},original_float6::double_format);
}
void decode_second(const U16* input,U32* output){
    original_float6::Extended12 value;
    for(unsigned i=0;i<6;++i)value[i]=input[i];
    (void)original_float6::convert_extended(value,std::span<U32>{output,1},original_float6::float_format);
}
void decode_first(U16* dst,U32* state,TableDecode fn,void* ctx){
    if(fn)fn(dst,state,0x00449f48U,ctx);
}
void decode_second(U16* dst,U32* state,TableDecode fn,void* ctx){
    if(fn)fn(dst,state,0x00449f60U,ctx);
}
void decode_forward_zero(
    void* object,U8* src,I32* state,void* dest,Decoder fn,void* ctx){
    if(fn)fn(object,src,state,dest,0,ctx);
}
void update_fpu_control(U32 requested,U32 mask,FpuControl fn,void* ctx){
    if(fn)fn(requested,mask&0xfff7ffffU,ctx);
}
namespace {
void count_initializer(){}
void fake_init(void* p){++*static_cast<I32*>(p);}
void* allocate(std::size_t bytes,void* p){
    if(bytes>32)return nullptr;
    return static_cast<void*>(static_cast<char*>(p));
}
bool good_code(const void* p,void*){return p==nullptr;}
bool good_range(const void* p,std::size_t s,void*){return p==nullptr||s>4;}
void record_input(U32 code,void* p){*static_cast<U32*>(p)=code;}
U32 record_overflow(U32,void* p){++*static_cast<I32*>(p);return 0xffffffffU;}
void record_unwind(U32 a,U32 b,void* p){*static_cast<U32*>(p)=a+b;}
void record_table(U16*,U32*,U32 address,void* p){*static_cast<U32*>(p)=address;}
void record_decode(void*,U8*,I32*,void*,I32 final,void* p){
    *static_cast<I32*>(p)=final;
}
void record_fpu(U32 a,U32 mask,void* p){
    *static_cast<U32*>(p)=a^mask;
}
}
bool verify_32_original_crt_functions() noexcept {
    char dst[32]{};
    if(copy_cstr(dst,"BTB")!=dst || std::strcmp(dst,"BTB")!=0)return false;
    Words96 w{1,2,3};
    zero_words(w);
    if(!are_words_zero(w))return false;
    const Words96 src{0xffffffffU,0,0};
    copy_words(w,src);
    if(are_words_zero(w)||w!=src)return false;
    U32 sum=0;
    if(add32_with_carry(0xffffffffU,1,sum)!=1||sum!=0)return false;
    shift_left_one(w);
    if(w[0]!=0xfffffffeU||w[1]!=1U)return false;
    shift_right_one(w);
    if(w!=src || !higher_parts_zero(w,31U))return false;
    w={0xffffffffU,0U,0U};
    if(!increment_masked(w,0)||w[0]!=0x7fffffffU)return false;
    w={0xffffffffU,1U,1U};
    add_words(w,Words96{1U,2U,3U});
    if(w!=Words96{0U,4U,4U})return false;
    if(msvc_allmul(0xffffffffU,0U,2U,0U)!=0x1fffffffeULL ||
       msvc_allshl(1U,0U,32U)!=0x100000000ULL ||
       msvc_allshl(3U,0U,64U)!=0)return false;
    U8 buffer[4]{};
    if(msvc_memset(buffer,0xab,4)!=buffer ||
       buffer[0]!=0xab||buffer[3]!=0xab ||
       bounded_length("ABCDE",3)!=3 ||
       bounded_length("ABC",10)!=3)return false;
    char duplicate_buffer[32]{};
    if(duplicate_cstr("TEST",allocate,duplicate_buffer)!=duplicate_buffer ||
       std::strcmp(duplicate_buffer,"TEST")!=0)return false;
    if(map_runtime_code(0x3a4)!=0x411 || map_runtime_code(0x3b6)!=0x404 ||
       map_runtime_code(2)!=0)return false;
    if(!executable_pointer_valid(buffer,good_code,nullptr) ||
       executable_pointer_valid(nullptr,good_code,nullptr) ||
       !readable_pointer_valid(buffer,4,good_range,nullptr) ||
       !writable_pointer_valid(buffer,4,good_range,nullptr))return false;
    std::array<U8,3> category{0,4,0};
    std::array<U16,2> classmask{0x20,0};
    if(!char_class_matches(0,0,4,category,classmask) ||
       !char_class_matches(0,0x20,0,category,classmask) ||
       char_class_matches(1,0,4,category,classmask))return false;
    const std::array<RegionRow,2> regions{
        RegionRow{0,0,0,0x100000U,0},
        RegionRow{0,0,0,0x400000U,0}};
    if(find_allocation_region(0x100005U,regions)!=&regions[0] ||
       find_allocation_region(0x800000U,regions)!=nullptr)return false;
    constexpr Initializer table[]{nullptr,count_initializer};
    run_initializers(table);
    U32 seen=0;
    forward_if_not_eof(0xffffffffU,record_input,&seen);
    if(seen!=0)return false;
    forward_if_not_eof(0x44U,record_input,&seen);
    if(seen!=0x44)return false;
    U8 bytes[3]{};
    WriteCursor cur{bytes,0,3,0};
    I32 overflow_count=0;
    write_stream_byte('A',cur,record_overflow,&overflow_count);
    repeat_stream_byte('Z',2,cur,record_overflow,&overflow_count);
    if(cur.total!=3||cur.offset!=3||bytes[0]!='A'||bytes[2]!='Z')
        return false;
    write_stream_byte('Q',cur,record_overflow,&overflow_count);
    if(cur.total!=-1||overflow_count!=1)return false;
    std::array<U32,8> frame{};
    frame[6]=15;frame[7]=27;
    U32 unwind_seen=0;
    unwind_frame(frame,record_unwind,&unwind_seen);
    if(unwind_seen!=42)return false;
    LegacyVptrObject obj{};
    I32 inits=0;
    if(construct_object_a(obj,fake_init,&inits)!=&obj ||
       obj.vtable!=0x0043bb54U)return false;
    if(construct_object_b(obj,fake_init,&inits)!=&obj ||
       obj.vtable!=0x0043bb84U||inits!=2)return false;
    U16 decoded[1]{};
    U32 state=0,decoded_table=0;
    decode_first(decoded,&state,record_table,&decoded_table);
    if(decoded_table!=0x00449f48U)return false;
    decode_second(decoded,&state,record_table,&decoded_table);
    if(decoded_table!=0x00449f60U)return false;
    I32 final=999;
    decode_forward_zero(&obj,bytes,&final,buffer,record_decode,&final);
    if(final!=0)return false;
    U32 fpu=0;
    update_fpu_control(0x111U,0xffffffffU,record_fpu,&fpu);
    return fpu==(0x111U^0xfff7ffffU);
}
} // namespace btb::original_crt32
