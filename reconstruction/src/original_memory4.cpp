#include "btb/original_memory4.hpp"
#include "btb/original_crt17.hpp"
#include <array>
#include <stdexcept>
namespace btb::original_memory4 {
void* allocate(const HeapState& s,U32 n,HeapServices& io){
 if(s.mode==3){if(n<=s.threshold3)if(auto* p=io.allocate3(n))return p;}
 else if(s.mode==2){const U32 rounded=n?(n+15U)&~15U:16U;if(rounded<=s.threshold2)if(auto* p=io.allocate2(rounded>>4))return p;return io.heap_allocate(s.heap,rounded);}
 return io.heap_allocate(s.heap,((n?n:1U)+15U)&~15U);
}
void release(const HeapState& s,void* p,HeapServices& io){
 if(!p)return;
 if(s.mode==3){if(const auto region=io.find3(p)){io.free3(region,p);return;}}
 else if(s.mode==2){Handle region{},page{};if(auto* block=io.find2(p,region,page)){io.free2(region,page,block);return;}}
 io.heap_free(s.heap,p);
}
void* copy_bytes(void* destination,const void* source,U32 n) noexcept {
 auto* d=static_cast<std::byte*>(destination);const auto* s=static_cast<const std::byte*>(source);
 const auto da=reinterpret_cast<Handle>(d),sa=reinterpret_cast<Handle>(s);
 if(sa<da && da-sa<n){for(U32 i=n;i>0;--i)d[i-1]=s[i-1];}
 else for(U32 i=0;i<n;++i)d[i]=s[i];
 return destination;
}
void sort_bytes(std::span<std::byte> b,U32 count,U32 width,original_runtime8::Comparator compare,void* ctx){
 if(count<2 || !width)return;
 if(!compare || static_cast<std::uint64_t>(count)*width>b.size())throw std::invalid_argument("invalid original qsort buffer");
 using Index=std::int64_t;std::array<std::pair<Index,Index>,30> stack{};std::size_t depth=0;Index low=0,high=count-1;
 const auto at=[&](Index i){return b.data()+static_cast<std::size_t>(i)*width;};
 const auto swap=[&](Index a,Index c){original_crt17::swap_forward_bytes({at(a),width},{at(c),width},width);};
 for(;;){
  const auto size=high-low+1;
  if(size<=8)original_runtime8::selection_sort_bytes({at(low),static_cast<std::size_t>(size)*width},width,compare,ctx);
  else {
   swap(low+size/2,low);Index left=low,right=high+1;
   for(;;){do{++left;}while(left<=high && compare(at(left),at(low),ctx)<=0);do{--right;}while(right>low && compare(at(right),at(low),ctx)>=0);if(left>right)break;swap(left,right);}
   swap(low,right);
   const auto push=[&](Index a,Index c){if(depth==stack.size())throw std::overflow_error("original qsort stack");stack[depth++]={a,c};};
   // Original compares byte extents, with an extra -1 in its left extent.
   if((right-low)*width-1 < (high-left)*width){if(left<high)push(left,high);if(low+1<right){high=right-1;continue;}}
   else {if(low+1<right)push(low,right-1);if(left<high){low=left;continue;}}
  }
  if(!depth)return;const auto range=stack[--depth];low=range.first;high=range.second;
 }
}
bool verify_4_original_memory_functions(){std::array<std::byte,4>b{std::byte{1},std::byte{2},std::byte{3},std::byte{4}};return copy_bytes(b.data()+1,b.data(),3)==b.data()+1 && b[3]==std::byte{3};}
}
