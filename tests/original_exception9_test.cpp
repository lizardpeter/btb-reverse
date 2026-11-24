#include "btb/original_exception9.hpp"
#include <cassert>
#include <cstring>
#include <new>
#include <unordered_set>
using namespace btb::original_exception9;
struct HeapTest final:Heap {
    bool fail{};std::unordered_set<char*> live;
    char* allocate(U32 bytes) override {if(fail)throw std::bad_alloc{};auto* p=new char[bytes];live.insert(p);return p;}
    void release(char* p) noexcept override {assert(live.erase(p)==1);delete[] p;}
};
int main() {
    assert(verify_9_original_exception_functions());
    HeapTest h;String s;assign_buffer(s,"payload",7,h);s.allocator_token=0x91;
    Object a,b,c;construct_logic(a,s,h);
    assert(a.base.vtable==0x43bb74 && a.base.owned==1 && !*static_cast<char*>(a.base.message));
    copy_logic(b,a,h);copy_runtime(c,a,h);
    assert(b.base.message!=a.base.message && b.text.data==s.data && c.text.data==s.data);
    assert(static_cast<unsigned char>(s.data[-1])==3 && b.text.allocator_token==0x91);
    destroy_runtime(c,h);destroy_logic_copy(b,h);destroy_logic(a,h);tidy(s,true,h);
    assert(h.live.empty());
    assign_buffer(s,"locked",6,h);s.data[-1]=static_cast<char>(255);
    construct_logic(a,s,h);assert(a.text.data!=s.data);
    copy_runtime(b,a,h);destroy_runtime(b,h);destroy_position(a,h);tidy(s,true,h);
    assert(h.live.empty());
    assign_buffer(s,"failure",7,h);s.data[-1]=static_cast<char>(255);h.fail=true;
    try{construct_logic(a,s,h);assert(false);}catch(const std::bad_alloc&){}
    h.fail=false;tidy(s,true,h);assert(h.live.empty());
    try{throw_string_too_long();}catch(const LengthError& e){
        LengthError copy(e);assert(!std::strcmp(copy.what(),"string too long"));
        assert(copy.retail_object().text.data==e.retail_object().text.data);
        assert(copy.retail_object().base.message!=e.retail_object().base.message);
    }
    try{throw_invalid_string_position();}catch(const PositionError& e){
        PositionError copy(e);assert(!std::strcmp(copy.what(),"invalid string position"));
    }
    NativeHeap n;String bad;
    try{grow(bad,0xfffffffeU,false,n);assert(false);}catch(const LengthError&){}
    try{assign_substring(bad,bad,1,0,n);assert(false);}catch(const PositionError&){}
}
