#include "btb/original_source9.hpp"
#include <array>
#include <cstring>
namespace btb::original_source9 {
bool verify_9_original_source_functions() noexcept {
 using namespace btb::retail_source_batch;
 if(!park_no_sample_overlap(0,0,10,10,{10,0,20,10})||
     park_no_sample_overlap(0,0,10,10,{5,5,15,15})||
     park_no_sample_overlap(0,0,10,10,{1,1,3,3})||
     park_no_sample_overlap(3,3,1,1,{0,0,10,10}))return false;
 ParkSortContext ctx{};
 ParkObject a{},b{};
 a.object_code=b.object_code=-1;
 if(compare_park_draw_objects(a,b,ctx)!=0)return false;
 a.object_code=7;
 if(compare_park_draw_objects(a,b,ctx)!=-1)return false;
 b.object_code=7;a.sort_layer=5;b.sort_layer=6;
 if(compare_park_draw_objects(a,b,ctx)!=-1)return false;
 b.sort_layer=5;a.bottom=100;b.bottom=110;
 a.record_index=20;b.record_index=101;
 if(compare_park_draw_objects(a,b,ctx)!=1)return false;
 a.object_code=b.object_code=300;a.bound_category=b.bound_category=2;
 if(compare_park_draw_objects(a,b,ctx)!=-1)return false;
 a.object_code=b.object_code=401;a.bound_category=b.bound_category=0;
 a.bound_variant=b.bound_variant=0;a.top=10;b.top=20;
 ctx.category_offsets[0]={0,10,0,30};
 if(compare_park_draw_objects(a,b,ctx)!=-1)return false;
 SpudBobAnimation bob{};bob.countdown=5;
 update_spud_bob(bob);
 if(bob.countdown!=4||bob.frame!=0||bob.dirty!=0)return false;
 bob.countdown=1;update_spud_bob(bob);
 if(bob.frame!=0||bob.countdown!=4||bob.dirty!=1)return false;
 bob.moving=1;bob.countdown=1;bob.frame=9;
 update_spud_bob(bob);
 if(bob.frame!=0||bob.countdown!=8)return false;
 bob.moving=0;bob.countdown=1;bob.action=0;
 bob.reverse=1;bob.direction=2;bob.frame=1;
 update_spud_bob(bob);
 if(bob.frame!=6||bob.countdown!=4)return false;
 bob.countdown=1;bob.reverse=0;bob.action=1;bob.frame=14;
 bob.action_cycles=2;bob.remaining=3;
 update_spud_bob(bob);
 if(bob.action_cycles!=1||bob.frame!=9||bob.remaining!=3)return false;
 bob.frame=14;bob.countdown=1;update_spud_bob(bob);
 if(bob.action!=0||bob.frame!=0||bob.remaining!=2)return false;
 PilchardAnimation pil{};pil.remaining=0;pil.frame=8;pil.countdown=1;
 update_pilchard(pil);
 if(pil.state!=1||pil.frame!=0||pil.countdown!=10)return false;
 pil.remaining=3;pil.bob_frame=1;pil.state=0;
 pil.countdown=1;pil.frame=15;pil.speed=16;update_pilchard(pil);
 if(pil.frame!=16||pil.y_anchor!=0||pil.countdown!=8)return false;
 pil.frame=23;pil.countdown=1;update_pilchard(pil);
 if(pil.frame!=0||pil.anim_complete!=1)return false;
 pil.state=2;pil.frame=35;pil.countdown=1;update_pilchard(pil);
 if(pil.frame!=0||pil.anim_complete!=1||pil.x_anchor!=9)return false;
 pil.state=4;pil.frame=15;pil.countdown=1;update_pilchard(pil);
 if(pil.frame!=9||pil.anim_complete!=0||pil.dirty!=1)return false;
 char str[9];std::memset(str,'?',sizeof(str));
 if(copy_string_bounded(str,"a",5)!=str || str[0]!='a'||
    str[1]!=0||str[4]!=0||str[5]!='?')return false;
 if(copy_string_bounded(str,"zzzzz",0)!=str || str[0]!='a')
   return false;
 if(copy_string_bounded(str,"abcdef",3)!=str ||
    str[0]!='a'||str[2]!='c')return false;
 if(compare_string_bounded("ab","ac",2)!=-1||
    compare_string_bounded("ab","a",2)!=1||
    compare_string_bounded("ab","ab",2)!=0||
    compare_string_bounded("a","b",0)!=0)return false;
 const char hi[]{static_cast<char>(0xff),0};
 const char lo[]{static_cast<char>(0x80),0};
 if(compare_string_bounded(hi,lo,2)!=1)return false;
 if(divide_unsigned_x86(13,0,5,0)!=2 ||
    divide_unsigned_x86(0,1,3,0)!=0x55555555ULL ||
    divide_unsigned_x86(7,4,0,1)!=4 ||
    divide_unsigned_x86(0xffffffffU,0xffffffffU,
                        0xffffffffU,0xffffffffU)!=1)return false;
 const std::array<std::byte,12> bytes{
   std::byte{0x78},std::byte{0x56},std::byte{0x34},std::byte{0x12},
   std::byte{0x08},std::byte{0x07},std::byte{0x06},std::byte{0x05},
   std::byte{0x04},std::byte{0x03},std::byte{0x02},std::byte{0x01}};
 LegacyReadCursor cursor{bytes.data(),bytes.size(),0};
 U32 v32{};U64 v64{};
 if(!read_original_u32(cursor,v32)||v32!=0x12345678U||cursor.offset!=4||
    !read_original_u64(cursor,v64)||v64!=0x0102030405060708ULL||
    cursor.offset!=12||read_original_u32(cursor,v32)||
    cursor.offset!=12)return false;
 // Differential against previously recovered typed Ghidra models, with
 // deterministic varied paths. This is not retail-exe differential testing.
 U32 state=0x734187afU;
 const auto next=[&](){state=state*1664525U+1013904223U;return state;};
 for(int i=0;i<1000;++i){
   const I32 x=static_cast<I32>(next()%60U);
   const I32 y=static_cast<I32>(next()%60U);
   const I32 w=1+static_cast<I32>(next()%60U);
   const I32 hh=1+static_cast<I32>(next()%60U);
   const Rect32 other{static_cast<I32>(next()%100U),
       static_cast<I32>(next()%100U),
       static_cast<I32>(next()%100U)+100,
       static_cast<I32>(next()%100U)+100};
   if(park_no_sample_overlap(x,y,w,hh,other)!=
       park_rectangles_do_not_overlap_by_samples(x,y,w,hh,other))
       return false;
   auto bb=SpudBobAnimation{};
   bb.frame=static_cast<I32>(next()%30U)-3;
   bb.countdown=static_cast<I32>(next()%10U);
   bb.moving=static_cast<I32>(next()%2U);
   bb.action=static_cast<I32>(next()%2U);
   bb.reverse=static_cast<I32>(next()%2U);
   bb.direction=static_cast<I32>(next()%4U);
   bb.game_tick_override=static_cast<I32>(next()%2U);
   bb.action_cycles=3;
   auto expected=bb;
   update_spud_bob(bb);advance_spud_bob_animation(expected);
   if(bb.frame!=expected.frame||bb.countdown!=expected.countdown||
      bb.moving!=expected.moving||bb.dirty!=expected.dirty||
      bb.action!=expected.action||bb.remaining!=expected.remaining)
     return false;
   PilchardAnimation pp{};
   pp.frame=static_cast<I32>(next()%40U);
   pp.countdown=static_cast<I32>(next()%8U);
   pp.remaining=static_cast<I32>(next()%4U);
   pp.bob_frame=static_cast<I32>(next()%20U);
   pp.speed=2+static_cast<I32>(next()%20U);
   pp.state=static_cast<I32>(next()%5U);
   auto pp_expected=pp;
   update_pilchard(pp);advance_pilchard_animation(pp_expected);
   if(pp.state!=pp_expected.state||pp.frame!=pp_expected.frame||
      pp.countdown!=pp_expected.countdown||
      pp.anim_complete!=pp_expected.anim_complete||
      pp.x_anchor!=pp_expected.x_anchor||
      pp.y_anchor!=pp_expected.y_anchor||pp.dirty!=pp_expected.dirty)
     return false;
 }
 return true;
}
}
