#include "btb/retail_media_game_closure.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

using namespace btb::retail_media;
namespace {
std::vector<int> events;
std::vector<std::string> paths;
void* fake= reinterpret_cast<void*>(0xB1);
bool movie_finished=false;
void volume(void* movie,I32 channel,I32 level) noexcept {
    assert(movie==fake&&channel==0);
    assert(level>=37);
    events.push_back(100+level);
}
void* open(std::string_view name,U32 flags) noexcept {
    assert(flags==0U||flags==0x04000000U);
    paths.emplace_back(name);
    events.push_back(flags==0?1:2);
    return name.find("CD/")==0 ? fake : nullptr;
}
void abort_missing() noexcept {events.push_back(3);}
void disable() noexcept {events.push_back(4);}
void make_surface(const std::array<U32,31>& spec) noexcept {
    assert(spec[0]==0x7c&&spec[1]==7);
    assert(spec[2]==480&&spec[3]==640&&spec[26]==0x40);
    events.push_back(5);
}
void bind(void* ds) noexcept {assert(ds==fake);events.push_back(6);}
void decode(void*) noexcept {events.push_back(7);}
bool lock_ok(void*) noexcept {events.push_back(8);return false;}
bool lock_fail(void*) noexcept {events.push_back(8);return true;}
void copy(void*) noexcept {events.push_back(9);}
void unlock(void*) noexcept {events.push_back(10);}
void stop(void*) noexcept {events.push_back(11);}
void destroy(void*,I32 flag) noexcept {assert(flag==1);events.push_back(12);}
void* create(I32 index) noexcept {assert(index==4);events.push_back(13);return fake;}
void play(void*,I32 pri,U32 flags) noexcept {assert(pri==0);events.push_back(14+static_cast<int>(flags));}
void close(void*) noexcept {events.push_back(16);}
void unregister_surface(void*) noexcept {events.push_back(17);}
void release_surface(void*) noexcept {events.push_back(18);}
void scroll(I32 value) noexcept {assert(value==99);events.push_back(19);}
void open_scene(std::string_view path) noexcept {paths.emplace_back(path);events.push_back(20);}
bool update_movie() noexcept {events.push_back(21);return movie_finished;}
void* open_file(std::string_view path,std::string_view mode) noexcept {
    assert(mode=="rb");paths.emplace_back(path);events.push_back(22);
    return path.find("CD/")==0 ? fake : nullptr;
}
std::uint64_t walk(I32 a,void* graph,void* parent,I32 b,I32 depth,I32 accum) noexcept {
    assert(a==7&&b==7&&graph==fake&&parent==nullptr);
    assert(depth==0&&accum==0);events.push_back(23);return 0x123456789ULL;
}
void sound_volume(void*,I32 value) noexcept {
    assert(value==73);events.push_back(24);
}
bool sequence(std::initializer_list<int> items) {
    return events==std::vector<int>(items);
}
}
int main() {
    // 00409210: no volume when movie missing or paused.
    events.clear();
    apply_global_bink_volume(nullptr,false,0,volume);
    apply_global_bink_volume(fake,true,0,volume);
    assert(events.empty());
    apply_global_bink_volume(fake,false,0,volume);
    assert(sequence({137}));
    // 00408E10: open generic first, fallback CD, apply existing global
    // movie volume even if generic open itself fails.
    events.clear();paths.clear();
    assert(open_bink_movie_with_fallback("movie.bik","CD/",
             open,fake,false,0,volume)==fake);
    assert(paths.size()==2&&paths[0]=="movie.bik"&&paths[1]=="CD/movie.bik");
    assert(sequence({2,2,137}));

    // 00408EB0 global Bink: abort only after both path attempts fail,
    // then apply volume and disable input when fallback succeeds.
    events.clear();paths.clear();
    GlobalBink global{};
    assert(open_global_bink_movie(global,"foo.bik","CD/",open,abort_missing,
           disable,0,volume));
    assert(global.movie==fake&&!global.input_enabled);
    assert(sequence({1,1,137,4}));
    events.clear();
    assert(!open_global_bink_movie(global,"missing.bik","",open,
            abort_missing,disable,0,volume));
    assert(sequence({1,3}));

    // 00408D90: Bink offscreen 640x480 / sound-system init.
    events.clear();BinkInitialize init{};
    initialize_bink_playback(init,make_surface,bind,fake);
    assert(sequence({5,6})&&!init.paused);
    // 00409070: decode, lock, CopyToBuffer, unlock; no frame advance.
    events.clear();
    assert(decode_bink_frame_without_advance(fake,fake,decode,lock_ok,copy,unlock)==0);
    assert(sequence({7,8,9,10}));
    events.clear();
    assert(decode_bink_frame_without_advance(fake,fake,decode,lock_fail,copy,unlock)==1);
    assert(sequence({7,8}));

    // 00428010 activity music stop/delete/create/play, latch=1.
    events.clear();ActivityMusic music{fake,0};
    play_activity_music_by_index(music,4,stop,destroy,create,play);
    assert(sequence({11,12,13,15}));
    assert(music.current==fake&&music.playback_latch==1);

    // 004280D0 walkthrough teardown guarded by offscreen surface.
    events.clear();Walkthrough w{fake,nullptr,99};
    close_walkthrough_movie(w,close,unregister_surface,release_surface,scroll);
    assert(events.empty());
    w.surface=fake;
    close_walkthrough_movie(w,close,unregister_surface,release_surface,scroll);
    assert(sequence({16,17,18,19})&&w.surface==nullptr);

    // 0042A240 scene-set movie: first open, then update. One completion.
    events.clear();paths.clear();
    SceneSetMovie scene{};
    assert(!update_scene_set_movie(scene,open_scene,update_movie));
    assert(scene.stage==1&&scene.input_gate==1);
    assert(paths.size()==1&&paths.front()=="Data/movies/sceneset.bik");
    movie_finished=false;
    assert(!update_scene_set_movie(scene,open_scene,update_movie));
    movie_finished=true;
    assert(update_scene_set_movie(scene,open_scene,update_movie));
    assert(scene.stage==0&&scene.completed_count==1);
    // 0042A1A0 startup per-clip input gate and completion count.
    events.clear();paths.clear();
    StartupMovies startup{};
    startup.count=2;
    const std::array<I32,2> modes{1,0};
    const std::array<std::string_view,2> clips{"one.bik","two.bik"};
    assert(!update_startup_movies(startup,modes,clips,open_scene,update_movie));
    assert(startup.showing&&!startup.input_gate);
    assert(startup.movie_state==1&&paths[0]=="one.bik");
    assert(!update_startup_movies(startup,modes,clips,open_scene,update_movie));
    assert(startup.current_index==1&&startup.movie_state==0);
    assert(!update_startup_movies(startup,modes,clips,open_scene,update_movie));
    assert(startup.input_gate&&paths[1]=="two.bik");
    assert(update_startup_movies(startup,modes,clips,open_scene,update_movie));
    assert(startup.current_index==2&&!startup.showing&&startup.input_gate);

    // 00428470 hovered help uses sound 456/475 and latches repeats.
    HelpHover hover{true,true,12,false,0,false};
    auto h=update_generic_help_hover(hover);
    assert(h.draw_help_button&&h.voice_id==456&&hover.hover_latched);
    h=update_generic_help_hover(hover);
    assert(h.voice_id==-1);
    hover.pointer_pulse_bit4=false;
    h=update_generic_help_hover(hover);
    assert(!hover.hover_latched);
    hover.pointer_pulse_bit4=true;hover.mode=1;
    h=update_generic_help_hover(hover);
    assert(h.voice_id==475);
    hover.cursor_is_default=false;
    h=update_generic_help_hover(hover);
    assert(h.voice_id==-1);

    // 004169A0 corner test. Overlapping rect with no corners inside should
    // NOT match: this is not general rectangle-overlap.
    constexpr Rect rectangle{10,10,20,20};
    static_assert(any_rect_corner_inside({12,12,30,30},rectangle));
    static_assert(!any_rect_corner_inside({0,14,30,16},rectangle));
    static_assert(!any_rect_corner_inside({9,9,21,21},rectangle));
    static_assert(any_rect_corner_inside({20,20,21,21},rectangle));

    // 00406E10 open original then CD fallback with supplied real fopen.
    events.clear();paths.clear();
    assert(open_game_data_with_cd_fallback(
       "data/file.txt","rb","CD/",open_file,abort_missing)==fake);
    assert(paths.size()==2&&paths[1]=="CD/data/file.txt");
    assert(sequence({22,22}));
    events.clear();paths.clear();
    assert(open_game_data_with_cd_fallback(
       "x","rb","",open_file,abort_missing)==nullptr);
    assert(sequence({22,3}));

    // 0041CBE0 and 00423220 both reset global maze search then call
    // their own recursive provider using the initial root arguments.
    events.clear();
    MazeSearchState state{3,7,8};
    assert(find_maze_shortest_path(state,7,fake,99,walk)==0x123456789ULL);
    assert(state.count==0&&state.best_distance==9999999&&state.requested_goal==99);
    state={3,7,8};
    assert(build_spud_maze_graph_path(state,7,fake,37,walk)==0x123456789ULL);
    assert(state.count==0&&state.best_distance==9999999&&state.requested_goal==37);
    assert(sequence({23,23}));

    // 004093C0 original volume route: UI, all game-scene buffers, music,
    // Bink and help sound in that exact order.
    events.clear();
    std::array<void*,3> many{fake,nullptr,fake};
    GlobalGameAudio audio{};
    audio.ui_context=12;audio.activity_sounds=many;
    audio.background_music=fake;audio.help_sound=fake;
    audio.bink_movie=fake;audio.game_volume=73;audio.music_looping=true;
    apply_global_game_volume(audio,stop,sound_volume,play,volume);
    // Two activity groups, then music stop/set/play, movie, help.
    assert(sequence({11,24,11,24,11,24,15,
                    100+btb::bink::bink_volume_from_game_volume(73),24}));
}
