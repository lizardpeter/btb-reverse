#include "btb/original_cxx12.hpp"
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
namespace cxx=btb::original_cxx12;
using U32=std::uint32_t;using I32=std::int32_t;
constexpr U32 DATA=0x800000,STOP=0x900000,REC=DATA+0x1000,FRAME=DATA+0x2000,INFO=DATA+0x3000,REG=DATA+0xa000;
struct Exit {};
struct Input {
 std::istringstream in;explicit Input(const std::string& line):in(line){}
 U32 n(){std::string word;in>>word;assert(!word.empty());return static_cast<U32>(std::stoull(word,nullptr,16));}
 std::vector<U32> numbers(){auto nitems=n();std::vector<U32> v;while(nitems--)v.push_back(n());return v;}
};
struct Services:cxx::Services {
 cxx::State state;cxx::Registration initial{},next{};std::vector<std::byte> data=std::vector<std::byte>(65536);std::map<U32,U32> globals;std::vector<U32> trace;
 U32 notify_target{},notify_frame{},notify_code{};bool throw_rethrow{},throw_other{},throw_translation{};
 Services(){next={nullptr,0x888,-1,FRAME};initial={&next,0x777,-1,FRAME};state.chain=&initial;}
 std::span<std::byte> bytes(U32 at,U32 n) override{if(at<DATA || at-DATA>data.size() || n>data.size()-(at-DATA))throw std::out_of_range("guest byte range");return {data.data()+at-DATA,n};}
 U32 read32(U32 at) override{if(at>=DATA&&at<=DATA+65532){U32 value;std::memcpy(&value,bytes(at,4).data(),4);return value;}return globals[at];}
 void write32(U32 at,U32 value) override{if(at>=DATA&&at<=DATA+65532)std::memcpy(bytes(at,4).data(),&value,4);else globals[at]=value;}
 void event(std::initializer_list<U32> values){trace.insert(trace.end(),values);}
 bool readable(U32 at,U32 n) override{event({10,at,n});return !(read32(DATA+0xf000)&1);}
 bool writable(U32 at,U32 n) override{event({11,at,n});return !(read32(DATA+0xf000)&2);}
 bool executable(U32 at) override{event({13,at});return !(read32(DATA+0xf000)&4);}
 U32 invoke_this(U32 target,U32 object,std::span<const U32> args) override{
  event({1,target,object,static_cast<U32>(args.size())});trace.insert(trace.end(),args.begin(),args.end());if(!args.empty()){const auto value=read32(args[0]);write32(object,value);}return 0x12345678;
 }
 U32 invoke_frame(U32 target,U32 bp) override{
  event({2,target,bp,notify_target,notify_frame,notify_code,read32(0x51c420),read32(0x51c424)});write32(FRAME-4,0xabcdef);
  if(throw_rethrow)throw cxx::Raised{DATA+0x1200,0};if(throw_other)throw cxx::Raised{DATA+0x1300,0};return read32(DATA+0xf004);
 }
 void notify(U32 target,U32 bp,U32 code) override{notify_target=target;notify_frame=bp;notify_code=code;}
 void invoke_handler(U32) override{std::abort();}
 void rtl_unwind(U32 frame,U32 instruction,U32 record,U32 value) override{event({3,frame,instruction,record,value});if(state.chain && state.chain->handler==0x42fddb)static_cast<cxx::Translation*>(state.chain)->unwound=true;state.chain=&next;}
 void raise_exception(U32,U32,std::span<const U32>) override{std::abort();}
 void report_error(U32 code) override{event({5,code});}
 void raise_signal(U32 code) override{event({6,code});}
 [[noreturn]] void exit_process(U32 code,U32 quick,U32 returning) override{event({7,code,quick,returning});throw Exit{};}
 U32 invoke_dispatch(U32,const std::array<U32,8>& args) override{event({14});trace.insert(trace.end(),args.begin(),args.end());return read32(DATA+0xf004);}
 void invoke_translation(U32 target,U32 code,U32 record,U32 context,cxx::Translation& r) override{event({15,target,code,record,context,r.handler,r.function_info,r.frame,static_cast<U32>(r.level),r.dispatch_frame});if(throw_translation)throw cxx::Raised{REC,context};}
 std::array<U32,2> stack_context() override{return {DATA+0xd000,DATA+0xe000};}
 U32 registration_address(const cxx::Registration&) override{return REG;}
 [[noreturn]] void resume(U32 target,U32 stack,U32 base) override{event({12,target,stack,base});throw cxx::ResumeTransfer{target,stack,base};}
 U32 chain_token()const{return state.chain==&initial?DATA+0x8000:state.chain==&next?DATA+0x9000:0;}
};
static I32 si(U32 x){return std::bit_cast<I32>(x);}
int main(int argc,char** argv){
 assert(argc==2);std::ifstream file(argv[1]);assert(file);std::string line;std::size_t count=0;
 while(std::getline(file,line)){
  Input input(line);std::string kind;input.in>>kind;auto args=input.numbers();Services io;U32 n=input.n();while(n--){const U32 at=input.n(),value=input.n();io.write32(at,value);}
  std::vector<std::pair<U32,U32>> outputs;n=input.n();while(n--){const U32 at=input.n(),value=input.n();outputs.emplace_back(at,value);}
  const auto trace=input.numbers();const U32 mode=input.n();const bool returns=!mode&&(kind=="INVOKE"||kind=="DISPATCH"||kind=="FRAME"||kind=="CATCH_HANDLER"||kind=="TRANSLATE"||kind=="TRANSHANDLER");
  const U32 expected=returns?input.n():0,chain=input.n(),nt=input.n(),nf=input.n(),nc=input.n();U32 result{},actual_mode{};
  try {
   if(kind=="COPY")cxx::copy_catch(io.state,args[0],args[1],args[2],args[3],io);
   else if(kind=="FINISH")cxx::finish_catch(io.state,{args[0],args[1],io.read32(DATA+0xe000-0x28),io.read32(DATA+0xe000-0x1c),io.read32(DATA+0xe000-0x20),args[2],args[3]!=0},io);
   else if(kind=="INVOKE")result=cxx::invoke_catch(io.state,args[0],args[1],args[2],args[3],args[4],si(args[5]),args[6],io);
   else if(kind=="DISPATCH")result=cxx::dispatch(io.state,args[0],args[1],args[2],args[3],args[4],si(args[5]),args[6],args[7]!=0,io);
   else if(kind=="FRAME")result=cxx::frame_handler(io.state,args[0],args[1],args[2],args[3],INFO,io);
   else if(kind=="CATCH_HANDLER")result=cxx::catch_handler(io.state,args[0],args[1],args[2],io);
   else if(kind=="SEARCH")cxx::search(io.state,args[0],args[1],args[2],args[3],args[4],args[5]!=0,si(args[6]),args[7],io);
   else if(kind=="FOREIGN")cxx::foreign_exception(io.state,args[0],args[1],args[2],args[3],args[4],si(args[5]),si(args[6]),args[7],io);
   else if(kind=="ENTER")cxx::enter_catch(io.state,args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],si(args[8]),args[9],io);
   else if(kind=="TRANSLATE")result=cxx::translate(io.state,args[0],args[1],args[2],args[3],args[4],si(args[5]),args[6],io);
   else if(kind=="TRANSHANDLER"){
    cxx::Translation r;r.handler=0x42fddb;r.previous=&io.next;r.function_info=io.read32(REG+8);r.frame=io.read32(REG+12);r.level=si(io.read32(REG+16));r.dispatch_frame=io.read32(REG+20);r.return_target=io.read32(REG+24);r.saved_stack=io.read32(REG+28);r.saved_base=io.read32(REG+32);r.unwound=io.read32(REG+36)!=0;
    result=cxx::translator_handler(io.state,args[0],r,args[2],io);io.write32(REG+36,r.unwound);
   }else if(kind=="RESUME")cxx::resume_catch(io.state,args[0],args[1],io);
   else {std::cerr<<"Unknown "<<kind<<'\n';return 1;}
  }catch(const Exit&){actual_mode=1;}catch(const cxx::ResumeTransfer&){actual_mode=2;}
  bool ok=mode==actual_mode&&trace==io.trace&&(!returns||result==expected)&&(mode!=1?io.chain_token()==chain:true)&&io.notify_target==nt&&io.notify_frame==nf&&io.notify_code==nc;
  for(auto [at,value]:outputs)ok=ok&&io.read32(at)==value;
  if(!ok){std::cerr<<"Mismatch row "<<count<<' '<<kind<<" mode "<<mode<<'/'<<actual_mode<<" result "<<std::hex<<expected<<'/'<<result<<"\ntrace ";for(auto x:io.trace)std::cerr<<x<<' ';std::cerr<<"\nexpected ";for(auto x:trace)std::cerr<<x<<' ';std::cerr<<"\nnotify "<<nt<<' '<<nf<<' '<<nc<<" / "<<io.notify_target<<' '<<io.notify_frame<<' '<<io.notify_code<<'\n';for(auto [at,value]:outputs)if(io.read32(at)!=value)std::cerr<<"memory "<<at<<' '<<value<<'/'<<io.read32(at)<<'\n';return 1;}
  ++count;
 }
 assert(count==3420);
 const auto setup=[](Services& io){
  io.write32(REC,0xe06d7363);io.write32(REC+16,3);io.write32(REC+20,0x19930520);io.write32(REC+24,DATA+0x7000);io.write32(REC+28,DATA+0x6000);io.write32(DATA+0x6004,STOP+0x620);
  io.write32(FRAME-4,DATA+0xd000);io.write32(0x51c420,DATA+0x1100);io.write32(0x51c424,0x1234);io.write32(INFO,0x19930520);io.write32(INFO+4,5);
  io.write32(DATA+0x1200,0xe06d7363);io.write32(DATA+0x1210,3);io.write32(DATA+0x1214,0x19930520);
  io.write32(DATA+0x1300,0xc0000005);
 };
 Services rethrow;setup(rethrow);rethrow.throw_rethrow=true;
 assert(cxx::invoke_catch(rethrow.state,REC,FRAME,0x77,INFO,STOP+0x630,0,0x100,rethrow)==0);
 assert(rethrow.state.chain==&rethrow.initial && rethrow.read32(FRAME-4)==DATA+0xd000 && rethrow.read32(0x51c420)==DATA+0x1100 && rethrow.read32(0x51c424)==0x1234);
 assert(rethrow.trace.size()==8); // exactly the funclet, no destructor
 Services other;setup(other);other.throw_other=true;bool propagated=false;
 try{(void)cxx::invoke_catch(other.state,REC,FRAME,0x77,INFO,STOP+0x630,0,0x100,other);}catch(const cxx::Raised& raised){propagated=raised.record==DATA+0x1300;}
 assert(propagated && other.state.chain==&other.initial && other.read32(FRAME-4)==DATA+0xd000 && other.trace.size()==12);
 Services translated;setup(translated);translated.throw_translation=true;translated.write32(0x51c428,STOP+0x810);
 assert(cxx::translate(translated.state,REC,FRAME,0x77,0,INFO,0,FRAME,translated)==1);
 assert(translated.state.chain==&translated.initial);
 std::cout<<count<<" original-x86 comparisons and catch/rethrow/translation integration checks passed\n";
}
