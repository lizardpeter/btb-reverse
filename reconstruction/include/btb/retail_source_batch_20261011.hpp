#pragma once
// Nine original PE32 functions reconstructed from Ghidra pseudocode.
// Retail EXE SHA256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Typed semantics, not claims of byte-identical original x86 output or ABI.
#include "btb/park_designer_data.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace btb::retail_source_batch {
using I32=std::int32_t;
using U32=std::uint32_t;
using U64=std::uint64_t;
using ParkObject=btb::park_designer::RetailObjectRecord32;

// 0040DD50, original predicate PointInsideRectExclusive at the nine
// source sample positions in each direction. Edge-touching is not overlap.
struct Rect32 { I32 left{},top{},right{},bottom{}; };
[[nodiscard]] constexpr bool point_inside_exclusive(I32 x,I32 y,Rect32 r) noexcept {
    return r.left<x&&x<r.right&&r.top<y&&y<r.bottom;
}
[[nodiscard]] constexpr std::array<std::pair<I32,I32>,9>
nine_sample_points(Rect32 r) noexcept {
    const I32 cx=r.left+(r.right-r.left)/2;
    const I32 cy=r.top+(r.bottom-r.top)/2;
    return {{{r.left,r.top},{r.right,r.top},{r.right,r.bottom},
             {r.left,r.bottom},{cx,cy},{cx,r.top},{r.right,cy},
             {cx,r.bottom},{r.left,cy}}};
}
[[nodiscard]] constexpr bool park_rectangles_do_not_overlap_by_samples(
    I32 x,I32 y,I32 width,I32 height,Rect32 other) noexcept {
    const Rect32 proposed{x,y,static_cast<I32>(x+width),static_cast<I32>(y+height)};
    for(const auto [px,py]:nine_sample_points(proposed))
        if(point_inside_exclusive(px,py,other))return false;
    for(const auto [px,py]:nine_sample_points(other))
        if(point_inside_exclusive(px,py,proposed))return false;
    return true;
}

// 0040D5B0: Preserve the unusual comparator ties in the Ghidra output.
// The original qsort does not impose a strict weak ordering on all records.
struct ParkSortContext {
    I32 selected_bandstand_bottom{}; // 0x004FCABC[0x00507E3C]
    std::array<Rect32,20> category_offsets{}; // table 0x00441B18..24
};
[[nodiscard]] constexpr int compare_park_objects_for_draw(
    const ParkObject& a,const ParkObject& b,const ParkSortContext& ctx) noexcept {
    if(a.object_code==-1&&b.object_code==-1)return 0;
    if(a.object_code==-1)return 1;
    if(b.object_code==-1)return -1;
    if(a.sort_layer<b.sort_layer)return -1;
    if(a.sort_layer>b.sort_layer)return 1;
    I32 ay=a.bottom;
    I32 by=b.bottom;
    if(a.object_code==300) {
        if(b.object_code==300)return a.bound_category<=b.bound_category?-1:1;
        if(a.bound_category>=1)ay=ctx.selected_bandstand_bottom;
    }
    if(b.object_code==300) {
        if(a.object_code==300)return b.bound_category<=a.bound_category?-1:1;
        if(b.bound_category>=1)by=ctx.selected_bandstand_bottom;
    }
    if(a.object_code<100&&a.bound_category==2&&a.bound_variant==2)ay-=10;
    if(b.object_code<100&&b.bound_category==2&&b.bound_variant==2)by-=10;
    if(a.object_code>399&&b.object_code>399) {
        const auto depth=[&](const ParkObject& p) constexpr -> I32 {
            const I32 index=p.bound_category*5+p.bound_variant;
            // Unlike retail out-of-bounds pointer math, invalid indices
            // retain the original record's bottom as a safe host fallback.
            if(index<0||index>=static_cast<I32>(ctx.category_offsets.size()))
                return p.bottom;
            const auto r=ctx.category_offsets[static_cast<std::size_t>(index)];
            return (r.top+r.bottom)/2+p.top;
        };
        ay=depth(a);by=depth(b);
    }
    if(a.record_index<100&&a.object_code==7)ay+=83;
    if(b.record_index<100&&b.object_code==7)by+=83;
    return by<=ay?1:-1;
}

