#include "btb/original_heap9.hpp"
#include "btb/original_crt_batch_32.hpp"
#include "btb/runtime_dependency_32.hpp"
#include <algorithm>
#include <bit>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <vector>
namespace btb::original_heap9 {
namespace {
constexpr U32 bad=0xffffffffU;
std::uint8_t byte(Services& s,U32 p){return static_cast<std::uint8_t>(s.read(p&~3U)>>((p&3)*8));}
void byte(Services& s,U32 p,U32 v){const U32 shift=(p&3)*8,at=p&~3U;s.write(at,(s.read(at)&~(255U<<shift))|((v&255)<<shift));}
I32 signed32(U32 x){return std::bit_cast<I32>(x);}
U32 bucket(U32 n){return std::min<U32>(63,static_cast<U32>((signed32(n)>>4)-1));}
void clear_bin(U32 row,U32 group,U32 bin,Services& s){
 const U32 control=s.read(row+16),which=bin<32?0:1,mask=~(0x80000000U>>(bin&31)),bits=control+(which?0xc4:0x44)+group*4;
 s.write(bits,s.read(bits)&mask);const auto count=static_cast<std::uint8_t>(byte(s,control+4+bin)-1);byte(s,control+4+bin,count);if(!count)s.write(row+which*4,s.read(row+which*4)&mask);
}
void set_bin(U32 row,U32 group,U32 bin,Services& s){
 const U32 control=s.read(row+16),which=bin<32?0:1,mask=0x80000000U>>(bin&31),bits=control+(which?0xc4:0x44)+group*4;
 const auto old=byte(s,control+4+bin);byte(s,control+4+bin,old+1);if(!old)s.write(row+which*4,s.read(row+which*4)|mask);s.write(bits,s.read(bits)|mask);
}
void unlink(U32 row,U32 group,U32 bin,U32 block,Services& s){
 const U32 next=s.read(block+4),prev=s.read(block+8);if(next==prev)clear_bin(row,group,bin,s);s.write(prev+4,next);s.write(next+8,prev);
}
void insert(U32 row,U32 group,U32 group_base,U32 bin,U32 block,Services& s){
 const U32 head=group_base+bin*8,next=s.read(head+4);s.write(block+4,next);s.write(block+8,head);s.write(head+4,block);s.write(next+8,block);if(next==head)set_bin(row,group,bin,s);
}
U32 pointer(void* p){const auto n=reinterpret_cast<std::uintptr_t>(p);if(n>bad)throw std::out_of_range("original PE32 heap pointer");return static_cast<U32>(n);}
void* host(U32 p){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(p));}
U32 find_region(State& s,U32 p,Services& io){
 if(s.base.regions>1048576)throw std::out_of_range("original heap table");std::vector<original_crt32::RegionRow> rows(s.base.regions);
 for(U32 i=0;i<s.base.regions;++i)for(U32 j=0;j<5;++j)rows[i][j]=io.read(s.base.table+i*20+j*4);
 const auto* found=original_crt32::find_allocation_region(p,rows);return found?s.base.table+static_cast<U32>(found-rows.data())*20:0;
}
void free_slot(State& s,U32 region,U32 page,U32 slot,Services& io){
 const U32 entry=region+24+((page-io.read(region+16))>>12)*8;U32 available=io.read(entry),pages=s.base.free_pages;auto value=byte(io,slot),indicator=static_cast<std::uint8_t>(io.read(entry+4));bool reclaimed=false;
 const auto save=[&]{io.write(entry,available);byte(io,slot,value);io.write(entry+4,indicator);s.base.free_pages=pages;};
 btb::runtime_batch_32::release_heap_slot(available,value,indicator,pages,[&](I32 count){save();reclaimed=true;original_heap4::decommit_small2(s.base,count,io);});if(!reclaimed)save();
}
U32 malloc_public(State& s,U32 n,Services& io){Link link(s,io);const original_memory4::HeapState state{s.mode,s.threshold3,s.threshold2,s.base.heap};for(;;){U32 result=0;if(n<=0xffffffe0U)result=pointer(original_memory4::allocate(state,n,link));if(result || !s.retry || !io.new_handler(n))return result;}}
void release_public(State& s,U32 p,Services& io){Link link(s,io);original_memory4::release({s.mode,s.threshold3,s.threshold2,s.base.heap},host(p),link);}
void copy(Services& io,U32 to,U32 from,U32 n){auto dest=io.bytes(to,n),src=io.bytes(from,n);original_memory4::copy_bytes(dest.data(),src.data(),n);}
}
U32 add_region2(State& s,Services& io){
 const U32 head=s.base.head;U32 region=head;if(io.read(head+16)!=bad){region=io.heap_allocate(s.base.heap,0,0x2020);if(!region)return 0;}
 const U32 reserve=io.virtual_allocate(0x400000,0x2000,4);
 if(reserve){const U32 committed=io.commit(reserve,0x10000);if(committed){
  if(region==head){if(!io.read(head))io.write(head,head);if(!io.read(head+4))io.write(head+4,head);}
  else {const U32 tail=io.read(head+4);io.write(region,head);io.write(region+4,tail);io.write(head+4,region);io.write(tail,region);}
  io.write(region+20,reserve+0x400000);io.write(region+12,region+0x98);io.write(region+16,reserve);io.write(region+8,region+24);
  for(U32 i=0;i<1024;++i){io.write(region+24+i*8,i<16?240:bad);io.write(region+28+i*8,241);}
  std::fill(io.bytes(reserve,0x10000).begin(),io.bytes(reserve,0x10000).end(),std::byte{0});
  for(U32 page=reserve;page<reserve+0x10000;page+=4096){byte(io,page+248,255);io.write(page,page+8);io.write(page+4,240);}return region;
 }io.virtual_free(reserve,0,0x8000);}
 if(region!=head)io.heap_free(s.base.heap,region);return 0;
}
I32 commit_group3(State& s,U32 row,Services& io){
 (void)s;const U32 control=io.read(row+16),free=io.read(row+8);if(!free)throw std::invalid_argument("original heap commit needs a free group");
 const U32 group=static_cast<U32>(std::countl_zero(free)),base=control+0x144+group*0x204;
 for(U32 i=0;i<63;++i){const U32 head=base+i*8;io.write(head+8,head);io.write(head+4,head);}
 const U32 address=io.read(row+12)+group*0x8000;if(!io.commit(address,0x8000))return -1;
 for(U32 i=0;i<8;++i){const U32 page=address+i*4096;io.write(page+8,bad);io.write(page+0xffc,bad);io.write(page+12,0xff0);io.write(page+16,page+0x100c);io.write(page+20,page-0xff4);io.write(page+0xff8,0xff0);}
 io.write(base+0x1fc,address+12);io.write(address+20,base+0x1f8);io.write(base+0x200,address+0x700c);io.write(address+0x7010,base+0x1f8);
 io.write(control+0x44+group*4,0);io.write(control+0xc4+group*4,1);const auto old=byte(io,control+0x43);byte(io,control+0x43,old+1);if(!old)io.write(row+4,io.read(row+4)|1);io.write(row+8,free&~(0x80000000U>>group));return static_cast<I32>(group);
}
U32 allocate3(State& s,U32 requested,Services& io){
 const U32 size=(requested+23)&~15U;const I32 index=(signed32(requested+23)>>4)-1;const U32 high=index<32?0xffffffffU:0xffffffffU>>(static_cast<U32>(index-32)&31),low=index<32?0xffffffffU>>(static_cast<U32>(index)&31):0;
 const U32 end=s.base.table+s.base.regions*20;U32 row=s.current3;
 if(row<s.base.table || row>end)throw std::out_of_range("original current heap region");
 const auto fits=[&](U32 p){return (io.read(p+4)&high) || (io.read(p)&low);};
 while(row<end && !fits(row))row+=20;
 if(row==end){row=s.base.table;while(row<s.current3 && !fits(row))row+=20;
  if(row==s.current3){while(row<end && !io.read(row+8))row+=20;
   if(row==end){row=s.base.table;while(row<s.current3 && !io.read(row+8))row+=20;if(row==s.current3){row=original_heap4::add_region3(s.base,io);if(!row)return 0;}}
   const I32 group=commit_group3(s,row,io);io.write(io.read(row+16),static_cast<U32>(group));if(group==-1)return 0;
  }
 }
 const U32 control=io.read(row+16);U32 group=io.read(control);
 const auto group_fits=[&](U32 g){return (io.read(control+0xc4+g*4)&high) || (io.read(control+0x44+g*4)&low);};
 if(group==bad || !group_fits(group)){for(group=0;group<32 && !group_fits(group);++group){}if(group==32)throw std::runtime_error("invalid original heap bitmap");}
 const U32 base=control+0x144+group*0x204;U32 bits=io.read(control+0x44+group*4)&low,bin=0;if(!bits){bits=io.read(control+0xc4+group*4)&high;bin=32;}if(!bits)throw std::runtime_error("empty original allocation mask");bin+=static_cast<U32>(std::countl_zero(bits));
 U32 block=io.read(base+bin*8+4);const U32 remainder=io.read(block)-size;const I32 next_bin=std::min<I32>(63,(signed32(remainder)>>4)-1);s.current3=row;
 if(next_bin!=static_cast<I32>(bin)){unlink(row,group,bin,block,io);if(remainder)insert(row,group,base,static_cast<U32>(next_bin),block,io);}
 if(remainder){io.write(block,remainder);io.write(block+remainder-4,remainder);}
 block+=remainder;io.write(block,size+1);io.write(block+size-4,size+1);const U32 live=io.read(base);io.write(base,live+1);if(!live && row==s.spare3 && group==s.spare_page)s.spare3=0;io.write(control,group);return block+4;
}
void release3(State& s,U32 row,U32 pointer_value,Services& io){
 const U32 control=io.read(row+16),group=(pointer_value-io.read(row+12))>>15,base=control+0x144+group*0x204;U32 block=pointer_value-4;const U32 marked=io.read(block);U32 size=marked-1;if(size&1)return;
 const U32 next_size=io.read(block+size),previous=io.read(pointer_value-8);
 if(!(next_size&1)){unlink(row,group,bucket(next_size),block+size,io);size+=next_size;}
 U32 bin=bucket(size),old_bin=bad;
 if(!(previous&1)){block-=previous;old_bin=bucket(previous);size+=previous;bin=bucket(size);if(old_bin!=bin)unlink(row,group,old_bin,block,io);}
 if((previous&1) || old_bin!=bin)insert(row,group,base,bin,block,io);
 io.write(block,size);io.write(block+size-4,size);const U32 live=io.read(base)-1;io.write(base,live);if(live)return;
 if(s.spare3){U32 spare=s.spare3,spare_control=io.read(spare+16);io.virtual_free(io.read(spare+12)+s.spare_page*0x8000,0x8000,0x4000);const U32 mask=io.read(spare+8)|(0x80000000U>>(s.spare_page&31));io.write(spare+8,mask);io.write(spare_control+0xc4+s.spare_page*4,0);const auto count=static_cast<std::uint8_t>(byte(io,spare_control+0x43)-1);byte(io,spare_control+0x43,count);if(!count)io.write(spare+4,io.read(spare+4)&~1U);
  if(mask==bad){io.virtual_free(io.read(spare+12),0,0x8000);io.heap_free(s.base.heap,spare_control);copy(io,spare,spare+20,s.base.regions*20-spare-20+s.base.table);--s.base.regions;if(spare<row)row-=20;s.current3=s.base.table;}
 }
 s.spare3=row;s.spare_page=group;
}
bool resize3(State& s,U32 row,U32 pointer_value,U32 requested,Services& io){
 (void)s;const U32 control=io.read(row+16),size=(requested+23)&~15U,group=(pointer_value-io.read(row+12))>>15,base=control+0x144+group*0x204;
 const U32 old=io.read(pointer_value-4)-1,next=pointer_value-4+old,next_size=io.read(next);
 if(signed32(old)<signed32(size)){
  if((next_size&1) || signed32(next_size+old)<signed32(size))return false;
  unlink(row,group,bucket(next_size),next,io);const U32 remain=next_size+old-size;
  if(signed32(remain)>0){const U32 block=pointer_value-4+size;insert(row,group,base,bucket(remain),block,io);io.write(block,remain);io.write(block+remain-4,remain);}
  io.write(pointer_value-4,size+1);io.write(pointer_value+size-8,size+1);
 }else if(signed32(size)<signed32(old)){
  U32 remain=old-size;io.write(pointer_value-4,size+1);const U32 block=pointer_value-4+size;io.write(block-4,size+1);
  if(!(next_size&1)){unlink(row,group,bucket(next_size),next,io);remain+=next_size;}
  insert(row,group,base,bucket(remain),block,io);io.write(block,remain);io.write(block+remain-4,remain);
 }return true;
}
U32 allocate_page2(U32 page,U32 available,U32 n,Services& io){
 if(!n || n>240)throw std::out_of_range("original heap paragraphs");const U32 start=io.read(page),limit=page+248;U32 at=start;
 const auto finish=[&](U32 p,U32 run){if(p+n<limit){io.write(page,p+n);io.write(page+4,run-n);}else {io.write(page+4,0);io.write(page,page+8);}byte(io,p,n);return (p+8)*16-page*15;};
 if(io.read(page+4)>=n)return finish(at,io.read(page+4));
 if(byte(io,at+io.read(page+4)))at+=io.read(page+4);
 std::size_t visits=0;
 for(unsigned pass=0;pass<2;++pass){if(pass)at=page+8;
  for(;;){if(pass && at>=start)return 0;if(at+n>=limit)break;
   const auto used=byte(io,at);if(used){at+=used;}else {U32 end=at+1;while(!byte(io,end)){++end;if(end>limit)throw std::runtime_error("original heap page lacks sentinel");}const U32 run=end-at;if(run>=n)return finish(at,run);
    if(!pass && at==start){io.write(page+4,run);at=end;}else {available-=run;at=end;if(available<n)return 0;}
   }if(++visits>4096)throw std::runtime_error("invalid original page map");
  }
 }return 0;
}
bool resize2(U32 region,U32 page,U32 slot,U32 n,Services& io){
 const U32 old=byte(io,slot),entry=region+24+((page-io.read(region+16))>>12)*8,limit=page+248;
 if(n<old){byte(io,slot,n);io.write(entry,io.read(entry)+old-n);io.write(entry+4,241);}
 else {if(n<=old)return false;const U32 end=slot+n;if(end>limit)return false;U32 at=slot+old;while(at<end && !byte(io,at))++at;if(at!=end)return false;byte(io,slot,n);
  const U32 cursor=io.read(page);if(slot<=cursor && cursor<end){if(end<limit){io.write(page,end);U32 count=0;while(!byte(io,end+count)){++count;if(end+count>limit)throw std::runtime_error("missing original sentinel");}io.write(page+4,count);}else {io.write(page+4,0);io.write(page,page+8);}}
  io.write(entry,io.read(entry)+old-n);
 }return true;
}
U32 allocate2(State& s,U32 n,Services& io){
 if(!n || n>240)throw std::out_of_range("original small-heap request");U32 region=s.base.current;std::size_t visits=0;
 for(;;){if(io.read(region+16)!=bad){const U32 cursor=io.read(region+8);U32 page=io.read(region+16)+((cursor-region-24)>>3)*4096;
   for(U32 entry=cursor;entry<region+0x2018;entry+=8,page+=4096){if(signed32(n)<=signed32(io.read(entry)) && n<io.read(entry+4)){const U32 result=allocate_page2(page,io.read(entry),n,io);if(result){s.base.current=region;io.write(entry,io.read(entry)-n);io.write(region+8,entry);return result;}io.write(entry+4,n);}}
   page=io.read(region+16);for(U32 entry=region+24;entry<cursor;entry+=8,page+=4096){if(signed32(n)<=signed32(io.read(entry)) && n<io.read(entry+4)){const U32 result=allocate_page2(page,io.read(entry),n,io);if(result){s.base.current=region;io.write(entry,io.read(entry)-n);io.write(region+8,entry);return result;}io.write(entry+4,n);}}
  }
  region=io.read(region);if(region==s.base.current){U32 candidate=s.base.head;while(io.read(candidate+16)==bad || !io.read(candidate+12)){candidate=io.read(candidate);if(candidate==s.base.head){candidate=add_region2(s,io);if(!candidate)return 0;const U32 page=io.read(candidate+16);byte(io,page+8,n);s.base.current=candidate;io.write(page,page+n+8);io.write(page+4,240-n);io.write(candidate+24,io.read(candidate+24)-(n&255));return page+256;}}
   const U32 first=io.read(candidate+12);U32 entry=first,count=0;const U32 page=io.read(candidate+16)+((first-candidate-24)>>3)*4096;
   while(io.read(entry)==bad && count<16){entry+=8;++count;}
   if(io.commit(page,count*4096)!=page)return 0;
   // Retail memset(page,count<<12,0) has a zero byte count; VirtualAlloc supplies zeros.
   for(U32 i=0;i<count;++i){const U32 p=page+i*4096;byte(io,p+248,255);io.write(p,p+8);io.write(p+4,240);io.write(first+i*8,240);io.write(first+i*8+4,241);}
   while(entry<candidate+0x2018 && io.read(entry)!=bad)entry+=8;s.base.current=candidate;io.write(candidate+12,entry<candidate+0x2018?entry:0);byte(io,page+8,n);io.write(candidate+8,first);io.write(first,io.read(first)-n);io.write(page+4,io.read(page+4)-n);io.write(page,page+n+8);return page+256;
  }if(++visits>1048576)throw std::runtime_error("invalid original heap region list");
 }
}
U32 reallocate(State& s,U32 p,U32 n,Services& io){
 if(!p)return malloc_public(s,n,io);if(!n){release_public(s,p,io);return 0;}
 U32 size=n;if(s.mode==2 && size<0xffffffe1U)size=(size+15)&~15U;
 for(;;){U32 result=0;
  if(size<0xffffffe1U){
   if(s.mode==3){U32 row=find_region(s,p,io);
    if(row){if(size<=s.threshold3 && resize3(s,row,p,size,io))result=p;
     else {if(size<=s.threshold3)result=allocate3(s,size,io);
      if(!result){size=((size?size:1)+15)&~15U;result=io.heap_allocate(s.base.heap,0,size);}
      if(result){copy(io,result,p,std::min(size,io.read(p-4)-1));row=find_region(s,p,io);release3(s,row,p,io);}
     }
    }else {size=((size?size:1)+15)&~15U;result=io.heap_resize(s.base.heap,p,size);}
   }else if(s.mode==2){U32 row{},page{};const U32 slot=original_heap4::find_small2(s.base,p,row,page,io);
    if(slot){if(size<s.threshold2 && resize2(row,page,slot,size>>4,io))result=p;
     else {if(size<s.threshold2)result=allocate2(s,size>>4,io);if(!result)result=io.heap_allocate(s.base.heap,0,size);if(result){copy(io,result,p,std::min(size,U32(byte(io,slot))*16));free_slot(s,row,page,slot,io);}}
    }else result=io.heap_resize(s.base.heap,p,size);
   }else {size=((size?size:1)+15)&~15U;result=io.heap_resize(s.base.heap,p,size);}
   if(result)return result;
  }if(!s.retry || !io.new_handler(size))return 0;
 }
}
void* Link::allocate3(U32 n){return host(original_heap9::allocate3(state_,n,services_));}
void* Link::allocate2(U32 n){return host(original_heap9::allocate2(state_,n,services_));}
std::uintptr_t Link::find3(void* p){return find_region(state_,pointer(p),services_);}
void Link::free3(std::uintptr_t r,void* p){release3(state_,static_cast<U32>(r),pointer(p),services_);}
void* Link::find2(void* p,std::uintptr_t& r,std::uintptr_t& pg){U32 row{},page{};const U32 slot=original_heap4::find_small2(state_.base,pointer(p),row,page,services_);if(slot){r=row;pg=page;}return host(slot);}
void Link::free2(std::uintptr_t r,std::uintptr_t pg,void* slot){free_slot(state_,static_cast<U32>(r),static_cast<U32>(pg),pointer(slot),services_);}
void* Link::heap_allocate(std::uintptr_t h,U32 n){return host(services_.heap_allocate(static_cast<U32>(h),0,n));}
void Link::heap_free(std::uintptr_t h,void* p){services_.heap_free(static_cast<U32>(h),pointer(p));}
}
