#include "btb/original_shared8.hpp"
namespace btb::original_shared8 {
bool verify_8_original_shared_ui_functions() noexcept {
 btb::front_end::ReplacementScreen authored{};
 btb::front_end::ReplacementRecord record{};
 record.hover_frame_count=2;
 record.secondary_frame_count=1;
 authored.records.push_back(record);
 UiScreen screen{};screen.index=1;
 auto p=original_shared8::load_generic_ui(screen,authored);
 if(p.error||screen.records.size()!=1||screen.original_loaded_count!=2)
  return false;
 screen.records[0].primary[0]=5;
 auto unloaded=original_shared8::unload_generic_ui(screen);
 if(unloaded.effects.empty()||screen.records[0].primary[0]!=0)
  return false;
 Options opt{};opt.volume_bounds={0,0,100,100};
 opt.button_down=true;opt.cursor_x=50;opt.cursor_y=50;
 auto options=original_shared8::update_options(opt,110);
 if(opt.audio_volume!=420||options.effects.empty())return false;
 Quit quit{};quit.buttons={{{0,0,100,100},{100,0,200,100}}};
 quit.x=50;quit.y=50;quit.press=true;
 auto qp=original_shared8::update_quit(quit,0);
 if(!quit.voice_queued||qp.effects.empty())return false;
 YesNo modal{};modal.phase=2;modal.bitmap_loaded=true;
 auto mp=original_shared8::update_yes_no(modal);
 if(!mp.early_return||modal.bitmap_loaded)return false;
 ProfileNamePopup name{};name.selected_profile=2;
 name.glyphs={{'B',0,12,0,30}};
 auto np=original_shared8::draw_profile_name(name);
 if(np.effects.empty()||name.next_x==0)return false;
 Bentley narrator{};narrator.phase=0;narrator.frame=7;
 narrator.delay=1;narrator.managed_playing=false;
 auto bn=original_shared8::update_bentley(narrator);
 if(bn.first||narrator.phase!=1||narrator.frame!=12)return false;
 Walkthrough movie{};movie.paths={"intro.bik"};
 auto wp=original_shared8::open_walkthrough(movie,0,
     [](const std::string&,std::size_t cap)
       ->std::optional<std::vector<U8>>{
        if(cap!=7000000)return std::nullopt;
        return std::vector<U8>{0x42,0x49,0x4e,0x4b};
     });
 return !wp.error&&movie.movie_opened&&
   movie.surface_description[0]==124&&movie.owned_bytes.size()==4;
}
}
