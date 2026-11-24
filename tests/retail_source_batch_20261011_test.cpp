#include "btb/retail_source_batch_20261011.hpp"
#include <array>
#include <cassert>
#include <cstring>
using namespace btb::retail_source_batch;

int main() {
    // 0040DD50: strictly exclusive sample points in both directions.
    static_assert(park_rectangles_do_not_overlap_by_samples(0,0,10,10,{10,0,20,10}));
    static_assert(!park_rectangles_do_not_overlap_by_samples(0,0,10,10,{5,5,15,15}));
    static_assert(!park_rectangles_do_not_overlap_by_samples(0,0,10,10,{1,1,3,3}));
    static_assert(!park_rectangles_do_not_overlap_by_samples(3,3,1,1,{0,0,10,10}));
    static_assert(park_rectangles_do_not_overlap_by_samples(0,0,10,10,{20,20,30,30}));

    // 0040D5B0: original qsort ties, special code 300/7, depth offsets.
    ParkSortContext context{};
    ParkObject a{},b{};
    a.object_code=b.object_code=-1;
    assert(compare_park_objects_for_draw(a,b,context)==0);
    a.object_code=7;
    assert(compare_park_objects_for_draw(a,b,context)==-1);
    b.object_code=7;
    a.sort_layer=5;b.sort_layer=6;
    assert(compare_park_objects_for_draw(a,b,context)==-1);
    b.sort_layer=5;
    a.bottom=100;b.bottom=110;a.record_index=20;b.record_index=101;
    assert(compare_park_objects_for_draw(a,b,context)==1); // +83 fountain
    a.object_code=b.object_code=300;
    a.bound_category=b.bound_category=2;
    assert(compare_park_objects_for_draw(a,b,context)==-1);
    a.object_code=b.object_code=401;
    a.bound_category=b.bound_category=0;
    a.bound_variant=b.bound_variant=0;
    a.top=10;b.top=20;
    context.category_offsets[0]={0,10,0,30};
    assert(compare_park_objects_for_draw(a,b,context)==-1);

    // 00422400: frame countdown, default idle, cycling and action termination.
    SpudBobAnimation bob{};
    bob.countdown=5;
    advance_spud_bob_animation(bob);
    assert(bob.countdown==4&&bob.frame==0&&bob.dirty==0);
    bob.countdown=1;
    advance_spud_bob_animation(bob);
    assert(bob.frame==0&&bob.countdown==4&&bob.dirty==1);
    bob.moving=1;bob.countdown=1;bob.frame=9;
    advance_spud_bob_animation(bob);
    assert(bob.frame==0&&bob.countdown==8);
    bob.moving=0;bob.countdown=1;bob.action=0;bob.reverse=1;
    bob.direction=2;bob.frame=1;
    advance_spud_bob_animation(bob);
    assert(bob.frame==6&&bob.countdown==4);
    bob.countdown=1;bob.reverse=0;bob.action=1;bob.frame=14;
    bob.action_cycles=2;bob.remaining=3;
    advance_spud_bob_animation(bob);
    assert(bob.action_cycles==1&&bob.frame==9&&bob.remaining==3);
    bob.frame=14;bob.countdown=1;
    advance_spud_bob_animation(bob);
    assert(bob.action==0&&bob.frame==0&&bob.remaining==2);

    // 004225E0: idle cycle, animation source frames, direction, completion.
    PilchardAnimation pil{};
    pil.remaining=0;pil.frame=8;pil.countdown=1;
    advance_pilchard_animation(pil);
    assert(pil.state==1&&pil.frame==0&&pil.countdown==10);
    pil.remaining=3;pil.bob_frame=1;pil.state=0;
    pil.countdown=1;pil.frame=15;pil.speed=16;
    advance_pilchard_animation(pil);
    assert(pil.frame==16&&pil.y_anchor==0&&pil.countdown==8);
    pil.frame=23;pil.countdown=1;
    advance_pilchard_animation(pil);
    assert(pil.frame==0&&pil.anim_complete==1&&pil.countdown==8);
    pil.state=2;pil.frame=35;pil.countdown=1;
    advance_pilchard_animation(pil);
    assert(pil.frame==0&&pil.anim_complete==1&&pil.x_anchor==9);
    pil.state=4;pil.frame=15;pil.countdown=1;
    advance_pilchard_animation(pil);
    assert(pil.frame==9&&pil.anim_complete==0&&pil.dirty==1);

    // 004302A0: null padding, count=0, and bounded non-NUL copying.
    char out[9];std::memset(out,'?',sizeof(out));
    assert(retail_strncpy(out,"a",5)==out);
    assert(out[0]=='a'&&out[1]==0&&out[2]==0&&out[4]==0&&out[5]=='?');
    assert(retail_strncpy(out,"zzzzz",0)==out&&out[0]=='a');
    assert(retail_strncpy(out,"abcdef",3)==out&&out[2]=='c');
    // 00430E50: unsigned-byte lexicographic compare, limited length.
    static_assert(retail_strncmp("ab","ac",2)==-1);
    static_assert(retail_strncmp("ab","a",2)==1);
    static_assert(retail_strncmp("ab","ab",2)==0);
    static_assert(retail_strncmp("a","b",0)==0);
    constexpr char hi[]={static_cast<char>(0xff),0};
    constexpr char lo[]={static_cast<char>(0x80),0};
    assert(retail_strncmp(hi,lo,2)==1);

    // 00430E90: full 64-bit unsigned divide (MSVC helper).
    static_assert(retail_aulldiv(13,0,5,0)==2);
    static_assert(retail_aulldiv(0,1,3,0)==0x55555555ULL);
    static_assert(retail_aulldiv(7,4,0,1)==4);
    static_assert(retail_aulldiv(0xffffffffU,0xffffffffU,
                                0xffffffffU,0xffffffffU)==1);

    // 00433150 / 0043315D: exact little-endian extraction and cursor advance.
    const std::array<std::byte,12> data{
        std::byte{0x78},std::byte{0x56},std::byte{0x34},std::byte{0x12},
        std::byte{0x08},std::byte{0x07},std::byte{0x06},std::byte{0x05},
        std::byte{0x04},std::byte{0x03},std::byte{0x02},std::byte{0x01}
    };
    LegacyReadCursor cursor{data.data(),data.size(),0};
    U32 value32{};U64 value64{};
    assert(retail_read_u32(cursor,value32)&&value32==0x12345678U&&cursor.offset==4);
    assert(retail_read_u64(cursor,value64)&&
           value64==0x0102030405060708ULL&&cursor.offset==12);
    assert(!retail_read_u32(cursor,value32)&&cursor.offset==12);
}
