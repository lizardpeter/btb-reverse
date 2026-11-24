#include "btb/original_string8.hpp"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>
#include <stdexcept>
#include <unordered_set>
#include <vector>
using namespace btb::original_string8;
struct TestHeap final:Heap {
    std::vector<U32> sizes;
    std::unordered_set<char*> live;
    unsigned failures{};
    char* allocate(U32 bytes) override {
        sizes.push_back(bytes);
        if(failures){--failures;throw std::runtime_error("original catch-all allocation test");}
        if(!bytes)throw std::bad_alloc{};
        auto* out=static_cast<char*>(std::malloc(bytes));if(!out)throw std::bad_alloc{};
        live.insert(out);return out;
    }
    void release(char* memory) noexcept override {
        assert(live.erase(memory)==1);std::free(memory);
    }
    ~TestHeap(){assert(live.empty());}
};
int main() {
    assert(verify_8_original_string_functions());
    TestHeap heap;
    String s{};s.allocator_token=19;
    assign_buffer(s,"abcdefghij",10,heap);
    assert(s.allocator_token==19 && s.capacity==31 && s.length==10 && heap.sizes.back()==33);
    char* original=s.data;
    assign_buffer(s,"abc",3,heap);assert(s.data==original && !std::strcmp(s.data,"abc"));
    grow(s,32,false,heap);assert(s.capacity==63 && s.length==3 && !std::strcmp(s.data,"abc"));
    auto allocations=heap.sizes.size();
    assign_buffer(s,"xy",2,heap);
    assert(s.capacity==31 && heap.sizes.size()==allocations+1); // large capacity is not reused
    // The original catches ANY exception and retries requested+2, not the
    // rounded allocation. Copied content survives non-trimming growth.
    heap.failures=1;allocations=heap.sizes.size();
    allocate_copy_buffer(s,50,heap);
    assert(heap.sizes[allocations]==65 && heap.sizes[allocations+1]==52);
    assert(s.capacity==50 && s.length==2 && !std::strcmp(s.data,"xy"));
    heap.failures=2;bool failed=false;
    try{allocate_copy_buffer(s,100,heap);}catch(const std::runtime_error&){failed=true;}
    assert(failed && s.capacity==50 && !std::strcmp(s.data,"xy")); // both allocations failed before release
    String copy{};
    assign_substring(copy,s,0,0xffffffffU,heap);
    assert(copy.data==s.data && static_cast<unsigned char>(s.data[-1])==1);
    assert(grow(copy,3,false,heap));
    assert(copy.data!=s.data && copy.data[-1]==0 && s.data[-1]==0);
    assign_buffer(copy,"changed",7,heap);assert(!std::strcmp(s.data,"xy"));
    // Zero-length grow retains unique storage unless trim is requested.
    original=copy.data;U32 cap=copy.capacity;
    assert(!grow(copy,0,false,heap) && copy.data==original && copy.capacity==cap && copy.length==0);
    assert(!grow(copy,0,true,heap) && !copy.data && copy.capacity==0);
    assign_substring(copy,s,0,2,heap);original=s.data;
    assert(!grow(copy,0,false,heap) && !copy.data && s.data==original && s.data[-1]==0);
    // Detach uses original strlen, including its treatment of embedded NUL.
    const char embedded[]={'a','\0','b'};
    assign_buffer(s,embedded,3,heap);assign_substring(copy,s,0,3,heap);
    assert(copy.length==3 && copy.data==s.data);
    detach_shared_buffer(copy,heap);assert(copy.length==1 && copy.data!=s.data && s.length==3);
    tidy(copy,true,heap);
    // Saturated FE and FF reference bytes prohibit sharing whole strings.
    for(unsigned count:{254,255}) {
        s.data[-1]=static_cast<char>(count);
        assign_substring(copy,s,0,3,heap);assert(copy.data!=s.data && copy.length==3);
        tidy(copy,true,heap);
    }
    s.data[-1]=0;
    assign_buffer(s,"0123456789",10,heap);
    assign_substring(copy,s,3,4,heap);assert(!std::strcmp(copy.data,"3456"));
    assign_substring(s,s,3,4,heap);assert(s.length==4 && !std::strcmp(s.data,"3456"));
    assign_substring(s,s,4,9,heap);assert(s.length==0 && s.data && s.data[0]=='\0');
    failed=false;try{assign_substring(copy,s,1,0,heap);}catch(const std::out_of_range&){failed=true;}assert(failed);
    failed=false;try{grow(copy,0xfffffffeU,false,heap);}catch(const std::length_error&){failed=true;}assert(failed);
    tidy(s,true,heap);tidy(copy,true,heap);
    // Compare all bounded overlap directions/counts against host memmove.
    for(unsigned src=0;src<16;++src)for(unsigned dst=0;dst<16;++dst)for(unsigned n=0;n<=16;++n) {
        std::array<char,32> a{},b{};
        for(unsigned i=0;i<a.size();++i)a[i]=b[i]=static_cast<char>(i);
        assert(move_bytes(a.data()+dst,a.data()+src,n)==a.data()+dst);
        std::memmove(b.data()+dst,b.data()+src,n);assert(a==b);
    }
}
