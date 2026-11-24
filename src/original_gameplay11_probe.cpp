#include "btb/original_gameplay11.hpp"
#include <cassert>
// Ensure even Windows Release exercises the source scenarios and fails safely.
#undef assert
#define assert(condition) do {if(!(condition))return false;}while(false)
#include <sstream>
#include <vector>
#include <algorithm>
using namespace btb::gameplay_data_20;
static int count(const Frame& f,Command c){return static_cast<int>(std::count_if(f.effects.begin(),f.effects.end(),[&](const Effect& e){return e.op==c;}));}
static int count(const ResourceState& f,Command c){return static_cast<int>(std::count_if(f.effects.begin(),f.effects.end(),[&](const Effect& e){return e.op==c;}));}
static bool has(const Frame& f,Command c,U32 a){return std::any_of(f.effects.begin(),f.effects.end(),[&](const Effect& e){return e.op==c&&e.address==a;});}
namespace btb::original_gameplay11 {
bool verify_11_gameplay_data_functions() noexcept {
    Golf g{};g.cursor_y=401;g.startup_phase=1;g.shared.intro_sound_pending=true;
    auto f=btb::original_gameplay11::advance_golf(g,false);
    assert(g.input_blocked==1&&has(f,Command::StartMusic,3)&&has(f,Command::Voice,123));
    assert(!g.shared.intro_sound_pending&&count(f,Command::DrawGolf)==1);
    f=btb::original_gameplay11::advance_golf(g,false);assert(g.startup_phase==2&&has(f,Command::Voice,101));
    g.course_state=0;f=btb::original_gameplay11::advance_golf(g,true);
    assert(count(f,Command::DrawGolf)==1&&count(f,Command::UnloadGolf)==0);
    g.has_earned_completion=true;f=btb::original_gameplay11::advance_golf(g,false);
    assert(f.saves&&g.shared.player_progress==1&&g.shared.movie_completion==3);
    assert(g.shared.outer_state==60&&g.shared.ui_context==54);
    Golf exited{};exited.shared.shutdown=true;exited.shared.movie_completion=3;
    f=btb::original_gameplay11::advance_golf(exited,true);
    assert(count(f,Command::UnloadGolf)==1&&exited.shared.outer_state==64);

    Squirrel s{};f=btb::original_gameplay11::advance_squirrel(s,true,0);
    assert(count(f,Command::SquirrelConveyor)==1&&count(f,Command::SquirrelPlacement)==1);
    s.screen_slide=1;f=btb::original_gameplay11::advance_squirrel(s,true,0);
    assert(count(f,Command::SquirrelSlide)==1&&count(f,Command::SquirrelPlacement)==0);
    s.screen_slide=0;s.shared.intro_sound_pending=true;
    f=btb::original_gameplay11::advance_squirrel(s,true,0);assert(has(f,Command::StartMusic,9)&&has(f,Command::Voice,643));
    f=btb::original_gameplay11::advance_squirrel(s,false,0);assert(has(f,Command::Voice,632)&&s.introduction_phase==1);
    f=btb::original_gameplay11::advance_squirrel(s,false,0);assert(has(f,Command::Voice,644)&&s.introduction_phase==5);
    s.ending_latch=1;f=btb::original_gameplay11::advance_squirrel(s,true,2);
    assert(has(f,Command::Voice,667)&&s.conclusion_voice_phase==1);
    f=btb::original_gameplay11::advance_squirrel(s,false,2);assert(s.conclusion_voice_phase==2);
    f=btb::original_gameplay11::advance_squirrel(s,false,2);
    assert(f.saves&&s.progress_done&&s.shared.movie_completion==5);
    assert(s.shared.outer_state==60&&s.shared.ui_context==40);
    Squirrel s_exit{};s_exit.shared.shutdown=true;s_exit.shared.movie_completion=5;
    f=btb::original_gameplay11::advance_squirrel(s_exit,false,0);assert(s_exit.shared.outer_state==64);

    Band b{};b.reset_pending=true;b.hit_region=3;b.click_pulse=true;
    f=btb::original_gameplay11::advance_band(b);assert(!b.reset_pending&&has(f,Command::BobsBandClick,3));
    for(const auto v:b.composition.cells())assert(v==-1);
    b.state=1;f=btb::original_gameplay11::advance_band(b);assert(has(f,Command::BobsBandCursorClick,3));
    b.state=2;f=btb::original_gameplay11::advance_band(b);
    assert(count(f,Command::BobsBandDraw)==1&&count(f,Command::BobsBandToolbar)==0);
    b.state=8;f=btb::original_gameplay11::advance_band(b);assert(b.state==9&&b.progress_done);
    assert(count(f,Command::PlayBandLoop)==1);
    b.loop_sound_playing=true;b.hit_region=22;f=btb::original_gameplay11::advance_band(b);
    assert(b.state==0&&count(f,Command::StopBandLoop)==1);
    b.state=10;f=btb::original_gameplay11::advance_band(b);
    assert(f.saves&&b.shared.outer_state==60&&b.shared.ui_context==46);
    assert(count(f,Command::SaveBobsBand)==1);
    Band b_exit{};b_exit.shared.shutdown=true;b_exit.shared.movie_completion=6;
    f=btb::original_gameplay11::advance_band(b_exit);assert(b_exit.shared.outer_state==64);

    std::ostringstream name_text;
    for(int i=0;i<60;++i)name_text<<"icon"<<i<<".bmp ";
    std::istringstream bitmap_stream(name_text.str());
    auto icons=btb::original_gameplay11::load_ui_bitmap_names(bitmap_stream);
    assert(icons.size()==50&&icons[49]=="icon49.bmp");
    std::istringstream startup("intro.bik menu.bik"),completed("end.bik");
    auto movies=btb::original_gameplay11::load_startup_completion_movie_names(startup,completed);
    assert(movies.start.size()==2&&movies.complete.size()==1);
    std::istringstream walkthrough("first.bik second.bik");
    auto binks=btb::original_gameplay11::load_bink_walkthroughs(walkthrough);
    assert(binks.size()==2&&binks[0]=="first.bik");
    std::istringstream wav("help.wav 5 end.wav -1 ignored.wav 7");
    auto help=btb::original_gameplay11::load_help_wav_table(wav);
    assert(help[5]=="help.wav"&&help[7].empty());
    std::istringstream credits("slide1.bmp slide2.bmp");
    auto slides=btb::original_gameplay11::load_credit_slideshow_names(credits);
    assert(slides.surfaces.size()==2&&slides.slide_delay==240&&slides.start_clock_bias==4);

    btb::fireworks::RetailGrid grid{};
    grid.fill(-1);grid[0]=8;grid[17]=11;
    ResourceState fire{};
    fire.handles[0x0050ab0cU]=19;
    fire.handles[0x0050a4b8U]=20;
    fire.handles[0x004fc084U]=21;
    fire.handles[0x0050aaacU]=22;
    std::ostringstream saved_fire(std::ios::binary);
    assert(btb::original_gameplay11::save_unload_fireworks(fire,grid,&saved_fire));
    assert(saved_fire.str().size()==72);
    std::istringstream restore_fire(saved_fire.str(),std::ios::binary);
    assert(btb::fireworks::read_retail_grid(restore_fire)==grid);
    assert(count(fire,Command::UnregisterSurface)==28);
    assert(count(fire,Command::ReleaseSurface)==2);
    assert(count(fire,Command::DestroySound)==1);
    assert(count(fire,Command::UnloadFireworkMovieBank)==1);
    assert(fire.effects.back().op==Command::UnloadFireworksEditor);

    ResourceState band_resource{};
    band_resource.handles[0x00513f1cU]=5;
    band_resource.handles[0x004fc084U]=6;
    btb::bobs_band::Composition composition;
    assert(composition.place({0,0},3));
    std::ostringstream band_file(std::ios::binary),last_file(std::ios::binary);
    assert(btb::original_gameplay11::save_unload_band(band_resource,composition,&band_file,&last_file,2));
    assert(band_file.str().size()==480&&last_file.str().size()==20);
    assert(count(band_resource,Command::UnregisterSurface)==42);
    assert(count(band_resource,Command::ReleaseSurface)==1);
    assert(count(band_resource,Command::DestroySound)==1);
    assert(band_resource.effects.back().op==Command::SetGlobalMusicLatch);
    assert(last_file.str()[0]==2&&last_file.str()[4]==2);

    std::vector<std::pair<I32,I32>> nodes(60,{0,0});
    nodes[1]={13,4};nodes[31]={103,204};
    assert(btb::original_gameplay11::distance_to_spud_screen_node(10.8f,.8f,nodes,0,1)==5);
    assert(btb::original_gameplay11::distance_to_spud_screen_node(100.9f,200.4f,nodes,1,1)==5);
    assert(!btb::original_gameplay11::distance_to_spud_screen_node(0,0,nodes,2,1));
    return true;
}
}
