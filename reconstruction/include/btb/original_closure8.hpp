#pragma once
// Eight original-address source-level reconstructions from the 2002 PE32.
// Native OS/DirectDraw/printing operations remain explicitly required providers.
// These are source models, not original x86 ABI implementations.
#include "btb/player_profiles.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace btb::original_closure8 {
using U32=std::uint32_t;
using I32=std::int32_t;
struct SurfaceState {
 std::map<U32,U32> handles; // exact PE32 global slot address => guest surface/COM handle
 U32& slot(U32 address) { return handles[address]; }
};
class SurfaceIO {
public:
 virtual ~SurfaceIO()=default;
 virtual void unregister(U32 slot_address)=0;
 virtual void release(U32 guest_surface)=0;
};
class ProfileIO:public SurfaceIO {
public:
 // false is equivalent to original fopen failure; original ignores the failure.
 virtual bool write_file(std::string_view filename,std::string_view content)=0;
};
struct ProfileState {
 SurfaceState surfaces;
 btb::profiles::ProfileTable metadata{};
 std::array<btb::progress::Record,btb::profiles::kProfileCount> records{};
};
// 0042D060; registered-slot deletion order and five ASCII player saves.
void save_and_unload_profiles(ProfileState&,ProfileIO&);
class ParkIO:public SurfaceIO {
public:
 virtual void save_park()=0;
 virtual void destroy_park_object(U32 object,U32 deletion_flag)=0;
 virtual void stop_music()=0;
};
struct ParkState {
 SurfaceState surfaces;
 std::array<U32,20> active_objects{};
 std::array<U32,300> object_table{};
 U32 activity_active{1},scene_active{1};
};
// 0040B190; save then object destruction, state reset and bitmap teardown.
void unload_park(ParkState&,ParkIO&);

struct QuitData {
 std::array<I32,8> metadata{};
 std::array<std::string,3> names{};
 // 6 rows of 4 strings; retail compares four fields at +/-0x300 and +/-0x600.
 std::array<std::array<std::string,4>,6> labels{};
};
// 004074F0; parse loaddata/quitsure.txt and clear matching sentinel strings.
void load_quit_confirmation(QuitData&,std::istream&,std::string_view missing_label);

class ProgressIO {
public:
 virtual ~ProgressIO()=default;
 virtual bool any_sound_playing()=0;
 virtual void play_sound(U32 id,U32 priority,U32 arbitration)=0;
 virtual void mark_last_sound_interruptible()=0;
 virtual void draw_background()=0;
 virtual void update_help_button()=0;
 virtual void update_help_hover()=0;
 virtual bool hover_slot(std::size_t original_progress_slot)=0;
 virtual U32 random_bit()=0;
 virtual void draw_star(std::size_t slot,I32 x,I32 y)=0;
 virtual void draw_back_button(bool down,bool hovered)=0;
};
struct ProgressState {
 U32 intro_voice_pending{};
 I32 hover_previous{-1};
 bool back_hover{},back_down{},back_clicked{},quit_requested{};
};
// 00427C30; recovered slots 50..64, star-coordinate table and voice groups.
void update_progress(ProgressState&,const btb::progress::Record&,ProgressIO&);

class RemovedDiscIO {
public:
 virtual ~RemovedDiscIO()=default;
 virtual void destroy_display()=0;
 virtual void show_error(std::string_view message,U32 flags)=0;
 virtual void post_quit(I32)=0;
 virtual void release_input()=0;
 virtual void shutdown_sound()=0;
 virtual void release_sound_manager()=0;
 virtual void unhook_input()=0;
 [[noreturn]] virtual void exit_process(U32 code)=0;
};
struct RemovedDiscState {
 bool shutting_down{},quit_posted{},display_present{},sound_manager_present{},hook_installed{};
 U32 pointer_cleanup{},hardware_active{1};
};
// 00406C00 stops after exit(0); disassembler's following shared tail is 00406CB0.
[[noreturn]] void abort_removed_disc(RemovedDiscState&,RemovedDiscIO&);
struct Drive {bool optical{};std::string volume_label;};
class DiscScanIO {
public:
 virtual ~DiscScanIO()=default;
 virtual void read_error_message_lines(std::array<std::string,6>&)=0;
 virtual Drive drive(char letter)=0;
 virtual void show_missing_disc()=0;
 [[noreturn]] virtual void exit_process(U32)=0;
};
struct DiscState {
 std::array<std::string,6> error_message{};
 std::string expected_volume;
 I32 chosen_drive{-1};
};
// 00406CB0 checks a..z optical drives by volume, not a hardcoded bypass.
void check_disc_volume(DiscState&,DiscScanIO&);

struct PixelCapture {
 U32 width{},height{},pitch_bytes{};
 std::array<U32,3> masks{}; // retail reads separate red, green, blue 16-bit masks
 std::vector<std::uint8_t> pixels; // locked 16-bit buffer, absolute pitch
};
// 00409460; preserve original BMP header byte counts, channel extraction and
// top-to-bottom input walk. Invalid/unsafe guest buffers are rejected.
[[nodiscard]] std::vector<std::uint8_t> export_print_bmp(const PixelCapture&);
struct Bgra {std::uint8_t b{},g{},r{},a{};};
class PrintIO {
public:
 virtual ~PrintIO()=default;
 virtual PixelCapture capture_back_buffer()=0;
 virtual void write_bitmap(std::string_view,std::span<const std::uint8_t>)=0;
 virtual void load_print_bitmap()=0;
 virtual bool create_compatible_dc()=0;
 virtual std::vector<Bgra> get_color_table()=0;
 virtual void create_palette(std::span<const Bgra>)=0;
 virtual void initialize_print_dialog(U32 original_handle)=0;
 virtual bool print_dialog()=0;
 virtual void printer_construct()=0;
 virtual void printer_set_image_and_palette()=0;
 virtual void printer_set_scale(U32)=0;
 virtual void printer_set_document(std::string_view)=0;
 virtual void printer_print()=0;
 virtual void release_dialog_allocations()=0;
 virtual void printer_destruct()=0;
};
// 00409730; calls recovered 00409460 and existing printer provider.
U32 export_and_print(U32 handle,PrintIO&);
}
