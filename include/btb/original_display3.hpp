#pragma once
#include "btb/original_imports3.hpp"
namespace btb::original_display3 {
using namespace original_imports3;
struct Rect {I32 left{},top{},right{},bottom{};bool operator==(const Rect&)const=default;};
struct Descriptor {U32 size{124},flags{},height{},width{},backbuffers{},caps{};};
struct State {Handle draw{},primary{},back{},window{};Rect destination{};U32 windowed{};bool extra_back_reference{};};
class Services:public original_imports3::Services {
public:
 virtual void reset(State&)=0;
 virtual I32 cooperative(Handle draw,Handle window,U32 flags)=0;
 virtual I32 display_mode(Handle draw,U32 width,U32 height,U32 bits,U32 refresh,U32 flags)=0;
 virtual I32 surface(Handle draw,const Descriptor&,Handle&)=0;
 virtual I32 attached(Handle surface,const std::array<U32,4>& caps,Handle&)=0;
 virtual void add_ref(Handle)=0;
 virtual I32 create_clipper(Handle draw,U32 flags,Handle& result,Handle outer)=0;
 virtual I32 clipper_window(Handle clipper,U32 flags,Handle window)=0;
 virtual I32 set_clipper(Handle surface,Handle clipper)=0;
 virtual void release(Handle)=0;
 virtual U32 window_long(Handle,I32 index)=0;
 virtual void window_long(Handle,I32 index,U32 value)=0;
 virtual Handle menu(Handle)=0;
 virtual void adjust_window(Rect&,U32 style,bool has_menu,U32 ex_style)=0;
 virtual void position(Handle window,Handle after,I32 x,I32 y,I32 width,I32 height,U32 flags)=0;
 virtual Rect work_area()=0;
 virtual Rect window_rectangle(Handle)=0;
 virtual void update_destination(State&)=0;
};
I32 fullscreen(State&,Handle window,U32 width,U32 height,U32 bits,Services&); //00403080
I32 windowed(State&,Handle window,U32 width,U32 height,Services&); //004031E0
struct WindowClass {U32 size{48},style{3};Handle procedure{},instance{},icon{},small_icon{},cursor{};U32 brush{6},menu{102};const char* name{"WINNAME"};};
class WindowServices {
public:
 virtual ~WindowServices()=default;
 virtual Handle find(const char* class_name,const char* title)=0;
 virtual Handle icon(Handle instance,U32 id)=0;
 virtual Handle cursor(Handle instance,U32 id)=0;
 virtual U32 register_class(const WindowClass&)=0;
 virtual Handle accelerators(Handle instance,U32 id)=0;
 virtual I32 metric(I32)=0;
 virtual Handle create(U32 ex_style,const char* class_name,const char* title,U32 style,I32 x,I32 y,I32 width,I32 height,Handle parent,Handle menu,Handle instance,Handle parameter)=0;
 virtual void show(Handle,I32)=0;
 virtual void update(Handle)=0;
 virtual Rect rectangle(Handle)=0;
};
I32 create_window(Handle instance,I32 show,Handle& window,Handle& accelerator,Rect& rectangle,Handle procedure,WindowServices&); //00401E20
}
