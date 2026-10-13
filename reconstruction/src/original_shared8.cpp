#include "btb/original_shared8.hpp"
// Independent native C++ function bodies, not forwarding wrappers.
namespace btb::original_shared8 {
using namespace btb::shared_game_batch_9;
[[nodiscard]] Plan load_generic_ui(
    UiScreen& s,const front_end::ReplacementScreen& authored){
    Plan out;
    if(s.index<0||s.index>=12||authored.records.size()>32){
        out.error=true;out.detail="original UI screen index or area limit invalid";
        return out;
    }
    out.emit(Kind::BeginLoading);
    s.menu_selection=-1;
    s.records.clear();
    for(std::size_t i=0;i<authored.records.size();++i){
        const auto& record=authored.records[i];
        if(record.hover_frame_count<0||record.secondary_frame_count<0||
           record.hover_frame_count>64||record.secondary_frame_count>64){
            out.error=true;out.detail="retail UI animation frame count out of range";
            return out;
        }
        UiNativeRecord restored{};restored.source=record;
        for(I32 j=0;j<record.hover_frame_count;++j){
            restored.primary.push_back(0);
            ++s.original_loaded_count;
            out.emit(Kind::LoadBitmap,static_cast<I32>(i),j,0);
            out.emit(Kind::RegisterBitmap,static_cast<I32>(i),j,0);
        }
        for(I32 j=0;j<record.secondary_frame_count;++j){
            restored.secondary.push_back(0);
            out.emit(Kind::LoadBitmap,static_cast<I32>(i),j,1);
            out.emit(Kind::RegisterBitmap,static_cast<I32>(i),j,1);
        }
        if(!record.hover_bitmap_base.empty()){
            out.emit(Kind::LoadBitmap,static_cast<I32>(i),0,2);
            out.emit(Kind::RegisterBitmap,static_cast<I32>(i),0,2);
            out.emit(Kind::ColorKeyBitmap,static_cast<I32>(i),0x00ff00ff,2);
        }
        if(!record.secondary_bitmap_base.empty()){
            out.emit(Kind::LoadBitmap,static_cast<I32>(i),0,3);
            out.emit(Kind::RegisterBitmap,static_cast<I32>(i),0,3);
        }
        s.records.push_back(std::move(restored));
    }
    out.emit(Kind::NativeLegacyHook);
    out.emit(Kind::EndLoading);
    return out;
}

[[nodiscard]] Plan unload_generic_ui(UiScreen& s){
    Plan out;s.menu_selection=-1;
    if(s.index==12)return out;
    for(std::size_t i=0;i<s.records.size();++i){
        auto& r=s.records[i];
        for(std::size_t j=0;j<r.primary.size();++j){
            if(r.primary[j])++s.original_loaded_count;
            out.emit(Kind::UnregisterBitmap,static_cast<I32>(i),
                static_cast<I32>(j),0);
            if(r.primary[j]){
                out.emit(Kind::ReleaseBitmap,static_cast<I32>(i),
                  static_cast<I32>(j),0);r.primary[j]=0;
            }
        }
        for(std::size_t j=0;j<r.secondary.size();++j){
            out.emit(Kind::UnregisterBitmap,static_cast<I32>(i),
                static_cast<I32>(j),1);
            if(r.secondary[j]){
                out.emit(Kind::ReleaseBitmap,static_cast<I32>(i),
                    static_cast<I32>(j),1);r.secondary[j]=0;
            }
        }
        if(r.hover_bitmap){
            out.emit(Kind::UnregisterBitmap,static_cast<I32>(i),0,2);
            out.emit(Kind::ReleaseBitmap,static_cast<I32>(i),0,2);
            r.hover_bitmap=0;
        }
        if(r.pressed_bitmap){
            out.emit(Kind::UnregisterBitmap,static_cast<I32>(i),0,3);
            out.emit(Kind::ReleaseBitmap,static_cast<I32>(i),0,3);
            r.pressed_bitmap=0;
        }
    }
    out.emit(Kind::StopAllManaged);
    return out;
}

[[nodiscard]] Plan update_options(Options& s,I32 source_slider_x){
    Plan out{};
    if(s.close_sound_queued&&!s.sound_playing){
        s.phase=0;s.close_sound_queued=false;
        out.emit(Kind::ResetMouseBounds);out.emit(Kind::ResumeBink);
        if(s.restore_cursor)out.emit(Kind::SetCursor,1);
    }
    if(s.phase>1){
        s.phase=0;out.emit(Kind::ResetMouseBounds);
        out.emit(Kind::ResumeBink);out.early_return=true;return out;
    }
    out.emit(Kind::DrawOverlay,0);
    out.emit(Kind::DrawOverlay,1);
    out.emit(Kind::DrawOverlay,2);
    if(s.button_down&&inclusive(s.volume_bounds,s.cursor_x,s.cursor_y)){
        s.slider_position=source_slider_x;
        s.audio_volume=(s.slider_position-100)*42;
        if(s.audio_volume!=s.active_volume)out.emit(Kind::ApplyVolume,s.audio_volume);
    }
    s.active_volume=s.audio_volume;
    if(strict(s.close_bounds,s.cursor_x,s.cursor_y)){
        if(s.mouse_press){
            out.emit(Kind::StopAllManaged);
            out.emit(Kind::PlayManaged,862,90,1);
            out.emit(Kind::MarkInterruptible,862);
            s.close_sound_queued=true;
        }
        out.emit(Kind::DrawSelection,0);
    }
    return out;
}

[[nodiscard]] Plan update_quit(Quit& s,U32 random){
    Plan out;
    if(s.voice_queued&&!s.managed_playing){
        s.leave_requested=true;out.emit(Kind::SetLeaveRequest);
    }
    if(s.phase>1){
        s.phase=0;out.emit(Kind::ResetMouseBounds);
        out.emit(Kind::ResumeBink);out.early_return=true;return out;
    }
    out.emit(Kind::DrawBackground);
    out.emit(Kind::DrawOverlay,1);
    if(s.voice_queued)return out;
    I32 hit=-1;
    for(I32 i=0;i<2;++i)
        if(strict(s.buttons[static_cast<std::size_t>(i)],s.x,s.y)){
            hit=i;break;
        }
    if(hit!=-1){
        out.emit(Kind::DrawSelection,hit);
        if(hit!=s.hover_index)out.emit(Kind::PlayManaged,453+hit,50,2);
    }
    s.hover_index=hit;
    if(hit!=-1&&s.down)out.emit(Kind::DrawSelection,hit+2);
    if(hit!=-1&&s.press){
        if(hit==1)s.phase=0;
        else{
            s.voice_queued=true;
            out.emit(Kind::StopAllManaged);
            out.emit(Kind::PlayManaged,626+static_cast<I32>(random%4U),90,1);
        }
    }
    return out;
}

[[nodiscard]] Plan update_yes_no(YesNo& s){
    Plan out;
    if(s.phase>=2){
        s.phase=0;
        out.emit(Kind::ResetMouseBounds);out.emit(Kind::ResumeBink);
        out.emit(Kind::UnregisterBitmap,0x0051c294);
        if(s.bitmap_loaded){
            out.emit(Kind::ReleaseBitmap,0x0051c294);
            s.bitmap_loaded=false;
        }
        out.early_return=true;return out;
    }
    if(s.context>=0&&s.context<=3)
        out.emit(Kind::DrawBackground,s.context);
    out.emit(Kind::DrawOverlay,0);
    I32 hit=-1;
    for(I32 i=0;i<2;++i)
        if(inclusive(s.buttons[static_cast<std::size_t>(i)],s.x,s.y)){
            hit=i;break;
        }
    if(s.down&&hit!=-1){
        if(hit==s.pressed_response){
            s.hover_response=hit+1;
            out.emit(Kind::DrawSelection,hit+2);
        }
    }
    if(s.press&&hit!=-1&&s.pressed_response==hit){
        s.reset_requested=(hit==0);
        s.phase=0;s.hover_response=-1;s.pressed_response=-1;
        out.emit(Kind::ResetMouseBounds);
        s.result=s.reset_requested?1:0;
        return out;
    }
    if(!s.down){
        if(hit>=0){
            s.hover_response=hit+1;s.pressed_response=hit;
            out.emit(Kind::DrawSelection,hit);
        }else s.pressed_response=-1;
    }
    return out;
}

[[nodiscard]] Plan draw_profile_name(ProfileNamePopup& state){
    Plan out;
    out.emit(Kind::DrawBackground);
    out.emit(Kind::DrawOverlay,296,103,state.selected_profile*50);
    I32 width=0;
    for(const auto& g:state.glyphs)width+=g.src_right-g.src_left;
    I32 x=322-width/2;
    for(const auto& g:state.glyphs){
        out.emit(Kind::DrawGlyph,x,200,g.code);
        x+=g.src_right-g.src_left;
    }
    state.next_x=x;
    if(++state.blink_tick>=33)state.blink_tick=0;
    if(state.blink_tick<=16)out.emit(Kind::DrawCursor,x-3,197);
    return out;
}

[[nodiscard]] std::pair<bool,Plan> update_bentley(Bentley& s){
    Plan out;
    static constexpr std::array<I32,4> bounds{44,40,56,44};
    s.scene_kind=3;s.visible_frame=0;
    switch(s.phase){
        case 0:
          if(--s.delay<1){
            s.delay=9;
            if(++s.frame>7){
                s.frame=0;
                if(!s.managed_playing){s.frame=12;s.phase=1;}
            }
          }
          break;
        case 1:
          if(--s.delay<1){
            s.delay=9;s.scroll_x+=5;++s.frame;
            if(s.frame>20){
                s.frame=12;
                if(s.scroll_x>299){s.frame=25;s.phase=2;}
            }
          }
          // Retail intentionally falls through to phase 2's animation tick.
          [[fallthrough]];
        case 2:
          if(--s.delay<1){
            s.delay=9;++s.frame;
            if(s.frame>39){
                s.frame=25;--s.loops;
                if(s.loops<0){
                    s.phase=3;s.voice_category=s.repairs_remaining>0?1:0;
                    const I32 id=s.voice_category?714:715;
                    out.emit(Kind::PlayManaged,id,50,1);
                    s.frame=bounds[static_cast<std::size_t>(s.voice_category)];
                }
            }
          }
          break;
        case 3:
        case 4:
          if(--s.delay<1){
            s.delay=9;++s.frame;
            if(s.frame>=bounds[static_cast<std::size_t>(s.voice_category)+2])
                s.frame=bounds[static_cast<std::size_t>(s.voice_category)];
          }
          if(!s.managed_playing){
            if(s.phase==4)return {true,std::move(out)};
            const I32 id=s.repairs_remaining<1?726+static_cast<I32>(s.random%2U)
                                                :716+static_cast<I32>(s.random%2U);
            out.emit(Kind::PlayManaged,id,50,1);
            ++s.phase;
          }
          break;
        case -1:
          s.restart_sound=true;
          if(--s.delay<1){
            s.delay=9;s.scroll_x+=5;++s.frame;
            if(s.frame>20){
                s.frame=12;
                if(s.scroll_x>149){
                    s.phase=0;s.frame=0;
                    out.emit(Kind::StopAllManaged);
                    out.emit(Kind::PlayManaged,713,50,1);
                }
            }
          }
          break;
    }
    return {false,std::move(out)};
}

[[nodiscard]] Plan open_walkthrough(
    Walkthrough& s,I32 index,
    const std::function<std::optional<std::vector<U8>>(const std::string&,std::size_t)>&
        read_asset){
    Plan out;
    if(index<0||static_cast<std::size_t>(index)>=s.paths.size()||!read_asset){
        out.error=true;out.detail="source walkthrough path or asset provider unavailable";
        return out;
    }
    constexpr std::size_t cap=7000000;
    const auto data=read_asset(s.paths[static_cast<std::size_t>(index)],cap);
    if(!data||data->size()>cap){
        out.error=true;out.detail="original Bink asset missing or larger than 7MB";
        return out;
    }
    s.owned_bytes=*data;
    s.surface_description.fill(0);
    s.surface_description[0]=124;
    s.surface_description[1]=7;
    s.surface_description[2]=179;
    s.surface_description[3]=282;
    s.surface_description[26]=64;
    s.selected=index;s.movie_opened=true;
    out.emit(Kind::OpenWalkthroughBuffer,index,static_cast<I32>(data->size()));
    out.emit(Kind::CreateBinkSurface,0x0051be3c);
    out.emit(Kind::OpenBinkMovie,index);
    return out;
}
} // namespace btb::original_shared8
