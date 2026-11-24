#include "btb/original_gameplay11.hpp"
namespace btb::original_gameplay11 {
using namespace btb::gameplay_data_20;

[[nodiscard]] Frame advance_golf(
    Golf& s,bool any_managed_playing) {
    Frame f;f.emit(Command::EnableInput);
    if(s.shared.leave_request||s.shared.shutdown) {
        f.emit(Command::UnloadGolf);
        finish_exit(s.shared,0x1c);return f;
    }
    if(s.course_state>0) {
        s.input_blocked=(s.cursor_y>400)?1:0;
        f.emit(Command::DrawGolf);
        f.emit(Command::UpdateGolf);
        if(s.startup_phase==1) {
            if(s.shared.intro_sound_pending) {
                s.shared.intro_sound_pending=false;
                f.emit(Command::StopIntro);
                f.emit(Command::StartMusic,3);
                f.emit(Command::Voice,123,50);
                f.emit(Command::MarkInterruptible,123);
                return f;
            }
            if(!any_managed_playing){
                s.startup_phase=2;f.emit(Command::Voice,101,50);
                f.emit(Command::MarkInterruptible,101);
            }
        }
        return f;
    }
    s.input_blocked=1;
    if(any_managed_playing){f.emit(Command::DrawGolf);return f;}
    if(s.shared.player_progress==0)s.shared.player_progress=1;
    if(s.has_earned_completion)s.shared.movie_completion=3;
    f.emit(Command::PreparePlayAgain);
    s.shared.outer_state=60;s.shared.ui_context=54;
    s.shared.previous_activity=28;
    f.emit(Command::UnloadGolf);
    s.shared.replay_latch=1;f.saves=true;return f;
}

[[nodiscard]] Frame advance_squirrel(
    Squirrel& s,bool managed_playing,U32 random_0_to_2) {
    Frame f;f.emit(Command::EnableInput);
    if(s.shared.leave_request||s.shared.shutdown) {
        f.emit(Command::UnloadSquirrel);
        finish_exit(s.shared,4);return f;
    }
    f.emit(Command::LegacyHook);
    if(s.ending_latch==0) {
        if(s.screen_slide==0){
            f.emit(Command::SquirrelConveyor);
            f.emit(Command::SquirrelDraw);
            f.emit(Command::SquirrelPlacement);
        } else {
            f.emit(Command::SquirrelSlide);
            f.emit(Command::SquirrelDraw);
        }
    } else {
        f.emit(Command::SquirrelDraw);
        if(s.conclusion_voice_phase==0){
            f.emit(Command::StopManagedSounds);
            f.emit(Command::Voice,665+static_cast<I32>(random_0_to_2%3),50);
            s.conclusion_voice_phase=1;
        } else if(s.conclusion_voice_phase==1) {
            if(!managed_playing)++s.conclusion_voice_phase;
        } else {
            s.shared.movie_completion=5;
            f.emit(Command::PreparePlayAgain);
            f.emit(Command::UnloadSquirrel);
            s.shared.outer_state=60;s.shared.ui_context=40;
            s.shared.previous_activity=-1;s.shared.replay_latch=1;
            s.progress_done=true;f.saves=true;
        }
    }
    // Even after finishing it can play voice/music in this SAME original frame.
    if(s.introduction_phase<5){
        if(s.shared.intro_sound_pending){
            f.emit(Command::StartMusic,9);
            f.emit(Command::Voice,643,50);
            f.emit(Command::MarkInterruptible,643);
            s.shared.intro_sound_pending=false;return f;
        }
        if(!managed_playing){
            if(s.introduction_phase==0) {
                f.emit(Command::Voice,632,50);
                f.emit(Command::MarkInterruptible,632);
                ++s.introduction_phase;
            } else if(s.introduction_phase==1){
                f.emit(Command::Voice,644,50);
                f.emit(Command::MarkInterruptible,644);
                s.introduction_phase=5;
            }
        }
    }
    return f;
}

[[nodiscard]] Frame advance_band(Band& s) {
    Frame f;f.emit(Command::EnableInput);
    if(s.shared.leave_request||s.shared.shutdown){
        f.emit(Command::SaveBobsBand);
        f.emit(Command::UnloadBobsBand);
        if(s.shared.shutdown){
            s.shared.shutdown=false;s.shared.previous_activity=42;
            s.shared.outer_state=42;
        }
        if(s.shared.movie_completion!=-1)s.shared.outer_state=64;
        f.saves=true;return f;
    }
    if(s.reset_pending){
        s.composition.clear();
        s.reset_pending=false;
    }
    f.emit(Command::LegacyHook);
    if(s.state==0){
        f.emit(Command::BobsBandDraw);
        f.emit(Command::BobsBandToolbar);
        if(s.click_pulse){
            if(s.hit_region!=-1)
                f.emit(Command::BobsBandClick,static_cast<U32>(s.hit_region));
            if(s.delete_cursor_active&&!s.delete_handled &&
                (s.cursor_y<290||s.cursor_y>400)) {
                s.delete_cursor_active=false;
                f.emit(Command::SetCursorDefault);
            }
            s.delete_handled=false;
        }
        if(!s.delete_cursor_active)f.emit(Command::BobsBandHelp);
    } else if(s.state==1) {
        f.emit(Command::BobsBandDraw);
        f.emit(Command::BobsBandToolbar);
        if(s.click_pulse&&s.hit_region!=-1)
            f.emit(Command::BobsBandCursorClick,static_cast<U32>(s.hit_region));
    } else if(s.state==2) {
        f.emit(Command::BobsBandDraw);
    } else if(s.state==8) {
        s.state=9;
        s.shared.movie_completion=6;s.progress_done=true;
        f.emit(Command::ResetClock);
        f.emit(Command::ResetBandLoop);
        f.emit(Command::PlayBandLoop);
    } else if(s.state==9){
        f.emit(Command::BobsBandPerformance);
        f.emit(Command::BobsBandToolbar);
        // Retail draws an additional play/stop overlay when region 0x16
        // is hovered, and a click terminates playback immediately.
        if(s.hit_region==22&&s.click_pulse){
            f.emit(Command::StopBandLoop);
            s.state=0;
        }
        if(!s.loop_sound_playing)s.state=0;
    } else if(s.state==10){
        f.emit(Command::PreparePlayAgain);
        s.shared.outer_state=60;s.shared.ui_context=46;
        f.emit(Command::SaveBobsBand);
        f.emit(Command::UnloadBobsBand);
        s.shared.replay_latch=0;f.saves=true;
        return f;
    }
    if(s.shared.intro_sound_pending){
        f.emit(Command::StopIntro);
        s.shared.intro_sound_pending=false;
        f.emit(Command::Voice,static_cast<U32>(s.start_voice),50);
        f.emit(Command::MarkInterruptible,static_cast<U32>(s.start_voice));
    }
    return f;
}

[[nodiscard]] std::vector<std::string> load_ui_bitmap_names(std::istream& in){
    return read_source_path_slots(in,kUiBitmapSlots);
}

[[nodiscard]] MovieNames load_startup_completion_movie_names(
    std::istream& start,std::istream& complete){
    return {read_source_path_slots(start,kStartMovieSlots),
            read_source_path_slots(complete,kCompleteMovieSlots)};
}

[[nodiscard]] std::vector<std::string> load_bink_walkthroughs(
    std::istream& in,std::size_t max_records){
    std::vector<std::string> names;
    std::string movie;
    while(names.size()<max_records && in>>movie){
        if(movie.size()>=260)break;
        names.push_back(movie);
    }
    return names;
}

[[nodiscard]] HelpWavSlots load_help_wav_table(std::istream& in) {
    HelpWavSlots out{};
    std::string wav;I32 index{};
    while(in>>wav>>index) {
        if(index==-1)break;
        if(index<0||index>=100||wav.size()>=260)continue;
        out[static_cast<std::size_t>(index)]=wav;
    }
    return out;
}

[[nodiscard]] CreditsLoad load_credit_slideshow_names(
    std::istream& in) {
    CreditsLoad out{};
    out.surfaces=read_source_path_slots(in,10);
    return out;
}

[[nodiscard]] bool save_unload_fireworks(
    ResourceState& s,const btb::fireworks::RetailGrid& grid,
    std::ostream* opened_profile_file) {
    // fwrite before touching bitmap and movie lifetime; fopen may fail.
    if(opened_profile_file){
        btb::fireworks::write_retail_grid(*opened_profile_file,grid);
        if(!*opened_profile_file)return false;
    }
    for(U32 a:{0x0050ab0cU,0x0050ab10U,0x00508c0cU,0x00508ad0U})
        s.bitmap(a);
    for(U32 i=0;i<12;++i) {
        s.bitmap(0x0050a4b8U+4*i);s.bitmap(0x0050a568U+4*i);
    }
    s.sound(0x004fc084U,0x004fc0d4U);
    s.emit(Command::SetCursorDefault);
    s.music_latch=false;s.emit(Command::SetGlobalMusicLatch,0,0);
    if(s.handles[0x0050aaacU]!=0)s.emit(Command::UnloadFireworkMovieBank);
    s.emit(Command::UnloadFireworksEditor);
    return true;
}

[[nodiscard]] bool save_unload_band(
    ResourceState& s,const btb::bobs_band::Composition& composition,
    std::ostream* opened_profile_file,std::ostream* optional_last_file,
    I32 conductor_index){
    for(U32 a:{0x00513f1cU,0x00513f20U,0x00508c0cU})s.bitmap(a);
    for(U32 i=0;i<10;++i)s.bitmap(0x00512718U+4*i);
    s.bitmap(0x005125a8U);
    s.sound(0x004fc084U,0x004fc174U);
    for(U32 i=0;i<8;++i)s.bitmap(0x0051276cU+i*4);
    for(U32 i=0;i<10;++i){
        s.bitmap(0x00512390U+i*4);s.bitmap(0x00512744U+i*4);
    }
    s.emit(Command::SetCursorDefault);
    s.music_latch=false;s.emit(Command::SetGlobalMusicLatch,0,0);
    if(opened_profile_file) {
        const auto raw=composition.encode_le_dwords();
        opened_profile_file->write(reinterpret_cast<const char*>(raw.data()),
            static_cast<std::streamsize>(raw.size()));
        if(!*opened_profile_file)return false;
        if(optional_last_file){
            for(int i=0;i<5;++i)
                optional_last_file->write(
                    reinterpret_cast<const char*>(&conductor_index),4);
            if(!*optional_last_file)return false;
        }
    }
    return true;
}

[[nodiscard]] std::optional<I32> distance_to_spud_screen_node(
    float x,float y,std::span<const std::pair<I32,I32>> nodes,
    I32 screen,I32 local_index) {
    const auto slot=static_cast<std::int64_t>(screen)*30+local_index;
    if(slot<0||static_cast<std::uint64_t>(slot)>=nodes.size())return std::nullopt;
    const auto& [target_x,target_y]=nodes[static_cast<std::size_t>(slot)];
    const auto dx=static_cast<double>(static_cast<I32>(x))-target_x;
    const auto dy=static_cast<double>(static_cast<I32>(y))-target_y;
    return static_cast<I32>(std::sqrt(dx*dx+dy*dy));
}
}
