#include "btb/original_help2.hpp"
#include "btb/full_game_runtime.hpp"
#include <cassert>
#include <fstream>
#include <bit>
#include <sstream>
#include <stdexcept>
using namespace btb;
struct Services final:help::Services {
    help::State& state;int sequence{};
    explicit Services(help::State& s):state(s){}
    void copy_front_buffer_to_help_surface() override {assert(sequence++==0);}
    void play_help_voice(std::int32_t sound,std::int32_t priority) override {
        assert(sequence++==1 && state.active==1 && state.activation_phase==0);
        assert(sound==476 && priority==1);
    }
};
int main(int argc,char** argv) {
    assert(argc==2);
    help::State state;state.active=99;state.activation_phase=3;
    std::istringstream input("group0 1 2 3 4 700 -1 0 0 0 0 group1 5 6 7 8 701 -1 0 0 0 0 END");
    help::load_original_help_info(state,input);
    assert(state.counts[0]==2 && state.counts[1]==2 && state.counts[24]==1);
    assert(state.regions[0][0].left==20 && state.regions[1][0].left==92);
    assert(state.regions[1][0].sound_id==477 && state.regions[1][1].sound_id==701);
    assert(state.regions[2][0].left==20 && state.active==0 && state.activation_phase==0);
    std::ostringstream all;
    for(int i=0;i<24;++i)all<<"group -1 0 0 0 0 ";
    std::istringstream every(all.str());help::load_original_help_info(state,every);
    for(auto n:state.counts)assert(n==1);
    std::ifstream file(argv[1]);assert(file);
    unsigned flow,popup,progress,active,context,last,count=0;
    while(file>>std::hex>>flow>>popup>>progress>>active>>context>>last) {
        Services io(state);state.activation_phase=2;state.last_region=99;
        full_game::GameRoot root;root.globals().dispatcher.current_state=static_cast<int>(flow);
        root.begin_original_help(state,popup!=0,progress!=0,io);
        assert(state.active==static_cast<int>(active) && state.context==std::bit_cast<std::int32_t>(context));
        assert(state.last_region==static_cast<int>(last) && io.sequence==2);
        ++count;
    }
    assert(count==272);
    std::istringstream bad("group 1 2");
    try{help::load_original_help_info(state,bad);assert(false);}catch(const std::runtime_error&){}
    full_game::OriginalAssetResolver missing("/missing-original-btb-root");
    try{help::load_original_help_info(state,missing);assert(false);}catch(const std::runtime_error&){}
}