// 00422400: globals 005109EC/F0,00514510..28,00510A00,0044DE0C.
struct SpudBobAnimation {
    I32 frame{},countdown{},moving{},game_tick_override{},dirty{};
    I32 action{},reverse{},direction{},action_cycles{},remaining{};
};
constexpr void advance_spud_bob_animation(SpudBobAnimation& s) noexcept {
    if(s.moving!=0) {
        if(s.countdown-1>0&&s.game_tick_override==0){--s.countdown;return;}
        s.countdown=8;++s.frame;s.dirty=1;
        if(s.frame>9||s.frame<0)s.frame=0;
        return;
    }
    if(s.countdown-1>0&&s.game_tick_override==0){--s.countdown;return;}
    const I32 next=s.frame+1;
    s.dirty=1;
    if(s.action==0) {
        if(s.reverse==0) {
            if(s.direction==0||s.direction==2){s.countdown=4;return;}
            if(next>9){s.frame=0;s.countdown=4;return;}
        } else {
            if(s.direction!=0) {
                if(s.direction==2) {
                    --s.frame;
                    if(s.frame<1||s.frame>=7)s.frame=6;
                    s.countdown=4;
                    return;
                }
                s.frame=next>24||next<=12?13:next;
                s.countdown=4;
                return;
            }
            if(next>6){s.frame=0;s.countdown=4;return;}
        }
        if(next<0){s.frame=0;s.countdown=4;return;}
    } else if(next>14) {
        --s.action_cycles;
        if(s.action_cycles>0){s.frame=9;s.countdown=4;return;}
        s.frame=0;s.action=0;--s.remaining;s.countdown=4;
        return;
    }
    s.countdown=4;s.frame=next;
}

// 004225E0: Pilchard activity animation globals 005107A4/AC/B0/EC,
// 005144C8/D4/CC,005109C8 and 00445EF8/FC.
struct PilchardAnimation {
    I32 state{},frame{},countdown{},speed{},remaining{},bob_frame{};
    I32 x_anchor{-1},y_anchor{-1},dirty{},anim_complete{};
};
constexpr void advance_pilchard_animation(PilchardAnimation& s) noexcept {
    s.x_anchor=-1;s.y_anchor=-1;s.dirty=0;
    if(s.remaining<1||s.bob_frame<0) {
        s.state=1;--s.countdown;
        if(s.countdown<1){s.countdown=10;++s.frame;if(s.frame>8)s.frame=0;}
        return;
    }
    const I32 style=s.state==4?3:s.state;
    if(s.state==0)s.y_anchor=0;
    else if(s.state==2)s.x_anchor=0;
    if(s.countdown-1>0){--s.countdown;return;}
    ++s.frame;
    if(style==0) {
        if(s.frame<16)s.frame=16;
        s.y_anchor=s.frame-16;
        if(s.frame>=24){s.frame=0;s.anim_complete=1;s.countdown=s.speed/2;return;}
    } else if(style==2) {
        if(s.frame<27)s.frame=27;
        s.x_anchor=s.frame-27;
        if(s.frame>=36){s.frame=0;s.anim_complete=1;s.countdown=s.speed/2;return;}
    } else if(style==1||style==3) {
        s.dirty=1;
        if(s.frame>15){s.anim_complete=0;s.frame=9;s.countdown=s.speed/2;return;}
        if(s.frame<9)s.frame=9;
    }
    s.countdown=s.speed/2;
}

// 004302A0: MSVC 1998 _strncpy. Preserves null padding and count semantics
// without original unsafe speculative aligned loads past the source buffer.
inline char* retail_strncpy(char* dest,const char* src,std::size_t count) noexcept {
    char* const base=dest;
    while(count!=0) {
        --count;const char ch=*src;*dest++=ch;
        if(ch==0){while(count--!=0)*dest++='\0';return base;}
        ++src;
    }
    return base;
}
// 00430E50: unsigned-byte comparison normalized to -1,0,1.
[[nodiscard]] constexpr int retail_strncmp(
    const char* a,const char* b,std::size_t count) noexcept {
    while(count--!=0) {
        const auto x=static_cast<unsigned char>(*a++);
        const auto y=static_cast<unsigned char>(*b++);
        if(x<y)return -1;
        if(x>y)return 1;
        if(x==0)return 0;
    }
    return 0;
}
// 00430E90: __aulldiv Microsoft x86 unsigned 64-bit quotient.
[[nodiscard]] constexpr U64 retail_aulldiv(
    U32 dividend_low,U32 dividend_high,U32 divisor_low,U32 divisor_high) noexcept {
    const U64 n=(static_cast<U64>(dividend_high)<<32)|dividend_low;
    const U64 d=(static_cast<U64>(divisor_high)<<32)|divisor_low;
    return n/d; // original CPU DIV likewise requires a nonzero divisor
}

// 00433150 and 0043315D: original x86 pointer-at-pointer 4/8 byte reads.
// Advance source cursor and decode little-endian values with checked bounds.
struct LegacyReadCursor {
    const std::byte* data{};std::size_t size{};std::size_t offset{};
};
[[nodiscard]] constexpr bool retail_read_u32(LegacyReadCursor& c,U32& out) noexcept {
    if(c.offset>c.size||c.size-c.offset<4)return false;
    out=0;
    for(unsigned i=0;i<4;++i)
        out|=static_cast<U32>(std::to_integer<unsigned>(c.data[c.offset+i]))<<(8*i);
    c.offset+=4;return true;
}
[[nodiscard]] constexpr bool retail_read_u64(LegacyReadCursor& c,U64& out) noexcept {
    if(c.offset>c.size||c.size-c.offset<8)return false;
    out=0;
    for(unsigned i=0;i<8;++i)
        out|=static_cast<U64>(std::to_integer<unsigned>(c.data[c.offset+i]))<<(8*i);
    c.offset+=8;return true;
}
} // namespace btb::retail_source_batch
