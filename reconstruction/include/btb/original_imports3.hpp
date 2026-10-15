#pragma once
#include <array>
#include <cstdint>
namespace btb::original_imports3 {
using U32=std::uint32_t;using I32=std::int32_t;using Handle=std::uintptr_t;using Guid=std::array<U32,4>;
class Services {
public:
 virtual ~Services()=default;
 virtual I32 direct_draw(const Guid* adapter,Handle& result,const Guid& iid,Handle outer)=0;
 virtual I32 direct_input(Handle instance,U32 version,const Guid& iid,Handle& result,Handle outer)=0;
 virtual I32 direct_sound(const Guid* device,Handle& result,Handle outer)=0;
};
I32 create_draw(Services&,const Guid*,Handle&,const Guid&,Handle); //0042EAD0
I32 create_input(Services&,Handle,U32,const Guid&,Handle&,Handle); //0042FB50
I32 create_sound(Services&,const Guid*,Handle&,Handle); //0042FB56 DSOUND ordinal 11
}
