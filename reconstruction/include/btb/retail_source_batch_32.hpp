#pragma once
// 32 original PE32 functions reconstructed as typed C++26 source-level models.
// Primary executable SHA256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Source evidence: analysis/ghidra_main_exe/functions/<address>.c
// These are compilable semantics, not verified original x86 ABI or identical assembly.
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <span>

namespace btb::retail_source_batch_32 {
using U8=std::uint8_t;
using U16=std::uint16_t;
using U32=std::uint32_t;
using U64=std::uint64_t;
using I32=std::int32_t;
using Words96=std::array<U32,3>;

// 00433630 FUN_00433630: MSVC word-optimized strcpy semantics.
// The retail aligned-copy optimization reads ahead, which a portable host
// implementation deliberately does not repeat.
inline char* copy_zero_terminated_string(char* destination,const char* source) noexcept {
    char* const original=destination;
    for(;;) {const char byte=*source++;*destination++=byte;if(byte=='\0')return original;}
}

// 004377BA, 0043779F, 004377C6: 3-word zero/copy/all-zero routines.
constexpr void reset_words96(Words96& target) noexcept {target={0,0,0};}
constexpr void copy_words96(Words96& target,const Words96& source) noexcept {
    for(std::size_t i=0;i<3;++i)target[i]=source[i];
}
[[nodiscard]] constexpr bool words96_are_zero(const Words96& value) noexcept {
    return value[0]==0&&value[1]==0&&value[2]==0;
}

// 004387C7: 32-bit unsigned add, carry output into return value.
[[nodiscard]] constexpr U32 add32_with_carry(U32 lhs,U32 rhs,U32& result) noexcept {
    result=lhs+rhs;
    return result<lhs||result<rhs?1U:0U;
}

// 00438846 / 00438874: unsigned 96-bit shifts across three little-endian limbs.
constexpr void shift_words96_left_one(Words96& value) noexcept {
    const U32 low=value[0],middle=value[1];
    value[0]=low<<1;
    value[1]=(middle<<1)|(low>>31);
    value[2]=(value[2]<<1)|(middle>>31);
}
constexpr void shift_words96_right_one(Words96& value) noexcept {
    const U32 middle=value[1];
    value[1]=(middle>>1)|(value[2]<<31);
    value[2]>>=1;
    value[0]=(value[0]>>1)|(middle<<31);
}

// 00437674: test current limb's low-bit mask and every higher-address limb.
// param_2 is a bit index in 0..95; out-of-range indices are rejected.
[[nodiscard]] constexpr bool words96_high_parts_are_zero(
    const Words96& words,unsigned bit_index) noexcept {
    if(bit_index>=96)return false;
    const unsigned limb=bit_index/32;
    const unsigned shift=31U-(bit_index%32U);
    const U32 mask=shift==0?0U:((1U<<shift)-1U);
    if((words[limb]&mask)!=0)return false;
    for(unsigned i=limb+1;i<3;++i)if(words[i]!=0)return false;
    return true;
}

// 004376BD: add 1<<(31-(index%32)) to indexed limb and propagate
// carry toward earlier 32-bit words, as in the retail implementation.
constexpr bool increment_words96_masked(Words96& words,unsigned index) noexcept {
    if(index>=96)return false;
    const unsigned limb=index/32;
    const U32 bit=U32{1}<<(31-(index%32));
    U32 result{};
    U32 carry=add32_with_carry(words[limb],bit,result);
    words[limb]=result;
    for(int i=static_cast<int>(limb)-1;i>=0&&carry!=0;--i){
        carry=add32_with_carry(words[static_cast<unsigned>(i)],1, result);
        words[static_cast<unsigned>(i)]=result;
    }
    return carry!=0;
}

// 004387E8: ___add_12, modular addition of 3x32-bit little-endian words.
constexpr void add_words96(Words96& dst,const Words96& src) noexcept {
    U32 carry=0;
    for(unsigned i=0;i<3;++i) {
        const U64 sum=static_cast<U64>(dst[i])+src[i]+carry;
        dst[i]=static_cast<U32>(sum);
        carry=static_cast<U32>(sum>>32);
    }
}

// 00436640 / 00436680: VS2003 64-bit multiplication/unsigned left shift.
[[nodiscard]] constexpr U64 msvc_allmul(U32 alo,U32 ahi,U32 blo,U32 bhi) noexcept {
    const U64 a=(static_cast<U64>(ahi)<<32)|alo;
    const U64 b=(static_cast<U64>(bhi)<<32)|blo;
    return a*b;
}
[[nodiscard]] constexpr U64 msvc_allshl(U32 low,U32 high,unsigned shift) noexcept {
    if(shift>=64)return 0;
    const U64 value=(static_cast<U64>(high)<<32)|low;
    return value<<shift;
}

// 00436540: MSVC _memset; return original pointer even when count==0.
inline void* msvc_memset(void* destination,int byte,std::size_t count) noexcept {
    void* const original=destination;
    auto* ptr=static_cast<unsigned char*>(destination);
    const auto value=static_cast<unsigned char>(byte);
    for(std::size_t i=0;i<count;++i)ptr[i]=value;
    return original;
}

// 0043991A: bounded C-string length, matching zero-byte stop and maximum.
[[nodiscard]] constexpr std::size_t bounded_string_length(
    const char* str,std::size_t limit) noexcept {
    std::size_t i=0;
    while(i<limit&&str[i]!='\0')++i;
    return i;
}

// 00439E9E: heap-allocated C-string duplication, using the original
// constructor-like malloc/copy coupling, with an injectable native allocator.
using Allocator=char*(*)(std::size_t);
[[nodiscard]] inline char* duplicate_string(
    const char* text,Allocator allocate) noexcept {
    if(text==nullptr||allocate==nullptr)return nullptr;
    const std::size_t size=bounded_string_length(
        text,std::numeric_limits<std::size_t>::max());
    if(size==std::numeric_limits<std::size_t>::max())return nullptr;
    char* dest=allocate(size+1);
    if(dest!=nullptr)copy_zero_terminated_string(dest,text);
    return dest;
}

// 00436EB6: original 4-way numeric-to-code mapping.
[[nodiscard]] constexpr U32 map_runtime_code(U32 original) noexcept {
    switch(original) {
    case 0x3a4:return 0x411;
    case 0x3a8:return 0x804;
    case 0x3b5:return 0x412;
    case 0x3b6:return 0x404;
    default:return 0;
    }
}

// 00436745 / 0043670D / 00436729: WinAPI IsBadCodePtr,
// IsBadReadPtr and IsBadWritePtr inverted return values. Probe callback
// preserves original platform dependency instead of assuming access validity.
using BadCodeProbe=bool (*)(const void*);
using BadRangeProbe=bool (*)(const void*,std::size_t);
[[nodiscard]] inline bool executable_pointer_valid(
    const void* address,BadCodeProbe probe) noexcept {
    return probe!=nullptr&&!probe(address);
}
[[nodiscard]] inline bool readable_pointer_range(
    const void* address,std::size_t size,BadRangeProbe probe) noexcept {
    return probe!=nullptr&&!probe(address,size);
}
[[nodiscard]] inline bool writable_pointer_range(
    void* address,std::size_t size,BadRangeProbe probe) noexcept {
    return probe!=nullptr&&!probe(address,size);
}

// 00438241: consult global byte-flag vector and UTF16 mask classifier.
[[nodiscard]] constexpr bool char_class_matches(
    U8 code,U32 mask,U8 category,
    std::span<const U8> category_flags,
    std::span<const U16> class_flags) noexcept {
    if(static_cast<std::size_t>(code)+1>=category_flags.size() ||
       code>=class_flags.size())return false;
    if((category_flags[static_cast<std::size_t>(code)+1]&category)!=0)return true;
    return mask!=0&&(static_cast<U32>(class_flags[code])&mask)!=0;
}

// 00434444: find an allocation block record whose 0xC owner field lies
// within 0x100000 bytes below the target address. Retail returns a pointer
// into a contiguous 20-byte-per-entry table; model returns its offset.
[[nodiscard]] constexpr I32 find_allocation_region_offset(
    U32 target,std::span<const std::array<U32,5>> records) noexcept {
    for(std::size_t i=0;i<records.size();++i) {
        if(target-records[i][3]<0x100000U)
            return static_cast<I32>(i*20);
    }
    return -1;
}

// 0043091D: call non-null C runtime initializer pointers in source order.
using Initializer=void(*)();
inline void run_initializer_table(std::span<const Initializer> functions) {
    for(const auto fn:functions)if(fn!=nullptr)fn();
}

// 00431D8A: suppress calls when value is MSVC EOF/error 0xFFFFFFFF.
using InputCallback=void(*)(U32);
inline void forward_if_not_eof(U32 value,InputCallback callback) {
    if(value!=0xffffffffU&&callback)callback(value);
}

// 004330B2 / 004330E7: original buffer-then-refill putc worker and
// repeat-byte wrapper. The native overflow writer remains injected.
using OverflowWrite=U32(*)(U32);
struct WriteCursor {
    std::span<U8> buffer{}; std::size_t offset{};
    I32 remaining{}; I32 total{};
};
inline void write_stream_byte(
    U32 value,WriteCursor& stream,OverflowWrite overflow) {
    --stream.remaining;
    U32 result=value&0xffU;
    if(stream.remaining<0||stream.offset>=stream.buffer.size()) {
        result=overflow!=nullptr?overflow(value):0xffffffffU;
    } else {
        stream.buffer[stream.offset++]=static_cast<U8>(value);
    }
    if(result==0xffffffffU){stream.total=-1;return;}
    ++stream.total;
}
inline void repeat_stream_byte(
    U32 value,I32 repetitions,WriteCursor& stream,OverflowWrite overflow) {
    while(repetitions-->0) {
        write_stream_byte(value,stream,overflow);
        if(stream.total==-1)return;
    }
}

// 00435FF5: reads exception-frame offsets +0x18/+0x1C then delegates
// to __local_unwind2, without inventing native SEH stack manipulation.
using UnwindCallback=void(*)(U32,U32);
inline void unwind_exception_frame(
    const std::array<U32,8>& frame,UnwindCallback unwind) {
    if(unwind)unwind(frame[6],frame[7]);
}

// 0043A0DD / 0043A1C5: call constructor and install the recovered vtable
// address as a 32-bit retail image constant, NOT a valid host code pointer.
using InitializeObject=void(*)();
struct LegacyVptrObject {U32 vtable_address{};};
inline LegacyVptrObject* construct_runtime_object_a(
    LegacyVptrObject& object,InitializeObject init) {
    if(init)init();
    object.vtable_address=0x0043bb54U;return &object;
}
inline LegacyVptrObject* construct_runtime_object_b(
    LegacyVptrObject& object,InitializeObject init) {
    if(init)init();
    object.vtable_address=0x0043bb84U;return &object;
}

// 004379DA / 004379F0: table-specific decoding function wrappers.
// Table addresses 0x00449F48 and 0x00449F60 are verified original globals.
using TableDecode=void(*)(U16*,U32*,U32);
inline void decode_using_first_table(U16* dst,U32* state,TableDecode decode) {
    if(decode)decode(dst,state,0x00449f48U);
}
inline void decode_using_second_table(U16* dst,U32* state,TableDecode decode) {
    if(decode)decode(dst,state,0x00449f60U);
}

// 00437EB3: forwarding wrapper with the original final argument = 0.
using DecoderCallback=void(*)(void*,U8*,I32*,void*,I32);
inline void decode_forward_zero(
    void* object,U8* source,I32* state,void* target,DecoderCallback callback) {
    if(callback)callback(object,source,state,target,0);
}

// 00437478: forwards to FPU control-word modification with bit 0x80000
// deliberately cleared from the original caller-supplied mask.
using FpuControlCallback=void(*)(U32,U32);
inline void update_control_word_masked(
    U32 desired,U32 mask,FpuControlCallback callback) {
    if(callback)callback(desired,mask&0xfff7ffffU);
}

} // namespace btb::retail_source_batch_32
