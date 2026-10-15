#include "btb/original_display3.hpp"
#include <algorithm>
#include <bit>
#include <limits>
namespace btb::original_display3 {
namespace {
constexpr I32 fail=std::bit_cast<I32>(0x80004005U);
constexpr Guid draw_iid{0x15e65ec0,0x11d23b9c,0x60002fb9,0x5bea9797};
I32 subtract(I32 a,I32 b){return std::bit_cast<I32>(static_cast<U32>(a)-static_cast<U32>(b));}
}
I32 fullscreen(State& state,Handle window,U32 width,U32 height,U32 bits,Services& io){
 io.reset(state);
 if(create_draw(io,nullptr,state.draw,draw_iid,0)<0)return fail;
 if(io.cooperative(state.draw,window,0x11)<0)return fail;
 if(io.display_mode(state.draw,width,height,bits,0,0)<0)return fail;
 Descriptor desc;desc.flags=0x21;desc.caps=0x2218;desc.backbuffers=1;
 if(io.surface(state.draw,desc,state.primary)<0)return fail;
 if(io.attached(state.primary,{4,0,0,0},state.back)<0)return fail;
 io.add_ref(state.back);state.extra_back_reference=true;state.window=window;state.windowed=0;io.update_destination(state);return 0;
}
I32 windowed(State& state,Handle window,U32 width,U32 height,Services& io){
 io.reset(state);
 if(create_draw(io,nullptr,state.draw,draw_iid,0)<0)return fail;
 if(io.cooperative(state.draw,window,8)<0)return fail;
 io.window_long(window,-16,(io.window_long(window,-16)&0x7f39ffffU)|0xc60000U);
 Rect area{0,0,std::bit_cast<I32>(width),std::bit_cast<I32>(height)};
 const U32 ex=io.window_long(window,-20);const bool has_menu=io.menu(window)!=0;const U32 style=io.window_long(window,-16);
 io.adjust_window(area,style,has_menu,ex);
 io.position(window,0,0,0,subtract(area.right,area.left),subtract(area.bottom,area.top),0x16);
 io.position(window,static_cast<Handle>(static_cast<std::intptr_t>(-2)),0,0,0,0,0x13);
 const Rect work=io.work_area();area=io.window_rectangle(window);
 io.position(window,0,std::max(area.left,work.left),std::max(area.top,work.top),0,0,0x15);
 Descriptor desc;desc.flags=1;desc.caps=0x200;
 if(io.surface(state.draw,desc,state.primary)<0)return fail;
 desc.flags=7;desc.height=height;desc.width=width;desc.caps=0x2040;
 if(io.surface(state.draw,desc,state.back)<0)return fail;
 Handle clipper{};if(io.create_clipper(state.draw,0,clipper,0)<0)return fail;
 if(io.clipper_window(clipper,0,window)<0){io.release(clipper);return fail;}
 if(io.set_clipper(state.primary,clipper)<0){io.release(clipper);return fail;}
 io.release(clipper);state.window=window;state.windowed=1;io.update_destination(state);return 0;
}
I32 create_window(Handle instance,I32 show,Handle& window,Handle& accelerator,Rect& rectangle,Handle procedure,WindowServices& io){
 if(io.find("WINNAME","Bob the Builder"))return 0;
 WindowClass cls;cls.procedure=procedure;cls.instance=instance;
 cls.icon=io.icon(instance,101);cls.small_icon=io.icon(instance,101);cls.cursor=io.cursor(0,32512);
 if(!io.register_class(cls))return fail;
 const Handle accel=io.accelerators(instance,103);const I32 border_x=io.metric(32),border_y=io.metric(33),menu=io.metric(15),caption=io.metric(4);
 const I32 width=std::bit_cast<I32>(static_cast<U32>(border_x)*2+640),height=std::bit_cast<I32>(static_cast<U32>(border_y)*2+480+static_cast<U32>(menu)+static_cast<U32>(caption));
 const Handle created=io.create(0,"WINNAME","Bob The Builder",0xce0000,std::numeric_limits<I32>::min(),std::numeric_limits<I32>::min(),width,height,0,0,instance,0);
 if(!created)return fail;
 io.show(created,show);io.update(created);rectangle=io.rectangle(created);window=created;accelerator=accel;return 0;
}
}
