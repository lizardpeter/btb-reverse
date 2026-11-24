#include "btb/original_crt17.hpp"
#include "btb/original_runtime5.hpp"
#include <array>
#include <cstring>
namespace btb::original_crt17 {
namespace {
struct Probe {I32 deletes{},destroyed{},allocated{},handler_calls{},raw_calls{};
 I32 fopen_calls{},scans{},format_calls{};U32 opened_flags{},radix{};bool negative{};};
Probe g{};
char buffer[128]{};
void release(Ptr) noexcept {++g.deletes;}
void destroy(Ptr) noexcept {++g.destroyed;}
Ptr alloc(std::size_t size,I32 flag) noexcept {
 ++g.allocated;return size<=sizeof(buffer)&&flag==1?buffer:nullptr;
}
Ptr raw(std::size_t count) noexcept {
 ++g.raw_calls;return count<=sizeof(buffer)&&g.raw_calls>=2?buffer:nullptr;
}
bool handler(std::size_t count) noexcept {
 ++g.handler_calls;return count<sizeof(buffer);
}
Ptr thread() noexcept {return buffer;}
Ptr opened(const char*,const char*,U32 f,Ptr state) noexcept {
 ++g.fopen_calls;g.opened_flags=f;return state;
}
I32 scanned(Ptr,const char*,Ptr) noexcept {++g.scans;return 17;}
void format(U32,char* out,U32 base,bool negative) noexcept {
 ++g.format_calls;g.radix=base;g.negative=negative;
 out[0]=negative?'-':'X';out[1]=0;
}
bool dialog(Ptr p) noexcept {return p==buffer;}
}
bool verify_17_original_crt_functions() noexcept {
 g={};
 U32 seed=0;
 retail_srand(seed,1);
 if(seed!=1||retail_rand(seed)!=41U)return false;
 const U32 next=retail_rand(seed);
 if(next>0x7fffU || next==41U)return false;
 delete_proxy(buffer,release);
 if(g.deletes!=1)return false;
 if(operator_new(24,alloc)!=buffer || g.allocated!=1)return false;
 if(crt_malloc(24,0,alloc)!=nullptr || g.allocated!=2)return false;
 g.raw_calls=0;g.handler_calls=0;
 if(nh_malloc(16,1,raw,handler)!=buffer ||
    g.raw_calls!=2||g.handler_calls!=1)return false;
 if(nh_malloc(0xffffffe1ULL,1,raw,handler)!=nullptr)return false;
 if(fopen_internal("a.txt","rb",3,thread,opened)!=buffer ||
    g.opened_flags!=3U)return false;
 if(fopen_native("b.txt","rt",thread,opened)!=buffer ||
    g.opened_flags!=0x40U||g.fopen_calls!=2)return false;
 if(fscanf_native(buffer,"%d",buffer,scanned)!=17||g.scans!=1)return false;
 NlgGlobals n{};
 nlg_notify(n,19,21,23);
 if(n.target!=19||n.eax!=21||n.ebp!=23)return false;
 nlg_from_stack(n,31,32,33);
 if(n.target!=31||n.eax!=32||n.ebp!=33)return false;
 Exception ex{0,buffer,1};
 exception_destruct(ex,0x0043b5bcU,release);
 if(ex.vtable!=0x0043b5bcU||g.deletes!=2)return false;
 TypeInfo ti{0,buffer};
 type_info_destruct(ti,0x0043b6ccU,release);
 if(ti.vtable!=0x0043b6ccU||g.deletes!=3)return false;
 std::array<std::byte,4> left{std::byte{1},std::byte{2},
                              std::byte{3},std::byte{4}};
 std::array<std::byte,4> right{std::byte{5},std::byte{6},
                               std::byte{7},std::byte{8}};
 swap_forward_bytes(left,right,4);
 if(left[0]!=std::byte{5}||left[3]!=std::byte{8}||
    right[0]!=std::byte{1}||right[3]!=std::byte{4})return false;
 char digits[8]{};
 if(retail_itoa(0xffffffffU,digits,10,format)!=digits ||
    digits[0]!='-'||!g.negative||g.radix!=10U)return false;
 if(retail_itoa(0xffffffffU,digits,16,format)!=digits ||
    digits[0]!='X'||g.negative||g.radix!=16U||
    g.format_calls!=2)return false;
    // A genuine original callgraph edge: original 00430F2F itoa calls
    // original 00430F5C formatting worker directly in this game image.
    if(retail_itoa(26U,digits,16,
         btb::original_runtime5::retail_write_radix)!=digits ||
       std::strcmp(digits,"1a")!=0)return false;
 if(deleting_exception(buffer,1,destroy,release)!=buffer ||
    g.destroyed!=1||g.deletes!=4)return false;
 if(deleting_exception(buffer,0,destroy,release)!=buffer ||
    g.destroyed!=2||g.deletes!=4)return false;
 if(!print_dialog_thunk(buffer,dialog)||
    print_dialog_thunk(nullptr,dialog))return false;
 return true;
}
}
