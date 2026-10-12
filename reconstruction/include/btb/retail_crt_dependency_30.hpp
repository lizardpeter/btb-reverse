#pragma once
// Thirty previously unreviewed original MSVC2003 runtime functions.
// Evidence: analysis/ghidra_main_exe/functions/<ADDRESS>.c;
// executable SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
// Faithful source-level state/branch/callback behavior where recoverable,
// not claims of original stack-based x86 ABI or in-process CRT replacement.
#include "btb/retail_source_batch_32.hpp"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <optional>
#include <span>
#include <utility>

namespace btb::crt_batch_30 {
using U8=std::uint8_t;
using U16=std::uint16_t;
using U32=std::uint32_t;
using I32=std::int32_t;
using U64=std::uint64_t;

// 00430862 and 00430873: identical 00430884 dispatch, but flags distinguish
// _exit() from exit(): retail passes (exit_code,0,0) and (exit_code,1,0).
using ExitDispatch=void (*)(U32 code,U32 cleanup,U32 reserved,void* context);
inline void exit_immediately(U32 code,ExitDispatch dispatch,void* ctx) {
    if(dispatch)dispatch(code,0,0,ctx);
}
inline void exit_with_cleanup(U32 code,ExitDispatch dispatch,void* ctx) {
    if(dispatch)dispatch(code,1,0,ctx);
}

// 00438230: test original character class with code, mask=0, category=4.
// The callback is already reconstructed as 00438241.
using CharacterClassifier=U32 (*)(U32 code,U32 mask,U32 category,void*);
inline U32 classify_code_category_four(
    U8 code,CharacterClassifier classify,void* ctx) {
    return classify?classify(code,0,4,ctx):0;
}

// 0043317B: forward control word with masks 0x10000 and 0x30000.
using ControlSetter=void (*)(void* object,U32 desired,U32 mask,void*);
inline void set_precision_control(
    void* object,ControlSetter setter,void* ctx) {
    if(setter)setter(object,0x10000,0x30000,ctx);
}

// 00436774 and 00437097: one-time initializer guarded by native global
// zero; only set latch after the external setup call completes.
using Initialization=void (*)(I32 argument,void*);
inline bool initialize_latch_zero(U32& latch,Initialization init,void* ctx) {
    if(latch!=0)return false;
    if(init)init(0,ctx);
    ++latch;
    return true;
}
inline bool initialize_minus_three(U32& latch,Initialization init,void* ctx) {
    if(latch!=0)return false;
    if(init)init(-3,ctx);
    latch=1;
    return true;
}

// 0043675D: retail fatal CRT path calls diagnostic 10,
// diagnostic 22, then __exit(3); callbacks preserve this exact ordering.
using FatalNotice=void (*)(U32,void*);
inline void fatal_runtime_error(FatalNotice error,FatalNotice diagnostic,
                                ExitDispatch exit_now,void* ctx) {
    if(error)error(10,ctx);
    if(diagnostic)diagnostic(22,ctx);
    exit_immediately(3,exit_now,ctx);
}

// 00431D70: putc-like stream cursor. --remaining then read byte,
// otherwise invoke the original 00433888 refill dependency.
using RefillCallback=U32 (*)(void*);
struct InputCursor {const U8* current{};I32 remaining{};void* context{};};
[[nodiscard]] inline U32 read_stream_byte(InputCursor& cursor,
                                            RefillCallback refill) {
    --cursor.remaining;
    if(cursor.remaining>=0&&cursor.current) {
        return *cursor.current++;
    }
    return refill?refill(cursor.context):0xffffffffU;
}

// 00435610: optional function pointer, normalize nonzero result to 1.
using Predicate=I32 (*)(U32,void*);
[[nodiscard]] inline U32 predicate_to_bool(
    U32 value,Predicate predicate,void* ctx) {
    return predicate&&predicate(value,ctx)!=0?1U:0U;
}

// 0043039E, 00430479, 00439FEE: original deleting destructors always
// invoke native destructor, then conditional delete when flag bit0 is set.
using Destruct=void (*)(void*,void*);
inline void* deleting_destructor(
    void* object,U8 flag,Destruct destruct,Destruct deallocate,void* ctx) {
    if(destruct)destruct(object,ctx);
    if((flag&1)!=0&&deallocate)deallocate(object,ctx);
    return object;
}
inline void* destroy_exception(
    void* object,U8 flag,Destruct destruct,Destruct deallocate,void* ctx) {
    return deleting_destructor(object,flag,destruct,deallocate,ctx);
}
inline void* destroy_auxiliary(
    void* object,U8 flag,Destruct destruct,Destruct deallocate,void* ctx) {
    return deleting_destructor(object,flag,destruct,deallocate,ctx);
}
inline void* destroy_format_exception(
    void* object,U8 flag,Destruct destruct,Destruct deallocate,void* ctx) {
    return deleting_destructor(object,flag,destruct,deallocate,ctx);
}

// 00439F94, 0043A0A4, 0043A18C: instantiate local 28-byte exception then
// call native exception throw function with original throw-info address.
// A C++ model cannot reproduce compiler-generated SEH stack unwind itself.
using PrepareException=void (*)(void* object,void* ctx);
using ThrowException=void (*)(void* object,U32 throwinfo,void* ctx);
inline void prepare_and_throw(std::array<U32,7>& object,U32 info,
                              PrepareException construct,
                              ThrowException thrower,void* ctx) {
    if(construct)construct(object.data(),ctx);
    if(thrower)thrower(object.data(),info,ctx);
}
inline void throw_format_exception(std::array<U32,7>& obj,
        PrepareException init,ThrowException raise,void* ctx) {
    prepare_and_throw(obj,0x0043c158U,init,raise,ctx);
}
inline void throw_exception_a(std::array<U32,7>& obj,
        PrepareException init,ThrowException raise,void* ctx) {
    prepare_and_throw(obj,0x0043c0e8U,init,raise,ctx);
}
inline void throw_exception_b(std::array<U32,7>& obj,
        PrepareException init,ThrowException raise,void* ctx) {
    prepare_and_throw(obj,0x0043c210U,init,raise,ctx);
}

// 00431DA1: increment output width then consume stream bytes until
// 00436598 character-class test says false; the LAST byte is returned.
using ClassifyByte=U32 (*)(U32,void*);
[[nodiscard]] inline U32 scan_until_class_fails(
    U32& width,InputCursor& cursor,RefillCallback refill,
    ClassifyByte classify,void* ctx) {
    for(;;) {
        ++width;
        const U32 c=read_stream_byte(cursor,refill);
        if(!classify||classify(c,ctx)==0)return c;
    }
}

// 0043360A: move the terminated string by signed displacement, including
// its zero. Caller must own the destination capacity and ensure nonnegative
// base; an actual negative displacement is supported with memmove.
inline void shift_terminated_string(char* str,I32 displacement) {
    if(displacement!=0) {
        const std::size_t n=std::strlen(str)+1;
        std::memmove(str+displacement,str,n);
    }
}

// 004373B5: array table of 8-byte records, grouped 32 per page.
// Original tests the byte at +4 of each entry for bit 0x40 and returns 0/64.
struct FileHandleRow {U32 value{};U8 flags{};std::array<U8,3> reserved{};};
static_assert(sizeof(FileHandleRow)==8);
[[nodiscard]] inline U8 file_handle_has_bit40(
    std::span<const FileHandleRow> rows,U32 index,U32 capacity) {
    if(index>=capacity||index>=rows.size())return 0;
    return rows[index].flags&0x40;
}

// 0043356F: set global conversion flag=1 for duration of forward call, then
// clear to zero. We intentionally do not synthesize a native SEH finally.
using ConversionCall=void (*)(U32,void*,I32,I32,void*);
inline void convert_with_global_flag(
    U32& flag,U32 a,void* b,I32 c,I32 d,
    ConversionCall callback,void* ctx) {
    flag=1;
    if(callback)callback(a,b,c,d,ctx);
    flag=0;
}

// 00436598: selector bit8 via multibyte classifier if locale codepage>1,
// otherwise bit8 directly from original character class table.
using MaskClassifier=U32 (*)(U32,U32,void*);
[[nodiscard]] inline U32 is_class_eight(
    U32 code,I32 locale,std::span<const U8> class_flags,
    MaskClassifier complex,void* ctx) {
    if(locale>1)return complex?complex(code,8,ctx):0;
    return code<class_flags.size()?class_flags[code]&8U:0;
}

// 00437A06 and 00437A33: same native converter input/6-word temporary,
// then different 004379DA or 004379F0 table-based formatting callback.
using FormatSetup=void (*)(void*,std::array<U16,6>&,U8*,void*);
using FormatFinish=void (*)(std::array<U16,6>&,U32*,U32,void*);
inline void format_using_table(
    void* object,U32* result,U8* input,U32 table,
    FormatSetup prepare,FormatFinish finish,void* ctx) {
    std::array<U16,6> scratch{};
    if(prepare)prepare(object,scratch,input,ctx);
    if(finish)finish(scratch,result,table,ctx);
}
inline void format_first_table(void* obj,U32* result,U8* input,
        FormatSetup prepare,FormatFinish finish,void* ctx) {
    format_using_table(obj,result,input,0x00449f48U,prepare,finish,ctx);
}
inline void format_second_table(void* obj,U32* result,U8* input,
        FormatSetup prepare,FormatFinish finish,void* ctx) {
    format_using_table(obj,result,input,0x00449f60U,prepare,finish,ctx);
}

// 00437443: x87 update merges the old control word with new masked bits,
// then delegates hardware install via 00437520. Native read is injected.
using ReadControl=U32 (*)(void*);
using WriteControl=void (*)(U32,void*);
[[nodiscard]] inline U32 merge_fpu_control(
    U32 desired,U32 mask,ReadControl read,
    WriteControl write,void* ctx) {
    const U32 old=read?read(ctx):0;
    const U32 value=(old&~mask)|(desired&mask);
    if(write)write(value,ctx);
    return value;
}

// 00431D39: test classification bit4, otherwise ASCII fold and subtract 7.
// Ghidra is unusual: (code & ~0x20)-7, not ordinary tolower().
[[nodiscard]] inline U32 fold_hex_style_character(
    U32 code,I32 locale,std::span<const U8> class_flags,
    MaskClassifier classify,void* ctx) {
    const U32 class4=locale<2
        ? (code<class_flags.size()?class_flags[code]&4U:0U)
        : (classify?classify(code,4U,ctx):0U);
    return class4?code:(code&~0x20U)-7U;
}

// 004301AE: original six global function pointer slots installed in order.
// Caller owns the actual module-mapped native function pointers.
using RuntimeFunction=void(*)();
struct ConversionFunctionTable {
    std::array<RuntimeFunction,6> entries{};
};
inline void install_conversion_functions(
    ConversionFunctionTable& target,
    const std::array<RuntimeFunction,6>& original) {
    target.entries=original;
}

// 00433118: repeated 004330B2 putc, stop the instant output total==-1.
// Character is taken as signed byte then cast to int in native wrapper;
// putc only writes low 8 bits.
using WriteByte=void (*)(U32,U32&,void*);
inline void write_string_bytes(
    const char* text,I32 count,I32& output_total,
    WriteByte write,void* ctx) {
    for(I32 i=0;i<count;++i) {
        if(write)write(static_cast<U8>(text[i]),
                       *reinterpret_cast<U32*>(&output_total),ctx);
        if(output_total==-1)return;
    }
}

// 00435732: on file mode !=0, flush if flags bit 0x10 and reset 0x0D
// flags, +0x18, +0x00 and +0x08. Do not invent native file flush.
using FlushStream=void (*)(void*,void*);
struct StreamReset {
    U32 cursor{},count{},remaining{},flags{},buffer_size{};
};
inline void reset_stream_if_buffered(
    I32 mode,StreamReset& state,void* original_file,FlushStream flush,void* ctx) {
    if((state.flags&0x10U)==0)return;
    if(flush)flush(original_file,ctx);
    if(mode==0)return;
    state.flags &=0xeeU;
    state.buffer_size=0;state.cursor=0;state.remaining=0;
}

// 00436322: validates index and flags bit0, otherwise sets original global
// errno=9 and os_error=0, returns 0xFFFFFFFF.
[[nodiscard]] inline U32 get_native_handle_or_error(
    std::span<const FileHandleRow> rows,U32 index,U32 capacity,
    U32& last_os_error,U32& err_no) {
    if(index<capacity&&index<rows.size()&&(rows[index].flags&1)!=0)
        return rows[index].value;
    last_os_error=0;err_no=9;
    return 0xffffffffU;
}

// 0043318D: compare a global floating/format counter against modulo from
// another two global ints. Divisor==0 fault is explicitly not emulated.
[[nodiscard]] constexpr std::optional<bool> compare_modulo_counter(
    I32 threshold,I32 total,I32 divisor) noexcept {
    if(divisor==0||(total==std::numeric_limits<I32>::min()&&divisor==-1))
        return std::nullopt;
    return threshold<(total%divisor);
}

// 004358B0: find matching three-word table record by its first word.
struct Triple {I32 key{},second{},third{};};
[[nodiscard]] inline const Triple* find_triple(
    std::span<const Triple> records,I32 key) noexcept {
    for(const auto& row:records)if(row.key==key)return &row;
    return nullptr;
}
} // namespace btb::crt_batch_30
