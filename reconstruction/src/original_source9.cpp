#include "btb/original_source9.hpp"
namespace btb::original_source9 {
bool park_no_sample_overlap(I32 x,I32 y,I32 width,I32 height,Rect32 other) noexcept {
 const Rect32 proposed{x,y,static_cast<I32>(x+width),static_cast<I32>(y+height)};
 const auto sample=[](Rect32 r) {
  const I32 cx=r.left+(r.right-r.left)/2,cy=r.top+(r.bottom-r.top)/2;
  return std::array<std::pair<I32,I32>,9>{{{r.left,r.top},{r.right,r.top},
   {r.right,r.bottom},{r.left,r.bottom},{cx,cy},{cx,r.top},
   {r.right,cy},{cx,r.bottom},{r.left,cy}}};
 };
 const auto inside=[](I32 px,I32 py,Rect32 rect){
  return rect.left<px&&px<rect.right&&rect.top<py&&py<rect.bottom;
 };
 for(const auto [px,py]:sample(proposed))
  if(inside(px,py,other))return false;
 for(const auto [px,py]:sample(other))
  if(inside(px,py,proposed))return false;
 return true;
}
int compare_park_draw_objects(
 const ParkObject& a,const ParkObject& b,const ParkSortContext& ctx) noexcept {
 if(a.object_code==-1&&b.object_code==-1)return 0;
 if(a.object_code==-1)return 1;
 if(b.object_code==-1)return -1;
 if(a.sort_layer<b.sort_layer)return -1;
 if(a.sort_layer>b.sort_layer)return 1;
 I32 ay=a.bottom,by=b.bottom;
 if(a.object_code==300){
  if(b.object_code==300)return a.bound_category<=b.bound_category?-1:1;
  if(a.bound_category>=1)ay=ctx.selected_bandstand_bottom;
 }
 if(b.object_code==300){
  if(a.object_code==300)return b.bound_category<=a.bound_category?-1:1;
  if(b.bound_category>=1)by=ctx.selected_bandstand_bottom;
 }
 if(a.object_code<100&&a.bound_category==2&&a.bound_variant==2)ay-=10;
 if(b.object_code<100&&b.bound_category==2&&b.bound_variant==2)by-=10;
 if(a.object_code>399&&b.object_code>399){
  const auto depth=[&](const ParkObject& p){
   const I32 ix=p.bound_category*5+p.bound_variant;
   if(ix<0||ix>=static_cast<I32>(ctx.category_offsets.size()))return p.bottom;
   const auto r=ctx.category_offsets[static_cast<std::size_t>(ix)];
   return (r.top+r.bottom)/2+p.top;
  };
  ay=depth(a);by=depth(b);
 }
 if(a.record_index<100&&a.object_code==7)ay+=83;
 if(b.record_index<100&&b.object_code==7)by+=83;
 return by<=ay?1:-1;
}
void update_spud_bob(SpudBobAnimation& s) noexcept {
 if(s.moving!=0){
  if(s.countdown-1>0&&s.game_tick_override==0){--s.countdown;return;}
  s.countdown=8;++s.frame;s.dirty=1;
  if(s.frame>9||s.frame<0)s.frame=0;
  return;
 }
 if(s.countdown-1>0&&s.game_tick_override==0){--s.countdown;return;}
 const I32 next=s.frame+1;s.dirty=1;
 if(s.action==0){
  if(s.reverse==0){
   if(s.direction==0||s.direction==2){s.countdown=4;return;}
   if(next>9){s.frame=0;s.countdown=4;return;}
  } else {
   if(s.direction!=0){
    if(s.direction==2){
     --s.frame;
     if(s.frame<1||s.frame>=7)s.frame=6;
     s.countdown=4;return;
    }
    s.frame=next>24||next<=12?13:next;
    s.countdown=4;return;
   }
   if(next>6){s.frame=0;s.countdown=4;return;}
  }
  if(next<0){s.frame=0;s.countdown=4;return;}
 }else if(next>14){
  --s.action_cycles;
  if(s.action_cycles>0){s.frame=9;s.countdown=4;return;}
  s.frame=0;s.action=0;--s.remaining;s.countdown=4;return;
 }
 s.countdown=4;s.frame=next;
}
void update_pilchard(PilchardAnimation& s) noexcept {
 s.x_anchor=-1;s.y_anchor=-1;s.dirty=0;
 if(s.remaining<1||s.bob_frame<0){
  s.state=1;--s.countdown;
  if(s.countdown<1){s.countdown=10;++s.frame;if(s.frame>8)s.frame=0;}
  return;
 }
 const I32 style=s.state==4?3:s.state;
 if(s.state==0)s.y_anchor=0;
 else if(s.state==2)s.x_anchor=0;
 if(s.countdown-1>0){--s.countdown;return;}
 ++s.frame;
 if(style==0){
  if(s.frame<16)s.frame=16;
  s.y_anchor=s.frame-16;
  if(s.frame>=24){s.frame=0;s.anim_complete=1;s.countdown=s.speed/2;return;}
 }else if(style==2){
  if(s.frame<27)s.frame=27;
  s.x_anchor=s.frame-27;
  if(s.frame>=36){s.frame=0;s.anim_complete=1;s.countdown=s.speed/2;return;}
 }else if(style==1||style==3){
  s.dirty=1;
  if(s.frame>15){s.anim_complete=0;s.frame=9;s.countdown=s.speed/2;return;}
  if(s.frame<9)s.frame=9;
 }
 s.countdown=s.speed/2;
}
char* copy_string_bounded(char* dst,const char* src,std::size_t n) noexcept {
 char* const original=dst;
 for(;n!=0;--n){
  const char c=*src;*dst++=c;
  if(c==0){while(--n!=0)*dst++=0;return original;}
  ++src;
 }
 return original;
}
int compare_string_bounded(const char* a,const char* b,std::size_t n) noexcept {
 for(;n>0;--n){
  const auto x=static_cast<unsigned char>(*a++);
  const auto y=static_cast<unsigned char>(*b++);
  if(x<y)return -1;
  if(x>y)return 1;
  if(x==0)return 0;
 }
 return 0;
}
U64 divide_unsigned_x86(U32 lo,U32 hi,U32 dlo,U32 dhi) noexcept {
 const U64 n=(static_cast<U64>(hi)<<32U)|lo;
 const U64 d=(static_cast<U64>(dhi)<<32U)|dlo;
 return n/d; // hardware original likewise traps when divisor is zero
}
bool read_original_u32(LegacyReadCursor& c,U32& out) noexcept {
 if(!c.data||c.offset>c.size||c.size-c.offset<4)return false;
 U32 v=0;for(unsigned i=0;i<4;++i)
  v|=static_cast<U32>(std::to_integer<unsigned>(c.data[c.offset+i]))<<(8*i);
 c.offset+=4;out=v;return true;
}
bool read_original_u64(LegacyReadCursor& c,U64& out) noexcept {
 if(!c.data||c.offset>c.size||c.size-c.offset<8)return false;
 U64 v=0;for(unsigned i=0;i<8;++i)
  v|=static_cast<U64>(std::to_integer<unsigned>(c.data[c.offset+i]))<<(8*i);
 c.offset+=8;out=v;return true;
}
} // namespace btb::original_source9
