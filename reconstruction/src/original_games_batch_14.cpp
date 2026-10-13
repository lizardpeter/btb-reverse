#include "btb/original_games_batch_14.hpp"
#include <bit>
#include <cstdint>
#include <limits>
namespace btb::original_games14 {
bool suppress_fullscreen_key(
    I32 hook,U32 message,I32 key,bool alt,bool ctrl) noexcept {
    if(hook!=0||!(message==0x100||message==0x101||
                  message==0x104||message==0x105))return false;
    if(alt&&(key==0x0d||key==0x73||key==9||key==0x1b))
        return true;
    if(ctrl&&key==0x1b)return true;
    if(alt&&ctrl&&key==0x2e)return true;
    return false;
}
I32 load_global_data_tables(Callback loader,void* ctx){
    for(I32 i=0;i<11;++i)if(loader)loader(i,ctx);
    return 1;
}
void update_dino_print(DinoPrintState& s,PrintSink sink,void* ctx) {
    const bool inside=s.x>=299&&s.x<=346&&s.y>=422&&s.y<=464;
    if(sink)sink(PrintEvent::Base,false,ctx);
    if(!s.click){
        if(!inside){s.hover_latched=false;return;}
        if(sink)sink(PrintEvent::Highlight,s.alternate,ctx);
        if(!s.hover_latched){
            if(sink)sink(PrintEvent::HoverVoice,false,ctx);
            s.hover_latched=true;
        }
    } else if(inside && sink) {
        sink(PrintEvent::StopAll,false,ctx);
        sink(PrintEvent::PrintImage,false,ctx);
        sink(PrintEvent::Print,false,ctx);
    }
}
Rect pond_source_rect(I32 segment,I32 leading,I32 stride,I32 height) noexcept {
    const auto index=segment>0?segment+leading:segment;
    const auto left=stride*index,right=stride*(index+1);
    return {left,0,right>2053?2053:right,height};
}
void finish_park_drag(ParkDrag& s,void(*reset)(void*),void* ctx) {
    if(s.mode==2&&s.selected>=0&&
       static_cast<std::size_t>(s.selected)<s.visibility.size()){
        const auto id=static_cast<std::size_t>(s.selected);
        if(s.selected<201||s.selected>299 ||
           id>=s.valid_object_flags.size()||s.valid_object_flags[id]!=1)
            s.visibility[id]=1;
    }
    s.mode=0;
    if(reset)reset(ctx);
}
namespace {
[[nodiscard]] bool grand_opening_fits(
    std::span<const I32> grid,I32 row,I32 column,I32 length) noexcept {
    if(length<=0)return true;
    if(row<0||column<0||column>=24)return false;
    const auto index=static_cast<std::int64_t>(row)*24+column;
    if(index<0||index>=static_cast<std::int64_t>(grid.size()))return false;
    for(I32 i=0;i<length;++i){
        if(column+i>=24||index+i>=static_cast<std::int64_t>(grid.size())||
           grid[static_cast<std::size_t>(index+i)]!=-1)return false;
    }
    return true;
}
[[nodiscard]] I32 abs32(I32 a,I32 b) noexcept {
    const auto diff=static_cast<U32>(a)-static_cast<U32>(b);
    const U32 mask=(diff&0x80000000U)?0xffffffffU:0;
    return std::bit_cast<I32>((diff^mask)-mask);
}
}
bool place_grand_opening(
    std::span<I32> grid,std::span<const I32> lengths,
    std::span<I32> pairs,I32 row,I32 col,I32 type,Callback sound,void* ctx) {
    if(type<0||static_cast<std::size_t>(type)>=lengths.size() ||
       static_cast<std::size_t>(type/2)>=pairs.size())return false;
    const auto length=lengths[static_cast<std::size_t>(type)];
    if(!grand_opening_fits(grid,row,col,length))return false;
    const auto index=static_cast<std::size_t>(row*24+col);
    grid[index]=type;
    for(I32 i=1;i<length;++i)grid[index+static_cast<std::size_t>(i)]=10;
    if(sound)sound(type-row*10,ctx);
    auto& pair=pairs[static_cast<std::size_t>(type/2)];
    if(pair==0)pair=type%2+1;
    return true;
}
I32 maybe_play_spud_voice(
    I32 cooldown,I32 last,Random rand,SoundActive active,
    PlayVoice play,void* ctx) {
    if(cooldown>=1||!rand)return cooldown;
    const U32 first=rand(ctx);
    if(first%3000!=0)return static_cast<I32>(first/3000U);
    for(I32 id=0x2d2;id<0x2d6;++id)
        if(active&&active(id,ctx))return 1;
    I32 selected=0x2d2+static_cast<I32>(rand(ctx)%4U);
    // Original repeats until different. Guard pathological fake host RNG.
    for(unsigned tries=0;selected==last&&tries<128;++tries)
        selected=0x2d2+static_cast<I32>(rand(ctx)%4U);
    if(selected==last)return 0;
    return play?play(selected,50,1,ctx):0;
}
bool move_squirrel_towards(
    I32& x,I32& y,I32 tx,I32 ty) noexcept {
    if(x<tx)x+=2;else if(x>tx)x-=2;
    if(y<ty)y+=2;else if(y>ty)y-=2;
    if(abs32(x,tx)<4)x=tx;
    if(abs32(y,ty)<4)y=ty;
    const bool arrived=abs32(x,tx)<4&&abs32(y,ty)<4;
    if(arrived){x=tx;y=ty;}
    return arrived;
}
void update_walkthrough(
    I32 x,I32 y,bool sound_playing,I32 help_id,PlayVoice voice,
    MovieOp decode,U32 frame,U32 total,MovieOp restart,void* ctx) {
    if(x>49&&x<365&&y>140&&y<350&&!sound_playing&&voice)
        (void)voice(help_id,90,1,ctx);
    if(decode)decode(ctx);
    if(total<=frame&&restart)restart(ctx);
}
I32 draw_generic_ui(
    bool play_again,UiBlit blit,void* ctx,std::size_t limit){
    if(!blit)return 1;
    if(play_again){
        (void)blit(0,false,ctx);
        (void)blit(1,true,ctx);
        return 1;
    }
    std::size_t attempts=0;
    for(;;){
        const U32 hr=blit(2,false,ctx);
        if(hr!=static_cast<U32>(btb::display::kDdErrWasStillDrawing))
            return 1;
        if(limit!=0&&++attempts>=limit)return 1;
    }
}
std::size_t load_video_sequence(
    std::span<I32> dest,ReadI32 reader,void* ctx) {
    std::size_t count=0;
    for(;count<dest.size()&&count<10;++count){
        I32 value{};
        if(!reader||!reader(&value,ctx))break;
        dest[count]=value;
    }
    return count;
}
std::size_t load_herding_coordinates(
    std::span<std::array<I32,2>> dest,ReadPair reader,void* ctx) {
    if(dest.empty())return 0;
    std::size_t count=0;
    for(;count+1<dest.size();++count){
        I32 x=0,y=0;
        if(!reader||!reader(x,y,ctx))break;
        dest[count]={x,y};
    }
    dest[count]={-1,-1};
    return count;
}
bool load_options(std::span<I32> out,ReadRecord read,void* ctx) {
    return read&&read(out,ctx);
}
bool load_play_again(std::span<I32> out,ReadRecord read,void* ctx) {
    return read&&read(out,ctx);
}
namespace {
struct Probe {I32 calls{};I32 first{};I32 last{};I32 played{};I32 ended{};U32 random_value{};};
void loader(I32 id,void* p){
    auto& s=*static_cast<Probe*>(p);
    if(s.calls++==0)s.first=id;
    s.last=id;
}
void report_print(PrintEvent event,bool,void* p){
    auto& s=*static_cast<Probe*>(p);
    ++s.calls;
    s.last=static_cast<I32>(event);
}
void reset(void* p){++static_cast<Probe*>(p)->ended;}
U32 random(void* p){return static_cast<Probe*>(p)->random_value;}
bool sound_active(I32,void*){return false;}
I32 voice(I32 sound,U32,I32,void* p){
    auto& s=*static_cast<Probe*>(p);s.played=sound;
    return 1;
}
void movie(void* p){++static_cast<Probe*>(p)->calls;}
U32 blit(I32 layer,bool,void* p){
    auto& s=*static_cast<Probe*>(p);
    ++s.calls;s.last=layer;
    return 0;
}
bool read_i32(I32* out,void* p){
    auto& s=*static_cast<Probe*>(p);
    if(s.calls==4)return false;
    *out=100+s.calls++;
    return true;
}
bool read_pair(I32& x,I32& y,void* p){
    auto& s=*static_cast<Probe*>(p);
    if(s.calls==2)return false;
    x=s.calls;y=s.calls+10;++s.calls;
    return true;
}
bool read_record(std::span<I32> record,void* p){
    auto& s=*static_cast<Probe*>(p);
    for(auto& x:record)x=++s.calls;
    return true;
}
}
bool verify_14_original_game_systems() noexcept {
    if(!suppress_fullscreen_key(0,0x104,13,true,false) ||
       suppress_fullscreen_key(1,0x104,13,true,false) ||
       !suppress_fullscreen_key(0,0x100,0x1b,false,true))return false;
    Probe p{};
    if(load_global_data_tables(loader,&p)!=1 ||
       p.calls!=11||p.first!=0||p.last!=10)return false;
    p={};
    DinoPrintState print{300,430,false,false,false};
    update_dino_print(print,report_print,&p);
    if(!print.hover_latched||p.calls!=3||p.last!=2)return false;
    print.click=true;p.calls=0;
    update_dino_print(print,report_print,&p);
    if(p.calls!=4||p.last!=5)return false;
    const auto pond=pond_source_rect(2,1,700,18);
    if(pond.left!=2100||pond.right!=2053||pond.bottom!=18)return false;
    std::array<I32,300> visible{};
    std::array<I32,300> valid{};
    ParkDrag drag{2,202,visible,valid};
    p={};
    finish_park_drag(drag,reset,&p);
    if(visible[202]!=1||drag.mode!=0||p.ended!=1)return false;
    std::array<I32,48> grid{};
    grid.fill(-1);
    const std::array<I32,3> lengths{1,2,3};
    std::array<I32,2> paired{};
    p={};
    if(!place_grand_opening(grid,lengths,paired,1,20,2,loader,&p) ||
       grid[44]!=2||grid[45]!=10||grid[46]!=10 ||
       paired[1]!=1||p.first!=-8)return false;
    p={};p.random_value=0;
    if(maybe_play_spud_voice(0,-1,random,sound_active,voice,&p)!=1 ||
       p.played!=0x2d2)return false;
    I32 x=3,y=3;
    if(!move_squirrel_towards(x,y,4,5) || x!=4||y!=5)return false;
    p={};
    update_walkthrough(100,200,false,0x88,voice,movie,4,4,movie,&p);
    if(p.played!=0x88||p.calls!=2)return false;
    p={};
    if(draw_generic_ui(true,blit,&p)!=1||p.calls!=2||p.last!=1)
        return false;
    p={};
    std::array<I32,10> videos{};
    if(load_video_sequence(videos,read_i32,&p)!=4 ||
       videos[0]!=100||videos[3]!=103)return false;
    p={};
    std::array<std::array<I32,2>,5> coords{};
    if(load_herding_coordinates(coords,read_pair,&p)!=2 ||
       coords[0][0]!=0||coords[1][1]!=11||
       coords[2]!=std::array<I32,2>{-1,-1})return false;
    p={};
    std::array<I32,3> options{},play_again{};
    if(!load_options(options,read_record,&p)||
       !load_play_again(play_again,read_record,&p)||
       options[0]!=1||play_again[2]!=6)return false;
    return true;
}
}
