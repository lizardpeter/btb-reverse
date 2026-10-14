#include "btb/original_help2.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
namespace btb::help {
void load_original_help_info(State& s,std::istream& input) {
    s.counts.fill(1);s.activation_phase=0;s.active=0;
    for(std::size_t group=0;group<24;++group) {
        s.regions[group][0]=group==1?Region{92,305,143,357,477}:Region{20,420,63,462,477};
        std::string marker;
        if(!(input>>marker))throw std::runtime_error("truncated help marker");
        if(marker=="END")break;
        for(std::size_t entry=1;;++entry) {
            if(entry>=30)throw std::runtime_error("help group exceeds original 30-region storage");
            auto& r=s.regions[group][entry];
            if(!(input>>r.left>>r.top>>r.right>>r.bottom>>r.sound_id))
                throw std::runtime_error("truncated help region");
            if(r.left==-1)break;
            ++s.counts[group];
        }
    }
}
void load_original_help_info(State& s,const full_game::OriginalAssetResolver& resolver) {
    const auto file=resolver.resolve("loaddata\\helpinfo.txt");
    if(!file.found())throw std::runtime_error("original helpinfo.txt unavailable");
    std::ifstream input(file.absolute_path);
    if(!input)throw std::runtime_error("cannot open original helpinfo.txt");
    load_original_help_info(s,input);
}
void enter_original_help_mode(State& s,std::int32_t outer,bool popup,bool progress,Services& io) {
    io.copy_front_buffer_to_help_surface();
    s.activation_phase=0;s.active=1;
    io.play_help_voice(kHelpIntroSoundId,1);
    s.last_region=0;
    const auto decision=enter_decision(outer,popup,progress);
    s.context=decision.context;s.active=decision.active?1:0;
}
}
