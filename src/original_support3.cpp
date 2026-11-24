#include "btb/original_support3.hpp"
#include "btb/original_runtime6.hpp"
#include "btb/original_runtime5.hpp"
#include <bit>
#include <cstring>
#include <string>
#include <stdexcept>
namespace btb::original_support3 {
U32 parse_integer(original_locale7::State& locale,const char* text,const char** end,I32 base,U32 flags,I32& error,original_locale7::NlsServices& io){
 const auto* p=text;auto classify=[&](unsigned char c,U32 m){return original_locale7::classify(locale,c,m,io);};
 while(classify(static_cast<unsigned char>(*p),8))++p;
 if(*p=='-'){flags|=2;++p;}else if(*p=='+')++p;
 if(base<0 || base==1 || base>36){if(end)*end=text;return 0;}
 if(!base)base=*p!='0'?10:(p[1]=='x'||p[1]=='X')?16:8;
 if(base==16 && p[0]=='0' && (p[1]=='x'||p[1]=='X'))p+=2;
 const U32 radix=static_cast<U32>(base),maximum=0xffffffffU/radix,remainder=0xffffffffU%radix;U32 value=0;
 for(;;){const auto c=static_cast<unsigned char>(*p);U32 digit;
  if(classify(c,4))digit=static_cast<U32>(static_cast<std::int8_t>(c)-'0');
  else {if(!classify(c,0x103))break;digit=original_runtime6::upper_character(locale,static_cast<U32>(static_cast<I32>(static_cast<std::int8_t>(c))),io)-55;}
  if(digit>=radix)break;
  if(value<maximum || (value==maximum && digit<=remainder)){value=value*radix+digit;flags|=8;}else flags|=12;++p;
 }
 if(!(flags&8)){value=0;if(end)p=text;}
 else if((flags&4) || (!(flags&1) && value>((flags&2)?0x80000000U:0x7fffffffU))){error=34;value=(flags&1)?0xffffffffU:0x7fffffffU+((flags&2)?1:0);}
 if(end)*end=p;return flags&2?0U-value:value;
}
I32 choose_heap(original_locale7::State& locale,I32& error,original_locale7::NlsServices& nls,Services& io){
 U32 major{},platform{};if(io.os_version(major,platform) && platform==2 && major>=5)return 1;
 std::array<char,4240> environment{};const U32 length=io.heap_environment(environment.data(),static_cast<U32>(environment.size()));
 if(length){if(length>=environment.size())throw std::out_of_range("original heap selection environment");for(auto& c:environment){if(!c)break;if(c>='a' && c<='z')c-=32;}
  const char* selected=nullptr;
  if(!std::strncmp("__GLOBAL_HEAP_SELECTED",environment.data(),22))selected=environment.data();
  else {std::array<char,260> module{};const U32 n=io.module_name(module.data(),260);if(!n || n>=260)throw std::out_of_range("original heap selection module name");for(auto& c:module){if(!c)break;if(c>='a' && c<='z')c-=32;}selected=original_runtime5::retail_strstr(environment.data(),module.data());}
  if(selected){const auto* comma=original_runtime16::find_character(const_cast<char*>(selected),',');if(comma){std::string value(comma+1);const auto semicolon=value.find(';');if(semicolon!=std::string::npos)value.resize(semicolon);const U32 heap=parse_integer(locale,value.c_str(),nullptr,10,0,error,nls);if(heap>=1 && heap<=3)return static_cast<I32>(heap);}}
 }
 return (original_runtime16::read_linker_version(io.module_image())&255)<6?2:3;
}
void report_error(U32 code,U32 mode,bool console,original_message1::State& messages,Services& io){
 const char* text=nullptr;for(const auto& row:error_messages)if(row.code==code){text=row.text;break;}if(!text)return;
 if(mode==1 || (!mode && console)){const auto handle=io.standard_error();io.write_error(handle,text,static_cast<U32>(std::strlen(text)));return;}
 if(code==252)return;
 std::array<char,260> buffer{};const U32 n=io.module_name(buffer.data(),260);if(n>=260)throw std::out_of_range("original runtime program name");std::string program=n?std::string(buffer.data()):"<program name unknown>";
 if(program.size()+1>60){program=program.substr(program.size()-59);program.replace(0,3,"...");}
 std::string message="Runtime Error!\n\nProgram: ";message+=program;message+="\n\n";message+=text;
 original_message1::show_message(messages,message.c_str(),"Microsoft Visual C++ Runtime Library",0x12010,io);
}
}
