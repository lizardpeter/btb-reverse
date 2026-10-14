#pragma once
#include "btb/help_system.hpp"
#include "btb/original_asset_resolver.hpp"
#include <istream>
namespace btb::help {
struct State {
    std::array<std::array<Region,30>,25> regions{};
    std::array<std::int32_t,25> counts{};
    std::int32_t activation_phase{},active{},last_region{},context{};
};
static_assert(sizeof(Region)==20);
// 00401030: load 24 groups, retain the 25th count, default exit region first.
void load_original_help_info(State&,std::istream&);
void load_original_help_info(State&,const full_game::OriginalAssetResolver&);
class Services {
public:
    virtual ~Services()=default;
    virtual void copy_front_buffer_to_help_surface()=0;
    virtual void play_help_voice(std::int32_t sound,std::int32_t priority)=0;
};
// 004011B0. Uses the already recovered 68-state context table.
void enter_original_help_mode(State&,std::int32_t outer_state,
    bool name_popup,bool progress_screen,Services&);
}
