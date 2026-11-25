#pragma once
// Read-only installed-data command-stream validation shared by native probes.
#include "btb/full_game_runtime.hpp"
#include "btb/original_asset_resolver.hpp"
#include "btb/full_game_blt_clip.hpp"
#include <array>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>

namespace btb::retail_probe {
namespace fg=btb::full_game;
namespace fs=std::filesystem;
using btb::game_flow::State;
using btb::progress::Slot;
inline void check(bool ok,const std::string& message) {
    if(!ok)throw std::runtime_error(message);
}
inline std::uint32_t word(const std::array<unsigned char,26>& b,int off) {
    return std::uint32_t(b[off])|(std::uint32_t(b[off+1])<<8)|
           (std::uint32_t(b[off+2])<<16)|(std::uint32_t(b[off+3])<<24);
}
struct Audit {
    fg::OriginalAssetResolver resolver;
    std::map<std::string,std::pair<int,int>> bitmaps;
    std::map<std::string,fs::path> sounds;
    std::uint64_t frames{},draws{},visible{};
    fs::path resolve(const std::string& name) {
        const auto a=resolver.resolve(name);
        check(a.found(),name+": "+a.error);
        return a.absolute_path;
    }
    void observe(const fg::ActivityFrameOutput& out) {
        check(!out.fatal_error,out.fatal_error.value_or(""));
        for(const auto& d:out.draws) {
            auto it=bitmaps.find(d.source_asset);
            if(it==bitmaps.end()) {
                const auto path=resolve(d.source_asset);
                std::ifstream in(path,std::ios::binary);
                std::array<unsigned char,26> b{};
                in.read(reinterpret_cast<char*>(b.data()),b.size());
                check(bool(in)&&b[0]=='B'&&b[1]=='M'&&word(b,14)>=40,
                      "invalid BMP header: "+path.string());
                const int w=std::bit_cast<std::int32_t>(word(b,18));
                const int h=std::bit_cast<std::int32_t>(word(b,22));
                check(w>0&&h!=0&&h!=std::numeric_limits<int>::min(),"invalid BMP dimensions");
                it=bitmaps.emplace(d.source_asset,std::pair{w,h<0?-h:h}).first;
            }
            const auto [w,h]=it->second;
            const auto r=d.source_rectangle.value_or(fg::Rect{0,0,w,h});
            check(r.left>=0&&r.top>=0&&r.right>r.left&&r.bottom>r.top&&r.right<=w&&r.bottom<=h,
                "invalid DirectDraw source crop: "+d.source_asset+" rect="+
                std::to_string(r.left)+","+std::to_string(r.top)+","+
                std::to_string(r.right)+","+std::to_string(r.bottom)+" extent="+
                std::to_string(w)+"x"+std::to_string(h));
            const auto clipped=fg::retail_clip_blt_to_rect(r,d.x,d.y,
                d.destination_clip.value_or(fg::Rect{0,0,640,480}));
            if(clipped.visible) {
                const auto& s=clipped.source;
                check(s.left>=r.left&&s.top>=r.top&&s.right<=r.right&&s.bottom<=r.bottom,
                      "clipped source escaped original rectangle");
                check(s.right-s.left==clipped.destination.right-clipped.destination.left&&
                      s.bottom-s.top==clipped.destination.bottom-clipped.destination.top,
                      "clip changed sprite scale");
                ++visible;
            }
            ++draws;
        }
        for(const auto& a:out.audio)
            if(!a.source_asset.empty()&&!sounds.contains(a.source_asset))
                sounds.emplace(a.source_asset,resolve(a.source_asset));
        ++frames;
    }
    void report() const {
        std::cout<<" frames="<<frames<<" draws="<<draws<<" visible="<<visible
                 <<" bitmap_paths="<<bitmaps.size()<<" audio_paths="<<sounds.size()
                 <<" completion=1\n";
    }
};
struct VoiceModel {
    int wait{},id{-1};
    void sample(fg::ActivityFrameInput& input) {
        input.managed_sound_playing=wait>0;
        input.squirrel_lofty_voice_playing.emplace();
        input.squirrel_lofty_voice_playing->fill(false);
        if(wait>0&&id>=633&&id<=638)(*input.squirrel_lofty_voice_playing)[id-633]=true;
        if(wait>0)--wait;
    }
    void observe(const fg::ActivityFrameOutput& out) {
        for(const auto& a:out.audio) {
            if(a.operation==fg::AudioOperation::StopManagedSounds){wait=0;id=-1;}
            if(a.operation==fg::AudioOperation::ManagedSoundId){wait=8;id=a.sound_id;}
        }
    }
};
inline void completion(const fg::GameRoot& game,const fg::ActivityFrameOutput& out,
                Slot slot,State init,int profile) {
    check(out.save_and_unload&&out.progress_writes.size()==1&&
          out.progress_writes[0].slot==slot&&out.progress_writes[0].value==1,
          "invalid completion progress write");
    for(int i=0;i<int(btb::progress::kPlayerCount);++i)
        check(game.globals().player_progress[i].get(slot)==(i==profile?1:0),
              "progress isolation failed");
    check(game.globals().dispatcher.current_state==int(State::PlayAgainYesNoSetup)&&
          game.globals().dispatcher.saved_state==int(init)&&
          game.globals().menu.replay_class==1,"incorrect replay transition");
}
}
