#include "batch20_services.hpp"
#include "batch20_vectors.hpp"
#include <iostream>
using namespace btb::original_runtime6;
using namespace batch20;
int main(int argc,char** argv){
 if(argc!=2 || !verify_6_original_runtime_functions())return 1;std::ifstream file(argv[1]);if(!file)return 2;
 std::string line,kind;unsigned checked{};Nls nls;auto state=locale();
 while(std::getline(file,line)) {
  std::istringstream in(line);in>>kind;bool ok=true;
  if(kind=="ATOI"){auto text=bytes(in);const auto expected=integer(in);ok=parse_integer(state,text.c_str(),nls)==expected;}
  else if(kind=="UPPER"){auto value=word(in);auto expected=word(in);ok=upper_character(state,value,nls)==expected;}
  else if(kind=="DECIMAL"){auto text=bytes(in);const auto decimal=static_cast<char>(word(in));auto expected=bytes(in);std::vector<char> buffer(text.size()+2);std::memcpy(buffer.data(),text.data(),text.size());insert_decimal_point(state,buffer,decimal,nls);ok=std::string(buffer.data())==expected;}
  else if(kind=="SHORT"){auto cursor=word(in);const auto value=word(in),expected=word(in),next=word(in);const std::array<std::byte,2> memory{std::byte{static_cast<std::uint8_t>(value)},std::byte{static_cast<std::uint8_t>(value>>8)}};const U32 base=cursor;ok=fetch_short_argument(memory,base,cursor)==expected && cursor==next;}
  else if(kind=="OPEN") {
   const auto mode=bytes(in);const auto defaults=word(in);const auto result=integer(in);const auto share=word(in),present=word(in),expected_allocations=word(in);
   std::array<U32,8> expected{};for(auto& v:expected)v=word(in);const auto expected_trace=trace(in);
   struct Open final:OpenServices {I32 result{};std::vector<U32> trace;I32 open_file(const char*,U32 flags,U32 share,U32 perm) override {trace={flags,share,perm};return result;}} io;io.result=result;
   btb::original_runtime16::Stream32 stream{{11,12,13,14,15,16,17,18}};U32 allocations=9;
   const auto* out=open_stream("file",mode.c_str(),share,stream,defaults,allocations,io);
   ok=static_cast<U32>(out!=nullptr)==present && allocations==expected_allocations && stream.words==expected && io.trace==expected_trace;
  } else if(kind=="CODEPAGE") {
   const I32 request=integer(in),expected=integer(in);const U32 system=word(in),cp=word(in),multi=word(in),id=word(in),a=word(in),b=word(in),c=word(in);
   std::array<std::uint8_t,257> flags{};std::array<std::uint8_t,256> mapping{};
   for(auto& v:flags)v=static_cast<std::uint8_t>(word(in));for(auto& v:mapping)v=static_cast<std::uint8_t>(word(in));
   auto s=locale();s.codepage=1252;s.case_map.fill(0x11);s.multibyte_flags.fill(2);
   btb::original_runtime8::Locale legacy;legacy.codepage=77;legacy.is_multibyte=2;legacy.locale_id=55;legacy.flags=1;legacy.state_a=2;legacy.state_b=3;legacy.table.fill(2);U32 selected=0;
   struct Codes final:SystemCodepages{I32 oem() override{return 932;}I32 ansi() override{return 1252;}} codes;
   const I32 result=set_multibyte_codepage(s,legacy,selected,request,codes,nls);
   ok=result==expected && selected==system && legacy.codepage==cp && legacy.is_multibyte==multi && legacy.locale_id==id && legacy.flags==a && legacy.state_a==b && legacy.state_b==c && legacy.table==flags && s.case_map==mapping;
  } else continue;
  if(!in || !ok){std::cerr<<"Runtime reference mismatch "<<checked<<": "<<line<<'\n';return 3;}++checked;
 }
 assert(checked==2914);
 // Exercise the real recovered classification branch instead of a stand-in.
 state.maximum_character_size=2;assert(parse_integer(state," -123stop",nls)==-123);
 state.locale=0x409;state.classes['a'+1]=2;assert(upper_character(state,'a',nls)=='a'); // API unavailable retains original value.
 auto cp_state=locale();btb::original_runtime8::Locale legacy;legacy.codepage=1252;U32 selected=9;
 struct Codes final:SystemCodepages{I32 oem() override{return 1;}I32 ansi() override{return 1;}} codes;
 assert(set_multibyte_codepage(cp_state,legacy,selected,1252,codes,nls)==0 && selected==0);
 struct SequenceNls final:Nls {
  unsigned calls{};
  bool codepage_info(U32,btb::original_locale7::CodepageInfo& info) override{++calls;if(calls!=1)return false;info.maximum_character_size=2;info.lead_bytes={0x81,0x81};return true;}
 } sequence;
 btb::original_runtime8::Locale unknown;auto ordinary=locale();
 assert(set_multibyte_codepage(ordinary,unknown,selected,777,codes,sequence)==0 && unknown.codepage==777 && unknown.is_multibyte==1);
 assert((unknown.table[0x81+1]&12)==12 && !(unknown.table[256]&8) && !(unknown.table[1]&8) && sequence.calls==2);
#ifdef _WIN32
 btb::original_locale7::Win32NlsServices native;auto windows=locale();windows.locale=0x409;windows.codepage=1252;windows.classes['a'+1]=2;
 assert(upper_character(windows,'a',native)=='A');
#endif
 std::cout<<checked<<" original runtime comparisons passed\n";
}
