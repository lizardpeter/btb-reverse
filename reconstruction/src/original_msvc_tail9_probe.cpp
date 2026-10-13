#include "btb/original_msvc_tail9.hpp"
namespace btb::original_msvc_tail9 {
bool verify_9_original_msvc_tail() noexcept {
 std::vector<I32> events{};
 const Callback report=[&](I32 x){events.push_back(x);};
 const auto hook=[&](){events.push_back(42);};
 runtime_processor_warning(1,0,report,hook);
 if(events!=std::vector<I32>{252,42,255})return false;
 events.clear();runtime_processor_warning(0,0,report,hook);
 if(!events.empty())return false;
 runtime_amessage_exit(1,18,hook,report,report);
 if(events!=std::vector<I32>{42,18,255})return false;
 events.clear();runtime_fatal_process(0,19,hook,report,report);
 if(events!=std::vector<I32>{19,255})return false;
 if(!check_processor_feature([]()->std::optional<bool>{return true;},
                             [](){return false;}))return false;
 if(!check_processor_feature([]()->std::optional<bool>{return std::nullopt;},
                             [](){return true;}))return false;
 if(check_processor_feature([]()->std::optional<bool>{return false;},
                             [](){return true;}))return false;
 U32 available=0xe0,full=31;U8 slot=0x10,indicator=0;
 events.clear();
 release_heap_slot(available,slot,indicator,full,report);
 if(available!=0xf0||slot!=0||indicator!=0xf1||full!=32||
    events!=std::vector<I32>{16})return false;
 events.clear();release_heap_slot(available,slot,indicator,full,report);
 if(!events.empty())return false;
 events.clear();call_setting_frame(0x100,report,hook,report);
 if(events!=std::vector<I32>{256,42,2})return false;
 events.clear();call_setting_frame(9,report,hook,report);
 if(events!=std::vector<I32>{9,42,9})return false;
 const auto two=assign_float_result(true,
   [](){return std::array<U32,2>{0x10203040U,0xabcdef01U};},
   [](){return 0U;});
 if(two!=std::array<U32,2>{0x10203040U,0xabcdef01U})return false;
 const auto one=assign_float_result(false,
   [](){return std::array<U32,2>{0U,0U};},
   [](){return 0x1234U;});
 if(one!=std::array<U32,2>{0x1234U,0U})return false;
 const std::vector<std::string> env{"HOME=/tmp","PATH=C:/Bob"};
 const auto dup=duplicate_environment(&env,
   [](const std::string& str)->std::optional<std::string>{return str;});
 if(!dup||*dup!=env||dup->data()==env.data())return false;
 if(duplicate_environment(nullptr,{}).has_value())return false;
 if(duplicate_environment(&env,[](const std::string&)
   ->std::optional<std::string>{return std::nullopt;}).has_value())return false;
 char buffer[8]{};I32 err=0;
 if(convert_wide_character(buffer,static_cast<U16>('B'),false,err,{})!=1||
    buffer[0]!='B'||err!=0)return false;
 const auto conv=[](U16 c)->std::optional<std::string>{
   return c==0x20acU?std::optional<std::string>{"EUR"}:std::nullopt;
 };
 if(convert_wide_character(buffer,0x20acU,true,err,conv)!=3||
    buffer[0]!='E'||buffer[1]!='U'||buffer[2]!='R')return false;
 err=0;
 if(convert_wide_character(buffer,0xffffU,true,err,conv)!=-1||
    err!=42)return false;
 err=0;
 if(convert_wide_character(nullptr,0xffffU,true,err,conv)!=0||err!=0)
   return false;
 // Generated native C++ must match source-level original models;
 // this still does not establish original retail executable parity.
 for(int x=0;x<32;++x){
   const I32 status=x-5;
   std::vector<I32> expected,actual;
   const auto cb1=[&](I32 y){expected.push_back(y);};
   const auto cb2=[&](I32 y){actual.push_back(y);};
   btb::runtime_batch_32::call_setting_frame(
       status,cb1,[&](){expected.push_back(42);},cb1);
   original_msvc_tail9::call_setting_frame(
       status,cb2,[&](){actual.push_back(42);},cb2);
   if(expected!=actual)return false;
 }
 return true;
}
}
