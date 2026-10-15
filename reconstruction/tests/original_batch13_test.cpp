#include "btb/original_memory4.hpp"
#include "btb/original_environment4.hpp"
#include "btb/original_message1.hpp"
#include "btb/original_printer2.hpp"
#include "btb/original_sound2.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
using U32=std::uint32_t;using I32=std::int32_t;using Handle=std::uintptr_t;
static std::vector<std::byte> unhex(const std::string& s){std::vector<std::byte>b;if(s=="-")return b;for(std::size_t i=0;i<s.size();i+=2)b.push_back(std::byte{static_cast<unsigned char>(std::stoul(s.substr(i,2),nullptr,16))});return b;}
static std::string text(const std::string& s){const auto b=unhex(s);return {reinterpret_cast<const char*>(b.data()),b.size()};}
struct Input {std::istringstream stream;explicit Input(const std::string& s):stream(s){}U32 n(){std::string s;stream>>s;assert(!s.empty());return static_cast<U32>(std::stoull(s,nullptr,16));}std::string word(){std::string s;stream>>s;assert(!s.empty());return s;}std::vector<U32> numbers(){auto count=n();std::vector<U32> v;while(count--)v.push_back(n());return v;}};
struct Heap:btb::original_memory4::HeapServices {
 std::vector<U32> trace;U32 small{};bool found{};
 void* allocate3(U32 n) override{trace.insert(trace.end(),{1,n});return reinterpret_cast<void*>(static_cast<Handle>(small));}
 void* allocate2(U32 n) override{trace.insert(trace.end(),{2,n});return reinterpret_cast<void*>(static_cast<Handle>(small));}
 Handle find3(void* p) override{trace.insert(trace.end(),{5,static_cast<U32>(reinterpret_cast<Handle>(p))});return found?0x12:0;}
 void free3(Handle h,void* p) override{trace.insert(trace.end(),{6,static_cast<U32>(h),static_cast<U32>(reinterpret_cast<Handle>(p))});}
 void* find2(void* p,Handle& region,Handle& page) override{trace.insert(trace.end(),{7,static_cast<U32>(reinterpret_cast<Handle>(p))});region=0x56;page=0x78;return found?reinterpret_cast<void*>(0x9a):nullptr;}
 void free2(Handle r,Handle pg,void* p) override{trace.insert(trace.end(),{8,static_cast<U32>(r),static_cast<U32>(pg),static_cast<U32>(reinterpret_cast<Handle>(p))});}
 void* heap_allocate(Handle h,U32 n) override{trace.insert(trace.end(),{3,static_cast<U32>(h),n});return reinterpret_cast<void*>(0x1234);}
 void heap_free(Handle h,void* p) override{trace.insert(trace.end(),{4,static_cast<U32>(h),static_cast<U32>(reinterpret_cast<Handle>(p))});}
};
using namespace std::string_literals;
struct Environment:btb::original_environment4::Services {
 U32 allocs{},frees{},resizes{},fail{},conversion_fail{},conversions{},block_frees{};bool wide_ok{true},narrow_ok{true},resize_failure{};
 std::vector<void*> storage;std::vector<std::pair<std::string,std::string>> os;
 std::u16string wide_block=u"A=one\0B=two\0"s;
 std::string narrow_block="A=one\0B=two\0"s;
 ~Environment(){for(auto* p:storage)std::free(p);}
 void* allocate(std::size_t n) override{++allocs;if(allocs==fail)return nullptr;auto* p=std::calloc(1,std::max<std::size_t>(1024,n));assert(p);storage.push_back(p);return p;}
 void* resize(void* p,std::size_t) override{++resizes;return resize_failure?nullptr:p;}
 void release(void*) override{++frees;}
 char** duplicate(char** p) override{if(!p)return nullptr;std::size_t n=0;while(p[n])++n;auto** out=static_cast<char**>(allocate((n+1)*sizeof(char*)));assert(out);for(std::size_t i=0;i<n;++i){out[i]=static_cast<char*>(allocate(std::strlen(p[i])+1));assert(out[i]);std::strcpy(out[i],p[i]);}out[n]=nullptr;return out;}
 I32 compare(std::string_view a,std::string_view b,std::size_t n) override{for(std::size_t i=0;i<n;++i){const auto x=std::tolower(static_cast<unsigned char>(a[i])),y=std::tolower(static_cast<unsigned char>(b[i]));if(x!=y)return x<y?1:3;}return 2;}
 char16_t* environment_wide() override{return wide_ok?wide_block.data():nullptr;}
 char* environment_narrow() override{return narrow_ok?narrow_block.data():nullptr;}
 void free_environment_wide(char16_t*) override{++block_frees;}
 void free_environment_narrow(char*) override{++block_frees;}
 U32 convert(U32 codepage,const char16_t* input,I32 count,char* out,U32 capacity) override{assert(codepage<=1);++conversions;if(conversions==conversion_fail)return 0;if(count==-1)count=static_cast<I32>(std::char_traits<char16_t>::length(input)+1);if(out){assert(capacity>=static_cast<U32>(count));for(I32 i=0;i<count;++i)out[i]=static_cast<char>(input[i]);}return static_cast<U32>(count);}
 void set_variable(const char* name,const char* value) override{os.emplace_back(name,value?value:"-");}
};
struct Messages:btb::original_message1::Services {
 U32 missing{},owner{};std::vector<U32> trace;
 Handle load_library(const char* s) override{assert(std::string(s)=="user32.dll");trace.push_back(1);return missing==1?0:0x80;}
 Handle get_proc(Handle,const char* s) override{const U32 id=std::string(s)=="MessageBoxA"?2:std::string(s)=="GetActiveWindow"?3:4;trace.push_back(id);return missing==id?0:id;}
 Handle active_window(Handle) override{trace.push_back(5);return owner;}
 Handle last_popup(Handle,Handle w) override{trace.insert(trace.end(),{6,static_cast<U32>(w)});return w+1;}
 I32 message_box(Handle,Handle h,const char*,const char*,U32 f) override{trace.insert(trace.end(),{7,static_cast<U32>(h),f});return 0x44;}
};
struct Geometry:btb::original_printer2::GeometryServices {
 std::array<I32,6> caps{};std::vector<U32> trace;
 I32 device_cap(Handle h,I32 index) override{trace.insert(trace.end(),{1,static_cast<U32>(h),static_cast<U32>(index)});const auto j=h==2&&index==88?4:h==2&&index==90?5:index==8?0:index==10?1:index==88?2:3;return caps[j];}
 Handle window_dc(Handle h) override{trace.insert(trace.end(),{2,static_cast<U32>(h)});return 2;}
};
struct Audio final:btb::original_wave7::SoundServices {
 std::vector<U32>& trace;explicit Audio(std::vector<U32>& t):trace(t){}
 U32 create_primary(Handle,std::span<const U32,9>,Handle&) override{std::abort();}
 U32 set_format(Handle,std::span<const std::byte,18>) override{std::abort();}
 void release(Handle) override{std::abort();}
 U32 status(Handle,U32*) noexcept override{trace.insert(trace.end(),{10,0});return 0x80004005;}
 U32 restore(Handle) noexcept override{std::abort();}
 void sleep(U32) noexcept override{std::abort();}
 U32 lock(Handle,U32,std::byte*&,U32&) override{std::abort();}
 U32 unlock(Handle,std::byte*,U32) override{std::abort();}
};
struct WaveIo final:btb::original_wave7::MmioServices {
 using Wave=btb::original_wave7::Wave;using Chunk=btb::original_wave7::Chunk;using Info=btb::original_wave7::Info;
 U32 opens{},cursor{};bool first_empty{},fallback_empty{};std::array<std::byte,18> format{};std::vector<std::unique_ptr<std::byte[]>> storage;
 Handle open(const char*,const std::byte*,U32,U32) noexcept override{++opens;cursor=0;return (opens==1?first_empty:fallback_empty)?0:1;}
 U32 close(Handle) noexcept override{return 0;}
 U32 descend(Handle,Chunk& c,const Chunk* parent,U32) noexcept override{if(!parent)c={0x46464952,48,0x45564157,12,0};else c.cksize=c.ckid==0x20746d66?16:8;return 0;}
 U32 ascend(Handle,Chunk&) noexcept override{return 0;}
 I32 read(Handle,std::byte* p,U32 n) noexcept override{std::memcpy(p,format.data()+cursor,n);cursor+=n;return static_cast<I32>(n);}
 I32 write(Handle,const std::byte*,U32) noexcept override{std::abort();}
 I32 seek(Handle,I32,U32) noexcept override{return 0;}
 U32 create_chunk(Handle,Chunk&,U32) noexcept override{std::abort();}
 U32 get_info(Handle,Info&) noexcept override{std::abort();}
 U32 set_info(Handle,const Info&) noexcept override{std::abort();}
 U32 advance(Handle,Info&) noexcept override{std::abort();}
 std::byte* allocate(U32 n) noexcept override{storage.push_back(std::make_unique<std::byte[]>(n));return storage.back().get();}
 void release(std::byte*) noexcept override{}
 Handle find_resource(const char*,const char*) noexcept override{return 0;}
 Handle load_resource(Handle) noexcept override{std::abort();}
 U32 resource_size(Handle) noexcept override{std::abort();}
 const std::byte* lock_resource(Handle) noexcept override{std::abort();}
};
struct Creation:btb::original_sound2::CreationServices {
 using Wave=btb::original_wave7::Wave;using Sound=btb::original_sound2::Sound;using Desc=btb::original_sound2::BufferDescription;
 U32 allocation{},failure{},create_code{},duplicate_failure{},next_duplicate{1};std::vector<U32> sizes,trace;std::vector<std::unique_ptr<Handle[]>> buffers;std::unique_ptr<Wave>wave;std::unique_ptr<Sound>sound;
 Handle* allocate_buffers(U32 count) override{sizes.push_back(count*4);if(++allocation==failure)return nullptr;buffers.push_back(std::make_unique<Handle[]>(count));return buffers.back().get();}
 void free_buffers(Handle*) override{}
 Wave* allocate_wave() override{sizes.push_back(0x90);if(++allocation==failure)return nullptr;wave=std::make_unique<Wave>();return wave.get();}
 void free_wave(Wave*) override{}
 Sound* allocate_sound() override{sizes.push_back(0x14);if(++allocation==failure)return nullptr;sound=std::make_unique<Sound>();return sound.get();}
 U32 create_buffer(Handle,const Desc& d,Handle& out) override{trace.insert(trace.end(),{1,d.size,d.flags,d.bytes});trace.insert(trace.end(),d.algorithm.begin(),d.algorithm.end());out=0x802000;return create_code;}
 U32 duplicate_buffer(Handle,Handle original,Handle& out) override{assert(original==0x802000);trace.insert(trace.end(),{2,next_duplicate});out=0x802000+next_duplicate*16;return next_duplicate++==duplicate_failure?0x80004005:0;}
 U32 set_position(Handle h,U32 n) override{trace.insert(trace.end(),{11,static_cast<U32>(h-0x802000),n});return 0x80004005;}
};
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1]);assert(file);std::string line;U32 cases=0;
 while(std::getline(file,line)){++cases;Input in(line);const auto kind=in.word();
 if(kind=="COPY"){auto src=in.n(),dst=in.n(),n=in.n();auto b=unhex(in.word());const auto result=in.n();const auto expected=unhex(in.word());assert(btb::original_memory4::copy_bytes(b.data()+dst,b.data()+src,n)==b.data()+result);assert(b==expected);}
 else if(kind=="SORT"){
  const U32 count=in.n(),width=in.n();auto b=unhex(in.word());const auto expected=unhex(in.word());const auto expected_trace=in.numbers();struct Context{const std::byte* base;U32 width;std::vector<U32> trace;} ctx{b.data(),width,{}};
  const auto cmp=[](const std::byte* a,const std::byte* b,void* p)->I32{auto& c=*static_cast<Context*>(p);c.trace.push_back(static_cast<U32>((a-c.base)/c.width));c.trace.push_back(static_cast<U32>((b-c.base)/c.width));I32 x,y;std::memcpy(&x,a,4);std::memcpy(&y,b,4);return (x>y)-(x<y);};
  btb::original_memory4::sort_bytes(b,count,width,cmp,&ctx);assert(b==expected);assert(ctx.trace==expected_trace);
 }
 else if(kind=="ALLOC"){btb::original_memory4::HeapState s;Heap io;s.mode=in.n();auto n=in.n();s.threshold2=in.n();s.threshold3=in.n();s.heap=0xab;io.small=in.n();const auto result=in.n();const auto expected=in.numbers();assert(reinterpret_cast<Handle>(btb::original_memory4::allocate(s,n,io))==result);assert(io.trace==expected);}
 else if(kind=="FREE"){btb::original_memory4::HeapState s;Heap io;s.mode=in.n();auto p=in.n();io.found=in.n()!=0;s.heap=0xab;const auto expected=in.numbers();btb::original_memory4::release(s,reinterpret_cast<void*>(static_cast<Handle>(p)),io);assert(io.trace==expected);}
 else if(kind=="MESSAGE"){Messages io;btb::original_message1::State s;io.missing=in.n();io.owner=in.n();const auto r1=in.n(),r2=in.n();const auto expected=in.numbers();assert(static_cast<U32>(btb::original_message1::show_message(s,"x","y",0x12010,io))==r1);assert(static_cast<U32>(btb::original_message1::show_message(s,"x","y",0x12010,io))==r2);assert(io.trace==expected);}
 else if(kind=="TARGET"){btb::original_printer7::Printer p;Geometry io;p.scale_mode=static_cast<I32>(in.n());std::array<U32,10> dib{};dib[1]=in.n();dib[2]=in.n();p.dib=reinterpret_cast<std::byte*>(dib.data());for(auto& cap:io.caps)cap=static_cast<I32>(in.n());std::array<U32,4> expected{};for(auto& n:expected)n=in.n();p.target={11,12,13,14};const auto trace=in.numbers();btb::original_printer2::compute_target(p,1,0x777,io);if(std::memcmp(&p.target,expected.data(),16)){std::cerr<<line<<"\nACT "<<p.target.left<<" "<<p.target.top<<" "<<p.target.right<<" "<<p.target.bottom<<"\n";std::abort();}assert(io.trace==trace);}
 else if(kind=="CLIP"){using Rect=btb::original_printer2::Rect;Rect target,dest,src;for(auto* r:{&target,&dest,&src}){r->left=static_cast<I32>(in.n());r->top=static_cast<I32>(in.n());r->right=static_cast<I32>(in.n());r->bottom=static_cast<I32>(in.n());}const auto result=in.n();std::array<U32,8> expected{};for(auto& n:expected)n=in.n();const auto actual=btb::original_printer2::map_clipped(target,dest,src);assert((actual&255)==result);assert(std::memcmp(&dest,expected.data(),16)==0);assert(std::memcmp(&src,expected.data()+4,16)==0);}
 else if(kind=="BLOCK"){btb::original_environment4::State s;Environment io;s.block_api=in.n();io.wide_ok=in.n()!=0;io.narrow_ok=in.n()!=0;io.conversion_fail=in.n();io.fail=in.n();const auto result=in.n(),mode=in.n(),allocs=in.n(),frees=in.n(),block_frees=in.n(),conversions=in.n();const auto expected=unhex(in.word());const auto* p=btb::original_environment4::get_environment_block(s,io);assert((p!=nullptr)==(result!=0));assert(s.block_api==mode && io.allocs==allocs && io.frees==frees && io.block_frees==block_frees && io.conversions==conversions);if(p)assert(std::memcmp(p,expected.data(),expected.size())==0);}
 else if(kind=="ENV"){
  btb::original_environment4::State s;Environment io;const auto n=in.n();std::vector<std::string> initial;for(U32 j=0;j<n;++j)initial.push_back(text(in.word()));std::vector<char*> table(64);for(U32 j=0;j<n;++j)table[j]=initial[j].data();char16_t* empty=nullptr;s.narrow=table.data();s.wide=&empty;
  auto value=text(in.word());const auto update=in.n();io.fail=in.n();const auto result=in.n(),size=in.n();std::vector<std::string> expected;for(U32 j=0;j<size;++j)expected.push_back(text(in.word()));const auto allocs=in.n(),frees=in.n(),resizes=in.n(),os_count=in.n();std::vector<std::pair<std::string,std::string>> os;for(U32 j=0;j<os_count;++j){auto name=text(in.word());auto val=text(in.word());os.emplace_back(name,val);}const auto lookup=in.word();
  assert(static_cast<U32>(btb::original_environment4::put_environment(s,value.data(),update!=0,io))==result);std::vector<std::string> actual;for(auto** p=s.narrow;*p;++p)actual.emplace_back(*p);assert(actual==expected);assert(io.allocs==allocs && io.frees==frees && io.resizes==resizes && io.os==os);const auto* found=btb::original_environment4::get_environment(s,"A",io);assert((found?std::string(found):std::string{})==text(lookup));
 }
 else if(kind=="WIDE"){btb::original_environment4::State s;Environment io;io.fail=in.n();io.conversion_fail=in.n();const auto result=in.n(),allocs=in.n(),frees=in.n(),resizes=in.n(),conversions=in.n(),n=in.n();std::vector<std::string> expected;for(U32 j=0;j<n;++j)expected.push_back(text(in.word()));std::vector<char*> table(64);s.narrow=table.data();char16_t a[]=u"A=one",b[]=u"B=two";char16_t* wide[]{a,b,nullptr};s.wide=wide;assert(static_cast<U32>(btb::original_environment4::convert_wide_environment(s,io))==result);std::vector<std::string> actual;for(auto** p=s.narrow;*p;++p)actual.emplace_back(*p);assert(actual==expected);assert(io.allocs==allocs && io.frees==frees && io.resizes==resizes && io.conversions==conversions);}
 else if(kind=="SOUND"){Creation io;WaveIo mm;Audio audio(io.trace);const auto n=in.n();std::array<U32,5> expected{};for(auto& x:expected)x=in.n();const auto sizes=in.numbers(),trace=in.numbers();std::vector<Handle> buffers;for(U32 j=0;j<n;++j)buffers.push_back(0x802000+j*16);btb::original_sound2::Sound s;btb::original_wave7::Wave w;assert(&btb::original_sound2::construct_sound(s,buffers,8,w,mm,audio,io)==&s);assert(s.vtable==expected[0] && s.size==expected[2] && s.count==expected[4] && s.wave==&w);assert(std::equal(buffers.begin(),buffers.end(),s.buffers));assert(io.sizes==sizes && io.trace==trace);}
 else if(kind=="CREATE"){Creation io;WaveIo mm;Audio audio(io.trace);const auto count=in.n();io.failure=in.n();mm.first_empty=in.n()!=0;mm.fallback_empty=in.n()!=0;const auto cd=static_cast<I32>(in.n());io.create_code=in.n();io.duplicate_failure=in.n();const auto result=in.n(),out_kind=in.n(),opens=in.n();const auto state=in.numbers(),sizes=in.numbers(),trace=in.numbers();mm.format[0]=std::byte{1};btb::original_sound2::Sound sentinel;auto* out=&sentinel;const auto actual=btb::original_sound2::create_sound(1,&out,"test.wav",0xface,{1,2,3,4},count,cd,mm,audio,io);assert(actual==result);assert((out==nullptr?0U:out==&sentinel?1U:2U)==out_kind);if(mm.opens!=opens){std::cerr<<line<<"\nOPENS "<<mm.opens<<" expected "<<opens<<"\n";std::abort();}if(out && out!=&sentinel)assert(state==std::vector<U32>({out->vtable,out->size,out->count}));assert(io.sizes==sizes && io.trace==trace);}
 else {std::cerr<<"Unknown "<<kind<<'\n';return 1;}
 }
 // Failure and ownership boundaries beyond the deterministic x86 corpus.
 {btb::original_environment4::State s;Environment io;s.enabled=0;assert(!btb::original_environment4::get_environment(s,"A",io));assert(!io.conversions);s.enabled=1;io.fail=1;char value[]="A=v";assert(btb::original_environment4::put_environment(s,value,false,io)==-1);assert(!s.narrow);}
 {btb::original_environment4::State s;Environment io;char a[]="A=old";char* table[]{a,nullptr};char16_t* empty=nullptr;s.narrow=table;s.initial=table;s.wide=&empty;char value[]="A=new";assert(!btb::original_environment4::put_environment(s,value,false,io));assert(s.narrow!=table && table[0]==a && s.narrow[0]==value);assert(!io.frees);}
 {btb::original_environment4::State s;Environment io;char a[]="A=v",b[]="B=w";char* table[]{a,b,nullptr};char16_t* empty=nullptr;s.narrow=table;s.wide=&empty;char value[]="A=";io.resize_failure=true;assert(!btb::original_environment4::put_environment(s,value,false,io));assert(s.narrow==table && table[0]==b && table[1]==nullptr && io.frees==1);}
 {Creation io;WaveIo mm;Audio audio(io.trace);btb::original_sound2::Sound sentinel;auto* p=&sentinel;assert(btb::original_sound2::create_sound(0,&p,"x",0,{},1,-1,mm,audio,io)==0x800401f0);assert(btb::original_sound2::create_sound(1,nullptr,"x",0,{},1,-1,mm,audio,io)==0x80070057);assert(btb::original_sound2::create_sound(1,&p,nullptr,0,{},1,-1,mm,audio,io)==0x80070057);assert(btb::original_sound2::create_sound(1,&p,"x",0,{},0,-1,mm,audio,io)==0x80070057);assert(p==&sentinel && io.sizes.empty());}
 assert(cases>=4000);std::cout<<cases<<" original x86 cases passed\n";
}
