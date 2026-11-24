#pragma once
#include "btb/original_runtime8.hpp"
namespace btb::original_environment4 {
using U32=std::uint32_t;using I32=std::int32_t;
struct State {U32 enabled{1},block_api{};char** narrow{};char** initial{};char16_t** wide{};original_runtime8::Locale locale{};};
class Services {
public:
 virtual ~Services()=default;
 virtual void* allocate(std::size_t)=0;
 virtual void* resize(void*,std::size_t)=0;
 virtual void release(void*)=0;
 // Raw-pointer bridge for 00439B24; provider must preserve fatal allocation policy.
 virtual char** duplicate(char**)=0;
 virtual I32 compare(std::string_view,std::string_view,std::size_t)=0; // CompareString result 1/2/3, zero failure
 virtual char16_t* environment_wide()=0;
 virtual char* environment_narrow()=0;
 virtual void free_environment_wide(char16_t*)=0;
 virtual void free_environment_narrow(char*)=0;
 virtual U32 convert(U32 codepage,const char16_t*,I32 count,char*,U32 capacity)=0;
 virtual void set_variable(const char*,const char*)=0;
};
// 00435C51, 0043931D, 00439945, 0043874A. Returned buffers retain original ownership.
char* get_environment_block(State&,Services&);
I32 convert_wide_environment(State&,Services&);
I32 put_environment(State&,char*,bool update_os,Services&);
char* get_environment(State&,const char*,Services&);
}
