#include "btb/fireworks_show_native.hpp"

#include <cassert>
#include <string>
#include <tuple>
#include <vector>

using namespace btb::fireworks;
using Clip=std::tuple<std::size_t,int,int>;

int main() {
    Sequence grid;
    assert(grid.commit_placement(0,0,FireworkType::RedAirbomb));
    RetailShowRuntime live;
    std::vector<Clip> decoded;
    std::vector<std::size_t> rewound;
    bool effect_finished=false;
    bool crowd_finished=false;
    bool top_finished=false;
    const ShowMovieStep decode=[&](std::size_t index,int x,int y,
                                    bool& finished,std::string& error) {
        decoded.emplace_back(index,x,y);
        finished=index==0 ? effect_finished :
                 index==21 ? crowd_finished :
                 index==20 ? top_finished : false;
        error.clear();return true;
    };
    const ShowMovieRestart restart=[&](std::size_t index,
                                       std::string& error) {
        rewound.push_back(index);error.clear();return true;
    };
    ShowFrameOutput first;
    std::string error;
    assert(advance_native_fireworks_show(grid,live,{.elapsed_ticks=0},
        decode,restart,first,error));
    assert(first.launched_events.size()==1);
    assert(first.launched_events[0].pool_slot==0);
    assert(live.event_pool().active_count()==1);
    assert((decoded==std::vector<Clip>{{21,0,340},{20,220,0}}));
    assert(rewound.empty());

    decoded.clear();
    effect_finished=true;
    crowd_finished=true;
    top_finished=true;
    ShowFrameOutput second;
    assert(advance_native_fireworks_show(grid,live,{.elapsed_ticks=1},
        decode,restart,second,error));
    assert((decoded==std::vector<Clip>{
        {21,0,340},{0,0,0},{20,220,0}}));
    assert(second.completed_event_slots.size()==1);
    assert(second.completed_event_slots[0]==0);
    assert(second.crowd.phase_after==1);
    assert(live.event_pool().active_count()==0);
    assert((rewound==std::vector<std::size_t>{21,0,20}));

    // Original row-one type 11 => index 23, beyond the 23-entry bank.
    // Do not silently replace it with type 11 or invent a different Bink.
    Sequence invalid;
    assert(invalid.commit_placement(1,0,FireworkType::BlueCandle));
    RetailShowRuntime invalid_runtime;
    decoded.clear();rewound.clear();
    ShowFrameOutput middle;
    assert(advance_native_fireworks_show(invalid,invalid_runtime,
        {.elapsed_ticks=100},decode,restart,middle,error));
    assert(invalid_runtime.event_pool().active_count()==1);
    ShowFrameOutput rejected;
    assert(!advance_native_fireworks_show(invalid,invalid_runtime,
        {.elapsed_ticks=101},decode,restart,rejected,error));
    assert(error.find("index 23")!=std::string::npos);

    // Decoder physical failure cannot become an artificial movie EOF.
    RetailShowRuntime unable;
    const ShowMovieStep no_decoder=[](std::size_t,int,int,bool&,
                                        std::string& err) {
        err="original BinkDoFrame could not decode";
        return false;
    };
    ShowFrameOutput failed;
    assert(!advance_native_fireworks_show(grid,unable,
        {.elapsed_ticks=0},no_decoder,restart,failed,error));
    assert(error.find("BinkDoFrame")!=std::string::npos);
    assert(unable.event_pool().active_count()==0);
}
