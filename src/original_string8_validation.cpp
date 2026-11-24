#include "btb/original_string8.hpp"
#include <cstring>
namespace btb::original_string8 {
bool verify_8_original_string_functions() {
    NativeHeap heap;
    struct Owner {
        String s;Heap& heap;
        ~Owner(){tidy(s,true,heap);}
    } a{{},heap},b{{},heap};
    assign_buffer(a.s,"original string",15,heap);
    if(a.s.capacity!=31 || a.s.length!=15 || a.s.data[-1]!=0)return false;
    assign_substring(b.s,a.s,0,0xffffffffU,heap);
    if(a.s.data!=b.s.data || a.s.data[-1]!=1)return false;
    detach_shared_buffer(b.s,heap);
    if(a.s.data==b.s.data || a.s.data[-1]!=0 || std::strcmp(a.s.data,b.s.data))return false;
    assign_substring(b.s,b.s,9,6,heap);
    if(std::strcmp(b.s.data,"string"))return false;
    if(!grow(b.s,40,false,heap) || b.s.capacity!=63 || b.s.length!=6)return false;
    // Exercise the separately reconstructed original catch handler body.
    auto exact=retry_exact_allocation(6,heap);
    attach_copy_buffer(b.s,exact,heap);
    if(b.s.capacity!=6 || std::strcmp(b.s.data,"string"))return false;
    char overlap[]="abcdef";
    return move_bytes(overlap+1,overlap,5)==overlap+1 && !std::strcmp(overlap,"aabcde");
}
} // namespace btb::original_string8
