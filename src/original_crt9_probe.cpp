#include "btb/original_crt9.hpp"
#include <array>
#include <string>
#include <vector>
namespace btb::original_crt9 {
bool verify_9_original_crt_functions() {
 std::vector<I32> events{};
 original_crt9::runtime_processor_warning(
     1,0,[&](I32 v){events.push_back(v);},
     [&]{events.push_back(77);});
 if(events!=std::vector<I32>{252,77,255})return false;
 events.clear();
 original_crt9::runtime_processor_warning(
     0,0,[&](I32 v){events.push_back(v);},{});
 if(!events.empty())return false;
 original_crt9::runtime_amessage_exit(
     1,12,[&]{events.push_back(91);},
     [&](I32 v){events.push_back(v);},
     [&](I32 v){events.push_back(v);});
 if(events!=std::vector<I32>{91,12,255})return false;
 events.clear();
 original_crt9::runtime_fatal_process(
     0,42,{},[&](I32 v){events.push_back(v);},
     [&](I32 v){events.push_back(v);});
 if(events!=std::vector<I32>{42,255})return false;
 std::string calls;
 if(!original_crt9::check_processor_feature(
     [&]()->std::optional<bool>{calls+='K';return std::nullopt;},
     [&]{calls+='F';return true;})||calls!="KF")return false;
 calls.clear();
 if(original_crt9::check_processor_feature(
     [&]()->std::optional<bool>{calls+='K';return false;},
     [&]{calls+='F';return true;})||calls!="K")return false;
 U32 available=239,full_pages=31;U8 slot=1,indicator=0;events.clear();
 original_crt9::release_heap_slot(
     available,slot,indicator,full_pages,
     [&](I32 value){events.push_back(value);});
 if(available!=240||slot||indicator!=0xf1||full_pages!=32||
    events!=std::vector<I32>{16})return false;
 slot=2;
 original_crt9::release_heap_slot(
     available,slot,indicator,full_pages,{});
 if(available!=242||full_pages!=32)return false;
 events.clear();
 original_crt9::call_setting_frame(
     256,[&](I32 value){events.push_back(value);},
     [&]{events.push_back(99);},
     [&](I32 value){events.push_back(value);});
 if(events!=std::vector<I32>{256,99,2})return false;
 const auto pair=original_crt9::assign_float_result(
     true,[]{return std::array<U32,2>{11,22};},{});
 const auto single=original_crt9::assign_float_result(
     false,{},[]{return 33U;});
 if(pair!=std::array<U32,2>{11,22}||
    single!=std::array<U32,2>{33,0})return false;
 std::vector<std::string> environment{"PATH=C:/", "SAVE=1"};
 const auto cloned=original_crt9::duplicate_environment(
     &environment,[](const std::string& value)
          ->std::optional<std::string>{return value;});
 if(!cloned||*cloned!=environment)return false;
 if(original_crt9::duplicate_environment(nullptr,{}))return false;
 if(original_crt9::duplicate_environment(
     &environment,[](const std::string&)
          ->std::optional<std::string>{return std::nullopt;}))return false;
 char target[8]{};I32 error=0;
 if(original_crt9::convert_wide_character(
     target,'A',false,error,{})!=1||target[0]!='A')return false;
 if(original_crt9::convert_wide_character(
     target,0x20ac,true,error,[](U16)
          ->std::optional<std::string>{
             return std::string("\xe2\x82\xac");
          })!=3)return false;
 if(original_crt9::convert_wide_character(
     target,0x1234,true,error,[](U16)
          ->std::optional<std::string>{return std::nullopt;})!=-1||
    error!=42)return false;
 if(original_crt9::convert_wide_character(
     nullptr,33,false,error,{})!=0)return false;
 return true;
}
}
