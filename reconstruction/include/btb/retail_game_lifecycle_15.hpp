#pragma once
// Fifteen original Bob Builds a Park gameplay functions, dependency-batched.
// Evidence: analysis/ghidra_main_exe/functions/00XXXXXX.c
// Retail exe SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// The models execute the original state/loop ordering, writing source-neutral
// actions for the existing native host to handle. They are NOT a DirectDraw,
// Bink, sound, input or x86 ABI implementation and not retail-differential proof.

#include "btb/park_designer_data.hpp"
#include "btb/spud_skate_runtime.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <istream>
#include <ostream>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace btb::game_batch_15 {
using I32=std::int32_t;
using U32=std::uint32_t;

enum class Op : std::uint8_t {
    UnregisterBitmap, ReleaseBitmap, CloseBink, FreeMovieBuffer,
    DeleteSound, ResetCursor, StopMusic, EnableInput,
    UpdatePlayback, StopIntroVoice, StartMusic, ManagedSound,
    PreparePlayAgain, UnloadSkate, UnloadMaze, SharedCompletion
};
struct Action { Op kind{}; U32 address{}; I32 argument{}; friend bool operator==(const Action&,const Action&)=default; };
struct ActivityResources {
    // Authenticated original PE32 addresses of global resource slots. A
    // nonzero 32-bit value is an original handle; no fake handles are created.
    std::unordered_map<U32,U32> slot{};
    std::vector<Action> actions{};
    bool activity_music_playing{};
    bool paused{};
    I32 audio_latch{}; // 00519940
    I32 shared_ui_latch{}; // 0051C334
    I32 herding_character_latch_a{}; // 0051071c
    I32 herding_character_latch_b{}; // 00510720
    I32 skate_input_blocked{}; // 0043ed18
    void emit(Op kind,U32 address=0,I32 argument=0) {
        actions.push_back({kind,address,argument});
    }
    [[nodiscard]] U32 handle(U32 address) const noexcept {
        auto it=slot.find(address);return it==slot.end()?0:it->second;
    }
    void bitmap(U32 address) {
        // Retail calls UnregisterBitmapSurface on *every* address, including
        // those whose IDirectDrawSurface7* is currently null.
        emit(Op::UnregisterBitmap,address);
        if(auto original=handle(address);original!=0) {
            emit(Op::ReleaseBitmap,address,static_cast<I32>(original));
            slot[address]=0;
        }
    }
    void bitmap_range(U32 first,U32 count) {
        for(U32 i=0;i<count;++i)bitmap(first+4*i);
    }
    void sound_slots() {
        // DAT_004FC084..0x004FC0D3: twenty 32-bit CSound* slots.
        for(U32 addr=0x004fc084U;addr<0x004fc0d4U;addr+=4) {
            if(auto original=handle(addr);original!=0) {
                emit(Op::DeleteSound,addr,1);slot[addr]=0;
            }
        }
    }
    void finish_music() {
        audio_latch=0;activity_music_playing=false;emit(Op::StopMusic);
    }
};

// 004110E0 - exact editor surfaces: four global resource slots followed by
// six pairs (12 slots), first half at 0x50A49C, alternating with 0x50A4A0.
inline void unload_fireworks_editor(ActivityResources& s) {
    for(U32 a:{0x0050ab18U,0x0050ab1cU,0x0050aaa8U,0x0050ab08U})
        s.bitmap(a);
    s.bitmap_range(0x0050a49cU,12);
}

// 004111C0 - 23 original Bink handles with matching surface and movie buffer.
// Each movie must be closed before its surface unregister/release and buffer
// cleanup. The pointer-to-Bink variable is set to zero by the original.
inline void unload_fireworks_movie_bank(ActivityResources& s) {
    for(U32 i=0;i<23;++i) {
        const U32 handle_slot=0x0050aaacU+i*4U;
        const U32 bitmap_slot=0x0050937cU+i*4U;
        const U32 buffer_slot=0x0050a50cU+i*4U;
        s.emit(Op::CloseBink,handle_slot,static_cast<I32>(s.handle(handle_slot)));
        s.bitmap(bitmap_slot);
        s.emit(Op::FreeMovieBuffer,buffer_slot,static_cast<I32>(s.handle(buffer_slot)));
        s.slot[handle_slot]=0;
        // FUN_00430D2A has host ownership of the source memory allocation.
    }
    for(U32 a:{0x0050a4b4U,0x0050a5ccU,0x0050a5c4U,0x0050a5c8U})s.bitmap(a);
}

