#include "btb/original_heap4.hpp"
#include <bit>
#include <limits>
#include <stdexcept>
namespace btb::original_heap4 {
U32 find_small2(State& s,U32 pointer,U32& region,U32& page,Services& io){
 U32 at=s.head;std::size_t visits=0;
 while(pointer<=io.read(at+16) || io.read(at+20)<=pointer){at=io.read(at);if(at==s.head)return 0;if(++visits>1048576)throw std::runtime_error("invalid original heap list");}
 if((pointer&15) || (pointer&4095)<256)return 0;
 region=at;page=pointer&0xfffff000U;return page+8+((pointer-page-256)>>4);
}
void release_region2(State& s,U32 region,Services& io){
 io.virtual_free(io.read(region+16),0,0x8000);
 if(s.current==region)s.current=io.read(region+4);
 if(region!=s.head){const U32 prev=io.read(region+4),next=io.read(region);io.write(prev,next);io.write(next+4,prev);io.heap_free(s.heap,region);}
 else io.write(s.head+16,0xffffffffU);
}
void decommit_small2(State& s,I32 requested,Services& io){
 U32 region=io.read(s.head+4);U32 remaining=static_cast<U32>(requested);std::size_t visits=0;
 for(;;){U32 next=region;
  if(io.read(region+16)!=0xffffffffU){U32 released=0;
   for(I32 i=1023;i>=0;--i){const U32 entry=region+24+static_cast<U32>(i)*8;
    if(io.read(entry)==240 && io.virtual_free(io.read(region+16)+static_cast<U32>(i)*4096,4096,0x4000)){
     io.write(entry,0xffffffffU);--s.free_pages;const U32 first=io.read(region+12);if(!first || entry<first)io.write(region+12,entry);++released;--remaining;if(!remaining)break;
    }
   }
   next=io.read(region+4);
   if(released && io.read(region+24)==0xffffffffU){U32 i=1;for(;i<1024 && io.read(region+24+i*8)==0xffffffffU;++i){}if(i==1024)release_region2(s,region,io);}
  }
  if(next==io.read(s.head+4) || std::bit_cast<I32>(remaining)<1)return;region=next;
  if(++visits>1048576)throw std::runtime_error("invalid original heap list");
 }
}
U32 add_region3(State& s,Services& io){
 if(s.regions==s.capacity){const U32 resized=io.heap_resize(s.heap,s.table,(s.capacity*5+80)*4);if(!resized)return 0;s.capacity+=16;s.table=resized;}
 const U32 record=s.table+s.regions*20;const U32 control=io.heap_allocate(s.heap,8,0x41c4);io.write(record+16,control);if(!control)return 0;
 const U32 reserve=io.virtual_allocate(0x100000,0x2000,4);io.write(record+12,reserve);
 if(!reserve){io.heap_free(s.heap,control);return 0;}
 io.write(record+8,0xffffffffU);io.write(record,0);io.write(record+4,0);++s.regions;io.write(control,0xffffffffU);return record;
}
void* LookupLink::find2(void* ptr,std::uintptr_t& region,std::uintptr_t& page){
 const auto address=reinterpret_cast<std::uintptr_t>(ptr);if(address>std::numeric_limits<U32>::max())throw std::out_of_range("original heap pointer exceeds PE32");
 U32 r{},p{};const U32 found=find_small2(state_,static_cast<U32>(address),r,p,services_);if(found){region=r;page=p;}return reinterpret_cast<void*>(static_cast<std::uintptr_t>(found));
}
}
