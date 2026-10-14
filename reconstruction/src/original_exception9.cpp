#include "btb/original_exception9.hpp"
#include <cstdlib>
#include <cstring>
#include <limits>
namespace btb::original_exception9 {
namespace {
void* allocate_message(std::size_t n){return std::malloc(n);}
void free_message(void* p) noexcept {std::free(p);}
void destroy_base(Object& o) noexcept {
    btb::original_crt17::exception_destruct(o.base,0x0043b5bc,free_message);
}
Object& copy_body(Object& out,const Object& from,Heap& heap,U32 table) {
    btb::original_runtime16::copy_exception(out.base,from.base,allocate_message);
    out.text={};out.text.allocator_token=from.text.allocator_token;
    try {assign_substring(out.text,from.text,0,std::numeric_limits<U32>::max(),heap);}
    catch(...) {destroy_base(out);throw;}
    out.base.vtable=table;return out;
}
void destroy_body(Object& o,Heap& heap,U32 table) noexcept {
    o.base.vtable=table;tidy(o.text,true,heap);destroy_base(o);
}
struct TemporaryString {
    NativeHeap heap;String text{};
    explicit TemporaryString(const char* message){
        assign_buffer(text,message,static_cast<U32>(std::strlen(message)),heap);
    }
    ~TemporaryString(){tidy(text,true,heap);}
};
}
Object& construct_logic(Object& out,const String& text,Heap& heap) {
    // DAT_00482574 lies in zero-initialized image storage: base message is "".
    btb::original_runtime16::construct_exception(out.base,"",allocate_message);
    out.text={};out.text.allocator_token=text.allocator_token;
    try {assign_substring(out.text,text,0,std::numeric_limits<U32>::max(),heap);}
    catch(...) {destroy_base(out);throw;}
    out.base.vtable=0x0043bb74;return out;
}
Object& copy_logic(Object& out,const Object& source,Heap& heap){return copy_body(out,source,heap,0x0043bb74);}
Object& copy_runtime(Object& out,const Object& source,Heap& heap){return copy_body(out,source,heap,0x0043b328);}
void destroy_runtime(Object& o,Heap& h) noexcept {destroy_body(o,h,0x0043b328);}
void destroy_logic(Object& o,Heap& h) noexcept {destroy_body(o,h,0x0043bb74);}
void destroy_logic_copy(Object& o,Heap& h) noexcept {destroy_body(o,h,0x0043bb74);}
void destroy_position(Object& o,Heap& h) noexcept {destroy_body(o,h,0x0043bb74);}
LengthError::LengthError(const String& s):std::length_error(s.data?s.data:"") {
    construct_logic(object_,s,heap_);object_.base.vtable=0x0043bb54;
}
LengthError::LengthError(const LengthError& s):std::length_error(s) {
    copy_logic(object_,s.object_,heap_);object_.base.vtable=0x0043bb54;
}
LengthError::~LengthError() noexcept {destroy_logic_copy(object_,heap_);}
PositionError::PositionError(const String& s):std::out_of_range(s.data?s.data:"") {
    construct_logic(object_,s,heap_);object_.base.vtable=0x0043bb84;
}
PositionError::PositionError(const PositionError& s):std::out_of_range(s) {
    copy_logic(object_,s.object_,heap_);object_.base.vtable=0x0043bb84;
}
PositionError::~PositionError() noexcept {destroy_position(object_,heap_);}
[[noreturn]] void throw_string_too_long(){TemporaryString s("string too long");throw LengthError(s.text);}
[[noreturn]] void throw_invalid_string_position(){TemporaryString s("invalid string position");throw PositionError(s.text);}
bool verify_9_original_exception_functions() {
    NativeHeap heap;String text{};assign_buffer(text,"original",8,heap);
    Object a,b,c;construct_logic(a,text,heap);copy_logic(b,a,heap);copy_runtime(c,a,heap);
    if(a.text.data!=text.data || b.text.data!=text.data || c.base.vtable!=0x0043b328)return false;
    destroy_runtime(c,heap);destroy_logic_copy(b,heap);destroy_logic(a,heap);tidy(text,true,heap);
    Object d;String empty;construct_logic(d,empty,heap);destroy_position(d,heap);
    try{throw_string_too_long();}catch(const LengthError& e){if(std::strcmp(e.what(),"string too long"))return false;}
    try{throw_invalid_string_position();}catch(const PositionError& e){return std::strcmp(e.what(),"invalid string position")==0;}
    return false;
}
}
