#pragma once
#include "btb/original_string8.hpp"
#include "btb/original_runtime16.hpp"
#include <stdexcept>
namespace btb::original_exception9 {
using namespace btb::original_string8;
#pragma pack(push,4)
struct Object {btb::original_crt17::Exception base{};String text{};};
#pragma pack(pop)
#if UINTPTR_MAX==UINT32_MAX
static_assert(sizeof(Object)==28 && offsetof(Object,text)==12);
#endif
// 00439F23 / 0043A00A / 00406620: constructors, with C++ unwinding.
Object& construct_logic(Object&,const String&,Heap&);
Object& copy_logic(Object&,const Object&,Heap&);
Object& copy_runtime(Object&,const Object&,Heap&);
// 00406580 / 00439FB1 / 0043A067 / 0043A14F.
void destroy_runtime(Object&,Heap&) noexcept;
void destroy_logic(Object&,Heap&) noexcept;
void destroy_logic_copy(Object&,Heap&) noexcept;
void destroy_position(Object&,Heap&) noexcept;
// Native throwing adapters preserve the original payload and ownership while
// retaining std::length_error/std::out_of_range compatibility for host callers.
class LengthError final:public std::length_error {
public:
    explicit LengthError(const String&);
    LengthError(const LengthError&);
    LengthError& operator=(const LengthError&)=delete;
    ~LengthError() noexcept override;
    const Object& retail_object() const noexcept {return object_;}
private:NativeHeap heap_;Object object_;
};
class PositionError final:public std::out_of_range {
public:
    explicit PositionError(const String&);
    PositionError(const PositionError&);
    PositionError& operator=(const PositionError&)=delete;
    ~PositionError() noexcept override;
    const Object& retail_object() const noexcept {return object_;}
private:NativeHeap heap_;Object object_;
};
// 00439EC9 / 0043A0F5, connected to original String methods.
[[noreturn]] void throw_string_too_long();
[[noreturn]] void throw_invalid_string_position();
bool verify_9_original_exception_functions();
}
