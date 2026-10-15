#pragma once
#include "btb/original_printer7.hpp"
namespace btb::original_printer2 {
using namespace original_printer7;
class GeometryServices {
public:
 virtual ~GeometryServices()=default;
 virtual I32 device_cap(Handle,I32)=0;
 virtual Handle window_dc(Handle)=0;
};
// 00405490 / 004056C0. Arithmetic assumes valid retail dimensions/DPI.
void compute_target(Printer&,Handle dc,Handle owner,GeometryServices&);
U32 map_clipped(const Rect& target,Rect& destination,Rect& source);
// Installs the recovered virtual slot on callers of original_printer7 render/print.
class LinkedServices:public Services,public GeometryServices {
public:
 explicit LinkedServices(Handle owner):owner_(owner){}
 void compute_target(Printer&,Handle) final;
private:Handle owner_;
};
}
