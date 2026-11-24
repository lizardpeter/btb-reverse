#pragma once
#include "btb/original_locale7.hpp"
#include "btb/original_runtime16.hpp"
#include "btb/original_message1.hpp"
namespace btb::original_support3 {
using U32=std::uint32_t;using I32=std::int32_t;
U32 parse_integer(original_locale7::State&,const char*,const char** end,I32 base,U32 flags,I32& error,original_locale7::NlsServices&); //00437ECA
class Services:public original_message1::Services {
public:
 virtual bool os_version(U32& major,U32& platform)=0;
 virtual U32 heap_environment(char*,U32 capacity)=0;
 virtual U32 module_name(char*,U32 capacity)=0;
 virtual std::span<const std::byte> module_image()=0;
 virtual std::uintptr_t standard_error()=0;
 virtual void write_error(std::uintptr_t,const char*,U32)=0;
};
I32 choose_heap(original_locale7::State&,I32& error,original_locale7::NlsServices&,Services&); //00434257
void report_error(U32 code,U32 mode,bool console,original_message1::State&,Services&); //00436049
}

#include <array>
#include <cstdint>
namespace btb::original_support3 {
struct Message {std::uint32_t code;const char* text;};
inline constexpr std::array<Message,18> error_messages{{
 {2,"R6002\r\n- floating point not loaded\r\n"},
 {8,"R6008\r\n- not enough space for arguments\r\n"},
 {9,"R6009\r\n- not enough space for environment\r\n"},
 {10,"\r\nabnormal program termination\r\n"},
 {16,"R6016\r\n- not enough space for thread data\r\n"},
 {17,"R6017\r\n- unexpected multithread lock error\r\n"},
 {18,"R6018\r\n- unexpected heap error\r\n"},
 {19,"R6019\r\n- unable to open console device\r\n"},
 {24,"R6024\r\n- not enough space for _onexit/atexit table\r\n"},
 {25,"R6025\r\n- pure virtual function call\r\n"},
 {26,"R6026\r\n- not enough space for stdio initialization\r\n"},
 {27,"R6027\r\n- not enough space for lowio initialization\r\n"},
 {28,"R6028\r\n- unable to initialize heap\r\n"},
 {120,"DOMAIN error\r\n"},
 {121,"SING error\r\n"},
 {122,"TLOSS error\r\n"},
 {252,"\r\n"},
 {255,"runtime error "}
}};
}
