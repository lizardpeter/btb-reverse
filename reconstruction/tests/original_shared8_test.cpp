#include "btb/original_shared8.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
// Keep all source tests live under Win32/Win64 Release NDEBUG.
#undef assert
#define assert(condition) do { \
    if(!(condition)){ \
        std::fprintf(stderr,"original shared UI test failed: %s at %s:%d\\n", \
                    #condition,__FILE__,__LINE__); \
        std::abort(); \
    } \
} while(false)
#include <sstream>
#include <string>
#include <vector>
using namespace btb::original_shared8;
static int count(const Plan& plan,Kind kind){
    return static_cast<int>(std::count_if(plan.effects.begin(),plan.effects.end(),
      [&](const Effect& e){return e.kind==kind;}));
}
static bool has(const Plan& p,Kind k,I32 arg){
    return std::any_of(p.effects.begin(),p.effects.end(),
      [&](const Effect& e){return e.kind==k&&e.address==arg;});
}
int main(){
    // 00429150 and 00428ED0 resource ownership from *original* UI table.
    btb::front_end::ReplacementScreen authored{};
    btb::front_end::ReplacementRecord first{};
    first.hover_bitmap_base="UI/hover";
    first.secondary_bitmap_base="UI/pressed";
    first.hover_frame_count=2;
    first.secondary_frame_count=1;
    authored.records.push_back(first);
    btb::front_end::ReplacementRecord second{};
    second.hover_frame_count=1;second.secondary_frame_count=0;
    authored.records.push_back(second);
    UiScreen screen{};screen.index=1;
    auto events=btb::original_shared8::load_generic_ui(screen,authored);
    assert(!events.error&&screen.menu_selection==-1);
    assert(screen.records.size()==2&&screen.original_loaded_count==3);
    assert(count(events,Kind::LoadBitmap)==6);
    assert(count(events,Kind::RegisterBitmap)==6);
    assert(count(events,Kind::ColorKeyBitmap)==1);
    assert(events.effects.front().kind==Kind::BeginLoading);
    assert(events.effects.back().kind==Kind::EndLoading);
    screen.records[0].primary[0]=5;
    screen.records[0].secondary[0]=6;
    screen.records[0].hover_bitmap=7;
    screen.records[0].pressed_bitmap=8;
    events=btb::original_shared8::unload_generic_ui(screen);
    assert(count(events,Kind::UnregisterBitmap)==6);
    assert(count(events,Kind::ReleaseBitmap)==4);
    assert(events.effects.back().kind==Kind::StopAllManaged);
    assert(screen.records[0].primary[0]==0);
    assert(screen.records[0].secondary[0]==0);
    assert(screen.original_loaded_count==4);
    UiScreen no_op{};no_op.index=12;
    events=btb::original_shared8::unload_generic_ui(no_op);
    assert(events.effects.empty());
    UiScreen rejected{};rejected.index=1;
    authored.records[0].hover_frame_count=999;
    events=btb::original_shared8::load_generic_ui(rejected,authored);
    assert(events.error);

    // 00429800 option slider x87 conversion provider, same-volume skip,
    // and sound-queued close branch.
    Options opts{};
    opts.volume_bounds={100,120,250,180};
    opts.close_bounds={260,200,350,260};
    opts.button_down=true;opts.cursor_x=140;opts.cursor_y=150;
    events=btb::original_shared8::update_options(opts,110);
    assert(opts.slider_position==110&&opts.audio_volume==420);
    assert(count(events,Kind::ApplyVolume)==1);
    events=btb::original_shared8::update_options(opts,110);
    assert(count(events,Kind::ApplyVolume)==0);
    opts.button_down=false;
    opts.cursor_x=300;opts.cursor_y=225;opts.mouse_press=true;
    events=btb::original_shared8::update_options(opts,110);
    assert(opts.close_sound_queued);
    assert(has(events,Kind::PlayManaged,862));
    assert(count(events,Kind::MarkInterruptible)==1);
    opts.mouse_press=false;opts.sound_playing=false;opts.restore_cursor=true;
    events=btb::original_shared8::update_options(opts,110);
    assert(!opts.close_sound_queued);
    assert(count(events,Kind::ResetMouseBounds)==1);
    assert(count(events,Kind::ResumeBink)==1);
    Options skip{};skip.phase=2;
    events=btb::original_shared8::update_options(skip,100);
    assert(events.early_return&&count(events,Kind::DrawOverlay)==0);

    // 00429AA0 actual delayed Quit after voice, not at button click.
    Quit quit{};
    quit.buttons={{{220,260,305,316},{316,260,390,316}}};
    quit.x=250;quit.y=290;quit.down=true;quit.press=true;
    events=btb::original_shared8::update_quit(quit,2);
    assert(quit.voice_queued&&!quit.leave_requested);
    assert(has(events,Kind::PlayManaged,628));
    assert(count(events,Kind::StopAllManaged)==1);
    quit.press=false;quit.down=false;quit.managed_playing=true;
    events=btb::original_shared8::update_quit(quit,0);
    assert(!quit.leave_requested);
    quit.managed_playing=false;events=btb::original_shared8::update_quit(quit,0);
    assert(quit.leave_requested&&count(events,Kind::SetLeaveRequest)==1);
    Quit cancel{};
    cancel.buttons=quit.buttons;cancel.x=350;cancel.y=285;
    cancel.press=true;events=btb::original_shared8::update_quit(cancel,0);
    assert(!cancel.voice_queued&&cancel.phase==0);
    cancel.phase=3;events=btb::original_shared8::update_quit(cancel,0);
    assert(events.early_return&&count(events,Kind::ResumeBink)==1);

    // 00429C90: source modal context underlying activity, inclusive hit,
    // same-button press/release gate and source surface lifetime.
    YesNo modal{};
    modal.context=3;modal.buttons={{{100,100,180,140},{220,100,300,140}}};
    modal.x=100;modal.y=100;modal.bitmap_loaded=true;
    events=btb::original_shared8::update_yes_no(modal);
    assert(count(events,Kind::DrawBackground)==1);
    assert(modal.pressed_response==0&&modal.hover_response==1);
    modal.down=true;modal.press=true;
    events=btb::original_shared8::update_yes_no(modal);
    assert(modal.reset_requested&&modal.result==1);
    assert(count(events,Kind::ResetMouseBounds)==1);
    YesNo no{};
    no.context=1;no.buttons=modal.buttons;no.x=250;no.y=120;
    events=btb::original_shared8::update_yes_no(no);
    assert(no.pressed_response==1);
    no.down=true;no.press=true;
    events=btb::original_shared8::update_yes_no(no);
    assert(!no.reset_requested&&no.result==0);
    YesNo close{};
    close.phase=2;close.bitmap_loaded=true;
    events=btb::original_shared8::update_yes_no(close);
    assert(events.early_return&&close.phase==0&&!close.bitmap_loaded);
    assert(has(events,Kind::ReleaseBitmap,0x0051c294));

    // 0042DB70 glyph widths, centered x=322, 33-frame blinking cursor.
    ProfileNamePopup popup{};
    popup.selected_profile=2;
    popup.glyphs={{'B',0,12,0,30},{'o',12,20,0,30}};
    events=btb::original_shared8::draw_profile_name(popup);
    assert(has(events,Kind::DrawOverlay,296));
    assert(has(events,Kind::DrawGlyph,312));
    assert(popup.next_x==332);
    assert(count(events,Kind::DrawCursor)==1);
    popup.blink_tick=16;
    events=btb::original_shared8::draw_profile_name(popup);
    assert(popup.blink_tick==17&&count(events,Kind::DrawCursor)==0);
    popup.blink_tick=32;
    events=btb::original_shared8::draw_profile_name(popup);
    assert(popup.blink_tick==0&&count(events,Kind::DrawCursor)==1);

    // 00423BE0 Bentley anim phases: source 9-tick frame stride,
    // 4 voice branches and completion gated on managed audio.
    Bentley narr{};
    narr.phase=0;narr.frame=7;narr.delay=1;narr.managed_playing=false;
    auto [done,narration]=btb::original_shared8::update_bentley(narr);
    assert(!done&&narr.phase==1&&narr.frame==12);
    narr.phase=2;narr.frame=39;narr.delay=1;narr.loops=0;
    narr.repairs_remaining=0;
    auto next=btb::original_shared8::update_bentley(narr);
    assert(!next.first&&narr.phase==3&&narr.frame==44);
    assert(has(next.second,Kind::PlayManaged,715));
    narr.managed_playing=false;narr.random=1;
    next=btb::original_shared8::update_bentley(narr);
    assert(narr.phase==4&&has(next.second,Kind::PlayManaged,727));
    next=btb::original_shared8::update_bentley(narr);
    assert(next.first);
    Bentley pre{};
    pre.phase=-1;pre.delay=1;pre.frame=20;pre.scroll_x=151;
    next=btb::original_shared8::update_bentley(pre);
    assert(pre.phase==0&&pre.frame==0);
    assert(has(next.second,Kind::PlayManaged,713));

    // 004281D0: preserve source-authored Bink 7MB cap + 31 word descriptor.
    Walkthrough movie{};movie.paths={"A.bik","B.bik"};
    events=btb::original_shared8::open_walkthrough(movie,1,
      [&](const std::string& name,std::size_t cap)
       ->std::optional<std::vector<U8>> {
          assert(name=="B.bik"&&cap==7000000);
          return std::vector<U8>{0x42,0x49,0x4e,0x4b};
       });
    assert(!events.error&&movie.movie_opened&&movie.selected==1);
    assert(movie.owned_bytes.size()==4);
    assert(movie.surface_description[0]==124);
    assert(movie.surface_description[1]==7);
    assert(movie.surface_description[2]==179);
    assert(movie.surface_description[3]==282);
    assert(movie.surface_description[26]==64);
    assert(count(events,Kind::CreateBinkSurface)==1);
    assert(count(events,Kind::OpenBinkMovie)==1);
    events=btb::original_shared8::open_walkthrough(movie,9,{});
    assert(events.error);
    events=btb::original_shared8::open_walkthrough(movie,0,[](const std::string&,std::size_t)
       ->std::optional<std::vector<U8>>{return std::nullopt;});
    assert(events.error);

}
