#include "btb/original_file7.hpp"
#include "btb/original_system2.hpp"
#include "btb/original_heap4.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
using U32=std::uint32_t;using I32=std::int32_t;using Handle=std::uintptr_t;
static std::vector<std::byte> unhex(const std::string& s){std::vector<std::byte>b;if(s=="-")return b;for(std::size_t i=0;i<s.size();i+=2)b.push_back(std::byte{static_cast<unsigned char>(std::stoul(s.substr(i,2),nullptr,16))});return b;}
static std::string text(const std::string& s){const auto b=unhex(s);return {reinterpret_cast<const char*>(b.data()),b.size()};}
struct Input {std::istringstream stream;explicit Input(const std::string& s):stream(s){}U32 n(){std::string s;stream>>s;assert(!s.empty());return static_cast<U32>(std::stoull(s,nullptr,16));}std::string word(){std::string s;stream>>s;assert(!s.empty());return s;}std::vector<U32> numbers(){auto count=n();std::vector<U32> v;while(count--)v.push_back(n());return v;}};
constexpr U32 DATA=0x800000;
static U32 hashbytes(const std::byte* p,std::size_t n){U32 value=2166136261;for(std::size_t i=0;i<n;++i)value=(value^std::to_integer<U32>(p[i]))*16777619U;return value;}
struct FileServices:btb::original_file7::Services {
 using Block=btb::original_startup11::DescriptorBlock;using Stream=btb::original_runtime16::Stream32;
 std::array<std::byte,65536> memory;std::vector<std::byte> file;std::vector<U32> trace;std::vector<std::unique_ptr<Block>> blocks;Stream stream;
 U32 stream_address{DATA+0x2000},position{},read_quantum{},write_quantum{},fail_at{},error_code{},file_kind{},step{},last{},allocations{};
 FileServices(){memory.fill(std::byte{0xcc});}
 bool event(U32 id,std::initializer_list<U32> args={}){trace.push_back(id);trace.insert(trace.end(),args);if(++step==fail_at){last=error_code;return true;}return false;}
 Block* allocate_block() override{if(event(10))return nullptr;blocks.push_back(std::make_unique<Block>());return blocks.back().get();}
 U32 standard_handle(U32) override{std::abort();}
 U32 file_type(U32) override{return event(5)?0:file_kind;}
 void set_handle_count(U32) override{std::abort();}
 bool close_handle(U32) override{return !event(6);}
 U32 seek_handle(U32,I32 offset,U32 origin) override{if(event(3,{static_cast<U32>(offset),origin}))return 0xffffffffU;const auto result=static_cast<std::int64_t>(offset)+static_cast<std::int64_t>(origin==1?position:origin==2?file.size():0);if(result<0){last=131;return 0xffffffffU;}assert(result<=16384);position=static_cast<U32>(result);return position;}
 U32 last_error() override{return last;}
 bool read_file(U32,std::byte* out,U32 n,U32& actual) override{if(event(1,{n})){actual=0;return false;}actual=std::min({n,read_quantum,position<file.size()?static_cast<U32>(file.size())-position:0});if(actual)std::memcpy(out,file.data()+position,actual);position+=actual;return true;}
 bool write_file(U32,const std::byte* data,U32 n,U32& actual) override{if(event(2,{n})){actual=0;return false;}actual=std::min(n,write_quantum);if(position+actual>file.size())file.resize(position+actual);if(actual)std::memcpy(file.data()+position,data,actual);position+=actual;return true;}
 U32 create_file(const char*,const btb::original_file7::CreateRequest& r) override{return event(4,{r.access,r.share,r.disposition,r.attributes,r.inherit?1U:0U})?0xffffffffU:0x77;}
 bool end_of_file(U32) override{if(event(7))return false;file.resize(position);return true;}
 void set_standard_handle(U32 a,U32 b) override{event(8,{a,b});}
 bool flush_file(U32) override{std::abort();}
 Stream* allocate_stream(std::size_t) override{std::abort();}
 U32 allocate_guest_bytes(U32 n) override{++allocations;return event(9,{n})?0:DATA+0x8000;}
 std::byte* guest_bytes(U32 address,U32 n) override{
  if(address>=stream_address && address-stream_address<=32 && n<=32-(address-stream_address))return reinterpret_cast<std::byte*>(&stream)+address-stream_address;
  if(address>=DATA && address-DATA<=memory.size() && n<=memory.size()-(address-DATA))return memory.data()+address-DATA;
  std::cerr<<"Bad guest bytes "<<std::hex<<address<<" size "<<n<<'\n';std::abort();
 }
 U32 guest_address(const std::byte* ptr) override{const auto p=reinterpret_cast<Handle>(ptr),m=reinterpret_cast<Handle>(memory.data()),s=reinterpret_cast<Handle>(&stream);if(p>=s && p-s<=32)return stream_address+static_cast<U32>(p-s);assert(p>=m && p-m<=memory.size());return DATA+static_cast<U32>(p-m);}
};
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
struct Environment:btb::original_environment4::Services {
 U32 allocations{},frees{};bool fail{};std::unique_ptr<char[]> saved;
 void* allocate(std::size_t n) override{++allocations;if(fail)return nullptr;saved=std::make_unique<char[]>(n);return saved.get();}
 void* resize(void*,std::size_t) override{std::abort();}
 void release(void*) override{++frees;}
 char** duplicate(char**) override{std::abort();}
 I32 compare(std::string_view a,std::string_view b,std::size_t n) override{const auto x=a.substr(0,n),y=b.substr(0,n);return x<y?1:x>y?3:2;}
 char16_t* environment_wide() override{std::abort();}
 char* environment_narrow() override{std::abort();}
 void free_environment_wide(char16_t*) override{std::abort();}
 void free_environment_narrow(char*) override{std::abort();}
 U32 convert(U32,const char16_t*,I32,char*,U32) override{std::abort();}
 void set_variable(const char*,const char*) override{std::abort();}
};
struct System:btb::original_system2::Services {
 btb::original_system2::ZoneInfo info{};U32 zone_result{},conversion_fail{},conversions{},directory_failure{};std::string directory;std::vector<U32> trace;
 U32 zone_information(btb::original_system2::ZoneInfo& out) override{out=info;return zone_result;}
 U32 convert_name(U32 cp,U32 flags,const char16_t* in,char* out,U32 n,bool& used) override{assert(cp==1252 && flags==0x220 && n==63);++conversions;used=conversion_fail==conversions+2;if(conversion_fail==conversions)return 0;const auto size=std::char_traits<char16_t>::length(in)+1;for(std::size_t i=0;i<size;++i)out[i]=static_cast<char>(in[i]);return static_cast<U32>(size);}
 bool set_directory(const char*) override{trace.push_back(1);return directory_failure!=1;}
 U32 get_directory(U32 n,char* out) override{trace.push_back(2);assert(n==261);if(directory_failure==2)return 0;std::strcpy(out,directory.c_str());return static_cast<U32>(directory.size());}
 bool set_variable(const char* name,const char* value) override{assert(name[0]=='=' && name[2]==':' && !name[3] && value==directory);trace.insert(trace.end(),{3,static_cast<U32>(std::strlen(name))});return directory_failure!=3;}
 U32 last_error() override{return 5;}
};
struct Heap:btb::original_heap4::Services {
 std::array<std::byte,65536> memory;std::array<std::byte,8224> head;std::vector<U32> trace;U32 step{},failure{};
 Heap(){memory.fill(std::byte{0xcc});head.fill(std::byte{0});}
 std::byte* at(U32 address,U32 count=4){if(address>=0x447748 && address-0x447748+count<=head.size())return head.data()+address-0x447748;assert(address>=DATA && address-DATA+count<=memory.size());return memory.data()+address-DATA;}
 U32 read(U32 p) override{U32 value;std::memcpy(&value,at(p),4);return value;}
 void write(U32 p,U32 value) override{std::memcpy(at(p),&value,4);}
 bool event(U32 id,std::initializer_list<U32> args){trace.push_back(id);trace.insert(trace.end(),args);return ++step==failure;}
 U32 heap_resize(U32 h,U32 p,U32 n) override{return event(11,{h,p,n})?0:DATA+0x2000;}
 U32 heap_allocate(U32 h,U32 flags,U32 n) override{return event(12,{h,flags,n})?0:DATA+0x7000;}
 void heap_free(U32 h,U32 p) override{event(15,{h,p});}
 U32 virtual_allocate(U32 n,U32 allocation,U32 protect) override{return event(13,{n,allocation,protect})?0:0x3000000;}
 bool virtual_free(U32 p,U32 n,U32 type) override{return !event(14,{p,n,type});}
 void reset(){std::fill(memory.begin()+0x1000,memory.begin()+0x1000+8224,std::byte{0});const U32 h=0x447748,r=DATA+0x1000;write(h,r);write(h+4,r);write(h+16,0x1000000);write(h+20,0x1400000);write(r,h);write(r+4,h);write(r+16,0x2000000);write(r+20,0x2400000);for(auto p:{h,r})for(U32 i=0;i<1024;++i)write(p+24+i*8,0xffffffffU);}
};
struct HeapLink final:btb::original_heap4::LookupLink {
 using Base=btb::original_heap4::LookupLink;using Base::Base;bool freed{};
 void* allocate3(U32) override{std::abort();}void* allocate2(U32) override{std::abort();}
 Handle find3(void*) override{std::abort();}void free3(Handle,void*) override{std::abort();}
 void free2(Handle region,Handle page,void* block) override{assert(region==DATA+0x1000 && page==0x2000000 && reinterpret_cast<Handle>(block)==0x2000008);freed=true;}
 void* heap_allocate(Handle,U32) override{std::abort();}void heap_free(Handle,void*) override{std::abort();}
};
int main(int argc,char** argv){assert(argc==2);std::ifstream file(argv[1]);assert(file);std::string line;std::vector<btb::original_runtime8::ErrorPair> errors;U32 cases=0;
 while(std::getline(file,line)){Input in(line);const auto kind=in.word();if(kind=="ERRORS"){const auto v=in.numbers();assert(v.size()%2==0);for(std::size_t i=0;i<v.size();i+=2)errors.push_back({static_cast<I32>(v[i]),static_cast<I32>(v[i+1])});continue;}++cases;
 if(kind=="READ" || kind=="WRITE" || kind=="RESIZE" || kind=="OPEN" || kind=="FLUSH" || kind=="FREAD" || kind=="FWRITE"){
  FileServices svc;const U32 flags=in.n(),look=in.n(),fd=in.n();svc.file=unhex(in.word());svc.position=in.n();svc.read_quantum=in.n();svc.write_quantum=in.n();svc.fail_at=in.n();svc.error_code=in.n();svc.file_kind=in.n();svc.stream_address=in.n();for(auto& w:svc.stream.words)w=in.n();
  btb::original_startup11::DescriptorBlock block;for(auto& d:block){d.handle=0xffffffffU;d.flags=0;d.reserved[0]=10;}block[0].handle=0x77;block[0].flags=static_cast<std::uint8_t>(flags);block[0].reserved[0]=static_cast<std::uint8_t>(look);
  btb::original_startup11::DescriptorTable table;table.blocks[0]=&block;table.limit=32;btb::original_runtime16::IoState io{btb::original_runtime16::DescriptorView(table.blocks.data(),&table.limit),72,71,true};btb::original_file7::Context context{io,table,svc,errors};U32 result=0;
  if(kind=="READ")result=static_cast<U32>(btb::original_file7::read_descriptor(context,fd,svc.guest_bytes(DATA+0x1000,512),in.n()));
  else if(kind=="WRITE"){const U32 n=in.n();const auto payload=unhex(in.word());std::copy(payload.begin(),payload.end(),svc.memory.begin()+0x1000);result=static_cast<U32>(btb::original_file7::write_descriptor(context,fd,svc.guest_bytes(DATA+0x1000,512),n));}
  else if(kind=="RESIZE")result=static_cast<U32>(btb::original_file7::resize_file(context,fd,static_cast<I32>(in.n())));
  else if(kind=="OPEN"){const U32 f=in.n(),share=in.n(),permissions=in.n();context.permission_mask=in.n();context.default_mode=in.n();block[0].handle=0xffffffffU;block[0].flags=0;result=static_cast<U32>(btb::original_file7::open_descriptor(context,"test",f,share,permissions));}
  else if(kind=="FLUSH")result=btb::original_file7::flush_byte(context,in.n(),svc.stream,svc.stream_address);
  else {const U32 size=in.n(),count=in.n();if(kind=="FWRITE"){const auto payload=unhex(in.word());std::copy(payload.begin(),payload.end(),svc.memory.begin()+0x1000);result=btb::original_file7::write_items(context,DATA+0x1000,size,count,svc.stream,svc.stream_address);}else result=btb::original_file7::read_items(context,DATA+0x1000,size,count,svc.stream,svc.stream_address);}
  const U32 expected=in.n(),error=in.n(),dos=in.n(),handle=in.n(),descriptor=in.n(),cached=in.n(),position=in.n();const auto data=unhex(in.word());std::array<U32,8> stream{};for(auto& w:stream)w=in.n();const auto allocations=in.n(),data_hash=in.n(),buffer_hash=in.n(),allocation_hash=in.n();const auto trace=in.numbers();
  if(result!=expected || static_cast<U32>(io.error)!=error || io.dos_error!=dos || block[0].handle!=handle || block[0].flags!=descriptor || block[0].reserved[0]!=cached || svc.position!=position || svc.file!=data || svc.stream.words!=stream || context.buffer_allocations!=allocations || hashbytes(svc.memory.data()+0x1000,512)!=data_hash || hashbytes(svc.memory.data()+0x3000,4096)!=buffer_hash || hashbytes(svc.memory.data()+0x8000,4096)!=allocation_hash || svc.trace!=trace){
   std::cerr<<"Mismatch case "<<cases<<" "<<kind<<"\n"<<line<<"\nactual ret/errors/fd/pos "<<std::hex<<result<<" "<<io.error<<" "<<io.dos_error<<" "<<block[0].handle<<" "<<static_cast<U32>(block[0].flags)<<" "<<static_cast<U32>(block[0].reserved[0])<<" "<<svc.position<<"\nstream ";for(auto w:svc.stream.words)std::cerr<<w<<" ";std::cerr<<"\nhashes "<<hashbytes(svc.memory.data()+0x1000,512)<<" "<<hashbytes(svc.memory.data()+0x3000,4096)<<" "<<hashbytes(svc.memory.data()+0x8000,4096)<<"\ntrace ";for(auto n:svc.trace)std::cerr<<n<<" ";std::cerr<<'\n';std::abort();}
 }
 else if(kind=="TZ" || kind=="ZONE"){
  btb::original_runtime15::TimeState t;t.timezone=1234;t.daylight=1;t.daylight_bias=-3600;t.start.year=5;t.end.year=6;t.system_rules=true;btb::original_system2::ZoneState state{t};std::strcpy(state.standard.data(),"STD");std::strcpy(state.daylight.data(),"DST");state.codepage=1252;System svc;Environment heap;Nls nls;btb::original_locale7::State locale;for(U32 i=0;i<256;++i)locale.classes[i+1]=(i>='0' && i<='9'?4:0)|(i==9 || i==10 || i==13 || i==32?8:0);
  btb::original_environment4::State environment;char* entries[2]{};environment.narrow=entries;std::string value,saved;
  if(kind=="TZ"){value=text(in.word());const bool cached=in.n()!=0;heap.fail=in.n()!=0;if(cached){saved=value;state.saved=saved.data();}value="TZ="+value;entries[0]=value.data();}
  else {svc.zone_result=in.n();svc.conversion_fail=in.n();const auto raw=unhex(in.word());assert(raw.size()==sizeof(svc.info));std::memcpy(&svc.info,raw.data(),raw.size());}
  const U32 zone=in.n(),daylight=in.n(),bias=in.n(),rules=in.n(),start=in.n(),end=in.n();const auto standard=unhex(in.word()),daylight_name=unhex(in.word());const U32 a=in.n(),b=kind=="TZ"?in.n():0;
  btb::original_system2::initialize_timezone(state,environment,heap,locale,nls,svc);
  assert(static_cast<U32>(t.timezone)==zone && static_cast<U32>(t.daylight)==daylight && static_cast<U32>(t.daylight_bias)==bias && t.system_rules==(rules!=0) && static_cast<U32>(t.start.year)==start && static_cast<U32>(t.end.year)==end);assert(std::memcmp(state.standard.data(),standard.data(),64)==0 && std::memcmp(state.daylight.data(),daylight_name.data(),64)==0);
  if(kind=="TZ")assert(heap.allocations==a && heap.frees==b);else {assert(svc.conversions==a);assert(std::memcmp(&t.standard_date,&svc.info.standard_date,sizeof(t.standard_date))==0 && std::memcmp(&t.daylight_date,&svc.info.daylight_date,sizeof(t.daylight_date))==0);}
 }
 else if(kind=="CHDIR"){System svc;Nls nls;btb::original_locale7::State locale;for(U32 i='a';i<='z';++i){locale.multibyte_flags[i+1]=32;locale.case_map[i]=static_cast<std::uint8_t>(i-32);}svc.directory=text(in.word());svc.directory_failure=in.n();const auto result=in.n(),error=in.n(),dos=in.n();const auto trace=in.numbers();btb::original_runtime16::IoState state{{},72,71};assert(static_cast<U32>(btb::original_system2::change_directory(state,"x",errors,locale,nls,svc))==result);assert(static_cast<U32>(state.error)==error && state.dos_error==dos && svc.trace==trace);}
 else if(kind=="HLOOK"){Heap svc;svc.reset();btb::original_heap4::State state{0x55,0x447748,DATA+0x1000,6};U32 pointer=in.n(),region=0xdead,page=0xbeef;const auto ret=in.n(),r=in.n(),p=in.n();assert(btb::original_heap4::find_small2(state,pointer,region,page,svc)==ret && region==r && page==p);}
 else if(kind=="HRELEASE"){Heap svc;svc.reset();const auto region=in.n(),current=in.n();svc.failure=in.n();std::array<U32,7> expected{};for(auto& x:expected)x=in.n();const auto trace=in.numbers();btb::original_heap4::State state{0x55,0x447748,current,6};btb::original_heap4::release_region2(state,region,svc);const U32 h=state.head,r=DATA+0x1000;const std::array<U32,7> actual{state.current,svc.read(h),svc.read(h+4),svc.read(h+16),svc.read(r),svc.read(r+4),svc.read(r+16)};assert(actual==expected && svc.trace==trace);}
 else if(kind=="HDECOMMIT"){Heap svc;svc.reset();const auto requested=static_cast<I32>(in.n());svc.failure=in.n();std::array<U32,3> slots{};for(auto& x:slots)x=in.n();const U32 h=0x447748,r=DATA+0x1000;for(auto p:{h,r})for(auto slot:slots)svc.write(p+24+slot*8,240);std::array<U32,12> expected{};for(auto& x:expected)x=in.n();const auto trace=in.numbers();btb::original_heap4::State state{0x55,h,r,6};btb::original_heap4::decommit_small2(state,requested,svc);const std::array<U32,12> actual{state.current,state.free_pages,svc.read(h),svc.read(h+4),svc.read(h+12),svc.read(h+16),svc.read(r),svc.read(r+4),svc.read(r+12),svc.read(r+16),hashbytes(svc.at(h+24,8192),8192),hashbytes(svc.at(r+24,8192),8192)};assert(actual==expected && svc.trace==trace);}
 else if(kind=="HADD"){Heap svc;const U32 count=in.n(),capacity=in.n();svc.failure=in.n();const U32 ret=in.n(),regions=in.n(),cap=in.n(),table=in.n();std::array<U32,5> words{};for(auto& x:words)x=in.n();const auto control=in.n();const auto trace=in.numbers();btb::original_heap4::State state{0x55,0x447748,0,0,DATA+0x1000,capacity,count};assert(btb::original_heap4::add_region3(state,svc)==ret);assert(state.regions==regions && state.capacity==cap && state.table==table);for(U32 i=0;i<5;++i)assert(svc.read(table+count*20+i*4)==words[i]);assert(svc.read(DATA+0x7000)==control && svc.trace==trace);}
 else {std::cerr<<"Unknown kind "<<kind<<'\n';return 1;}
 }
 // Full source linkage: parser/open, descriptor allocation, buffered write/flush,
 // descriptor seek, refill/read and existing fclose cleanup use one state.
 {
  FileServices svc;svc.read_quantum=svc.write_quantum=4096;svc.file_kind=1;
  btb::original_startup11::DescriptorTable table;
  btb::original_runtime16::IoState state;state.console_application=true;
  btb::original_file7::Context context{state,table,svc,errors};btb::original_file7::Link link(context);
  auto* stream=btb::original_runtime6::open_stream("test","w+b",0x40,svc.stream,0,context.buffer_allocations,link);
  assert(stream==&svc.stream && context.buffer_allocations==1 && table.limit==32 && table.blocks[0]);
  assert(&state.descriptors[0]==&(*table.blocks[0])[0] && state.descriptors[0].handle==0x77);
  const std::array<std::byte,3> input{std::byte{'A'},std::byte{10},std::byte{'B'}};
  std::copy(input.begin(),input.end(),svc.memory.begin()+0x1000);
  assert(btb::original_file7::write_items(context,DATA+0x1000,1,3,*stream,svc.stream_address)==3);
  assert(svc.file.empty() && context.buffer_allocations==2);assert(!btb::original_runtime16::flush_stream(*stream,link));
  assert(svc.file==std::vector<std::byte>(input.begin(),input.end()));
  assert(!btb::original_startup11::seek_descriptor(state,0,0,0,svc,errors));
  assert(btb::original_file7::read_items(context,DATA+0x1000+32,1,3,*stream,svc.stream_address)==3);
  assert(std::equal(input.begin(),input.end(),svc.memory.begin()+0x1000+32));
  btb::original_crt10::File file{std::bit_cast<I32>(stream->words[1]),std::bit_cast<I32>(stream->words[3]),0,stream->words[0],stream->words[2],0};
  U32 freed=0;btb::original_crt10::Io upstream;upstream.ctx=&freed;upstream.deallocate=[](U32,void* p){++*static_cast<U32*>(p);};
  btb::original_runtime16::BufferedIoLink close_link(state,link,upstream);
  const auto callbacks=btb::original_startup11::connect_close(close_link,state,link,svc,errors);
  assert(!btb::original_crt10::fclose_original(file,callbacks));assert(freed==1 && file.flags==0 && state.descriptors[0].flags==0 && state.descriptors[0].handle==0xffffffffU);
 }
 {
  Heap services;services.reset();btb::original_heap4::State state{0x55,0x447748,DATA+0x1000,6};HeapLink link(state,services);btb::original_memory4::HeapState heap{2,0,0,0x55};
  btb::original_memory4::release(heap,reinterpret_cast<void*>(0x2000100),link);assert(link.freed);
 }
 {
  btb::original_runtime15::TimeState time;btb::original_system2::ZoneState zone{time};
  Environment heap;Nls nls;System services;btb::original_environment4::State env;char tz[]="TZ=UTC0";char* entries[]{tz,nullptr};env.narrow=entries;
  btb::original_locale7::State locale;for(U32 i='0';i<='9';++i)locale.classes[i+1]=4;
  btb::original_system2::TimeLink link(zone,env,heap,locale,nls,services);
  assert(btb::original_runtime15::calendar_to_time(time,1970,1,1,0,0,0,0,link,{0,0})==0);
  assert(time.initialized==1 && time.timezone==0 && !time.system_rules && heap.allocations==1);
 }
 {
  FileServices svc;svc.write_quantum=1;svc.read_quantum=4096;svc.file_kind=1;std::fill_n(svc.memory.begin()+0x1000,32,std::byte{10});
  btb::original_startup11::DescriptorBlock block;block[0].handle=0x77;block[0].flags=0x81;block[0].reserved[0]=10;
  btb::original_startup11::DescriptorTable table;table.blocks[0]=&block;table.limit=32;
  btb::original_runtime16::IoState state{block};btb::original_file7::Context context{state,table,svc,errors};
  svc.stream.words={DATA+0x3000,0,DATA+0x3000,0x102,0,0,16,0};bool guarded=false;
  try{btb::original_file7::write_items(context,DATA+0x1000,1,32,svc.stream,svc.stream_address);}catch(const std::runtime_error&){guarded=true;}
  assert(guarded && svc.file.size()==1 && svc.file[0]==std::byte{13});
 }
 assert(cases>3500);std::cout<<cases<<" original x86 cases passed\n";
}
