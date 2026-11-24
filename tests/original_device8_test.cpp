#include "btb/original_display3.hpp"
#include "btb/original_game_device2.hpp"
#include <algorithm>
#include <bit>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
namespace d=btb::original_display3;namespace g=btb::original_game_device2;namespace imports=btb::original_imports3;
using U32=std::uint32_t;using I32=std::int32_t;using Handle=std::uintptr_t;using Guid=imports::Guid;
constexpr U32 DATA=0x800000,DRAW=DATA+0x1000,PRIMARY=DATA+0x2000,BACK=DATA+0x3000,CLIP=DATA+0x4000;
static I32 si(U32 value){return std::bit_cast<I32>(value);}
static U32 crc(std::string_view s){U32 result=0xffffffff;for(unsigned char c:s){result^=c;for(int b=0;b<8;++b)result=(result>>1)^((result&1)?0xedb88320:0);}return result^0xffffffff;}
struct Input {std::istringstream in;explicit Input(const std::string& s):in(s){}U32 n(){std::string s;in>>s;assert(!s.empty());return static_cast<U32>(std::stoull(s,nullptr,16));}std::vector<U32> numbers(){auto count=n();std::vector<U32> v;while(count--)v.push_back(n());return v;}};
struct Device:d::Services,d::WindowServices,g::BitmapServices {
 U32 fail{},steps{},surface_count{},bitmap_count{},style{0x80c80000},width{},height{},exists{},window_failure{};d::Rect wr{-100,-100,600,500};std::vector<U32> trace;
 template<class... A>void event(A... a){(trace.push_back(static_cast<U32>(a)),...);}
 I32 status(){return ++steps==fail?si(0x88760001):1;}
 I32 direct_draw(const Guid* adapter,Handle& out,const Guid& iid,Handle outer) override{assert(!adapter);event(2,0,iid[0],iid[1],iid[2],iid[3],outer);auto hr=status();if(hr>=0)out=DRAW;return hr;}
 I32 direct_input(Handle instance,U32 version,const Guid& iid,Handle& out,Handle outer) override{event(21,instance,version,iid[0],iid[1],iid[2],iid[3],outer);auto hr=status();if(hr>=0)out=DATA+0x6000;return hr;}
 I32 direct_sound(const Guid* id,Handle& out,Handle outer) override{assert(!id);event(22,0,outer);auto hr=status();if(hr>=0)out=DATA+0x6100;return hr;}
 void reset(d::State& s) override{event(1);s={};}
 I32 cooperative(Handle draw,Handle window,U32 flags) override{event(3,draw,window,flags);return status();}
 I32 display_mode(Handle draw,U32 w,U32 h,U32 bits,U32 refresh,U32 flags) override{event(4,draw,w,h,bits,refresh,flags);return status();}
 I32 surface(Handle draw,const d::Descriptor& desc,Handle& out) override{event(5,draw,desc.size,desc.flags,desc.height,desc.width,desc.backbuffers,desc.caps);auto hr=status();if(hr>=0)out=surface_count++?BACK:PRIMARY;return hr;}
 I32 attached(Handle primary,const std::array<U32,4>& caps,Handle& out) override{event(6,primary,caps[0],caps[1],caps[2],caps[3]);auto hr=status();if(hr>=0)out=BACK;return hr;}
 void add_ref(Handle back) override{event(7,back);}
 I32 create_clipper(Handle draw,U32 flags,Handle& out,Handle outer) override{event(8,draw,flags,outer);auto hr=status();if(hr>=0)out=CLIP;return hr;}
 I32 clipper_window(Handle clip,U32 flags,Handle window) override{event(9,clip,flags,window);return status();}
 I32 set_clipper(Handle primary,Handle clip) override{event(10,primary,clip);return status();}
 void release(Handle object) override{event(11,object);}
 U32 window_long(Handle h,I32 index) override{event(13,h,index);return index==-16?style:0x200;}
 void window_long(Handle h,I32 index,U32 value) override{event(14,h,index,value);style=value;}
 Handle menu(Handle h) override{event(15,h);return 0x77;}
 void adjust_window(d::Rect& r,U32 st,bool menu,U32 ex) override{event(16,r.left,r.top,r.right,r.bottom,st,menu,ex);r.left-=7;r.top-=30;r.right+=7;r.bottom+=7;}
 void position(Handle h,Handle after,I32 x,I32 y,I32 w,I32 height,U32 flags) override{event(17,h,after,x,y,w,height,flags);const auto oldw=wr.right-wr.left,oldh=wr.bottom-wr.top;if(!(flags&2)){wr.left=x;wr.top=y;}wr.right=wr.left+((flags&1)?oldw:w);wr.bottom=wr.top+((flags&1)?oldh:height);}
 d::Rect work_area() override{event(18,0x30,0,0);return {10,20,1920,1080};}
 d::Rect window_rectangle(Handle h) override{event(19,h);return wr;}
 void update_destination(d::State& state) override{event(12);state.destination=state.windowed?d::Rect{wr.left+7,wr.top+30,wr.left+7+static_cast<I32>(width),wr.top+30+static_cast<I32>(height)}:d::Rect{0,0,static_cast<I32>(width),static_cast<I32>(height)};}
 Handle find(const char* name,const char* title) override{event(100,crc(name),crc(title));return exists;}
 Handle icon(Handle h,U32 id) override{event(101,h,id);return 0x400+id;}
 Handle cursor(Handle h,U32 id) override{event(102,h,id);return 0x500+id;}
 U32 register_class(const d::WindowClass& c) override{event(104,c.size,c.style,c.procedure,0,0,c.instance,c.icon,c.cursor,c.brush,c.menu,crc(c.name),c.small_icon);return window_failure==1?0:1;}
 Handle accelerators(Handle h,U32 id) override{event(105,h,id);return 0x501;}
 I32 metric(I32 n) override{event(106,n);return n==32||n==33?8:n==15?19:25;}
 Handle create(U32 ex,const char* cls,const char* title,U32 st,I32 x,I32 y,I32 w,I32 height,Handle parent,Handle menu,Handle instance,Handle parameter) override{event(107,ex,crc(cls),crc(title),st,x,y,w,height,parent,menu,instance,parameter);return window_failure==2?0:0x1234;}
 void show(Handle h,I32 n) override{event(108,h,n);}
 void update(Handle h) override{event(109,h);}
 d::Rect rectangle(Handle h) override{return window_rectangle(h);}
 Handle load(Handle draw,const char* path,U32 w,U32 h) override{event(30,draw,crc(path),w,h);return ++bitmap_count==fail?0:0xa000+crc(path)%4096;}
 void register_surface(U32 slot,Handle& surface,const char* path) override{event(31,slot,surface,crc(path));}
 void mark_keyed(U32 slot) override{event(32,slot);}
 void colour_key(Handle surface,U32 colour) override{event(33,surface,colour);}
};
static bool float_close(U32 a,U32 b){if(a==b)return true;const auto x=std::bit_cast<float>(a),y=std::bit_cast<float>(b);if(!std::isfinite(x)||!std::isfinite(y))return false;const auto ordered=[](U32 x){return x&0x80000000U?~x:x|0x80000000U;};const U32 aa=ordered(a),bb=ordered(b),ulp=aa>bb?aa-bb:bb-aa;return ulp<=2 || (std::abs(x)<1 && std::abs(y)<1 && std::abs(x-y)<=0.00002f);}
int main(int argc,char** argv){assert(argc==2);std::ifstream file(argv[1]);assert(file);std::string line;std::size_t count=0,movement_different_bits=0;
 while(std::getline(file,line)){
  Input input(line);std::string kind;input.in>>kind;Device io;std::vector<U32> expected,got;U32 result{},wanted{};
  if(kind=="FULL"||kind=="WIN"){
   io.fail=input.n();const auto window=input.n();io.width=input.n();io.height=input.n();const auto bits=input.n();wanted=input.n();for(int i=0;i<10;++i)expected.push_back(input.n());const auto trace=input.numbers();d::State state;
   result=kind=="FULL"?d::fullscreen(state,window,io.width,io.height,bits,io):d::windowed(state,window,io.width,io.height,io);
   got={static_cast<U32>(state.draw),static_cast<U32>(state.primary),static_cast<U32>(state.back),0,static_cast<U32>(state.window),static_cast<U32>(state.destination.left),static_cast<U32>(state.destination.top),static_cast<U32>(state.destination.right),static_cast<U32>(state.destination.bottom),state.windowed};
   assert(state.extra_back_reference==(kind=="FULL"&&result==0));if(trace!=io.trace)got.push_back(0xffffffff);
   if(trace!=io.trace){std::cerr<<"Trace mismatch "<<count<<' '<<kind<<"\ngot ";for(auto x:io.trace)std::cerr<<std::hex<<x<<' ';std::cerr<<"\nexpected ";for(auto x:trace)std::cerr<<std::hex<<x<<' ';std::cerr<<'\n';}
  }else if(kind=="WINDOW"){
   io.exists=input.n();io.window_failure=input.n();wanted=input.n();for(int i=0;i<6;++i)expected.push_back(input.n());const auto trace=input.numbers();Handle window=0xaaaa,accel=0xbbbb;d::Rect rect{77,88,99,111};result=d::create_window(0x99,5,window,accel,rect,0x4020d0,io);got={static_cast<U32>(window),static_cast<U32>(accel),static_cast<U32>(rect.left),static_cast<U32>(rect.top),static_cast<U32>(rect.right),static_cast<U32>(rect.bottom)};if(trace!=io.trace)got.push_back(0xffffffff);
  }else if(kind=="DRAW"||kind=="INPUT"||kind=="SOUND"){
   io.fail=input.n();Guid id;for(auto& x:id)x=input.n();wanted=input.n();expected.push_back(input.n());const auto trace=input.numbers();Handle out=0xeeee;
   result=kind=="DRAW"?imports::create_draw(io,nullptr,out,id,0):kind=="INPUT"?imports::create_input(io,0x99,0x800,id,out,0):imports::create_sound(io,nullptr,out,0);got.push_back(static_cast<U32>(out));if(trace!=io.trace)got.push_back(0xffffffff);
  }else if(kind=="FIRE"){
   io.fail=input.n();for(int i=0;i<14;++i)expected.push_back(input.n());const auto trace=input.numbers();g::Fireworks state;g::load_fireworks(state,DRAW,io);for(auto x:state.surfaces)got.push_back(static_cast<U32>(x));for(auto x:state.aliases)got.push_back(static_cast<U32>(x));if(trace!=io.trace)got.push_back(0xffffffff);
  }else if(kind=="MOVE"){
   g::Movement state;state.obstruction_a=si(input.n());state.obstruction_b=si(input.n());state.moving=input.n()!=0;const I32 speed=si(input.n());const I32 nx=si(input.n()),ny=si(input.n());std::array<U32,8> raw{};for(auto& x:raw)x=input.n();for(int i=0;i<8;++i)expected.push_back(input.n());g::Actor actor;std::memcpy(&actor,raw.data(),32);const std::array<std::pair<I32,I32>,1> node{{{nx,ny}}};g::move_pilchard(state,0,actor,speed,node);std::memcpy(raw.data(),&actor,32);got.assign(raw.begin(),raw.end());for(int i=0;i<2;++i)if(got[i]!=expected[i]&&float_close(got[i],expected[i])){got[i]=expected[i];++movement_different_bits;}
  }else {std::cerr<<"Unknown "<<kind<<'\n';return 1;}
  if(got!=expected || result!=wanted){std::cerr<<"Mismatch "<<count<<' '<<kind<<" result "<<std::hex<<result<<'/'<<wanted<<"\ngot ";for(auto x:got)std::cerr<<x<<' ';std::cerr<<"\nexpected ";for(auto x:expected)std::cerr<<x<<' ';std::cerr<<'\n';return 1;}++count;
 }
 assert(count==3580);std::cout<<count<<" original-x86 comparisons passed; "<<movement_different_bits<<" movement components used declared float tolerance\n";
}
