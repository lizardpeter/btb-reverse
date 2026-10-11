#include "btb/retail_crt_batch70.hpp"
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
using namespace btb::retail_crt70;
namespace {
int allocs=0;
int deletes=0;
int freemem=0;
int handler_calls=0;
U32 last_flags=0;
U32 last_radix=0;
bool last_negative=false;
Ptr allocation(std::size_t n,I32 flags) noexcept {
    assert(n==32&&flags==1);++allocs;return reinterpret_cast<void*>(0x100);
}
Ptr raw_malloc(std::size_t) noexcept {
    ++allocs;return allocs<3?nullptr:reinterpret_cast<void*>(0x100);
}
bool new_handler(std::size_t) noexcept {++handler_calls;return true;}
void release(Ptr) noexcept {++deletes;}
Ptr state() noexcept {return reinterpret_cast<void*>(0xB1);}
Ptr open(const char*,const char*,U32 flags,Ptr thread) noexcept {
    assert(thread==reinterpret_cast<void*>(0xB1));
    last_flags=flags;
    return reinterpret_cast<void*>(0x77);
}
I32 scan(Ptr file,const char* fmt,Ptr va) noexcept {
    assert(file==reinterpret_cast<void*>(0x77));
    assert(fmt!=nullptr&&va!=nullptr);
    return 2;
}
void formatter(U32,char*,U32 radix,bool neg) noexcept {
    last_radix=radix;last_negative=neg;
}
void destroy_exception(Ptr) noexcept {++freemem;}
bool printer(Ptr conf) noexcept {
    return conf==reinterpret_cast<void*>(0xABC);
}
}
int main() {
    // 0042FFBA/0042FFC4: exact MSVC RNG.
    U32 seed=0;
    crt_srand(seed,1);
    constexpr auto first=1U*0x343FDU+0x269EC3U;
    assert(crt_rand(seed)==((first>>16)&0x7FFF));
    assert(seed==first);
    // 0042FBDC proxy deletion accepts null (native callback decides).
    deletes=0;operator_delete_proxy(nullptr,release);assert(deletes==1);
    // 0042FBCE new and 00430D93 malloc wrapper.
    allocs=0;
    assert(crt_operator_new(32,allocation)==reinterpret_cast<void*>(0x100));
    assert(crt_malloc(32,1,allocation)==reinterpret_cast<void*>(0x100));
    assert(allocs==2);
    // 00430DA5 retrying heap allocation respects newhandler flag.
    allocs=handler_calls=0;
    assert(crt_nh_malloc(5,1,raw_malloc,new_handler,4)==
           reinterpret_cast<void*>(0x100));
    assert(allocs==3&&handler_calls==2);
    allocs=handler_calls=0;
    assert(crt_nh_malloc(0xFFFFFFE1ULL,1,raw_malloc,new_handler)==nullptr);
    assert(allocs==0);
    // 00430A1F/00430A3F shared thread + fopen mode0x40.
    assert(crt_fopen("x","rb",state,open)==reinterpret_cast<void*>(0x77));
    assert(last_flags==0x40);
    assert(crt_fopen_internal("x","rb",8,state,open)==
           reinterpret_cast<void*>(0x77));
    assert(last_flags==8);
    // 0042FBB8: opaque variadic fscanf cursor and return.
    int variadics=9;
    assert(crt_fscanf(reinterpret_cast<void*>(0x77),"%d%d",
                       &variadics,scan)==2);
    // 0042FF99 / 0042FFA2 update three source globals.
    NlgGlobals nlg{};
    nlg_notify1(nlg,7,8,9);
    assert(nlg.state==7&&nlg.eax==8&&nlg.ebp==9);
    nlg_notify_from_stack(nlg,11,12,13);
    assert(nlg.state==11&&nlg.eax==12&&nlg.ebp==13);
    // 00430441/00430464 original destructors restore vptr first.
    deletes=0;
    LegacyException e{0,reinterpret_cast<void*>(0x2),1};
    legacy_exception_destructor(e,0x1234,release);
    assert(e.vtable==0x1234&&deletes==1);
    e.owned=0;
    legacy_exception_destructor(e,0x1234,release);
    assert(deletes==1);
    LegacyTypeInfo info{0,reinterpret_cast<void*>(0x8)};
    legacy_typeinfo_destructor(info,0x55,release);
    assert(info.vtable==0x55&&deletes==2);
    // 00430CFE forward swap.
    std::array<std::byte,3> lhs{std::byte{1},std::byte{2},std::byte{3}};
    std::array<std::byte,3> rhs{std::byte{4},std::byte{5},std::byte{6}};
    swap_source_bytes(lhs,rhs,3);
    assert(lhs[0]==std::byte{4}&&rhs[0]==std::byte{1});
    assert(lhs[2]==std::byte{6}&&rhs[2]==std::byte{3});
    // 00430F2F decimal signed flag.
    char buffer[24]{};
    assert(crt_itoa(0xFFFFFFFFU,buffer,10,formatter)==buffer);
    assert(last_radix==10&&last_negative);
    assert(crt_itoa(0xFFFFFFFFU,buffer,16,formatter)==buffer);
    assert(last_radix==16&&!last_negative);
    // 00406600 scalar dtor always destructor, delete only low bit.
    freemem=deletes=0;
    Ptr address=reinterpret_cast<void*>(0xABC);
    assert(exception_scalar_deleting_destructor(address,0,
           destroy_exception,release)==address);
    assert(freemem==1&&deletes==0);
    assert(exception_scalar_deleting_destructor(address,1,
           destroy_exception,release)==address);
    assert(freemem==2&&deletes==1);
    // 0042FB5C import thunk is a real WINAPI forwarding call.
    assert(thunk_print_dialog(address,printer));
    assert(!thunk_print_dialog(nullptr,printer));
}
