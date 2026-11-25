#pragma once
#include "btb/full_game_resources.hpp"
#include <algorithm>
#include <cstdint>
#include <vector>
#include <span>
#include <unordered_map>
namespace btb::enhanced {
// Render-only interpolation of stable, unique sprite identities. We do not
// extrapolate, blend sprite artwork, or invent identities for repeated draws.
// Source hit tests, timer, RNG, audio, saves and movie state are untouched.
class SpriteMotion {
    struct Move {std::size_t slot;int from_x,from_y,to_x,to_y;};
    std::vector<full_game::ResolvedDraw> previous_,sample_;
    std::vector<Move> moves_;
    std::uint64_t previous_ns_{},start_ns_{},duration_ns_{};
    int state_{-1};
    bool sample_valid_{};
    unsigned last_phase_{1000};
    static bool same(const full_game::ResolvedDraw& a,const full_game::ResolvedDraw& b){
        return a.source.absolute_path==b.source.absolute_path&&
            a.original.source_asset==b.original.source_asset&&
            a.original.source_rectangle==b.original.source_rectangle&&
            a.original.destination_clip==b.original.destination_clip&&
            a.original.color_keyed==b.original.color_keyed&&
            a.original.after_activity_movie==b.original.after_activity_movie;
    }
public:
    void reset() noexcept {previous_.clear();sample_.clear();moves_.clear();previous_ns_=start_ns_=duration_ns_=0;sample_valid_=false;last_phase_=1000;}
    // Only interpolate gameplay run states, never menus, setup, replay or
    // credits. The selected unique asset must remain in the same draw slot.
    static constexpr bool gameplay_state(int s) noexcept {
        switch(s){case 0x0f:case 0x15:case 0x1b:case 0x21:case 0x25:
            case 0x29:case 0x2f:case 0x33:case 0x37:case 0x3b:return true;default:return false;}
    }
    void state_changed(int state) noexcept {if(state_!=state){reset();state_=state;}}
    void ingest(const std::vector<full_game::ResolvedDraw>& next,std::uint64_t now){
        moves_.clear();sample_valid_=false;last_phase_=1000;
        const auto dt=now>previous_ns_?now-previous_ns_:0;
        if(gameplay_state(state_)&&next.size()==previous_.size()&&next.size()<=8192&&
           dt>=4000000&&dt<=50000000){
            // Require the whole painter-order signature to match; this
            // prevents interpolating across modal or artwork changes.
            bool matching=true;
            std::unordered_map<std::filesystem::path,unsigned> counts;
            for(std::size_t i=0;i<next.size();++i){
                if(!same(previous_[i],next[i])){matching=false;break;}
                ++counts[next[i].source.absolute_path];
            }
            if(matching)for(std::size_t i=0;i<next.size();++i){
                const auto& a=previous_[i].original;const auto& b=next[i].original;
                // Opaque backdrops and input-bound cursor-like objects must
                // not be interpolated. Duplicate assets are ambiguous.
                if(!b.color_keyed||counts[next[i].source.absolute_path]!=1||
                   b.source_asset=="Data/ui/cursor.bmp"||b.after_activity_movie)continue;
                const std::int64_t dx=std::int64_t(b.x)-a.x,dy=std::int64_t(b.y)-a.y;
                if((!dx&&!dy)||dx<-32||dx>32||dy<-32||dy>32)continue;
                moves_.push_back({i,a.x,a.y,b.x,b.y});
            }
        }
        previous_=next;previous_ns_=now;
        start_ns_=now;duration_ns_=moves_.empty()?0:dt;
        if(duration_ns_){sample_=next;last_phase_=1001;}
    }
    bool active()const noexcept{return duration_ns_!=0;}
    // A null result means the current source image can be reused. A final
    // exact sample is emitted once so the animation never stops short.
    const std::vector<full_game::ResolvedDraw>* sample(std::uint64_t now){
        if(!duration_ns_)return nullptr;
        const auto elapsed=now>start_ns_?now-start_ns_:0;
        const unsigned phase=elapsed>=duration_ns_?1000:unsigned(elapsed*1000/duration_ns_);
        if(sample_valid_&&phase==last_phase_)return nullptr;
        bool changed=!sample_valid_;
        const auto blend=[&](int a,int b){const std::int64_t d=std::int64_t(b)-a;
            const auto amount=(d>=0?d*phase+500:d*phase-500)/1000;
            return int(std::int64_t(a)+amount);};
        for(const auto& m:moves_){
            auto& d=sample_[m.slot].original;
            const auto x=blend(m.from_x,m.to_x),y=blend(m.from_y,m.to_y);
            changed|=x!=d.x||y!=d.y;d.x=x;d.y=y;
        }
        last_phase_=phase;sample_valid_=true;
        if(phase==1000)duration_ns_=0;
        return changed?&sample_:nullptr;
    }
    const std::vector<full_game::ResolvedDraw>* current_sample() const noexcept {
        return sample_valid_?&sample_:nullptr;
    }
};
}
