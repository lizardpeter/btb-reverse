#pragma once
#include <cstdint>
namespace btb::original_message1 {
using Handle=std::uintptr_t;using U32=std::uint32_t;using I32=std::int32_t;
struct State {Handle message{},active{},popup{};};
class Services {
public:
 virtual ~Services()=default;
 virtual Handle load_library(const char*)=0;
 virtual Handle get_proc(Handle,const char*)=0;
 virtual Handle active_window(Handle procedure)=0;
 virtual Handle last_popup(Handle procedure,Handle window)=0;
 virtual I32 message_box(Handle procedure,Handle owner,const char*,const char*,U32)=0;
};
I32 show_message(State&,const char*,const char*,U32,Services&); // 00438272
}
