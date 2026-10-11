#pragma once
// BTB original PE32 CRT and import wrappers. Every entry below corresponds to
// a Ghidra-discovered native code address (not an invented original API).
// Host callback symbols resolve CRT/WinAPI functions without binding x86 IAT.
#include <cstddef>
#include <cstdint>
#include <span>
#include <bit>

namespace btb::retail_crt70 {
using I32=std::int32_t;
using U32=std::uint32_t;
using Ptr=void*;
using Free=void (*)(void*) noexcept;

// 0042FFBA original srand global and 0042FFC4 MSVC 15-bit rand.
constexpr void crt_srand(U32& seed,U32 value) noexcept {seed=value;}
[[nodiscard]] constexpr U32 crt_rand(U32& seed) noexcept {
    seed=seed*0x343FDU+0x269EC3U;
    return (seed>>16U)&0x7FFFU;
}

// 0042FBDC operator delete proxy calls FUN_00430d2a unconditionally.
inline void operator_delete_proxy(Ptr p,Free native_delete) noexcept {
    if(native_delete)native_delete(p);
}

using Allocate=Ptr (*)(std::size_t,I32) noexcept;
// 0042FBCE retail operator_new always requests a retrying heap allocation.
[[nodiscard]] inline Ptr crt_operator_new(std::size_t size,Allocate alloc) noexcept {
    return alloc?alloc(size,1):nullptr;
}
// 00430D93 malloc uses DAT_0051c448 new-handler flag, not always 1.
[[nodiscard]] inline Ptr crt_malloc(
    std::size_t size,I32 handler_flag,Allocate alloc) noexcept {
    return alloc?alloc(size,handler_flag):nullptr;
}

// 00430DA5 __nh_malloc: refuse huge size; allocate, invoke new-handler
// on failure only if enabled; repeat until success or handler returns false.
using RawMalloc=Ptr (*)(std::size_t) noexcept;
using NewHandler=bool (*)(std::size_t) noexcept;
[[nodiscard]] inline Ptr crt_nh_malloc(
    std::size_t size,I32 handler_flag,RawMalloc raw,NewHandler handler,
    std::size_t max_attempts=0) noexcept {
    if(size>=0xFFFFFFE1ULL||!raw)return nullptr;
    std::size_t n=0;
    for(;;) {
        if(max_attempts!=0 && n++>=max_attempts)return nullptr;
        if(Ptr p=raw(size))return p;
        if(handler_flag==0||!handler||!handler(size))return nullptr;
    }
}

// 00430A1F shared CRT fopen_internal: resolve thread state and call helper
// with mode flags. 00430A3F fopen wrapper passes required flags 0x40.
using NativeFopen=Ptr (*)(const char*,const char*,U32,Ptr) noexcept;
using AcquireThreadState=Ptr (*)() noexcept;
[[nodiscard]] inline Ptr crt_fopen_internal(
    const char* name,const char* mode,U32 flags,
    AcquireThreadState thread,NativeFopen open) noexcept {
    Ptr state=thread?thread():nullptr;
    return state&&open?open(name,mode,flags,state):nullptr;
}
[[nodiscard]] inline Ptr crt_fopen(
    const char* name,const char* mode,AcquireThreadState thread,
    NativeFopen open) noexcept {
    return crt_fopen_internal(name,mode,0x40,thread,open);
}

// 0042FBB8 fscanf accepts opaque argument cursor passed to true parser.
using NativeFscanf=I32 (*)(Ptr,const char*,Ptr) noexcept;
[[nodiscard]] inline I32 crt_fscanf(
    Ptr file,const char* format,Ptr variadic_cursor,NativeFscanf scan) noexcept {
    return scan?scan(file,format,variadic_cursor):-1;
}

// 0042FF99 and 0042FFA2 NLG notifications store return/stack registers in
// three original globals, not synthetic thread-local state.
struct NlgGlobals {U32 state{},eax{},ebp{};};
constexpr void nlg_notify1(NlgGlobals& g,U32 target,U32 eax,U32 ebp) noexcept {
    g.state=target;g.eax=eax;g.ebp=ebp;
}
constexpr void nlg_notify_from_stack(
    NlgGlobals& g,U32 stack_target,U32 eax,U32 ebp) noexcept {
    nlg_notify1(g,stack_target,eax,ebp);
}

// 00430441 exception destructor resets vptr, frees owned message
// iff the original owned flag at +8 is nonzero.
struct LegacyException {
    U32 vtable{};
    Ptr message{};
    U32 owned{};
};
inline void legacy_exception_destructor(
    LegacyException& ex,U32 original_vtable,Free release) noexcept {
    ex.vtable=original_vtable;
    if(ex.owned&&release)release(ex.message);
}

// 00430464 type_info destructor resets vptr then frees string pointer
// when non-null. Original leaves pointer dangling after free.
struct LegacyTypeInfo {U32 vtable{};Ptr decorated_name{};};
inline void legacy_typeinfo_destructor(
    LegacyTypeInfo& info,U32 vtable,Free release) noexcept {
    info.vtable=vtable;
    if(info.decorated_name&&release)release(info.decorated_name);
}

// 00430CFE byte-wise forward swapping (NOT equivalent to memmove for
// overlapping ranges); span bounds are explicit to avoid host OOB.
inline void swap_source_bytes(
    std::span<std::byte> lhs,std::span<std::byte> rhs,
    std::size_t n) noexcept {
    if(lhs.data()==rhs.data()||n==0)return;
    if(n>lhs.size())n=lhs.size();
    if(n>rhs.size())n=rhs.size();
    for(std::size_t i=0;i<n;++i) {
        auto tmp=lhs[i];lhs[i]=rhs[i];rhs[i]=tmp;
    }
}

// 00430F2F itoa sets negative flag only for base 10 and sign bit set,
// then calls the original generic integer conversion worker.
using IntegerFormatter=void (*)(U32,char*,U32,bool) noexcept;
[[nodiscard]] inline char* crt_itoa(
    U32 value,char* output,U32 radix,IntegerFormatter format) noexcept {
    const bool negative=(radix==10U && std::bit_cast<I32>(value)<0);
    if(format)format(value,output,radix,negative);
    return output;
}

// 00406600 scalar-deleting exception destructor: always destructor;
// operator delete only if flag low bit set, always returns "this".
using DestroyException=void (*)(Ptr) noexcept;
[[nodiscard]] inline Ptr exception_scalar_deleting_destructor(
    Ptr value,U32 flag,DestroyException destroy,Free release) noexcept {
    if(destroy)destroy(value);
    if((flag&1U)!=0 && release)release(value);
    return value;
}

// 0042FB5C: imported PrintDlgA IAT jump. The Ghidra listing misleadingly
// appears self-recursive; executable evidence is the import thunk.
using NativePrintDialog=bool (*)(Ptr) noexcept;
[[nodiscard]] inline bool thunk_print_dialog(
    Ptr config,NativePrintDialog native) noexcept {
    return native&&native(config);
}

} // namespace btb::retail_crt70