// 0040A530 - dinosaur activity teardown: native bitmap, source-defined
// variable movie/dinosaur surface count, managed sound array, cursor and music.
inline void unload_dino(ActivityResources& s,I32 dino_surface_count) {
    for(U32 a:{0x004fc9e8U,0x0050a5c4U,0x0050a5c8U})s.bitmap(a);
    for(I32 i=0;i<dino_surface_count;++i)
        s.bitmap(0x004fc9ecU+static_cast<U32>(i)*4U);
    s.sound_slots();
    for(U32 a:{0x004fc2acU,0x004fc434U,0x00508ad0U})s.bitmap(a);
    s.emit(Op::ResetCursor);
    s.finish_music();
}

// 00419390 - herding teardown; note original last thirteen surfaces have a
// non-monotonic 0x510738 placement relative to 0x510740..44.
inline void unload_herding(ActivityResources& s) {
    for(U32 a:{0x00510724U,0x00510728U})s.bitmap(a);
    s.sound_slots();
    for(U32 a:{0x0051072cU,0x00510730U,0x00510734U,0x0051073cU,
               0x00510740U,0x00510744U,0x00510738U,0x00510748U,
               0x0051074cU,0x00510750U,0x00510754U,0x00510758U,0x0051075cU})
        s.bitmap(a);
    s.bitmap_range(0x005105a0U,3);
    s.herding_character_latch_a=0;
    s.herding_character_latch_b=0;
    s.finish_music();
}

// 0041A390 - Maze releases ordered 3 contiguous, 6 discrete and 20 sounds.
// Unlike some other activities, final 0x51C334 global is also cleared.
inline void unload_maze(ActivityResources& s) {
    s.bitmap_range(0x00512100U,3);
    for(U32 a:{0x0051210cU,0x00512110U,0x00512120U,
               0x00512114U,0x00512118U,0x0051211cU,0x005144d0U})
        s.bitmap(a);
    s.sound_slots();s.finish_music();s.shared_ui_latch=0;
}

// 00414410 - Golf: one header slot, then 3 interleaved arrays of 3,
// followed by eight stand-alone surfaces, then 20 sounds.
inline void unload_golf(ActivityResources& s) {
    s.skate_input_blocked=1; // shared 0043ED18 input-block latch
    s.bitmap(0x0050aedcU);
    for(U32 i=0;i<3;++i) {
        s.bitmap(0x0050ab88U+i*4);
        s.bitmap(0x0050adfcU+i*4);
        s.bitmap(0x0050addcU+i*4);
    }
    for(U32 a:{0x0050aee0U,0x0050aee4U,0x0050aee8U,0x0050aef0U,
               0x0050aef4U,0x0050aef8U,0x0050adecU,0x0050aeecU})
        s.bitmap(a);
    s.sound_slots();s.emit(Op::ResetCursor);s.finish_music();
}

// 00424100 - Spud Skate: primary surface; managed sounds; 5 Bink source
// streams; overlay/score/end resources. Retail does not stop music here.
inline void unload_spud_skate(ActivityResources& s) {
    s.skate_input_blocked=1;
    s.bitmap(0x00514ea0U);
    s.sound_slots();
    s.audio_latch=0;
    for(U32 i=0;i<5;++i) {
        const U32 movie=0x00514b2cU+i*4;
        s.emit(Op::CloseBink,movie,static_cast<I32>(s.handle(movie)));
        s.bitmap(0x00514e8cU+i*4);
        s.emit(Op::FreeMovieBuffer,0x00514620U+i*4,
               static_cast<I32>(s.handle(0x00514620U+i*4)));
    }
    for(U32 a:{0x005148b4U,0x00514b60U,0x00514b64U,0x00514b68U})
        s.bitmap(a);
}

// 00424F00 - Squirrel activity: 4 lead surfaces, 12 more; 20 managed
// sound slots; cursor reset and stop music (correct order).
inline void unload_squirrel(ActivityResources& s) {
    for(U32 a:{0x00515068U,0x0051506cU,0x00515070U,0x00515074U,
               0x0051509cU,0x005150a0U,0x005150a4U,0x005150a8U,
               0x005150acU,0x005150b0U,0x005150b4U,0x005150b8U,
               0x005150bcU,0x005150c0U,0x005150c4U,0x005150c8U})
        s.bitmap(a);
    s.sound_slots();s.emit(Op::ResetCursor);s.finish_music();
}

