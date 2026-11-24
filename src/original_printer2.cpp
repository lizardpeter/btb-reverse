#include "btb/original_printer2.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <limits>
namespace btb::original_printer2 {
namespace {
I32 sub(I32 a,I32 b){return std::bit_cast<I32>(static_cast<U32>(a)-static_cast<U32>(b));}
I32 add(I32 a,I32 b){return std::bit_cast<I32>(static_cast<U32>(a)+static_cast<U32>(b));}
I32 trunc_low(long double n){
 // Original __ftol returns signed i64 then caller consumes low 32 bits.
 if(!std::isfinite(n) || n>=9223372036854775808.0L || n< -9223372036854775808.0L)return 0;
 return std::bit_cast<I32>(static_cast<U32>(static_cast<std::int64_t>(n)));
}
I32 dword(const std::byte* p){I32 n;std::memcpy(&n,p,4);return n;}
}
void compute_target(Printer& p,Handle dc,Handle owner,GeometryServices& io){
 const I32 horizontal=io.device_cap(dc,8),vertical=io.device_cap(dc,10),dx=io.device_cap(dc,88),dy=io.device_cap(dc,90);
 if(p.scale_mode==0){const I32 h=dword(p.dib+8);const I32 absolute=h<0?sub(0,h):h;p.target={0,0,horizontal,trunc_low(static_cast<long double>(absolute)*(static_cast<long double>(horizontal)/dword(p.dib+4))*(static_cast<long double>(dy)/dx)+0.5L)};}
 else if(p.scale_mode==1){const I32 h=dword(p.dib+8);const I32 absolute=h<0?sub(0,h):h;p.target={0,0,trunc_low((static_cast<long double>(vertical)/absolute)*(static_cast<long double>(dx)/dy)*dword(p.dib+4)+0.5L),vertical};}
 else if(p.scale_mode==2){
  const Handle screen=io.window_dc(owner);const float sx=static_cast<float>(io.device_cap(screen,88)),sy=static_cast<float>(io.device_cap(screen,90));
  const float px=static_cast<float>(io.device_cap(dc,88));const auto py=static_cast<long double>(io.device_cap(dc,90));
  const float x=static_cast<float>(sx>px?static_cast<long double>(sx)/px:static_cast<long double>(px)/sx);
  const float y=static_cast<float>((sy>py?static_cast<long double>(sy)/py:py/sy)*1.5L);
  const auto w=io.device_cap(dc,8),h=io.device_cap(dc,10);
  // Truncate the negative product before signed halving, then subtract its magnitude.
  p.target.top=add(h/2,trunc_low(static_cast<long double>(y)* -480.0L)/2);
  p.target.left=add(w/2,trunc_low(static_cast<long double>(x)* -640.0L)/2);
  p.target.right=trunc_low(static_cast<long double>(x)*640.0L);p.target.bottom=trunc_low(static_cast<long double>(y)*480.0L);
  // The original leaks the window DC.
 }
}
U32 map_clipped(const Rect& target,Rect& dest,Rect& src){
 dest={std::max(dest.left,target.left),std::max(dest.top,target.top),std::min(dest.right,target.right),std::min(dest.bottom,target.bottom)};
 if(dest.left>=dest.right || dest.top>=dest.bottom){dest={};return 0;}
 const auto rx=static_cast<long double>(sub(target.right,target.left))/sub(src.right,src.left);
 // Unlike X, Y ratio is stored to an actual double local before reloading.
 const double ry=static_cast<double>(static_cast<long double>(sub(target.bottom,target.top))/sub(src.bottom,src.top));
 src.left=trunc_low(static_cast<long double>(sub(dest.left,target.left))/rx+0.5L);
 src.right=add(src.left,trunc_low(static_cast<long double>(sub(dest.right,dest.left))/rx+0.5L));
 src.top=trunc_low(static_cast<long double>(sub(dest.top,target.top))/ry+0.5L);
 src.bottom=add(src.top,trunc_low(static_cast<long double>(sub(dest.bottom,dest.top))/ry+0.5L));
 return (static_cast<U32>(src.bottom)&0xffffff00U)|1U;
}
void LinkedServices::compute_target(Printer& p,Handle dc){original_printer2::compute_target(p,dc,owner_,*this);}
}
