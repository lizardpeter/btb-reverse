#include "btb/original_heap9.hpp"
#include "btb/original_float2.hpp"
#include "btb/original_support3.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using U32=std::uint32_t;using I32=std::int32_t;using Handle=std::uintptr_t;
static std::vector<std::byte> unhex(const std::string& s){std::vector<std::byte>b;if(s=="-")return b;for(std::size_t i=0;i<s.size();i+=2)b.push_back(std::byte{static_cast<unsigned char>(std::stoul(s.substr(i,2),nullptr,16))});return b;}
static std::string text(const std::string& s){const auto b=unhex(s);return {reinterpret_cast<const char*>(b.data()),b.size()};}
struct Input {std::istringstream stream;explicit Input(const std::string& s):stream(s){}U32 n(){std::string s;stream>>s;assert(!s.empty());return static_cast<U32>(std::stoull(s,nullptr,16));}std::string word(){std::string s;stream>>s;assert(!s.empty());return s;}std::vector<U32> numbers(){auto count=n();std::vector<U32> v;while(count--)v.push_back(n());return v;}};
struct Nls:btb::original_locale7::NlsServices {
 using U16=std::uint16_t;
 I32 multibyte_to_wide(U32,U32,const char*,I32,U16*,I32) override{std::abort();}
 I32 wide_to_multibyte(U32,U32,const U16*,I32,char*,I32) override{std::abort();}
 I32 map_ansi(U32,U32,const char*,I32,char*,I32) override{std::abort();}
 I32 map_wide(U32,U32,const U16*,I32,void*,I32) override{std::abort();}
 bool type_ansi(U32,U32,const char*,I32,U16*) override{std::abort();}
 bool type_wide(U32,const U16*,I32,U16*) override{std::abort();}
 bool codepage_info(U32,btb::original_locale7::CodepageInfo&) override{std::abort();}
};
constexpr U32 DATA=0x800000;
static U32 crc32(const std::byte* data,std::size_t n,U32 previous=0){
 static const auto table=[](){std::array<U32,256> out{};for(U32 i=0;i<256;++i){U32 x=i;for(unsigned b=0;b<8;++b)x=(x>>1)^((x&1)?0xedb88320U:0);out[i]=x;}return out;}();
 U32 crc=previous^0xffffffffU;for(std::size_t i=0;i<n;++i)crc=table[(crc^std::to_integer<U32>(data[i]))&255]^(crc>>8);return crc^0xffffffffU;
}
struct Heap:btb::original_heap9::Services {
 std::vector<std::byte> memory=std::vector<std::byte>(0x1800000,std::byte{0xcc});std::array<std::byte,8224> head{};
 U32 heap_cursor{},reserve_cursor{},steps{},fail{},retry{};std::map<U32,U32> allocations,reserves;std::set<U32> committed;std::vector<U32> trace;
 std::span<std::byte> bytes(U32 p,U32 n) override{
  if(p>=0x447748 && p-0x447748<=head.size() && n<=head.size()-(p-0x447748))return {head.data()+p-0x447748,n};
  if(p<DATA || p-DATA>memory.size() || n>memory.size()-(p-DATA)){std::cerr<<"Bad guest range "<<std::hex<<p<<" "<<n<<'\n';std::abort();}return {memory.data()+p-DATA,n};
 }
 U32 read(U32 p) override{U32 value;std::memcpy(&value,bytes(p,4).data(),4);return value;}
 void write(U32 p,U32 value) override{std::memcpy(bytes(p,4).data(),&value,4);}
 bool event(U32 id,std::initializer_list<U32> values){trace.push_back(id);trace.insert(trace.end(),values);return ++steps==fail;}
 U32 heap_allocate(U32,U32 flags,U32 n) override{if(event(1,{flags,n}))return 0;const U32 p=heap_cursor;heap_cursor+=(n+15)&~15U;allocations[p]=n;auto range=bytes(p,n);std::fill(range.begin(),range.end(),flags&8?std::byte{0}:std::byte{0xcc});return p;}
 U32 heap_resize(U32,U32 old,U32 n) override{if(event(2,{old,n}))return 0;const U32 p=heap_cursor;heap_cursor+=(n+15)&~15U;allocations[p]=n;const U32 before=allocations.at(old),copied=std::min(before,n);auto range=bytes(p,n);std::fill(range.begin(),range.end(),std::byte{0xcc});std::memcpy(range.data(),bytes(old,copied).data(),copied);return p;}
 void heap_free(U32,U32 p) override{event(5,{p});}
 U32 virtual_allocate(U32 n,U32 allocation,U32 protect) override{assert(allocation==0x2000 && protect==4);if(event(3,{0,n,allocation,protect}))return 0;const U32 p=reserve_cursor;if(p+n>0x2000000)return 0;reserve_cursor+=(n+0xfffff)&~0xfffffU;reserves[p]=n;return p;}
 U32 commit(U32 p,U32 n) override{if(event(3,{p,n,0x1000,4}))return 0;for(U32 page=p;page<p+n;page+=4096)if(committed.insert(page).second){auto range=bytes(page,4096);std::fill(range.begin(),range.end(),std::byte{0});}return p;}
 bool virtual_free(U32 p,U32 n,U32 type) override{if(event(4,{p,n,type}))return false;if(type==0x8000)n=reserves.contains(p)?reserves.at(p):0;for(U32 page=p;page<p+n;page+=4096)committed.erase(page);return true;}
 bool new_handler(U32 n) override{event(6,{n});const auto old=retry;retry=0;return old!=0;}
 void reset(U32 fail_at){heap_cursor=0xb00140;reserve_cursor=0x1000000;steps=retry=0;fail=fail_at;trace.clear();allocations.clear();reserves.clear();committed.clear();allocations[0xb00000]=320;std::fill(memory.begin(),memory.end(),std::byte{0xcc});head.fill(std::byte{0});write(0x447748,0x447748);write(0x44774c,0x447748);write(0x447758,0xffffffffU);}
 U32 checksum(){U32 crc=crc32(head.data(),head.size());crc=crc32(bytes(0xb00000,heap_cursor-0xb00000).data(),heap_cursor-0xb00000,crc);for(auto p:committed)crc=crc32(bytes(p,4096).data(),4096,crc);return crc;}
};
struct System:btb::original_support3::Services {
 U32 major{},platform{},linker{},mode{};bool version_ok{},module_ok{true};std::string environment,module;std::vector<U32> trace;std::vector<std::string> captured;std::array<std::byte,512> image{};
 bool os_version(U32& m,U32& p) override{trace.push_back(7);m=major;p=platform;return version_ok;}
 U32 heap_environment(char* out,U32 cap) override{trace.push_back(8);assert(cap==4240);std::strcpy(out,environment.c_str());return static_cast<U32>(environment.size());}
 U32 module_name(char* out,U32 cap) override{trace.push_back(9);assert(cap==260);std::strcpy(out,module.c_str());return module_ok?static_cast<U32>(module.size()):0;}
 std::span<const std::byte> module_image() override{image[0]=std::byte{'M'};image[1]=std::byte{'Z'};U32 offset=0x100;std::memcpy(image.data()+0x3c,&offset,4);image[offset+26]=std::byte{static_cast<std::uint8_t>(linker)};return image;}
 Handle standard_error() override{return 0x90;}
 void write_error(Handle h,const char* data,U32 n) override{assert(h==0x90);captured.emplace_back(data,n);}
 Handle load_library(const char* name) override{assert(std::string(name)=="user32.dll");return 0x80;}
 Handle get_proc(Handle,const char* name) override{return std::string(name)=="MessageBoxA"?1:0;}
 Handle active_window(Handle) override{std::abort();}
 Handle last_popup(Handle,Handle) override{std::abort();}
 I32 message_box(Handle,Handle owner,const char* text,const char* title,U32 flags) override{assert(!owner && flags==0x12010 && std::string(title)=="Microsoft Visual C++ Runtime Library");captured.emplace_back(text);return 1;}
};
static btb::original_locale7::State locale(){btb::original_locale7::State s;for(U32 i=0;i<256;++i)s.classes[i+1]=(i>='0' && i<='9'?4:0)|(i>=9 && i<=13 || i==32?8:0)|(i>='A'&&i<='Z'?1:i>='a'&&i<='z'?2:0);return s;}
int main(int argc,char** argv){assert(argc==2);std::ifstream file(argv[1]);assert(file);std::string line;U32 cases=0;Heap heap;btb::original_heap4::State base{0x55,0x447748,0x447748,0,0xb00000,16,0};btb::original_heap9::State state{base,0xb00000};
 while(std::getline(file,line)){++cases;Input in(line);const auto kind=in.word();
 if(kind=="MUL"){btb::original_float6::Extended12 a,b,expected;for(auto& x:a)x=static_cast<std::uint16_t>(in.n());for(auto& x:b)x=static_cast<std::uint16_t>(in.n());for(auto& x:expected)x=static_cast<std::uint16_t>(in.n());btb::original_float2::multiply(a,b);if(a!=expected){std::cerr<<line<<"\nactual ";for(auto x:a)std::cerr<<std::hex<<x<<" ";std::cerr<<'\n';return 1;}}
 else if(kind=="SCALE"){btb::original_float6::Extended12 a,expected;for(auto& x:a)x=static_cast<std::uint16_t>(in.n());const I32 n=static_cast<I32>(in.n());const bool retain=in.n()!=0;for(auto& x:expected)x=static_cast<std::uint16_t>(in.n());btb::original_float2::scale_decimal(a,n,retain);if(a!=expected){std::cerr<<line<<"\nactual ";for(auto x:a)std::cerr<<std::hex<<x<<" ";std::cerr<<'\n';return 1;}}
 else if(kind=="HBEGIN"){const U32 mode=in.n(),fail=in.n();heap.reset(fail);base={0x55,0x447748,0x447748,0,0xb00000,16,0};state.current3=0xb00000;state.spare3=state.spare_page=0;state.mode=mode;state.retry=false;}
 else if(kind=="HCONFIG"){heap.fail=in.n();state.retry=in.n()!=0;heap.retry=in.n();}
 else if(kind=="HOP"){
  const auto operation=in.word();const auto args=in.numbers();heap.trace.clear();btb::original_heap9::Link link(state,heap);const btb::original_memory4::HeapState hs{state.mode,state.threshold3,state.threshold2,0x55};U32 result=0;
  if(operation=="ALLOC")result=static_cast<U32>(reinterpret_cast<Handle>(btb::original_memory4::allocate(hs,args[0],link)));
  else if(operation=="A3")result=btb::original_heap9::allocate3(state,args[0],heap);
  else if(operation=="A2")result=btb::original_heap9::allocate2(state,args[0],heap);
  else if(operation=="FREE")btb::original_memory4::release(hs,reinterpret_cast<void*>(static_cast<Handle>(args[0])),link);
  else if(operation=="REALLOC")result=btb::original_heap9::reallocate(state,args[0],args[1],heap);
  else {std::cerr<<"bad operation";return 1;}
  std::array<U32,12> expected{};for(auto& x:expected)x=in.n();const auto trace=in.numbers();const std::array<U32,12> actual{result,base.current,base.free_pages,base.table,base.capacity,base.regions,state.current3,state.spare3,state.spare_page,heap.heap_cursor,heap.reserve_cursor,heap.checksum()};
  if(actual!=expected || heap.trace!=trace){std::cerr<<"case "<<cases<<'\n'<<line<<"\nACT ";for(auto x:actual)std::cerr<<std::hex<<x<<" ";std::cerr<<"\nTRACE ";for(auto x:heap.trace)std::cerr<<x<<" ";std::cerr<<'\n';return 1;}
 }
 else if(kind=="PAGE"){const U32 n=in.n(),expected=in.n();const auto bytes=unhex(in.word());const U32 page=0x1000000;std::fill(heap.bytes(page,4096).begin(),heap.bytes(page,4096).end(),std::byte{0});heap.write(page,page+8);heap.write(page+4,240);heap.bytes(page+248,1)[0]=std::byte{255};assert(btb::original_heap9::allocate_page2(page,240,n,heap)==expected);assert(std::equal(bytes.begin(),bytes.end(),heap.bytes(page,256).begin()));}
 else if(kind=="RSLOT"){const U32 slot=in.n(),old=in.n(),n=in.n(),obstacle=in.n(),expected=in.n();const auto data=unhex(in.word());const U32 available=in.n(),indicator=in.n(),page=0x1000000,region=DATA+0x1000;auto bytes=heap.bytes(page,4096);std::fill(bytes.begin(),bytes.end(),std::byte{0});heap.write(region+16,page);heap.write(page,page+8);heap.write(page+4,240);heap.bytes(page+248,1)[0]=std::byte{255};heap.bytes(page+slot,1)[0]=std::byte{static_cast<std::uint8_t>(old)};heap.bytes(page+obstacle,1)[0]=std::byte{5};heap.write(region+24,200);heap.write(region+28,241);assert(btb::original_heap9::resize2(region,page,page+slot,n,heap)==(expected!=0));assert(std::equal(data.begin(),data.end(),bytes.begin()) && heap.read(region+24)==available && heap.read(region+28)==indicator);}
 else if(kind=="PARSE"){auto s=locale();Nls io;const auto value=text(in.word());const auto radix=static_cast<I32>(in.n());const U32 flags=in.n(),expected=in.n(),end=in.n(),error=in.n();I32 actual_error=71;const char* cursor=nullptr;const auto actual=btb::original_support3::parse_integer(s,value.c_str(),&cursor,radix,flags,actual_error,io);if(actual!=expected || static_cast<U32>(cursor-value.c_str())!=end || static_cast<U32>(actual_error)!=error){std::cerr<<line<<"\nACT "<<actual<<" "<<(cursor-value.c_str())<<" "<<actual_error<<'\n';return 1;}}
 else if(kind=="SELECT"){auto s=locale();Nls io;System svc;svc.major=in.n();svc.platform=in.n();svc.version_ok=in.n()!=0;svc.environment=text(in.word());svc.module=text(in.word());svc.linker=in.n();const U32 expected=in.n(),error=in.n();const auto trace=in.numbers();I32 actual_error=71;const auto actual=btb::original_support3::choose_heap(s,actual_error,io,svc);assert(static_cast<U32>(actual)==expected && static_cast<U32>(actual_error)==error && svc.trace==trace);}
 else if(kind=="REPORT"){const U32 code=in.n(),mode=in.n();const bool console=in.n()!=0;System svc;svc.module=text(in.word());svc.module_ok=in.n()!=0;const U32 n=in.n();std::vector<std::string> expected;for(U32 i=0;i<n;++i)expected.push_back(text(in.word()));btb::original_message1::State messages;btb::original_support3::report_error(code,mode,console,messages,svc);assert(svc.captured==expected);}
 else {std::cerr<<"Unknown kind "<<kind<<'\n';return 1;}
 }
 assert(cases>5000);std::cout<<cases<<" original x86 fixture rows passed\n";
}
