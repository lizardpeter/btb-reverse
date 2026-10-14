#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
namespace btb::original_string8 {
using U32=std::uint32_t;
#pragma pack(push,4)
struct String {
    std::uint8_t allocator_token{};
    std::array<std::uint8_t,3> padding{};
    char* data{};
    U32 length{},capacity{};
};
#pragma pack(pop)
#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(String)==16 && offsetof(String,data)==4 && offsetof(String,length)==8);
#endif
class Heap {
public:
    virtual ~Heap()=default;
    virtual char* allocate(U32 bytes)=0; // throws; first byte is reference count
    virtual void release(char*) noexcept=0;
};
class NativeHeap final:public Heap {
public:
    char* allocate(U32) override;
    void release(char*) noexcept override;
};
struct Allocation {char* base{};U32 capacity{};};
// 004067E9 original catch-all handler, exact-size retry.
Allocation retry_exact_allocation(U32 requested,Heap&);
// 00406810 continuation consumes EBX=this, ESI=capacity, EBP+8=new base.
void attach_copy_buffer(String&,Allocation,Heap&);
// 00406790, 004063A0, 00406460, 004068A0, 00406150
void allocate_copy_buffer(String&,U32 requested,Heap&);
String& assign_buffer(String&,const char*,U32 length,Heap&);
bool grow(String&,U32 requested,bool trim,Heap&);
void detach_shared_buffer(String&,Heap&);
String& assign_substring(String&,const String&,U32 offset,U32 count,Heap&);
// 00430500: overlap-aware source memmove, also used by substring erasure.
void* move_bytes(void* destination,const void* source,U32 count) noexcept;
// Bridge to the existing compiled 00406350/00406440 original functions.
void tidy(String&,bool release,Heap&) noexcept;
void set_end(String&,U32 length);
bool verify_8_original_string_functions();
} // namespace btb::original_string8
