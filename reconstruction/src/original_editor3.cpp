#include "btb/original_editor3.hpp"
#include <algorithm>
#include <stdexcept>
namespace btb::original_editor3 {
I32 hit_test_fireworks(FireworksState& s,std::span<const Region> regions,U32& seed,FireworksServices& io) {
    if(s.mode==1){s.x+=10;s.y+=10;}
    I32 bob=s.bob,wendy=s.wendy;
    auto random=[&](){return static_cast<I32>(original_crt17::retail_rand(seed));};
    auto sound=[&](I32 id){io.play_sound(id,50,2);bob=s.bob;wendy=s.wendy;};
    auto pick=[&](std::size_t i,const Region& r,bool pointer_first){
        if(pointer_first){s.x=r.left;s.y=r.top-25;}
        s.drag_type=s.grid[i];s.hot=s.drag_type;io.set_cursor(s.drag_type);
        if(i<12)s.bob=1;else s.wendy=1;
        s.drag_type=s.grid[i];s.grid[i]=-1;
        if(!pointer_first){s.x=r.left;s.y=r.top-25;}
    };
    for(std::size_t i=0;i<regions.size();++i) {
        const auto& r=regions[i];if(r.value==-1)continue;
        I32 dx=15,dy=50;
        if(i>=18){dx=0;dy=0;}
        else if(!bob && !wendy){dx=-16;dy=0;}
        if(!(r.left<s.x+dx && s.x+dx<r.right && r.top<s.y+dy && s.y+dy<r.bottom))continue;
        if(s.mode==13){s.hot=static_cast<I32>(i);return r.value;}
        if(i<12) {
            if(bob==1){s.hot=static_cast<I32>(i);return r.value;}
            if(wendy==1)sound(914+random()%2);
            else if(s.grid[i]!=-1){pick(i,r,false);return -1;}
            else sound(871+random()%3);
        } else if(i<18) {
            if(wendy==1){s.hot=static_cast<I32>(i);return r.value;}
            if(bob==1)sound(878+random()%2);
            else {
                sound(907+random()%3);
                if(s.grid[i]!=-1){pick(i,r,true);return -1;}
            }
        } else if(r.value<8) {
            if(!bob && !wendy) {
                s.bob_type=r.value;s.hot=static_cast<I32>(i);s.x=r.left-30;s.y=r.top-20;return r.value;
            }
        } else if(r.value<12 && !wendy && !bob) {
            s.wendy_type=r.value;s.hot=static_cast<I32>(i);s.x=r.left-30;s.y=r.top-20;return r.value;
        }
    }
    if(s.mode==1){s.x-=20;s.y-=20;}
    if(bob==1)io.play_sound(875+random()%3,50,2);
    else if(wendy==1)io.play_sound(911+random()%3,50,2);
    return -1;
}

namespace {
constexpr std::array<I32,147> table{0,16,33,0,0,0,12,12,9,0,0,12,3,3,3,5,12,7,5,0,0,12,0,12,0,0,0,12,5,5,5,5,5,5,12,12,0,0,0,0,0,0,0,12,9,3,3,12,3,3,3,0,12,7,0,0,0,12,0,12,3,3,3,12,5,5,5,0,0,0,12,12,0,0,0,1,1,1,13,13,8,1,1,13,2,2,2,4,13,6,4,1,1,13,1,13,1,1,1,13,4,4,4,4,4,4,13,13,1,1,1,1,1,1,13,13,8,2,2,13,2,2,2,1,13,6,1,1,1,13,1,13,2,2,2,13,4,4,4,1,1,1,13,13,1,1,1};
I32 lookup(I32 index){if(index<0 || index>=147)throw std::out_of_range("original Squirrel motion table");return table[static_cast<std::size_t>(index)];}
}
void select_squirrel_motion(SquirrelState& s) {
    if(s.row<0 || s.row>=3)throw std::out_of_range("original Squirrel row");
    s.timer=0;s.step=0;
    I32 index=s.phase;
    if(index>0)--index;
    if(index>1){--index;if(index>1)--index;}
    if(index>2)--index;
    if(index<0 || index>=4)throw std::out_of_range("original Squirrel placement slot");
    const auto& plan=s.plan[s.row];const auto& placed=s.placed[s.row];
    I32 from=placed[index];const I32 linear=from/9+(from%9)*4;
    I32 to;
    if(s.phase==0){from=lookup(plan[0]);to=from;}
    else if(s.phase==2){from=lookup(plan[1]);to=placed[0]/9+(placed[0]%9)*4;}
    else if(s.phase==1){from=linear;to=lookup(plan[0]);}
    else if(s.phase==3 || s.phase==5){from=linear;to=lookup(plan[2]);}
    else if(s.phase==4){from=lookup(plan[2]);to=linear;}
    else if(s.phase==6){from=lookup(plan[3]);to=linear;}
    else to=from;
    s.motion={lookup(to+39),lookup(from+75),lookup(from+3),lookup(from+111)};
    for(auto& value:s.motion)if(value<0)value=0;
}
void generate_squirrel_choices(SquirrelState& s,U32& seed) {
    if(s.difficulty<0 || s.difficulty>2 || s.row<0 || s.row>=3)
        throw std::out_of_range("original Squirrel choices input");
    const I32 good=s.difficulty==2?2:3,bad=s.difficulty;
    const auto& plan=s.plan[s.row];
    I32 variant=-1;
    if(s.stages[s.row]==0){s.target=plan[0];variant=plan[1];}
    else if(s.stages[s.row]==1){s.target=plan[1];variant=plan[2];}
    else if(s.stages[s.row]==2){s.target=plan[2];variant=plan[3];}
    if(s.target<0 || s.target>2 || variant < -1 || variant>2)
        throw std::out_of_range("original Squirrel connector");
    auto random=[&](){return static_cast<I32>(original_crt17::retail_rand(seed));};
    std::array<std::array<I32,2>,4> correct{},decoy{};std::array<I32,4> correct_id{},decoy_id{};
    for(I32 i=0;i<good;++i) {
        const I32 v=variant==-1?random()%3:variant;
        correct[i]={s.target,v};correct_id[i]=s.target*3+v;
        if(variant!=-1)s.variant=variant;
    }
    I32 first_other=0;
    for(I32 i=0;i<bad;++i) {
        I32 category=s.target;
        if(!i){do{category=random()%3;first_other=category;}while(category==s.target);}
        else {while(category==s.target || category==first_other)category=random()%3;}
        I32 v;do{v=random()%3;}while(v==variant);
        const I32 id=v+category*3;
        // The original stores the combined inner ID in this first field.
        decoy[i]={id,v};decoy_id[i]=id;
    }
    s.outer=random()%7;
    I32 chosen_good=0,chosen_bad=0,out=0;
    while(out<good+bad) {
        if(random()%2==0) {
            if(chosen_good>=good)continue;
            s.correct[out]=1;s.pairs[out]=correct[chosen_good];
            s.piece_ids[out]=squirrel::kOuterVariantPermutations[s.outer][out]*9+correct_id[chosen_good++];
        } else {
            if(chosen_bad>=bad)continue;
            s.correct[out]=0;s.pairs[out]=decoy[chosen_bad];
            s.piece_ids[out]=squirrel::kOuterVariantPermutations[s.outer][out]*9+decoy_id[chosen_bad++];
        }
        ++out;
    }
}
squirrel::RunAssemblyMotionKeys original_motion_keys(SquirrelState& s) {
    select_squirrel_motion(s);return {s.motion[0],s.motion[1],s.motion[2],s.motion[3]};
}
void begin_original_run_assembly(SquirrelState& s,squirrel::RunAssemblyAnimationRuntime& runtime,const squirrel::Data& data) {
    squirrel::begin_run_assembly_animation(runtime,data,original_motion_keys(s));
}
bool verify_3_original_editor_functions() {
    struct Services final:FireworksServices {
        I32 cursor{-1},sound{-1};void set_cursor(I32 type) override{cursor=type;}
        void play_sound(I32 id,I32,I32) override{sound=id;}
    } io;
    FireworksState f;f.grid.fill(-1);f.grid[0]=3;f.x=40;f.y=50;
    std::array<Region,1> regions{Region{20,20,80,80,0}};U32 seed=1;
    if(hit_test_fireworks(f,regions,seed,io)!=-1 || f.grid[0]!=-1 || f.bob!=1 || io.cursor!=3)return false;
    SquirrelState s;s.plan[0]={0,1,2,-1};s.placed[0]={0,1,2,3};
    auto keys=original_motion_keys(s);
    if(keys.a!=0 || keys.b!=1 || keys.c!=0 || keys.d!=1)return false;
    generate_squirrel_choices(s,seed);
    return s.correct[0]+s.correct[1]+s.correct[2]==3;
}
}
