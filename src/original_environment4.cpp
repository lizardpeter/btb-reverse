#include "btb/original_environment4.hpp"
#include "btb/original_memory4.hpp"
#include <cstring>
#include <vector>
namespace btb::original_environment4 {
namespace {
I32 compare(std::string_view a,std::string_view b,std::size_t n,void* p){return static_cast<Services*>(p)->compare(a,b,n);}
}
char* get_environment_block(State& state,Services& io){
 char16_t* wide=nullptr;char* narrow=nullptr;
 if(state.block_api==0){wide=io.environment_wide();if(wide)state.block_api=1;else {narrow=io.environment_narrow();if(!narrow)return nullptr;state.block_api=2;}}
 if(state.block_api==1){
  if(!wide)wide=io.environment_wide();if(!wide)return nullptr;
  const auto* end=wide;while(*end){while(*end)++end;++end;}const auto count=static_cast<I32>(end-wide+1);
  const U32 size=io.convert(0,wide,count,nullptr,0);char* result=nullptr;
  if(size){result=static_cast<char*>(io.allocate(size));if(result && !io.convert(0,wide,count,result,size)){io.release(result);result=nullptr;}}
  io.free_environment_wide(wide);return result;
 }
 if(state.block_api!=2)return nullptr;
 if(!narrow)narrow=io.environment_narrow();if(!narrow)return nullptr;
 const auto* end=narrow;while(*end){while(*end)++end;++end;}const auto size=static_cast<U32>(end-narrow+1);
 auto* out=static_cast<char*>(io.allocate(size));if(out)original_memory4::copy_bytes(out,narrow,size);
 io.free_environment_narrow(narrow);return out;
}
I32 convert_wide_environment(State& state,Services& io){
 for(auto** entry=state.wide;*entry;++entry){const U32 size=io.convert(1,*entry,-1,nullptr,0);if(!size)return -1;
  auto* bytes=static_cast<char*>(io.allocate(size));if(!bytes || !io.convert(1,*entry,-1,bytes,size))return -1; // Retail does not free failed conversion allocation.
  put_environment(state,bytes,false,io); // Original ignores insertion failure and continues.
 }
 return 0;
}
I32 put_environment(State& state,char* text,bool update_os,Services& io){
 if(!text)return -1;
 const auto* equal=reinterpret_cast<const char*>(original_runtime8::multibyte_chr(reinterpret_cast<const std::uint8_t*>(text),'=',state.locale.is_multibyte!=0,state.locale.table));
 if(!equal || equal==text)return -1;const bool remove=equal[1]==0;
 if(state.narrow==state.initial)state.narrow=io.duplicate(state.narrow);
 if(!state.narrow){
  if(!update_os || !state.wide){
   if(remove)return 0;
   state.narrow=static_cast<char**>(io.allocate(sizeof(char*)));if(!state.narrow)return -1;state.narrow[0]=nullptr;
   if(!state.wide){state.wide=static_cast<char16_t**>(io.allocate(sizeof(char16_t*)));if(!state.wide)return -1;state.wide[0]=nullptr;}
  }else if(convert_wide_environment(state,io)!=0)return -1;
 }
 std::vector<std::string_view> entries;for(auto** p=state.narrow;*p;++p)entries.emplace_back(*p);
 I32 index=original_runtime8::environment_find(entries,{text,static_cast<std::size_t>(equal-text)},compare,&io);
 auto** table=state.narrow;
 if(index<0 || !table[0]){
  if(remove)return 0;if(index<0)index=-index;
  auto** grown=static_cast<char**>(io.resize(table,(static_cast<std::size_t>(index)+2)*sizeof(char*)));if(!grown)return -1;
  table=grown;table[index]=text;table[index+1]=nullptr;state.narrow=table;
 }else if(!remove)table[index]=text; // Original replacement does not free old string.
 else {
  io.release(table[index]);auto cursor=index;while(table[cursor]){table[cursor]=table[cursor+1];++cursor;}
  if(auto** shrunk=static_cast<char**>(io.resize(table,static_cast<std::size_t>(cursor)*sizeof(char*))))state.narrow=shrunk;
 }
 if(update_os){const auto size=std::strlen(text);auto* name=static_cast<char*>(io.allocate(size+2));if(name){original_memory4::copy_bytes(name,text,static_cast<U32>(size+1));name[equal-text]=0;io.set_variable(name,remove?nullptr:name+(equal-text)+1);io.release(name);}}
 return 0;
}
char* get_environment(State& state,const char* name,Services& io){
 if(!state.enabled)return nullptr;
 if(!state.narrow && (!state.wide || convert_wide_environment(state,io)!=0 || !state.narrow))return nullptr;
 if(!name)return nullptr;const auto n=std::strlen(name);
 for(auto** p=state.narrow;*p;++p)if(n<std::strlen(*p) && (*p)[n]=='=' && original_runtime8::mbsnbicoll(*p,name,n,compare,&io)==0)return *p+n+1;
 return nullptr;
}
}
