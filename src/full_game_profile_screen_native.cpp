#include "btb/full_game_profile_screen_native.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <string_view>
#include <utility>

namespace btb::full_game {
namespace {
constexpr std::string_view kBackground="Data/ui/name signs.bmp";
constexpr std::string_view kBrownSheet="Data/ui/enter name boxed font.bmp";
constexpr std::string_view kBlueSheet="Data/ui/blue name boxed font.bmp";
constexpr std::string_view kBadgeSheet="Data/ui/fonttools.bmp";

constexpr std::array<std::array<const char*,2>,3> kButtonArt{{
    {"Data/ui/helpred.bmp","Data/ui/helpreddep.bmp"},
    {"Data/ui/enternamedeletered.bmp","Data/ui/enternamedeletedep.bmp"},
    {"Data/ui/backred.bmp","Data/ui/backreddep.bmp"},
}};
constexpr std::array<std::array<int,2>,3> kButtonPosition{{
    {{17,425}},{{293,421}},{{576,424}},
}};

bool permitted_activity_file(std::string_view filename) {
    for(std::size_t profile=0;profile<profiles::kProfileCount;++profile)
        for(const auto& member:profiles::activity_files_for_profile(profile))
            if(member==filename)return true;
    return false;
}
}

NativeProfileScreenAdapter::NativeProfileScreenAdapter(
    std::filesystem::path retail_root,std::filesystem::path profile_dir)
    :assets_(std::move(retail_root)),
     profile_dir_(std::move(profile_dir)) {
    for(int i=0;i<6;++i)
        badges_[static_cast<std::size_t>(i)]={
            i*50,0,i*50+50,45
        };
}

bool NativeProfileScreenAdapter::load_font(
    const char* name,std::array<Rect,kRetailFontRows>& out,
    std::string& error) {
    const auto asset=assets_.resolve(name);
    if(!asset.found()) {
        error=std::string("original profile font unavailable: ")+name;
        return false;
    }
    std::ifstream source(asset.absolute_path);
    if(!source) {
        error=std::string("cannot open original font coordinates: ")+name;
        return false;
    }
    for(auto& rect:out) {
        if(!(source>>rect.left>>rect.top>>rect.right>>rect.bottom) ||
           rect.left<0 || rect.top<0 || rect.right<=rect.left ||
           rect.bottom<rect.top || rect.right>65535 ||
           rect.bottom>=65535) {
            error=std::string("invalid original profile font rectangle: ")+name;
            return false;
        }
    }
    error.clear();
    return true;
}

bool NativeProfileScreenAdapter::initialize(std::string& error) {
    if(!load_font("loaddata/fontdatabrown.txt",brown_font_,error) ||
       !load_font("loaddata/fontdatablue.txt",blue_font_,error))
        return false;
    // These are required by the ordinary native path. Do not substitute
    // rendered text, fabricated fonts, or modern vector buttons.
    for(const auto name:{
            kBackground,kBrownSheet,kBlueSheet,kBadgeSheet,
            std::string_view("Data/ui/enter name.bmp"),
            std::string_view("Data/ui/textcursor.bmp")}) {
        if(!assets_.resolve(name).found()) {
            error="original player screen bitmap unavailable: "+std::string(name);
            return false;
        }
    }
    initialized_=true;
    error.clear();
    return true;
}

bool NativeProfileScreenAdapter::begin_frame(
    const ActivityFrameInput& input,
    ActivityFrameOutput& output,std::string& error) {
    if(!initialized_) {
        error="source profile screen cannot run without original font assets";
        return false;
    }
    input_=input;
    output_=&output;
    frame_error_.clear();
    backing_present_=input.backing_track_playing;
    error.clear();
    return true;
}

bool NativeProfileScreenAdapter::end_frame(std::string& error) {
    output_=nullptr;
    if(!frame_error_.empty()) {
        error=std::exchange(frame_error_,{});
        return false;
    }
    error.clear();
    return true;
}

void NativeProfileScreenAdapter::draw_background() {
    output_->draws.push_back({std::string(kBackground),0,0});
}
void NativeProfileScreenAdapter::update_help_button() {}
void NativeProfileScreenAdapter::update_help_hover(bool) {}
void NativeProfileScreenAdapter::enable_input() {}
bool NativeProfileScreenAdapter::any_managed_playing() {
    return input_.managed_sound_playing;
}
bool NativeProfileScreenAdapter::sound_playing(int id) {
    // These five source IDs are measured against the original reverse
    // ID-to-slot map and observed DirectSound status on every native frame.
    // In particular, 0x135 empty-slot hover narration must NOT masquerade
    // as the 0x12f/0x130/0x131 click blockers.
    for(std::size_t i=0;i<kProfileGateSoundIds.size();++i)
        if(kProfileGateSoundIds[i]==id)
            return input_.profile_sound_playing[i];
    frame_error_="original profile queried unobserved sound ID "+
                 std::to_string(id);
    return false;
}
unsigned NativeProfileScreenAdapter::next_random() {
    return static_cast<unsigned>(input_.random_value);
}
void NativeProfileScreenAdapter::play_voice(
    int id,int priority,int arbitration_class) {
    output_->audio.push_back({
        AudioOperation::ManagedSoundId,{},id,priority,arbitration_class
    });
}
void NativeProfileScreenAdapter::stop_managed() {
    output_->audio.push_back({AudioOperation::StopManagedSounds});
}
void NativeProfileScreenAdapter::mark_last_voice_interruptible() {
    if(!output_->audio.empty() &&
       output_->audio.back().operation==AudioOperation::ManagedSoundId)
        output_->audio.back().input_interruptible=true;
    else frame_error_="original profile interrupted voice has no source sound";
}
int NativeProfileScreenAdapter::glyph_width(int code) {
    if(code<0 || static_cast<std::size_t>(code)>=brown_font_.size()) {
        frame_error_="original profile name glyph index is out of range";
        return 0;
    }
    const auto& rect=brown_font_[static_cast<std::size_t>(code)];
    return rect.right-rect.left;
}
void NativeProfileScreenAdapter::draw_glyph(int code,int x,int y) {
    if(code<0 || static_cast<std::size_t>(code)>=brown_font_.size()) {
        frame_error_="original profile glyph draw index is invalid";
        return;
    }
    auto rect=brown_font_[static_cast<std::size_t>(code)];
    ++rect.bottom;
    output_->draws.push_back({
        std::string(kBrownSheet),x,y,rect,true
    });
}
void NativeProfileScreenAdapter::draw_badge(int badge,int x,int y) {
    if(badge<0 || badge>=6) {
        frame_error_="original profile badge index is invalid";
        return;
    }
    output_->draws.push_back({
        std::string(kBadgeSheet),x,y,badges_[static_cast<std::size_t>(badge)],true
    });
}
void NativeProfileScreenAdapter::draw_button(int button,bool pressed) {
    if(button<5 || button>7)return;
    const auto index=static_cast<std::size_t>(button-5);
    const auto pos=kButtonPosition[index];
    output_->draws.push_back({
        kButtonArt[index][pressed?1:0],pos[0],pos[1],{},true
    });
}
void NativeProfileScreenAdapter::restore_quit_snapshot() {
    // The shared quit confirmation UI, rather than the profile screen,
    // owns the modal. The GameRoot already has an original Yes/No gate.
    output_->open_yes_no_confirmation=true;
}
void NativeProfileScreenAdapter::truncate_activity_file(
    const std::string& filename) {
    if(!permitted_activity_file(filename)) {
        frame_error_="profile deletion requested an unknown source save file";
        return;
    }
    const auto path=profile_dir_/filename;
    std::ofstream output(path,std::ios::binary|std::ios::trunc);
    if(!output || !output.flush())
        frame_error_="cannot truncate original per-profile activity file";
}
bool NativeProfileScreenAdapter::backing_present() {
    return backing_present_;
}
void NativeProfileScreenAdapter::stop_backing() {
    if(backing_present_)
        output_->audio.push_back({AudioOperation::StopBackingTrack});
    backing_present_=false;
}
void NativeProfileScreenAdapter::release_backing() {
    backing_present_=false;
}
void NativeProfileScreenAdapter::open_delete_overlay() {
    output_->draws.push_back({"Data/ui/deletegame.bmp",0,0});
    output_->open_yes_no_confirmation=true;
}
void NativeProfileScreenAdapter::use_delete_cursor(bool on) {
    delete_cursor_=on;
    // The original Win32 cursor is a separate real HCURSOR resource.
    // No fake persistent cursor sprite is submitted by this bridge.
}
} // namespace btb::full_game
