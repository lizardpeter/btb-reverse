#include "btb/original_system2.hpp"
#include "btb/original_runtime6.hpp"
#include "btb/original_memory4.hpp"
#include <bit>
#include <cstring>
#include <stdexcept>
namespace btb::original_system2 {
namespace {
I32 wrap(U32 x){return std::bit_cast<I32>(x);}
bool digit(char x){return x>='0' && x<='9';}
void copy_three(std::array<char,64>& out,const char* s){bool ended=false;for(unsigned i=0;i<3;++i){if(!ended){out[i]=*s;if(*s)++s;else ended=true;}else out[i]=0;}out[3]=0;}
}
void initialize_timezone(ZoneState& s,original_environment4::State& environment,original_environment4::Services& heap,original_locale7::State& locale,original_locale7::NlsServices& nls,Services& io){
 auto& t=s.time;t.system_rules=false;t.start.year=-1;t.end.year=-1;
 const auto* tz=original_environment4::get_environment(environment,"TZ",heap);
 if(!tz){
  const U32 result=io.zone_information(s.info);t.standard_date=s.info.standard_date;t.daylight_date=s.info.daylight_date;
  if(result==0xffffffffU)return;
  t.timezone=wrap(static_cast<U32>(s.info.bias)*60U);t.system_rules=true;
  if(s.info.standard_date.month)t.timezone=wrap(static_cast<U32>(t.timezone)+static_cast<U32>(s.info.standard_bias)*60U);
  if(!s.info.daylight_date.month || !s.info.daylight_bias){t.daylight=0;t.daylight_bias=0;}
  else {t.daylight=1;t.daylight_bias=wrap((static_cast<U32>(s.info.daylight_bias)-static_cast<U32>(s.info.standard_bias))*60U);}
  bool used=false;U32 converted=io.convert_name(s.codepage,0x220,s.info.standard_name.data(),s.standard.data(),63,used);
  if(!converted || used)s.standard[0]=0;else s.standard[63]=0;
  converted=io.convert_name(s.codepage,0x220,s.info.daylight_name.data(),s.daylight.data(),63,used);
  if(!converted || used)s.daylight[0]=0;else s.daylight[63]=0;return;
 }
 if(!*tz || (s.saved && !std::strcmp(tz,s.saved)))return;
 heap.release(s.saved);s.saved=static_cast<char*>(heap.allocate(std::strlen(tz)+1));if(!s.saved)return;
 original_memory4::copy_bytes(s.saved,tz,static_cast<U32>(std::strlen(tz)+1));copy_three(s.standard,tz);
 if(std::strlen(tz)<3)throw std::invalid_argument("original TZ expects three prefix bytes");
 const char* cursor=tz+3;const bool negative=*cursor=='-';if(negative)++cursor;
 U32 zone=static_cast<U32>(original_runtime6::parse_integer(locale,cursor,nls))*3600U;
 while(*cursor=='+' || digit(*cursor))++cursor;
 if(*cursor==':'){++cursor;zone+=static_cast<U32>(original_runtime6::parse_integer(locale,cursor,nls))*60U;while(digit(*cursor))++cursor;
  if(*cursor==':'){++cursor;zone+=static_cast<U32>(original_runtime6::parse_integer(locale,cursor,nls));while(digit(*cursor))++cursor;}
 }
 t.timezone=wrap(negative?0U-zone:zone);t.daylight=static_cast<std::int8_t>(*cursor);
 if(t.daylight)copy_three(s.daylight,cursor);else s.daylight[0]=0;
}
I32 change_directory(original_runtime16::IoState& state,const char* path,std::span<const original_runtime8::ErrorPair> errors,original_locale7::State& locale,original_locale7::NlsServices& nls,Services& io){
 if(io.set_directory(path)){
  std::array<char,265> current{};const U32 length=io.get_directory(261,current.data());
  if(length){if(length>=261)throw std::out_of_range("original GetCurrentDirectory buffer");
   if(((current[0]!='\\') && (current[0]!='/')) || current[0]!=current[1]){
    const U32 upper=original_locale7::upper_multibyte_character(locale,static_cast<unsigned char>(current[0]),nls);const char name[]{'=',static_cast<char>(upper),':',0};if(!io.set_variable(name,current.data()))goto failure;
   }return 0;
  }
 }
 failure:state.dos_error=io.last_error();state.error=original_runtime8::map_win_error(std::bit_cast<I32>(state.dos_error),errors);return -1;
}
void TimeLink::initialize_timezone(original_runtime15::TimeState& state){if(&state!=&zone_.time)throw std::logic_error("timezone link state mismatch");original_system2::initialize_timezone(zone_,environment_,environment_services_,locale_,nls_,services_);}
}
