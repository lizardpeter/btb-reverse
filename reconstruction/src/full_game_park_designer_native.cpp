#include "btb/full_game_park_designer_native.hpp"
#include "btb/park_designer_placement.hpp"

#include <algorithm>
#include <utility>

namespace btb::full_game {
namespace p=park_designer;
using S=p::TrailingStateIndex;
namespace {
void bitmap(ActivityFrameOutput& o,std::string_view name,int x,int y,
    std::optional<p::Rect> r={},std::optional<Rect> clip={}) {
    std::optional<Rect> source;
    if(r)source=Rect{r->left,r->top,r->right,r->bottom};
    o.draws.push_back({std::string(name),x,y,source,true,clip});
}
bool inside(p::Rect r,int x,int y){return r.left<x&&x<r.right&&r.top<y&&y<r.bottom;}
int maximum_page(int m){return m==2?3:m==3?0:2;}
void update_scroll(p::SaveData& d,p::EditorControlState& c,int m) {
    auto& page=d.state(p::kCurrentPageStateByEditorMode[m]);
    const int max=std::clamp(d.state(p::kMaxPageStateByEditorMode[m]),0,maximum_page(m));
    page=std::clamp(page,0,max);
    c.scroll_up_available=m<3&&page>0&&(m==2||page!=1);
    c.scroll_down_available=m<3&&page<max;
}
std::optional<p::RetailSprite> cursor_sprite(p::EditorMode m,int page,int variant,p::Season season) {
    if(variant<0||variant>=5)return {};
    if(m!=p::EditorMode::Pond)return p::retail_park_sprite(m,season,page,variant);
    if(page==0)return p::RetailSprite{p::kRetailPondSheets[variant].for_season(season),*p::retail_pond_segment_rect(variant,0)};
    if(page==1)return p::RetailSprite{p::kRetailFountainSheet.for_season(season),{variant*1976,0,variant*1976+152,155}};
    if(page==2){
        const auto dim=p::kRetailPrimarySpriteDimensions[variant+1];
        return p::RetailSprite{p::kRetailPrimaryObjects[variant+1].for_season(season),{0,0,dim.x,dim.y}};
    }
    return {};
}
p::Vec2i hotspot(p::EditorMode mode,int page,int variant,p::Rect sprite) {
    // 00507EF0 initialization in 0040B7E0, decoration families.
    // Win32 supplies absolute logical pixels, not the old relative
    // DirectInput cursor warp; non-decoration previews use their center.
    if(mode==p::EditorMode::Decorate){
        constexpr p::Vec2i offsets[]={{68,42},{50,155},{41,54},{52,45}};
        auto point=offsets[page];
        if(page==2&&variant==4)point.y=77;
        return point;
    }
    return {(sprite.right-sprite.left)/2,(sprite.bottom-sprite.top)/2};
}
}
void RetailParkDesignerNativeService::begin_session() noexcept {
    controls_={};controls_.first_mode_voice_pending=true;
    selection_.reset();queued_.reset();hover_=-1;view_hover_=-1;
    backing_started_=false;back_hover_=false;
}
void RetailParkDesignerNativeService::cancel_drag(p::SaveData& d) noexcept {
    if(selection_&&selection_->slot){
        auto& o=d.objects[*selection_->slot];
        if(o.object_code!=-1)o.enabled_or_active=1;
    }
    selection_.reset();controls_.cursor_mode=0;
}
std::optional<std::pair<int,int>> RetailParkDesignerNativeService::take_queued_voice() noexcept {
    return std::exchange(queued_,std::nullopt);
}
void RetailParkDesignerNativeService::voice(ActivityFrameOutput& out,const ActivityFrameInput& in,int id,int context) {
    if(id<0)return;
    if(in.managed_sound_playing&&context!=0&&!queued_)queued_=std::pair{id,context};
    out.audio.push_back({AudioOperation::ManagedSoundId,{},id,50,1,true});
}
bool RetailParkDesignerNativeService::render(p::SaveData& d,const p::BoundAreas&,ActivityFrameOutput& o,std::string& error) {
    if(!plan_retail_park_scene(d,scene_,error))return false;
    o.draws.insert(o.draws.end(),scene_.draws.begin(),scene_.draws.end());
    return true;
}
bool RetailParkDesignerNativeService::emit_sound(const p::LoopSound& s,ActivityFrameOutput& out,std::string& error) {
    switch(s.kind){
    case p::LoopSoundKind::StopIntroSound:
        if(!backing_started_)out.audio.push_back({AudioOperation::StopBackingTrack});break;
    case p::LoopSoundKind::StartParkBackingTrack:
        if(s.id!=2){error="unexpected Park Designer music index";return false;}
        if(!backing_started_){
            out.audio.push_back({AudioOperation::StartBackingTrack,"Data/music/dyp.wav"});
            backing_started_=true;
        }break;
    case p::LoopSoundKind::ManagedVoice:
    case p::LoopSoundKind::InputInterruptibleVoice:
        out.audio.push_back({AudioOperation::ManagedSoundId,{},s.id,50,1,true});break;
    }
    return true;
}
bool RetailParkDesignerNativeService::update_editor(p::SaveData& d,const p::BoundAreas& bounds,
    const ActivityFrameInput& in,ActivityFrameOutput& out,std::string& error) {
    int m=d.state(S::ActiveEditorMode),s=d.state(S::Season);
    if(m<0||m>3||s<0||s>1){error="invalid retail Park Designer editor mode/season";return false;}
    auto mode=static_cast<p::EditorMode>(m);
    auto season=static_cast<p::Season>(s);
    update_scroll(d,controls_,m);
    const int x=in.pointer_x,y=in.pointer_y;
    const bool click=in.click_pulse;
    int hit=p::retail_toolbar_hit(x,y);
    bool consumed=false;
    // 004084B0 shared back-button bounds are sourced from .data
    // 0043ECF4/F8/FC/ED00. 00428470 draws the 52px rollover at 574,418.
    const bool back=!selection_&&controls_.cursor_mode==0&&x>572&&x<617&&y>422&&y<467;
    if(back){
        bitmap(out,in.pointer_button_held?"Data/ui/backreddep.bmp":"Data/ui/backred.bmp",574,418);
        if(!back_hover_)out.audio.push_back({AudioOperation::ManagedSoundId,{},456,50,2});
        if(click){out.open_leave_activity_confirmation=true;consumed=true;}
    }
    back_hover_=back;
    // Original view-specific hover/buttons are processed before generic toolbar.
    if(m==3){
        int v=-1;
        for(int i=0;i<4;++i){
            const auto r=p::kRetailViewRegions[i];
            const bool over=inside(r,x,y);
            if(over)v=i;
            const auto& sheet=p::kRetailViewToolbarSheets[i];
            auto asset=over?(in.pointer_button_held?sheet.pressed:sheet.hover):sheet.normal;
            if(i==s)asset=sheet.pressed;
            bitmap(out,asset,r.left,r.top);
        }
        if(v>=0&&v!=view_hover_)out.audio.push_back({AudioOperation::ManagedSoundId,{},p::view_toolbar_hover_sound_id(static_cast<p::ViewToolbarItem>(v)),50,2});
        view_hover_=v;
        if(click&&v>=0&&v<3){
            const int draw=v<2?rng_.next_rand()%2:0;
            const auto action=p::apply_park_view_click(d,static_cast<p::ViewToolbarItem>(v),draw);
            if(action.managed_voice)voice(out,in,*action.managed_voice,99);
            if(action.action==p::ControlAction::PrintImage){
                bitmap(out,"Data/ui/printbar.bmp",0,400);
                out.request_original_frame_print=true;
            }
            consumed=true;
        }
    } else view_hover_=-1;

    const auto hovered=p::update_toolbar_hover(p::kRetailToolbarRegions,x,y,hover_,mode);
    hover_=hovered.next_hover_index;
    if(hovered.new_voice_id)out.audio.push_back({AudioOperation::ManagedSoundId,{},*hovered.new_voice_id,50,2});
    if(hovered.highlighted_region){
        const auto i=*hovered.highlighted_region;
        const auto& sheets=p::kRetailModeToolbarSheets[i-7];
        bitmap(out,in.pointer_button_held?sheets.pressed:sheets.hover,p::kRetailToolbarRegions[i].left,p::kRetailToolbarRegions[i].top);
    }

    if(click&&!consumed&&hit>=0&&(m<3||hit>=7)){
        if(selection_)cancel_drag(d);
        const auto a=p::apply_park_toolbar_click(d,controls_,hit);
        if(a.action==p::ControlAction::SelectPlacement){
            const int page=d.state(p::kCurrentPageStateByEditorMode[m]);
            selection_=Selection{mode,page,hit,{}};
            controls_.cursor_mode=1;
            if(m==0&&page==0&&d.state(S::PondSelectedPrimary)!=-1)voice(out,in,289,99);
            if(m==1&&page==0)voice(out,in,215,99);
        } else if(a.action==p::ControlAction::ConfirmDeleteAll){
            queued_.reset();
            voice(out,in,300,1);
            out.open_yes_no_confirmation=true;out.yes_no_context=3;
        } else if(a.action==p::ControlAction::SwitchMode)controls_.cursor_mode=0;
        consumed=true;
    }

    if(click&&!consumed&&y<400&&m!=3){
        if(controls_.cursor_mode==5){
            auto pick=p::find_topmost_object(d,bounds,scene_.sorted_slots,x,y,1,d.state(S::BandstandSelectedRecord),1,1);
            if(pick){
                auto r=p::delete_retail_park_object(d,pick->record_slot,rng_);
                voice(out,in,r.voice,r.voice_context);
            }
        } else if(selection_){
            const auto sel=*selection_;
            auto sprite=cursor_sprite(sel.mode,sel.page,sel.variant,season);
            if(!sprite){error="invalid original Park Designer selection";return false;}
            const auto off=hotspot(sel.mode,sel.page,sel.variant,sprite->source);
            const int ox=x-off.x,oy=y-off.y;
            bool placed=false;
            if(sel.mode==p::EditorMode::Decorate){
                auto r=p::commit_retail_decoration(d,ox,oy,sel.variant,sel.slot,rng_);
                if(r.managed_voice)voice(out,in,*r.managed_voice,r.voice_context);
                placed=r.status==p::DecorationCommitStatus::Placed;
            } else {
                auto r=sel.mode==p::EditorMode::Pond?p::commit_retail_pond(d,ox,oy,sel.variant,sel.slot,rng_):
                    p::commit_retail_bandstand(d,ox,oy,sel.variant,sel.slot,rng_);
                voice(out,in,r.voice,r.voice_context);
                if(r.scroll_animation)controls_.scroll_animation=r.scroll_animation;
                placed=r.status==p::PlacementStatus::Placed;
            }
            if(placed){selection_.reset();controls_.cursor_mode=0;}
        } else {
            auto pick=p::find_topmost_object(d,bounds,scene_.sorted_slots,x,y,0,d.state(S::BandstandSelectedRecord),1,1);
            if(pick){
                auto& record=d.objects[pick->record_slot];
                const int family=pick->record_slot<100?0:pick->record_slot>=300?2:1;
                if(record.bound_category>=0&&record.bound_category<=maximum_page(family)&&record.bound_variant>=0&&record.bound_variant<5){
                    d.state(S::ActiveEditorMode)=family;
                    d.state(p::kCurrentPageStateByEditorMode[family])=record.bound_category;
                    selection_=Selection{static_cast<p::EditorMode>(family),record.bound_category,record.bound_variant,pick->record_slot};
                    controls_.cursor_mode=2;record.enabled_or_active=2;
                }
            }
        }
    }

    m=d.state(S::ActiveEditorMode);
    update_scroll(d,controls_,m);
    const auto mode_button=p::kRetailToolbarRegions[7+m];
    bitmap(out,p::kRetailModeToolbarSheets[m].pressed,mode_button.left,mode_button.top);
    if(m<3){
        const auto page=d.state(p::kCurrentPageStateByEditorMode[m]);
        auto& scroll=controls_.scroll_animation;
        scroll=scroll<0?std::min(0,scroll+5):std::max(0,scroll-5);
        bitmap(out,p::kRetailPaletteTrays[m],235,400);
        // Source scroll rectangles can transiently reach beyond a sheet.
        // Clip and adjust destination by the clipped top, without scaling.
        const int top=page*61+scroll,bottom=top+61;
        const int upper=std::max(0,top),lower=std::min((maximum_page(m)+1)*61,bottom);
        if(upper<lower)bitmap(out,p::kRetailPaletteSheets[m],245,401+upper-top,p::Rect{0,upper,170,lower},Rect{245,401,415,462});
        if(controls_.scroll_up_available)bitmap(out,"Data/SubGameDYP/arrows.bmp",425,402,p::Rect{0,0,25,48});
        if(controls_.scroll_down_available)bitmap(out,"Data/SubGameDYP/arrows.bmp",425,428,p::Rect{25,0,50,48});
        if(controls_.first_mode_voice_pending){
            controls_.first_mode_voice_pending=false;
            if(m==0&&page==0&&d.state(S::PondSelectedPrimary)==-1)voice(out,in,274,0);
            if(m==1&&page==0&&d.state(S::BandstandSelectedRecord)==-1)voice(out,in,201,0);
        }
    }
    if(selection_){
        const auto& sel=*selection_;
        if(y<400){
            auto sprite=cursor_sprite(sel.mode,sel.page,sel.variant,season);
            if(!sprite){error="invalid Park Designer cursor bitmap";return false;}
            const auto off=hotspot(sel.mode,sel.page,sel.variant,sprite->source);
            bitmap(out,sprite->bitmap,x-off.x,y-off.y,sprite->source,Rect{0,0,640,400});
        } else {
            bitmap(out,p::kRetailPaletteSheets[static_cast<int>(sel.mode)],x-17,y-30,p::retail_palette_thumbnail(sel.page,sel.variant));
        }
    }else if(controls_.cursor_mode==5)bitmap(out,"Data/SubGameDYP/delcurs.bmp",x,y);
    return true;
}
}
