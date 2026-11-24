#include "btb/original_runtime5.hpp"
#include <cstring>
namespace btb::original_runtime5 {
bool verify_5_original_runtime_functions() noexcept {
 if(retail_strlen("Bob the Builder")!=15||
    retail_strlen("")!=0)return false;
 if(retail_strcmp("A","B")!=-1||retail_strcmp("B","A")!=1||
    retail_strcmp("A","A")!=0)return false;
 const char* haystack="banana";
 if(retail_strstr(haystack,"ana")!=haystack+1||
    retail_strstr(haystack,"zz")!=nullptr||
    retail_strstr(haystack,"")!=haystack)return false;
 char number[48]{};
 retail_write_radix(0xffffffffU,number,10,true);
 if(std::strcmp(number,"-1")!=0)return false;
 retail_write_radix(26U,number,16,false);
 if(std::strcmp(number,"1a")!=0)return false;
 retail_write_radix(0U,number,2,false);
 if(std::strcmp(number,"0")!=0)return false;
 if(translate_x87_control(0)!=0x20000U||
    translate_x87_control(1U)!=0x20010U||
    translate_x87_control(0x400U)!=0x20100U)return false;
 return true;
}
}
