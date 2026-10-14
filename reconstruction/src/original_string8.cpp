#include "btb/original_string8.hpp"
#include "btb/original_exception9.hpp"
#include "btb/original_system14.hpp"
#include <algorithm>
#include <bit>
#include <cstring>
#include <cstdlib>
#include <new>
#include <stdexcept>
namespace btb::original_string8 {
namespace {
bool shared(const String& s) noexcept {
    return s.data && static_cast<unsigned char>(s.data[-1])!=0 &&
        static_cast<unsigned char>(s.data[-1])!=255;
}
U32 allocation_bytes(U32 capacity) noexcept {
    const U32 bytes=capacity+2;
    return std::bit_cast<std::int32_t>(bytes)<0?0:bytes;
}
void check_length(U32 n) {
    if(n>static_cast<U32>(btb::original_system14::string_error_code_minus_three()))
        btb::original_exception9::throw_string_too_long();
}
// Original REP MOVSD followed by REP MOVSB, with each word loaded before
// it is written. This avoids imposing memcpy's overlap precondition.
void copy_forward(char* dest,const char* src,U32 count) {
    while(count>=4) {
        std::array<char,4> word;
        std::memcpy(word.data(),src,4);std::memcpy(dest,word.data(),4);
        dest+=4;src+=4;count-=4;
    }
    while(count--)*dest++=*src++;
}
}
char* NativeHeap::allocate(U32 bytes) {
    // Zero-size signed-overflow requests cannot describe a usable host
    // string allocation. Reject them instead of permitting corruption.
    if(!bytes)throw std::bad_alloc{};
    auto* memory=static_cast<char*>(std::malloc(bytes));
    if(!memory)throw std::bad_alloc{};
    return memory;
}
void NativeHeap::release(char* memory) noexcept {std::free(memory);}
void tidy(String& s,bool release,Heap& heap) noexcept {
    char* old=s.data;
    btb::retail_system::MsvcString view{
        old?std::span<char>{old,static_cast<std::size_t>(s.capacity)+1}:std::span<char>{},
        std::bit_cast<std::int32_t>(s.length),std::bit_cast<std::int32_t>(s.capacity),
        old?static_cast<unsigned char>(old[-1]):static_cast<unsigned char>(0)};
    const auto result=btb::original_system14::msvc_string_tidy(view,release);
    if(result.decrement_reference)old[-1]=static_cast<char>(view.refcount);
    if(result.free_allocation)heap.release(old-1);
    s.data=nullptr;s.length=0;s.capacity=0;
}
void set_end(String& s,U32 length) {
    btb::retail_system::MsvcString view{
        s.data?std::span<char>{s.data,static_cast<std::size_t>(s.capacity)+1}:std::span<char>{},
        std::bit_cast<std::int32_t>(s.length),std::bit_cast<std::int32_t>(s.capacity),0};
    if(!btb::original_system14::msvc_string_set_end(view,std::bit_cast<std::int32_t>(length)))
        throw std::out_of_range("invalid original string end");
    s.length=length;
}
Allocation retry_exact_allocation(U32 requested,Heap& heap) {
    return {heap.allocate(allocation_bytes(requested)),requested};
}
void attach_copy_buffer(String& s,Allocation allocation,Heap& heap) {
    if(!allocation.base)throw std::bad_alloc{};
    const U32 copied=std::min(s.length,allocation.capacity);
    if(copied)copy_forward(allocation.base+1,s.data,copied);
    tidy(s,true,heap);
    s.data=allocation.base+1;s.capacity=allocation.capacity;
    s.data[-1]=0;set_end(s,copied);
}
void allocate_copy_buffer(String& s,U32 requested,Heap& heap) {
    check_length(requested);
    U32 rounded=requested|0x1f;
    if(rounded>0xfffffffdU)rounded=requested;
    Allocation allocation;
    try {allocation={heap.allocate(allocation_bytes(rounded)),rounded};}
    catch(...) {allocation=retry_exact_allocation(requested,heap);}
    attach_copy_buffer(s,allocation,heap);
}
String& assign_buffer(String& s,const char* data,U32 length,Heap& heap) {
    check_length(length);
    if(!shared(s)) {
        if(!length){tidy(s,true,heap);return s;}
        if(!(s.capacity<32 && length<=s.capacity)) {
            tidy(s,true,heap);allocate_copy_buffer(s,length,heap);
        }
    } else {
        if(!length){tidy(s,true,heap);return s;}
        allocate_copy_buffer(s,length,heap);
    }
    copy_forward(s.data,data,length);set_end(s,length);return s;
}
bool grow(String& s,U32 requested,bool trim,Heap& heap) {
    check_length(requested);
    if(shared(s)) {
        if(!requested){tidy(s,true,heap);return false;}
        allocate_copy_buffer(s,requested,heap);return true;
    }
    if(!requested) {
        if(trim)tidy(s,true,heap);
        else if(s.data)set_end(s,0);
        return false;
    }
    if(trim) {
        if(s.capacity>=32 || s.capacity<requested) {
            tidy(s,true,heap);allocate_copy_buffer(s,requested,heap);
        }
    } else if(s.capacity<requested)allocate_copy_buffer(s,requested,heap);
    return true;
}
void detach_shared_buffer(String& s,Heap& heap) {
    if(!shared(s))return;
    char* old=s.data;
    old[-1]=static_cast<char>(static_cast<unsigned char>(old[-1])-1);
    s.data=nullptr;s.length=0;s.capacity=0;
    const auto length=std::strlen(old);
    if(length>0xfffffffdULL)btb::original_exception9::throw_string_too_long();
    assign_buffer(s,old,static_cast<U32>(length),heap);
}
void* move_bytes(void* destination,const void* source,U32 count) noexcept {
    auto* out=static_cast<unsigned char*>(destination);
    auto* in=static_cast<const unsigned char*>(source);
    const auto d=reinterpret_cast<std::uintptr_t>(out),s=reinterpret_cast<std::uintptr_t>(in);
    if(d>s && d-s<count) {
        for(U32 i=count;i>0;--i)out[i-1]=in[i-1];
    } else for(U32 i=0;i<count;++i)out[i]=in[i];
    return destination;
}
String& assign_substring(String& dest,const String& source,U32 offset,U32 count,Heap& heap) {
    if(offset>source.length)btb::original_exception9::throw_invalid_string_position();
    const U32 length=std::min(count,source.length-offset);
    if(&dest==&source) {
        const U32 end=offset+length;
        detach_shared_buffer(dest,heap);
        if(end>dest.length)throw std::out_of_range("invalid detached string position");
        if(dest.length>end) {
            if(grow(dest,end,false,heap))set_end(dest,end);
        }
        detach_shared_buffer(dest,heap);
        const U32 erased=std::min(offset,dest.length);
        if(!erased)return dest;
        const U32 left=dest.length-erased;
        move_bytes(dest.data,dest.data+erased,left);
        if(grow(dest,left,false,heap))set_end(dest,left);
        return dest;
    }
    if(length && length==source.length && source.data &&
       static_cast<unsigned char>(source.data[-1])<254) {
        tidy(dest,true,heap);
        dest.data=source.data;dest.length=source.length;dest.capacity=source.capacity;
        dest.data[-1]=static_cast<char>(static_cast<unsigned char>(dest.data[-1])+1);
        return dest;
    }
    check_length(length);
    if(!shared(dest)) {
        if(!length){tidy(dest,true,heap);return dest;}
        if(!(dest.capacity<32 && length<=dest.capacity)) {
            tidy(dest,true,heap);allocate_copy_buffer(dest,length,heap);
        }
    } else {
        if(!length){tidy(dest,true,heap);return dest;}
        allocate_copy_buffer(dest,length,heap);
    }
    copy_forward(dest.data,source.data+offset,length);set_end(dest,length);
    return dest;
}
} // namespace btb::original_string8