// 00420360 - Spud Maze: ten first surfaces, ten following distinct slots,
// 20 sound slots, five groups each with variable 0..11 elements.
inline bool unload_spud_maze(
    ActivityResources& s,const std::array<I32,5>& family_counts) {
    // Don't partially unload if the external global counts are invalid.
    for(const auto n:family_counts)if(n<0||n>11)return false;
    s.bitmap_range(0x005144e4U,5);
    for(U32 a:{0x005144f8U,0x005144fcU,0x0051450cU,0x00514500U,
               0x00514504U,0x00514508U,0x00514494U,0x005144d0U,
               0x005144b8U,0x005144c4U})
        s.bitmap(a);
    s.sound_slots();
    for(U32 group=0;group<5;++group)
        s.bitmap_range(0x005141ecU+group*44U,
                       static_cast<U32>(family_counts[group]));
    s.finish_music();
    return true;
}

// 0040AC90: precise 400x76 + 28x4 binary save, as tracked in the original
// persistent global list. A null FILE* means no writes. std::ostream is the
// native file-open provider and retains its errors rather than inventing data.
[[nodiscard]] bool save_park_designer(
    const park_designer::SaveData& data,std::ostream* file);

// 0040AEA0: load exactly 60 retail polygon arrays before optional binary
// save. A missing save is legitimate. Loaded Winter=1 invokes ApplySeason;
// truncated saves are an error, not a synthetic blank park.
struct ParkLoadOutcome {
    bool polygons_loaded{};
    bool save_loaded{};
    bool applied_winter{};
    std::string error{};
};
using SeasonCallback=std::function<bool(park_designer::Season)>;
[[nodiscard]] ParkLoadOutcome load_park_designer(
    std::istream& original_polygons,std::istream* optional_save,
    park_designer::SaveData& data,park_designer::BoundAreas& bounds,
    const SeasonCallback& apply);

// 0040DBE0: native code chooses surface array index from object_code
// (codes >99 resolve to surface 0), calls GetSurfaceDesc twice, then
// overwrites record geometry with 6x124 and looks up 0x0043FAC4 sort depth.
// Native surface calls remain explicit, not fake descriptions.
using SurfaceQuery=std::function<bool(U32 native_surface_bank_index)>;
[[nodiscard]] bool refresh_park_geometry(
    park_designer::SaveData& data,U32 slot,
    std::span<const I32> depth_lookup,const SurfaceQuery& query);

