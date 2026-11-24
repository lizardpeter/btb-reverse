#pragma once
// Ten source-derived original PE32/MSVC CRT procedures with connected
// native C++ dependencies. Ghidra: analysis/ghidra_main_exe/functions.
// Native pointer/global and Win32 Heap API dependencies are explicit
// typed host services, not claimed as byte-for-byte x86 ABI replacements.
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
namespace btb::original_crt10 {
using U32=std::uint32_t;using I32=std::int32_t;
struct File{
    I32 count{},flags{},file_index{};
    U32 pointer{},base{},tmpname{};
};
using Free=void(*)(U32,void*);
using Flush=I32(*)(File&,void*);
using Close=I32(*)(I32,void*);
using Lock=I32(*)(File&,void*);
using Unlock=void(*)(I32,File&,void*);
using Scan=I32(*)(File&,const char*,void*);
struct Io {
    void* ctx{};
    Free deallocate{};
    Flush flush{};
    Close close_handle{},sync_handle{};
    Lock lock{};
    Unlock unlock{};
    Scan scan{};
};
// 004311DC native dependent allocator reset.
[[nodiscard]] bool free_file_buffer(File&,const Io&);
// 0042FB62 depends directly on compiled 004311DC.
[[nodiscard]] I32 fclose_original(File&,const Io&);
// 00431207 original flush-one.
[[nodiscard]] I32 flush_one(File*,const Io&,I32(*)(void*),void*);
// 004312A7 calls compiled 00431207 on real File elements.
[[nodiscard]] I32 flush_all(std::span<File*>,I32,const Io&);
// 00430FB8 lock -> scan -> unlock.
[[nodiscard]] I32 formatted_scan(File&,const char*,const Io&);
struct Stream {
    std::byte* pointer{};std::byte* base{};
    U32 flags{},capacity{},count{};
    std::array<std::byte,2> inline_buffer{};
};
using Allocate=void*(*)(std::size_t,void*);
// 00437371 CRT malloc(4096) or inline 2 bytes.
void allocate_file_buffer(Stream&,U32&,Allocate,void*);
struct Heap {
    U32 heap_handle{},root{},current{},control{},step{},count{},next{},algorithm{};
};
using SlabAlloc=U32(*)(U32,std::size_t,void*);
using HeapCreate=U32(*)(bool,std::size_t,void*);
using HeapDestroy=void(*)(U32,void*);
using Mode=I32(*)(void*);
using InitSmall=bool(*)(void*);
// 004343FC allocates original 320-byte heap slab.
[[nodiscard]] bool initialize_heap_slab(Heap&,U32,SlabAlloc,void*);
// 0043439F calls native 004343FC when algorithm==3.
[[nodiscard]] bool initialize_heap(
    Heap&,I32,HeapCreate,Mode,SlabAlloc,InitSmall,HeapDestroy,void*);
using Setup=void(*)(void*);
using ProcessorFeature=bool(*)(void*);
struct CrtStartup{bool cpu_feature{};};
// 00430196 exact original pointer-init -> CPU query -> precision call order.
void initialize_crt(CrtStartup&,Setup,ProcessorFeature,Setup,void*);
// 00433596 scoped global float formatting guard.
using Format=void(*)(U32,char*,std::size_t,void*);
void float_conversion(U32&,U32,char*,std::size_t,Format,void*);
[[nodiscard]] bool verify_10_original_crt_io_functions() noexcept;
}
