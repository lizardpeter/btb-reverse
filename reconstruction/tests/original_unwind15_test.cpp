#include "btb/original_unwind13.hpp"
#include "btb/original_ftol1.hpp"
#include "btb/original_heap9.hpp"
#include <array>
#include <bit>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
namespace eh=btb::original_unwind13;
using U32=std::uint32_t;using I32=std::int32_t;
struct Exit {};
struct Input {
 std::istringstream in;explicit Input(const std::string& line):in(line){}
 U32 n(){std::string word;in>>word;assert(!word.empty());return static_cast<U32>(std::stoull(word,nullptr,16));}
 std::vector<U32> numbers(){U32 count=n();std::vector<U32> values;while(count--)values.push_back(n());return values;}
};
struct Services:eh::Services,btb::original_heap9::SizeServices {
 eh::State state;eh::Registration initial{},next{};std::map<U32,U32> memory;std::vector<U32> trace;std::string kind;bool throw_handler{},throw_cleanup{};
 U32 notify_target{},notify_frame{},notify_code{};
 Services(){next={nullptr,0x888,-1,0x800600};initial={&next,0x777,-1,0x800500};state.chain=&initial;}
 U32 read32(U32 at) override{return memory[at];}
 void write32(U32 at,U32 value) override{memory[at]=value;}
 U32 read(U32 at) override{return read32(at);}
 void write(U32 at,U32 value) override{write32(at,value);}
 void event(std::initializer_list<U32> values){trace.insert(trace.end(),values);}
 U32 invoke_this(U32 target,U32 object,std::span<const U32> args) override {
  event({1,target,object,static_cast<U32>(args.size())});trace.insert(trace.end(),args.begin(),args.end());U32 sum{};for(auto x:args)sum+=x;return target^object^sum;
 }
 U32 invoke_frame(U32 target,U32 bp) override {
  const auto* r=state.chain;event({2,target,bp,notify_target,notify_frame,notify_code,r->handler});
  if(kind=="LOCAL")event({static_cast<U32>(r->level),r->frame,read32(0x80000c)});
  else if(kind=="CATCH")event({static_cast<U32>(r->level),r->frame,r->function_info});
  else if(kind=="CPP")event({static_cast<U32>(r->level),read32(0x800008)});
  if(throw_cleanup)throw eh::GuestException{0xe06d7363U};
  return target^bp^0x12345678;
 }
 void notify(U32 target,U32 bp,U32 code) override{notify_target=target;notify_frame=bp;notify_code=code;}
 void invoke_handler(U32 target) override{event({4,target});if(throw_handler)throw eh::GuestException{0xe06d7363U};}
 void rtl_unwind(U32 frame,U32 instruction,U32 record,U32 value) override{event({3,frame,instruction,record,value});state.chain=&next;}
 void raise_exception(U32 code,U32 flags,std::span<const U32> args) override{event({8,code,flags,static_cast<U32>(args.size())});trace.insert(trace.end(),args.begin(),args.end());}
 void report_error(U32 code) override{event({5,code});}
 void raise_signal(U32 code) override{event({6,code});}
 [[noreturn]] void exit_process(U32 code,U32 quick,U32 returning) override{event({7,code,quick,returning});throw Exit{};}
 U32 heap_size(U32 heap,U32 flags,U32 pointer) override{event({9,heap,flags,pointer});return pointer^0x12345678;}
 // These services must never be needed by _msize.
 U32 heap_resize(U32,U32,U32) override{std::abort();}
 U32 heap_allocate(U32,U32,U32) override{std::abort();}
 void heap_free(U32,U32) override{std::abort();}
 U32 virtual_allocate(U32,U32,U32) override{std::abort();}
 bool virtual_free(U32,U32,U32) override{std::abort();}
 U32 commit(U32,U32) override{std::abort();}
 std::span<std::byte> bytes(U32,U32) override{std::abort();}
 bool new_handler(U32) override{std::abort();}
 U32 chain_token()const{return state.chain==&initial?0x808000:state.chain==&next?0x809000:0;}
};
static I32 si(U32 x){return std::bit_cast<I32>(x);}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1]);assert(file);std::string line;std::size_t count=0;
 while(std::getline(file,line)){
  Input input(line);std::string kind;input.in>>kind;
  if(kind=="FTOL"){
   const auto low=input.n(),high=input.n(),se=input.n(),cw=input.n(),outlow=input.n(),outhigh=input.n(),outcw=input.n();
   const auto got=btb::original_ftol1::truncate_integer({low,high,static_cast<std::uint16_t>(se)});
   if(got!=((std::uint64_t{outhigh}<<32)|outlow)){std::cerr<<"FTOL mismatch row "<<count<<' '<<std::hex<<low<<' '<<high<<' '<<se<<" got "<<got<<" expected "<<outhigh<<outlow<<'\n';return 1;}
   assert(cw==outcw);++count;continue;
  }
  Services io;io.kind=kind;auto args=input.numbers();U32 memcount=input.n();
  while(memcount--){const auto address=input.n(),value=input.n();io.memory[address]=value;}
  io.initial.handler=io.read32(0x808004)?io.read32(0x808004):0x777;
  io.initial.level=io.memory.contains(0x808008)?si(io.read32(0x808008)):-1;
  io.initial.frame=io.read32(0x80800c)?io.read32(0x80800c):0x800500;
  U32 outcount=input.n();std::vector<std::pair<U32,U32>> outputs;
  while(outcount--){const auto address=input.n(),value=input.n();outputs.emplace_back(address,value);}
  const auto expected_trace=input.numbers();const bool expected_exit=input.n()!=0;const bool returns=!expected_exit && (kind=="OBJ"||kind=="CATCH"||kind=="ABNORMAL"||kind=="RANGE"||kind=="SIZE");
  const U32 expected=returns?input.n():0,chain=input.n(),nt=input.n(),nf=input.n(),nc=input.n();U32 result{};bool exited=false;
  try {
   if(kind=="THROW")eh::throw_exception(args[0],args[1],io);
   else if(kind=="OBJ"){
    // Both byte-identical original thiscall trampolines are linked and exercised.
    result=(count&1)?eh::call_object(args[0],args[1],std::span<const U32>(args).subspan(2),io):eh::call_object_virtual(args[0],args[1],std::span<const U32>(args).subspan(2),io);
   }else if(kind=="CATCH")result=eh::call_catch(io.state,args[0],args[1],args[2],si(args[3]),args[4],io);
   else if(kind=="LOCAL")eh::local_unwind(io.state,args[0],si(args[1]),0x80f000,io);
   else if(kind=="CPP")eh::unwind_cpp(io.state,args[0],args[1],args[2],si(args[3]),io);
   else if(kind=="ABNORMAL")result=eh::abnormal_termination(io.state,io);
   else if(kind=="UNWIND"){
    eh::unwind_to(io.state,args[0],args[1],io);io.write32(0x808000,io.initial.previous==&io.next?0x809000:0);
   }else if(kind=="GLOBAL")eh::global_unwind(args[0],io);
   else if(kind=="DESTROY")eh::destroy_exception(io.state,args[0],io);
   else if(kind=="TERMINATE"||kind=="UNEXPECTED"){
    io.state.terminate_handler=io.read32(0x51c42c);io.state.unexpected_handler=io.read32(0x4475b4);
    if(kind=="TERMINATE")eh::terminate(io.state,io);else eh::unexpected(io.state,io);
   }else if(kind=="RANGE"){
    result=eh::find_try_range(io.state,args[0],si(args[1]),si(args[2]),io.memory[args[3]],io.memory[args[4]],io);
   }else if(kind=="SIZE"){
    btb::original_heap4::State base;base.heap=io.read32(0x51da40);base.regions=io.read32(0x51da34);base.table=io.read32(0x51da38);
    btb::original_heap9::State state{base};state.mode=io.read32(0x51da44);result=btb::original_heap9::allocation_size(state,args[0],io);
   }else {std::cerr<<"unknown kind "<<kind<<'\n';return 1;}
  }catch(const Exit&){exited=true;}
  const bool terminal=expected_exit;
  bool ok=io.trace==expected_trace && (!returns||result==expected) && (terminal||io.chain_token()==chain) && (terminal||!exited) && io.notify_target==nt && io.notify_frame==nf && io.notify_code==nc;
  for(const auto [at,value]:outputs)ok=ok&&io.read32(at)==value;
  if(!ok){std::cerr<<"Mismatch "<<count<<' '<<kind<<" result "<<std::hex<<result<<" expected "<<expected<<"\ntrace ";for(auto x:io.trace)std::cerr<<x<<' ';std::cerr<<"\nexpected ";for(auto x:expected_trace)std::cerr<<x<<' ';std::cerr<<'\n';return 1;}
  assert(!terminal||exited);++count;
 }
 assert(count==6020);
 // Exceptions from a terminate/unexpected handler are swallowed before abort.
 Services io;io.state.terminate_handler=1;io.state.unexpected_handler=2;io.throw_handler=true;
 try{eh::unexpected(io.state,io);assert(false);}catch(const Exit&){}
 assert((io.trace==std::vector<U32>{4,2,4,1,5,10,6,22,7,3,1,0}));assert(io.state.chain==&io.initial);
 // A C++ exception thrown from an unwind funclet invokes terminate.
 Services cleanup;cleanup.kind="CPP";cleanup.throw_cleanup=true;cleanup.memory={{0x800008,0},{0x801004,1},{0x801008,0x802000},{0x802000,0xffffffff},{0x802004,0x900300}};
 try{eh::unwind_cpp(cleanup.state,0x800000,0,0x801000,-1,cleanup);assert(false);}catch(const Exit&){}
 assert(cleanup.state.chain==&cleanup.initial);
 std::cout<<count<<" original-x86 comparisons passed\n";
}
