#pragma once
#include "btb/fireworks_sequence.hpp"
#include "btb/fireworks_movie_bank_manifest.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>

namespace btb::fireworks {

// Movie step MUST call the original Bink decoder, physically copy the source
// image to an offscreen DD7 surface and enqueue it in composition order.
// 'finished' is observed from that source HBINK, never from a fabricated clock.
using ShowMovieStep = std::function<bool(
    std::size_t index,int screen_x,int screen_y,
    bool& finished,std::string& error)>;
using ShowMovieRestart = std::function<bool(
    std::size_t index,std::string& error)>;

// Source 0x00413450 orders operations:
//  1. crowd-loop or crowd-end movie
//  2. the previously active 40-slot effect pool
//  3. always-present topmiddle.bik
//  4. allocate due events for the NEXT frame (not this one)
// The 3x6 grid comes from original 18xint32 Fireworks profile save files.
// Source quirks, including row-one type+12 and out-of-bank index 23,
// fail explicitly, never normalize a different firework movie.
[[nodiscard]] inline bool advance_native_fireworks_show(
    const Sequence& sequence,
    RetailShowRuntime& runtime,
    ShowFrameInput input,
    const ShowMovieStep& step,
    const ShowMovieRestart& restart,
    ShowFrameOutput& out,
    std::string& error) {

    if(!step || !restart) {
        error="original Fireworks Bink decoder/restart provider absent";
        return false;
    }
    const auto crowd=static_cast<std::size_t>(
        crowd_movie_index_for_phase(runtime.crowd_phase()));
    if(!step(crowd,0,340,input.crowd_movie_finished,error))
        return false;
    // Retail rewinds a completed crowd-loop immediately, BEFORE walking the
    // authored pool; a row-one firework can alias this same Bink index.
    if(crowd==kOriginalCrowdLoopMovie &&
       input.crowd_movie_finished &&
       !restart(kOriginalCrowdLoopMovie,error))return false;

    const auto& active=runtime.event_pool().records();
    for(std::size_t slot=0;slot<active.size();++slot) {
        const auto& record=active[slot];
        if(record.free())continue;
        if(record.row<0 || record.row>=3 ||
           record.type<0 || record.type>=12) {
            error="original Fireworks active-event record invalid";
            return false;
        }
        const auto row=static_cast<std::size_t>(record.row);
        const auto movie=static_cast<std::size_t>(record.type)+
            (row==1?12u:0u);
        if(movie>=kOriginalFireworkMovieCount) {
            error="retail Fireworks row-one type+12 reaches "
                  "Bink index 23 outside original bank";
            return false;
        }
        const int x=row==0?0:(row==1?440:220);
        const int y=row==2?200:0;
        if(!step(movie,x,y,input.active_event_movie_finished[slot],error))
            return false;
        // Like the source 40-record loop, rewind a completed clip before
        // the next record can use that same original Bink stream.
        if(input.active_event_movie_finished[slot] &&
           !restart(movie,error))return false;
    }

    if(!step(kOriginalTopMiddleMovie,220,0,
             input.top_middle_movie_finished,error))
        return false;
    if(input.top_middle_movie_finished &&
       !restart(kOriginalTopMiddleMovie,error))return false;

    // The original scheduling/phase/pool model owns all source decisions.
    // Its new events intentionally did not pass through BinkDoFrame above.
    out=runtime.update(sequence,input);

    // Source movie rewinds already occurred directly after each physical
    // decode. The source model now releases corresponding pool slots and
    // launches the NEW events, which do not draw until the next tick.
    error.clear();
    return true;
}
} // namespace btb::fireworks
