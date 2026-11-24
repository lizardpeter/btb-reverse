#pragma once
// Bob Builds a Park retail PE32 Ghidra-addressed C++26 source reconstruction.
// Semantic host-neutral models, not binary-identical original function ABIs.
// Source executable SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.

#include "btb/dsutil.hpp"
#include "btb/sound_manager.hpp"
#include "btb/display_manager.hpp"
#include "btb/printing_system.hpp"
#include "btb/input_runtime.hpp"
#include "btb/retail_leaf_runtime.hpp"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>

namespace btb::retail_shared {

using btb::retail_leaf::Rect32;

struct SurfaceDescription {
    std::uint32_t size{0x7c};
    std::uint32_t flags{6};
    std::int32_t height{};
    std::int32_t width{};
};
struct CursorSurfaceState {
    void* current{};
    void* default_surface{};
    Rect32 source{};
};
// 00407AF0: nullptr falls back to default without resetting source rect;
// a live DirectDraw GetSurfaceDesc fills dimensions and the rect resets.
using GetSurfaceDescription = void (*)(void*,SurfaceDescription&) noexcept;
inline void set_cursor_surface(CursorSurfaceState& s,void* surface,
                               GetSurfaceDescription get_desc) noexcept {
    if (surface==nullptr) { s.current=s.default_surface; return; }
    s.current=surface;
    SurfaceDescription desc{};
    if (get_desc) get_desc(surface,desc);
    s.source={0,0,desc.width,desc.height};
}

// 00407960: initial loading cursor + sound replay when latch==1.
// Callbacks stand in for real DirectSound calls; no fake audio generated.
struct BeginLoadingState {
    void* cursor_surface{};
    void* loading_surface{};
    Rect32 source{};
    std::int32_t loading_latch{};
    std::int32_t voice_restart_latch{};
    void* restart_sound{};
};
using SoundCommand = void (*)(void*) noexcept;
inline void begin_loading_cursor(BeginLoadingState& s,
                                 SoundCommand reset,SoundCommand play) noexcept {
    s.source={0,0,100,100};
    s.cursor_surface=s.loading_surface;
    s.loading_latch=1;
    if (s.voice_restart_latch==1) {
        if(reset) reset(s.restart_sound);
        if(play) play(s.restart_sound);
    }
}

// 00407E10: original action plan already recovered at input_runtime.hpp.
[[nodiscard]] constexpr btb::input::AcquireStateStep
direct_input_acquire_step(bool first_device_present,bool frame_active) noexcept {
    return btb::input::acquire_state_step(first_device_present,frame_active);
}

// 004280A0: native ordering Stop; delete; clear global pointer. A valid
// null pointer produces no calls. The legacy deleting destructor uses flag 1.
using DeleteSound = void (*)(void*,std::int32_t) noexcept;
inline void stop_activity_music(void*& current,SoundCommand stop,
                                DeleteSound destroy) noexcept {
    if (current==nullptr) return;
    void* old=current;
    if (stop) stop(old);
    if (current && destroy) destroy(current,1);
    current=nullptr;
}

// 00402CC0: mark slot as stopped then call CSound_Stop/Reset, in that order.
inline void stop_sound_slot(btb::sound::RetailSoundManager32& state,
                            std::size_t slot,void* sound,
                            SoundCommand stop,SoundCommand reset) noexcept {
    if(slot>=btb::sound::kManagedSlotCount) return; // host safety only
    btb::sound::stop_slot_metadata(state,slot);
    if(stop) stop(sound);
    if(reset) reset(sound);
}

// 00402BC0: release managed group and clear priority/state/id and reverse map.
// Original has unchecked array accesses; corrupt ID guarded on modern hosts.
inline void release_sound_slot(btb::sound::RetailSoundManager32& state,
                               std::size_t slot,void* sound,
                               DeleteSound destroy) noexcept {
    if(slot>=btb::sound::kManagedSlotCount) return;
    if (state.buffer_group_ptr32[slot] && destroy) destroy(sound,1);
    const auto id=state.sound_id_by_slot[slot];
    state.buffer_group_ptr32[slot]=0;
    state.priority[slot]=-1;
    state.slot_state[slot]=0;
    if(state.valid_sound_id(id)) state.slot_by_sound_id[static_cast<std::size_t>(id)]=-1;
    state.sound_id_by_slot[slot]=-1;
}

// 00410FD0: flat 6-wide fireworks buffer. The native test does not check
// row boundaries, only that N consecutive slots are -1.
[[nodiscard]] inline bool can_place_firework(
    std::span<const std::int32_t> grid,
    std::int32_t row,std::int32_t column,std::int32_t required) noexcept {
    if(required<=0) return true;
    const std::int64_t index=static_cast<std::int64_t>(row)*6+column;
    if(index<0 || index+required>static_cast<std::int64_t>(grid.size()))
        return false; // host bounds guard
    for(std::int32_t i=0;i<required;++i)
        if(grid[static_cast<std::size_t>(index+i)]!=-1) return false;
    return true;
}
// 00411080: retail first tests placement for TYPE 0, then reads type at cell
// and clears exactly that type's contiguous occupied cells.
[[nodiscard]] inline std::int32_t remove_firework(
    std::span<std::int32_t> grid,std::span<const std::int32_t> type_lengths,
    std::int32_t row,std::int32_t col) noexcept {
    if(type_lengths.empty() ||
       can_place_firework(grid,row,col,type_lengths[0])) return -1;
    const std::int64_t start=static_cast<std::int64_t>(row)*6+col;
    if(start<0||start>=static_cast<std::int64_t>(grid.size())) return -1;
    const auto type=grid[static_cast<std::size_t>(start)];
    if(type<0||type>=static_cast<std::int32_t>(type_lengths.size())) return -1;
    const auto count=type_lengths[static_cast<std::size_t>(type)];
    if(count<1||start+count>static_cast<std::int64_t>(grid.size()))
        return -1; // host guard
    for(std::int32_t i=0;i<count;++i)
        grid[static_cast<std::size_t>(start+i)]=-1;
    return type;
}

// 0041F3A0: original 24-column band grid test checks column boundary inside
// its while condition and fails on a filled slot.
[[nodiscard]] inline bool can_place_grand_opening(
    std::span<const std::int32_t> grid,std::int32_t row,
    std::int32_t column,std::int32_t required) noexcept {
    if(required<=0) return true;
    const std::int64_t start=static_cast<std::int64_t>(row)*24+column;
    if(start<0 || column<0 || start>=static_cast<std::int64_t>(grid.size()))
        return false;
    for(std::int32_t i=0;i<required;++i) {
        const auto index=start+i;
        if(column+i>=24 || index>=static_cast<std::int64_t>(grid.size()) ||
           grid[static_cast<std::size_t>(index)]!=-1) return false;
    }
    return true;
}

// 0040D420: sentinel-terminated array of rectangle records, strict bounds.
[[nodiscard]] constexpr std::int32_t hit_test_park_toolbar(
    std::span<const Rect32> bounds,std::int32_t x,std::int32_t y) noexcept {
    for(std::size_t i=0;i<bounds.size();++i) {
        if(bounds[i].left==-1) break;
        if(btb::retail_leaf::point_inside_rect_exclusive(x,y,bounds[i]))
            return static_cast<std::int32_t>(i);
    }
    return -1;
}

// 0040DC90: 100 pond slots in the original memory range (76-byte stride).
// Separate global side effects are part of the source semantics.
struct PondRecord { std::int32_t primary_type{-1}; };
struct PondGlobals {
    std::int32_t value_00507a28{};
    std::int32_t value_005079fc{};
};
inline void clear_pond_primary_by_selector(
    std::span<PondRecord> records,std::int32_t selector,PondGlobals& globals) noexcept {
    for(auto& r:records) {
        if(selector==0) { r.primary_type=-1; globals.value_00507a28=-1; }
        else if(selector==1 && r.primary_type==7) {
            r.primary_type=-1; globals.value_005079fc=-1;
        } else if(selector==2 && r.primary_type!=7 && r.primary_type<100)
            r.primary_type=-1;
    }
}
// 0040DCE0: category 0/1/2 removed by selector 0, only category 2 when 1.
struct BandstandRecord {
    std::int32_t active_object{-1};
    std::int32_t category{};
};
inline void clear_bandstand_by_category(
    std::span<BandstandRecord> records,std::int32_t selector) noexcept {
    for(auto& r:records) {
        if((selector==0 && r.category>=0 && r.category<=2) ||
           (selector==1 && r.category==2))
            r.active_object=-1;
    }
}

// 0040D780. Sentinel sort layers (-1) sort before normal layers;
// when both are real, the function returns -1 for <=, +1 for >.
// This is NOT a strict ordering for equal layers (as per original).
[[nodiscard]] constexpr std::int32_t compare_park_sort_layer(
    std::int32_t a_type,std::int32_t b_type,
    std::int32_t a_layer,std::int32_t b_layer) noexcept {
    if(a_type==-1) return b_type==-1 ? 0 : -1;
    if(b_type==-1) return 1;
    return a_layer<=b_layer ? -1 : 1;
}

// 00416310. Special entity types (15/16) are ordered first, and
// ordinary entities compare their source depth = bottom-top+originY.
struct HerdingSortEntry {
    std::int32_t category{};
    std::int32_t top{};
    std::int32_t bottom{};
    std::int32_t origin_y{};
};
[[nodiscard]] inline std::int32_t compare_herding_depth(
    const HerdingSortEntry& a,const HerdingSortEntry& b) noexcept {
    if(a.category==15||a.category==16) return -1;
    if(b.category==15||b.category==16) return 1;
    const auto first=static_cast<std::uint32_t>(a.bottom)-
                     static_cast<std::uint32_t>(a.top)+
                     static_cast<std::uint32_t>(a.origin_y);
    const auto second=static_cast<std::uint32_t>(b.bottom)-
                      static_cast<std::uint32_t>(b.top)+
                      static_cast<std::uint32_t>(b.origin_y);
    return std::bit_cast<std::int32_t>(second) <
           std::bit_cast<std::int32_t>(first) ? 1 : -1;
}

// 00425980: exact 9-column indexed atlas with frames width 89, height 133.
[[nodiscard]] constexpr Rect32 squirrel_frame_rect(std::int32_t frame) noexcept {
    const auto x=(frame/9+(frame%9)*4)*89;
    return {x,0,x+89,133};
}

// 00409260/004092A0/00408F50: movie pause/resume/close signal order.
struct GlobalMovieState {
    void* movie{};
    std::int32_t paused{};
    std::int32_t input_gate{};
};
using MoviePause = void (*)(void*,std::int32_t) noexcept;
using MovieVolume = void (*)(void*,std::int32_t,std::int32_t) noexcept;
using MovieClose = void (*)(void*) noexcept;
using MovieVolumeRestore = void (*)() noexcept;
inline void pause_global_movie(GlobalMovieState& s,MoviePause pause,
                               MovieVolume set_volume) noexcept {
    if(!s.movie) return;
    if(pause) pause(s.movie,1);
    s.paused=1;
    if(set_volume) set_volume(s.movie,0,0);
    s.input_gate=1;
}
inline void resume_global_movie(GlobalMovieState& s,MoviePause pause,
                                MovieVolumeRestore apply_volume) noexcept {
    if(!s.movie) return;
    if(pause) pause(s.movie,0);
    s.paused=0;
    if(apply_volume) apply_volume();
    s.input_gate=0;
}
inline void close_global_movie(GlobalMovieState& s,MovieClose close) noexcept {
    if(close) close(s.movie); // retail calls even if null
    s.movie=nullptr;
    s.input_gate=1;
}

// 00402FC0. Constructor zeros ONLY original first five dwords and sets vptr.
constexpr void construct_display_manager(btb::display::RetailDisplayManager32& d) noexcept {
    d.vtable_ptr32=btb::display::kDisplayManagerVtable32;
    d.direct_draw7_ptr32=0;
    d.primary_surface_ptr32=0;
    d.render_surface_ptr32=0;
    d.unused_legacy_surface_ptr32=0;
}

// 00404510. Constructor writes 4 fields without clearing other storage.
constexpr void construct_wave_file(btb::dsutil::RetailCWaveFile32& w) noexcept {
    w.m_pwfx=0; w.m_hmmio=0; w.m_dwSize=0; w.m_bIsReadingFromMemory=0;
}
// 00404DD0. Source-backed original 0x124-byte printer constructor.
constexpr void construct_bitmap_printer(
    btb::printing::RetailBitmapPrinter32& printer) noexcept {
    printer.initialize_retail_defaults();
}

// 00403BB0/00403BC0. CSoundManager contains one pointer-sized original field.
constexpr void construct_sound_manager(
    btb::dsutil::RetailCSoundManager32& manager) noexcept {
    manager.m_pDS=0;
}
using ReleaseCom = void (*)(void*) noexcept;
inline void destruct_sound_manager(
    btb::dsutil::RetailCSoundManager32& manager,void* actual_ds,
    ReleaseCom release) noexcept {
    if (manager.m_pDS && release) release(actual_ds);
    if(manager.m_pDS) manager.m_pDS=0;
}

// 004043F0. CSound::SetVolume calls every real buffer and ORs results.
using SetBufferVolume = std::uint32_t (*)(void*,std::int32_t) noexcept;
struct SoundVolumeBuffer { void* buffer{}; SetBufferVolume set_volume{}; };
[[nodiscard]] inline std::uint32_t sound_set_volume(
    const btb::dsutil::RetailCSound32& sound,
    std::span<const SoundVolumeBuffer> buffers,std::int32_t volume) noexcept {
    if(sound.m_dwNumBuffers>buffers.size()) return
        static_cast<std::uint32_t>(btb::dsutil::kCoENotInitialized);
    std::uint32_t result=0;
    for(std::size_t i=0;i<sound.m_dwNumBuffers;++i) {
        if(!buffers[i].buffer||!buffers[i].set_volume)
            return static_cast<std::uint32_t>(btb::dsutil::kCoENotInitialized);
        result|=buffers[i].set_volume(buffers[i].buffer,volume);
    }
    return result;
}

} // namespace btb::retail_shared
