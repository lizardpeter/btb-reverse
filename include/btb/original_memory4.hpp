#pragma once
#include "btb/original_runtime8.hpp"
namespace btb::original_memory4 {
using U32=std::uint32_t;using Handle=std::uintptr_t;
struct HeapState {U32 mode{},threshold3{},threshold2{};Handle heap{};};
class HeapServices {
public:
 virtual ~HeapServices()=default;
 virtual void* allocate3(U32)=0;
 virtual void* allocate2(U32 paragraphs)=0;
 virtual Handle find3(void*)=0;
 virtual void free3(Handle,void*)=0;
 virtual void* find2(void*,Handle& region,Handle& page)=0;
 virtual void free2(Handle,Handle,void*)=0;
 virtual void* heap_allocate(Handle,U32)=0;
 virtual void heap_free(Handle,void*)=0;
};
// 00430DD1 / 00430D2A: original_heap9::Link supplies both recovered small heaps.
void* allocate(const HeapState&,U32,HeapServices&);
void release(const HeapState&,void*,HeapServices&);
// 00433B60, original overlap-aware copy. Valid accessible byte ranges required.
void* copy_bytes(void*,const void*,U32) noexcept;
// 00430B5C, original partition/selection hybrid, retaining comparator order.
void sort_bytes(std::span<std::byte>,U32 count,U32 width,original_runtime8::Comparator,void*);
bool verify_4_original_memory_functions();
}