// 00412BD0/00412D50: original Fireworks palette, remove and place input
// actions. Event list is fed into the existing editor and native sound host.
enum class FireAction : std::uint8_t {
    CursorFirework, CursorDefault, SelectPalette, Sound,
    RemoveCell, PlaceCell, DeleteAllConfirm, StartShow, ClearPlacementTool
};
struct FireEvent {
    FireAction kind{};
    I32 a{},b{},c{};
    friend bool operator==(const FireEvent&,const FireEvent&)=default;
};
struct FireState {
    std::array<I32,18> cells{};
    I32 selected_type{};
    I32 bob_actor_state{};
    I32 wendy_actor_state{};
    I32 editor_state{};
    I32 cursor_type{-1};
    I32 actor_follow_bias{};
    bool open_delete_all{};
    bool cursor_cleared{};
    std::vector<FireEvent> events{};
    FireState() {cells.fill(-1);}
    void emit(FireAction kind,I32 a=0,I32 b=0,I32 c=0) {
        events.push_back({kind,a,b,c});
    }
};
using RemoveCell=std::function<void(I32 row,I32 column)>;
using PlaceCell=std::function<bool(I32 row,I32 col,I32 firework_type)>;
inline constexpr I32 kFirePaletteSlots=12;
// Ghidra's DecodeSelectedFireworkGridCell uses the original pointer to
// a decoded row/col. Caller passes those actual values when control==12.
inline void begin_fireworks_action(
    FireState& s,I32 control,I32 selected_row,I32 selected_col,U32 random,
    const RemoveCell& remove) {
    if(control>=0&&control<12) {
        s.cursor_type=control;
        s.emit(FireAction::CursorFirework,control);
        if(control<8) {
            if(s.bob_actor_state==0) {
                s.emit(FireAction::Sound,862+static_cast<I32>(random%5U),50,2);
                s.selected_type=control;s.bob_actor_state=1;
            }
        } else if(s.wendy_actor_state==0) {
            s.emit(FireAction::Sound,899+static_cast<I32>(random%5U),50,2);
            s.selected_type=control;s.wendy_actor_state=1;
        }
        return;
    }
    if(control==12) {
        if(selected_row>=0&&selected_row<3&&selected_col>=0&&selected_col<6) {
            const auto item=s.cells[static_cast<std::size_t>(selected_row*6+selected_col)];
            if(item>=0&&item<12) {
                s.emit(FireAction::RemoveCell,selected_row,selected_col);
                if(remove)remove(selected_row,selected_col);
            }
        }
    } else if(control==25) {
        s.open_delete_all=true;
        s.emit(FireAction::DeleteAllConfirm);
    } else if(control==27) {
        s.editor_state=14;
        s.emit(FireAction::StartShow);
    }
}
inline void complete_fireworks_action(
    FireState& s,I32 control,I32 row,I32 col,U32 random,
    const PlaceCell& place) {
    if(control>=0&&control<12) {
        s.selected_type=control;s.emit(FireAction::SelectPalette,control);
        return;
    }
    if(control!=12)return;
    s.cursor_type=-1;s.emit(FireAction::CursorDefault);
    if(row<0||row>=3||col<0||col>=6)return;
    const I32 current=s.cells[static_cast<std::size_t>(row*6+col)];
    if(current>=0&&current<12)return;
    if(row==2) {
        s.wendy_actor_state=10;
        s.emit(FireAction::Sound,904+static_cast<I32>(random%4U),50,2);
    } else {
        s.bob_actor_state=10;
        s.emit(FireAction::Sound,867+static_cast<I32>(random%4U),50,2);
    }
    s.emit(FireAction::PlaceCell,row,col,s.selected_type);
    if(place&&place(row,col,s.selected_type)) {
        s.actor_follow_bias=0;
        s.cursor_cleared=true;
        s.emit(FireAction::ClearPlacementTool);
    }
}

// 00424D10: exact Spud Skate activity dispatcher, including the surprising
// original shutdown branch calling UnloadMazeActivityResources, not Skate.
// Host performs callbacks in the returned original ordering.
struct SkateLoop {
    I32 phase{};                  // 00514EAC
    I32 cursor_y{};               // 004FBD30
    I32 startup_sound_pending{};  // 0051C30C
    I32 global_leave_flag{};      // 0044DDB0
    I32 global_shutdown{};        // 0051C2E8
    I32 shared_completion{-1};    // 00446F38
    I32 ui_input_blocked{};       // 0043ED18
    I32 outer_state{};            // 0044DE14
    I32 game_selection{};         // 00446CE8
    I32 replay_class{};           // 0051B418
    I32 playback_latch{};         // 0051C2FC
};
[[nodiscard]] inline I32 advance_spud_skate_activity(
    SkateLoop& s,std::vector<Action>& events) {
    events.push_back({Op::EnableInput});
    if(s.global_leave_flag==0&&s.global_shutdown==0) {
        if(s.phase==2) {
            s.ui_input_blocked=1;
            events.push_back({Op::PreparePlayAgain});
            events.push_back({Op::UnloadSkate});
            s.outer_state=0x3c;s.game_selection=0x16;
            s.replay_class=0x1a;s.playback_latch=0;
            return 0;
        }
        s.ui_input_blocked=s.cursor_y>400?1:0;
        events.push_back({Op::UpdatePlayback});
        if(s.startup_sound_pending==1) {
            events.push_back({Op::StopIntroVoice});
            events.push_back({Op::StartMusic,0,8});
            s.startup_sound_pending=0;
            events.push_back({Op::ManagedSound,0,0x2f6});
        }
        return 0;
    }
    s.ui_input_blocked=1;
    events.push_back({Op::UnloadMaze}); // as in retail 00424D10
    if(s.global_shutdown!=0) {
        s.global_shutdown=0;s.outer_state=0x16;
    }
    if(s.shared_completion!=-1)s.outer_state=0x40;
    return 1;
}

} // namespace btb::game_batch_15
