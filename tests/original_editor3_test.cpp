#include "btb/original_editor3.hpp"
#include <bit>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace btb::original_editor3;
U32 word(std::istream& in){U32 v{};in>>std::hex>>v;return v;}
I32 signed_word(std::istream& in){return std::bit_cast<I32>(word(in));}
FireworksState fire(std::istream& in){
    FireworksState s;
    for(auto* p:{&s.mode,&s.x,&s.y,&s.hot,&s.drag_type,&s.bob,&s.wendy,&s.bob_type,&s.wendy_type})*p=signed_word(in);
    for(auto& v:s.grid)v=signed_word(in);return s;
}
SquirrelState squirrel(std::istream& in){
    SquirrelState s;
    for(auto* p:{&s.difficulty,&s.row,&s.phase,&s.target,&s.variant,&s.outer,&s.timer,&s.step})*p=signed_word(in);
    for(auto& v:s.stages)v=signed_word(in);
    for(auto& row:s.plan)for(auto& v:row)v=signed_word(in);
    for(auto& row:s.placed)for(auto& v:row)v=signed_word(in);
    for(auto& v:s.correct)v=signed_word(in);
    for(auto& v:s.piece_ids)v=signed_word(in);
    for(auto& v:s.motion)v=signed_word(in);
    for(auto& row:s.pairs)for(auto& v:row)v=signed_word(in);
    return s;
}
struct Services final:FireworksServices {
    std::vector<I32> trace;
    void set_cursor(I32 type) override {trace.insert(trace.end(),{2,type,0,0});}
    void play_sound(I32 id,I32 p,I32 a) override {trace.insert(trace.end(),{1,id,p,a});}
};
int main(int argc,char** argv) {
    if(argc!=2 || !verify_3_original_editor_functions())return 1;
    std::ifstream file(argv[1]);if(!file)return 2;
    std::string line,kind;unsigned checked=0;
    while(std::getline(file,line)) {
        std::istringstream in(line);in>>kind;
        bool okay=false;
        if(kind=="FIRE") {
            U32 seed=word(in);auto state=fire(in);
            const I32 expected_return=signed_word(in);const U32 expected_seed=word(in);
            const auto expected=fire(in);const unsigned traces=word(in);
            std::vector<I32> expected_trace;for(unsigned i=0;i<traces*4;++i)expected_trace.push_back(signed_word(in));
            std::array<Region,30> regions;
            for(unsigned j=0;j<30;++j) {
                I32 x,y,w,h;
                if(j<18){x=20+j%6*70;y=40+j/6*70;w=60;h=60;}
                else{x=400+(j-18)%3*60;y=300+(j-18)/3*35;w=50;h=30;}
                regions[j]={x,y,x+w,y+h,static_cast<I32>(j%12)};
            }
            Services io;
            const auto result=hit_test_fireworks(state,regions,seed,io);
            okay=result==expected_return && seed==expected_seed && state==expected && io.trace==expected_trace;
        } else if(kind=="SQUIRREL") {
            const auto mode=word(in);U32 seed=word(in);auto state=squirrel(in);
            const U32 expected_seed=word(in);const auto expected=squirrel(in);
            if(mode==0)select_squirrel_motion(state);else generate_squirrel_choices(state,seed);
            okay=state==expected && seed==expected_seed;
        }
        if(!in || !okay){std::cerr<<"Mismatch at editor vector "<<checked<<": "<<line<<'\n';return 3;}
        ++checked;
    }
    if(checked!=3000)return 4;
    SquirrelState s;s.plan[0]={0,1,2,0};s.placed[0]={0,1,2,3};
    btb::squirrel::RunAssemblyAnimationRuntime animation;btb::squirrel::Data data;
    data.animation_step_delay=4;begin_original_run_assembly(s,animation,data);
    if(animation.delay_remaining!=4 || animation.motion_keys.b!=s.motion[1])return 5;
    // Strict edges, absent regions and the original mode-1 net pointer shift.
    FireworksState empty;empty.mode=1;empty.x=50;empty.y=60;empty.grid.fill(-1);
    U32 seed=12;Services io;
    if(hit_test_fireworks(empty,{},seed,io)!=-1 || empty.x!=40 || empty.y!=50 || seed!=12)return 6;
    empty.mode=13;empty.x=10;empty.y=20;
    std::array<Region,1> absent{Region{0,0,100,100,-1}};
    if(hit_test_fireworks(empty,absent,seed,io)!=-1 || !io.trace.empty())return 7;
    std::cout<<checked<<" original x86 editor state/RNG/trace comparisons pass\n";
}
