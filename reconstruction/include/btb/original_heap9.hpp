#pragma once
#include "btb/original_heap4.hpp"
#include <span>
namespace btb::original_heap9 {
using namespace original_heap4;
class Services:public original_heap4::Services {
public:
 virtual U32 commit(U32 address,U32 bytes)=0; // VirtualAlloc(address,bytes,MEM_COMMIT,PAGE_READWRITE)
 virtual std::span<std::byte> bytes(U32 address,U32 count)=0;
 virtual bool new_handler(U32)=0;
};
struct State {original_heap4::State& base;U32 current3{},spare3{},spare_page{},mode{},threshold3{1016},threshold2{480};bool retry{};};
U32 add_region2(State&,Services&); //00434F43
I32 commit_group3(State&,U32 region,Services&); //00434B52
U32 allocate3(State&,U32 bytes,Services&); //00434798
void release3(State&,U32 region,U32 pointer,Services&); //0043446F
bool resize3(State&,U32 region,U32 pointer,U32 bytes,Services&); //00434C4D
U32 allocate_page2(U32 page,U32 available,U32 paragraphs,Services&); //00435443
bool resize2(U32 region,U32 page,U32 slot,U32 paragraphs,Services&); //00435567
U32 allocate2(State&,U32 paragraphs,Services&); //0043523B
U32 reallocate(State&,U32 pointer,U32 bytes,Services&); //00439B8B
// Connects recovered allocation/free dispatch to both real small-heap implementations.
class Link final:public original_memory4::HeapServices {
public:
 Link(State& s,Services& io):state_(s),services_(io){}
 void* allocate3(U32) override;void* allocate2(U32) override;
 std::uintptr_t find3(void*) override;void free3(std::uintptr_t,void*) override;
 void* find2(void*,std::uintptr_t&,std::uintptr_t&) override;
 void free2(std::uintptr_t,std::uintptr_t,void*) override;
 void* heap_allocate(std::uintptr_t,U32) override;void heap_free(std::uintptr_t,void*) override;
private:State& state_;Services& services_;
};
}
