#include "btb/original_file7.hpp"
#include "btb/original_memory4.hpp"
#include <algorithm>
#include <bit>
#include <cstring>
#include <stdexcept>
namespace btb::original_file7 {
namespace {
constexpr U32 bad=0xffffffffU;
I32 invalid(Context& c){c.state.dos_error=0;c.state.error=9;return -1;}
void error(Context& c,U32 code){c.state.dos_error=code;c.state.error=original_runtime8::map_win_error(std::bit_cast<I32>(code),c.errors);}
U32 seek(Context& c,U32 fd,I32 delta,U32 origin){return original_startup11::seek_descriptor(c.state,fd,delta,origin,c.services,c.errors);}
bool valid(Context& c,U32 fd){return fd<c.state.descriptors.size() && (c.state.descriptors[fd].flags&1);}
I32 failure(Context& c,U32 code){if(code==5){c.state.error=9;c.state.dos_error=5;}else error(c,code);return -1;}
std::byte* bytes(Context& c,U32 ptr,U32 size){return c.services.guest_bytes(ptr,size);}
void allocate_buffer(Context& c,Stream32& f,U32 address){
 original_crt10::Stream native;auto& w=f.words;native.flags=w[3];Link link(c);
 original_crt10::allocate_file_buffer(native,c.buffer_allocations,[](std::size_t n,void* p)->void*{return static_cast<Link*>(p)->allocate(n);},&link);
 U32 base;if(native.base==native.inline_buffer.data()){base=address+20;}else base=c.services.guest_address(native.base);
 w[2]=w[0]=base;w[3]=native.flags;w[1]=native.count;w[6]=native.capacity;
}
U32 refill(Context& c,Stream32& f,U32 address){
 auto& w=f.words;original_crt10::Stream native;original_memory4::copy_bytes(native.inline_buffer.data(),bytes(c,address+20,2),2);native.flags=w[3];native.capacity=w[6];native.count=w[1];native.pointer=w[0]?bytes(c,w[0],0):nullptr;native.base=w[2]?bytes(c,w[2],0):nullptr;Link link(c);
 const U32 value=original_runtime15::refill_input(native,w[4],c.buffer_allocations,link);
 if(native.base==native.inline_buffer.data()){
  const U32 offset=static_cast<U32>(native.pointer-native.base);w[2]=address+20;w[0]=w[2]+offset;original_memory4::copy_bytes(bytes(c,w[2],2),native.inline_buffer.data(),2);
 }else {w[2]=native.base?c.services.guest_address(native.base):0;w[0]=native.pointer?c.services.guest_address(native.pointer):0;}
 w[1]=native.count;w[3]=native.flags;w[6]=native.capacity;return value;
}
}
I32 read_descriptor(Context& c,U32 fd,std::byte* out,U32 n){
 if(!valid(c,fd))return invalid(c);auto& d=c.state.descriptors[fd];
 if(!n || (d.flags&2))return 0;
 U32 cached=0;if((d.flags&0x48) && d.reserved[0]!=10){out[0]=std::byte{d.reserved[0]};d.reserved[0]=10;cached=1;--n;}
 U32 actual=0;if(!c.services.read_file(d.handle,out+cached,n,actual)){const U32 code=c.services.last_error();if(code==109)return 0;return failure(c,code);}
 if(actual>n)throw std::out_of_range("original ReadFile output size");
 if(!(d.flags&0x80))return std::bit_cast<I32>(cached+actual);
 if(actual && out[0]==std::byte{10})d.flags|=4;else d.flags&=0xfb;
 const U32 total=cached+actual;U32 input=0,output=0;
 while(input<total){const auto value=out[input++];if(value==std::byte{26}){if(!(d.flags&0x40))d.flags|=2;break;}
  if(value!=std::byte{13})out[output++]=value;
  else if(input<total){if(out[input]==std::byte{10}){++input;out[output++]=std::byte{10};}else out[output++]=std::byte{13};}
  else {
   std::byte next{};U32 read=0;const bool okay=c.services.read_file(d.handle,&next,1,read);
   if((!okay && c.services.last_error()!=0) || !read)out[output++]=std::byte{13};
   else if(!(d.flags&0x48)){
    if(!output && next==std::byte{10})out[output++]=std::byte{10};
    else {seek(c,fd,-1,1);if(next!=std::byte{10})out[output++]=std::byte{13};}
   }else if(next==std::byte{10})out[output++]=std::byte{10};
   else {out[output++]=std::byte{13};d.reserved[0]=std::to_integer<std::uint8_t>(next);}
  }
 }
 return std::bit_cast<I32>(output);
}
I32 write_descriptor(Context& c,U32 fd,const std::byte* data,U32 n){
 if(!valid(c,fd))return invalid(c);auto& d=c.state.descriptors[fd];if(!n)return 0;
 if(d.flags&0x20)seek(c,fd,0,2);
 U32 written=0,newlines=0,code=0;
 if(!(d.flags&0x80)){U32 actual=0;if(!c.services.write_file(d.handle,data,n,actual))code=c.services.last_error();else written=actual;}
 else {
  std::array<std::byte,1028> block{};U32 cursor=0;
  while(cursor<n){U32 size=0;do{if(cursor==n)break;const auto ch=data[cursor++];if(ch==std::byte{10}){++newlines;block[size++]=std::byte{13};}block[size++]=ch;}while(size<1024);
   U32 actual=0;if(!c.services.write_file(d.handle,block.data(),size,actual)){code=c.services.last_error();break;}written+=actual;if(actual<size || cursor==n)break;
  }
 }
 if(written)return std::bit_cast<I32>(written-newlines);
 if(code)return failure(c,code);
 if((d.flags&0x40) && data[0]==std::byte{26})return 0;
 c.state.error=28;c.state.dos_error=0;return -1;
}
I32 resize_file(Context& c,U32 fd,I32 length){
 if(!valid(c,fd)){c.state.error=9;return -1;}
 const U32 old=seek(c,fd,0,1);if(old==bad)return -1;const U32 end=seek(c,fd,0,2);if(end==bad)return -1;
 I32 result=0;U32 remaining=static_cast<U32>(length)-end;
 if(std::bit_cast<I32>(remaining)<0){seek(c,fd,length,0);const auto handle=c.state.descriptors[fd].handle;if(!c.services.end_of_file(handle)){result=-1;c.state.error=13;c.state.dos_error=c.services.last_error();}}
 else if(std::bit_cast<I32>(remaining)>0){
  std::array<std::byte,4096> zero{};const I32 mode=set_descriptor_mode(c.state,fd,0x8000);
  do{const U32 n=std::min<U32>(4096,remaining);const I32 wrote=write_descriptor(c,fd,zero.data(),n);if(wrote==-1){if(c.state.dos_error==5)c.state.error=13;result=-1;break;}if(!wrote)throw std::runtime_error("original file growth makes no progress");remaining-=static_cast<U32>(wrote);}while(std::bit_cast<I32>(remaining)>0);
  set_descriptor_mode(c.state,fd,mode);
 }
 seek(c,fd,std::bit_cast<I32>(old),0);return result;
}
I32 open_descriptor(Context& c,const char* path,U32 flags,U32 share,U32 permissions){
 CreateRequest request;request.inherit=!(flags&0x80);std::uint8_t descriptor=flags&0x80?0x10:0;
 if(!(flags&0x8000) && ((flags&0x4000)||c.default_mode!=0x8000))descriptor|=0x80;
 const auto invalid_argument=[&](){c.state.error=22;c.state.dos_error=0;return -1;};
 switch(flags&3){case 0:request.access=0x80000000;break;case 1:request.access=0x40000000;break;case 2:request.access=0xc0000000;break;default:return invalid_argument();}
 switch(share){case 0x10:request.share=0;break;case 0x20:request.share=1;break;case 0x30:request.share=2;break;case 0x40:request.share=3;break;default:return invalid_argument();}
 switch(flags&0x700){case 0:case 0x400:request.disposition=3;break;case 0x100:request.disposition=4;break;case 0x200:case 0x600:request.disposition=5;break;case 0x300:request.disposition=2;break;case 0x500:case 0x700:request.disposition=1;break;}
 request.attributes=(flags&0x100) && !(~c.permission_mask&permissions&0x80)?1:0x80;
 if(flags&0x40){request.attributes|=0x4000000;request.access|=0x10000;}
 if(flags&0x1000)request.attributes|=0x100;
 if(flags&0x20)request.attributes|=0x8000000;else if(flags&0x10)request.attributes|=0x10000000;
 c.state.descriptors=DescriptorView(c.table.blocks.data(),&c.table.limit);
 const I32 index=original_startup11::allocate_descriptor(c.table,c.services);if(index==-1){c.state.dos_error=0;c.state.error=24;return -1;}
 const U32 fd=static_cast<U32>(index),handle=c.services.create_file(path,request);
 if(handle!=bad){const U32 type=c.services.file_type(handle);if(type){
   if(type==2)descriptor|=0x40;else if(type==3)descriptor|=8;Link link(c);assign_os_handle(c.state,fd,handle,link);auto& d=c.state.descriptors[fd];d.flags=descriptor|1;
   if(!(descriptor&0x48) && (descriptor&0x80) && (flags&2)){
    const U32 last=seek(c,fd,-1,2);bool failed=false;
    if(last==bad){if(c.state.dos_error!=131)failed=true;}
    else {std::byte ch{};const I32 read=read_descriptor(c,fd,&ch,1);if((read==0 && ch==std::byte{26} && resize_file(c,fd,std::bit_cast<I32>(last))==-1) || seek(c,fd,0,0)==bad)failed=true;}
    if(failed){original_startup11::close_descriptor(c.state,fd,link,c.services,c.errors);return -1;}
   }
   if(!(descriptor&0x48) && (flags&8))d.flags|=0x20;return index;
  }c.services.close_handle(handle);
 }
 error(c,c.services.last_error());return -1;
}
U32 flush_byte(Context& c,U32 value,Stream32& f,U32 address){
 auto& w=f.words;U32 flags=w[3];const U32 fd=w[4];
 if(!(flags&0x82) || (flags&0x40)){w[3]|=0x20;return bad;}
 if(flags&1){w[1]=0;if(!(flags&0x10)){w[3]|=0x20;return bad;}w[0]=w[2];w[3]&=~1U;}
 flags=w[3];w[1]=0;w[3]=(flags&~0x10U)|2;
 Link link(c);if(!(flags&0x10c) && ((address!=0x4498b0 && address!=0x4498d0) || !link.is_terminal(fd)))allocate_buffer(c,f,address);
 U32 expected=1;I32 actual;
 if(w[3]&0x108){expected=w[0]-w[2];w[0]=w[2]+1;w[1]=w[6]-1;actual=0;
  if(std::bit_cast<I32>(expected)>0)actual=write_descriptor(c,fd,bytes(c,w[2],expected),expected);
  else {if(fd!=bad && fd>=c.state.descriptors.size())throw std::out_of_range("original buffered descriptor");const auto descriptor_flags=fd==bad?c.invalid_flags:c.state.descriptors[fd].flags;if(descriptor_flags&0x20)seek(c,fd,0,2);}
  bytes(c,w[2],1)[0]=std::byte{static_cast<std::uint8_t>(value)};
 }else {const std::byte byte{static_cast<std::uint8_t>(value)};actual=write_descriptor(c,fd,&byte,1);}
 if(static_cast<U32>(actual)==expected)return value&255;
 w[3]|=0x20;return bad;
}
U32 write_items(Context& c,U32 data,U32 size,U32 count,Stream32& f,U32 address){
 const U32 total=size*count;if(!total)return 0;U32 remaining=total;auto& w=f.words;U32 block=w[3]&0x10c?w[6]:4096;Link link(c);
 do{if((w[3]&0x108) && w[1]){const U32 n=std::min(remaining,w[1]);original_memory4::copy_bytes(bytes(c,w[0],n),bytes(c,data,n),n);w[1]-=n;w[0]+=n;remaining-=n;data+=n;}
  else if(block<=remaining){if((w[3]&0x108) && flush_stream(f,link)!=0)return (total-remaining)/size;const U32 n=block?remaining-remaining%block:remaining;const I32 wrote=write_descriptor(c,w[4],bytes(c,data,n),n);if(wrote==-1){w[3]|=0x20;return (total-remaining)/size;}if(wrote< -1)throw std::runtime_error("original short text write has negative accounting");remaining-=static_cast<U32>(wrote);if(static_cast<U32>(wrote)<n){w[3]|=0x20;return (total-remaining)/size;}data+=static_cast<U32>(wrote);}
  else {if(flush_byte(c,std::to_integer<U32>(bytes(c,data,1)[0]),f,address)==bad)return (total-remaining)/size;++data;--remaining;block=std::max<U32>(1,w[6]);}
 }while(remaining);return count;
}
U32 read_items(Context& c,U32 data,U32 size,U32 count,Stream32& f,U32 address){
 const U32 total=size*count;if(!total)return 0;U32 remaining=total;auto& w=f.words;U32 block=w[3]&0x10c?w[6]:4096;
 do{if((w[3]&0x10c) && w[1]){const U32 n=std::min(remaining,w[1]);original_memory4::copy_bytes(bytes(c,data,n),bytes(c,w[0],n),n);w[1]-=n;w[0]+=n;remaining-=n;data+=n;}
  else if(remaining<block){const U32 value=refill(c,f,address);if(value==bad)return (total-remaining)/size;bytes(c,data++,1)[0]=std::byte{static_cast<std::uint8_t>(value)};block=w[6];--remaining;}
  else {const U32 n=block?remaining-remaining%block:remaining;const I32 got=read_descriptor(c,w[4],bytes(c,data,n),n);if(!got){w[3]|=0x10;return (total-remaining)/size;}if(got==-1){w[3]|=0x20;return (total-remaining)/size;}remaining-=static_cast<U32>(got);data+=static_cast<U32>(got);}
 }while(remaining);return count;
}
void Link::set_standard_handle(U32 a,U32 b){context_.services.set_standard_handle(a,b);}
bool Link::flush_file(U32 h){return context_.services.flush_file(h);}
U32 Link::last_error(){return context_.services.last_error();}
U32 Link::write_descriptor(U32 fd,U32 address,U32 n){return static_cast<U32>(original_file7::write_descriptor(context_,fd,bytes(context_,address,n),n));}
Stream32* Link::allocate_stream(std::size_t n){return context_.services.allocate_stream(n);}
bool Link::is_terminal(U32 fd){return fd<context_.state.descriptors.size() && (context_.state.descriptors[fd].flags&0x40);}
U32 Link::allocate_guest_bytes(U32 n){return context_.services.allocate_guest_bytes(n);}
I32 Link::open_file(const char* p,U32 f,U32 s,U32 m){return open_descriptor(context_,p,f,s,m);}
void* Link::allocate(std::size_t n){const U32 address=allocate_guest_bytes(static_cast<U32>(n));return address?bytes(context_,address,static_cast<U32>(n)):nullptr;}
I32 Link::read_descriptor(U32 fd,std::byte* out,U32 n){return original_file7::read_descriptor(context_,fd,out,n);}
std::uint8_t Link::descriptor_flags(U32 fd){return fd<context_.state.descriptors.size()?context_.state.descriptors[fd].flags:context_.invalid_flags;}
}
