#pragma once
#include "btb/original_memory4.hpp"
namespace btb::original_heap4 {
using U32=std::uint32_t;using I32=std::int32_t;
class Services {
public:
 virtual ~Services()=default;
 virtual U32 read(U32)=0;
 virtual void write(U32,U32)=0;
 virtual U32 heap_resize(U32 heap,U32 pointer,U32 bytes)=0;
 virtual U32 heap_allocate(U32 heap,U32 flags,U32 bytes)=0;
 virtual void heap_free(U32 heap,U32 pointer)=0;
 virtual U32 virtual_allocate(U32 bytes,U32 allocation,U32 protect)=0;
 virtual bool virtual_free(U32 pointer,U32 bytes,U32 type)=0;
};
struct State {U32 heap{},head{0x447748},current{},free_pages{},table{},capacity{},regions{};};
U32 find_small2(State&,U32 pointer,U32& region,U32& page,Services&); //0043519F
void release_region2(State&,U32 region,Services&); //00435087
void decommit_small2(State&,I32 count,Services&); //004350DD
U32 add_region3(State&,Services&); //00434AA1
// Supplies the actual lookup used by original_memory4::release; other heap operations remain required.
class LookupLink:public original_memory4::HeapServices {
public:
 LookupLink(State& state,Services& services):state_(state),services_(services){}
 void* find2(void*,std::uintptr_t&,std::uintptr_t&) final;
private:State& state_;Services& services_;
};
}
