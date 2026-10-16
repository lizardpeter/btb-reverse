#include "btb/full_game_dinosaur.hpp"
#include <array>
#include <fstream>
#include <stdexcept>

namespace btb::full_game {
namespace {
std::array<int,2> bitmap_size(const std::filesystem::path& path) {
    std::array<unsigned char,26> header{};
    std::ifstream stream(path,std::ios::binary);
    if (!stream.read(reinterpret_cast<char*>(header.data()),header.size()) ||
        header[0]!='B' || header[1]!='M')
        throw std::runtime_error("invalid original Dinosaur piece bitmap: "+path.string());
    const auto word=[&](int i) {
        return std::uint32_t(header[i]) | (std::uint32_t(header[i+1])<<8) |
            (std::uint32_t(header[i+2])<<16) | (std::uint32_t(header[i+3])<<24);
    };
    const auto width=static_cast<std::int32_t>(word(18));
    auto height=static_cast<std::int32_t>(word(22));
    if (height<0 && height>=-480) height=-height;
    if (word(14)<40 || width<=0 || width>640 || height<=0 || height>480)
        throw std::runtime_error("invalid original Dinosaur piece dimensions");
    return {width,height};
}
}

void DinosaurDriver::configure_menu_state(const RetailMenuState& menu) noexcept {
    if (menu.selected_subgame>=0 && menu.selected_subgame<3)
        species_=menu.selected_subgame;
    if (menu.source_variant>=0 && menu.source_variant<3)
        difficulty_=menu.source_variant;
}

bool DinosaurDriver::initialize(int player,std::string& error) {
    if (initialized_ || player<0 || player>=5) {
        error="Dinosaur initializer requires an unloaded activity and original player slot";
        return false;
    }
    try {
        constexpr std::array<const char*,3> species{"Raptor","Tric","Trex"};
        constexpr std::array<const char*,3> difficulty{"Easy","Medium","Hard"};
        const auto prefix=std::string("Data/SubGameDino/Dino/")+species[species_]+
            "/"+difficulty[difficulty_]+"/";
        const auto level_path=assets_.resolve(prefix+"dino.txt");
        if (!level_path.found()) throw std::runtime_error(level_path.error);
        const auto level=dino::parse_level_file(level_path.absolute_path.string());
        original_coordinators2::DinoState next{};
        next.profile_index=player;
        next.level_variant=species_;
        next.score_index=species_*3+difficulty_;
        next.radius=dino::Runtime::snap_tolerance(next.score_index);
        next.profile_completed.resize(500);
        next.feedback_frames={-1,-1};
        std::vector<std::string> paths;
        for (std::size_t slot=0; slot<level.piece_count(); ++slot) {
            // 0040A670 loads piece<slot>.bmp, then maps the target through
            // the permutation. The bitmap is NOT indexed by permutation.
            auto name=prefix+"piece"+std::to_string(slot)+".bmp";
            const auto resolved=assets_.resolve(name);
            if (!resolved.found()) throw std::runtime_error(resolved.error);
            const auto size=bitmap_size(resolved.absolute_path);
            const auto start=level.start_positions[slot];
            const auto id=level.piece_permutation[slot];
            const auto target=level.target_positions[static_cast<std::size_t>(id)];
            next.pieces.push_back({start.x,start.y,size[0],size[1],
                target.x,target.y,0,1,id,static_cast<std::uint32_t>(slot+1)});
            paths.push_back(std::move(name));
        }
        background_=prefix+"background.bmp";
        if (!assets_.resolve(background_).found())
            throw std::runtime_error("original Dinosaur background missing");
        state_=std::move(next);
        pieces_=std::move(paths);
        animations_={};print_button_={};certificate_.clear();
        carried_surface_=0;progress_emitted_=false;initialized_=true;
        error.clear();return true;
    } catch (const std::exception& e) { error=e.what();return false; }
}

ActivityFrameOutput DinosaurDriver::advance(const ActivityFrameInput& input) {
    ActivityFrameOutput output;
    if (!initialized_) { output.fatal_error="Dinosaur advanced before initialization";return output; }
    input_=input;output_=&output;
    state_.cursor_x=input.pointer_x;state_.cursor_y=input.pointer_y;
    state_.input_down=input.click_pulse;
    const int prior_phase=state_.phase;
    for (std::size_t i=0;i<2;++i)
        state_.feedback_gates[i]=static_cast<int>(
            animations_.channel(static_cast<dino::Character>(i)).mode());
    try {
        static_cast<void>(original_coordinators2::update_dino_activity(state_,*this));
        if (prior_phase==0 && state_.phase==1)
            output.pointer_warp=std::array<int,2>{state_.cursor_x,state_.cursor_y};
        output.hide_system_cursor=carried_surface_!=0;
        if (carried_surface_) {
            const auto i=static_cast<std::size_t>(carried_surface_-1);
            const auto& p=state_.pieces.at(i);
            output.draws.push_back({pieces_.at(i),state_.cursor_x,state_.cursor_y,
                Rect{0,0,p.width,p.height},true});
        }
        if (state_.marked_profile>=0 && !progress_emitted_) {
            output.progress_writes.push_back({dino::progress_slot_for_species(
                static_cast<dino::Species>(species_)),1});
            output.shared_completion_code=1;progress_emitted_=true;
        }
        if (state_.next_game_state) output.next_outer_state=state_.next_game_state;
    } catch (const std::exception& e) { output.fatal_error=e.what(); }
    output_=nullptr;return output;
}

void DinosaurDriver::stop_all_managed_sounds() {
    output_->audio.push_back({AudioOperation::StopManagedSounds});
}
void DinosaurDriver::sound(int id,int priority,int group) {
    output_->audio.push_back({AudioOperation::ManagedSoundId,{},id,priority,group});
}
std::uint32_t DinosaurDriver::random32() { return static_cast<std::uint32_t>(random_.next_rand()); }
bool DinosaurDriver::cursor_in_rectangle(original_coordinators2::Rect r) {
    return dino::cursor_inside_rect({state_.cursor_x,state_.cursor_y},
        {r.left,r.top},{r.right,r.bottom});
}
bool DinosaurDriver::cursor_near_point(int x,int y,int radius) {
    return dino::cursor_near_point({state_.cursor_x,state_.cursor_y},{x,y},radius);
}
void DinosaurDriver::load_completion_sprite(int species) {
    certificate_=dino::completion_artwork_filename(static_cast<dino::Species>(species));
    if (!assets_.resolve(certificate_).found())
        throw std::runtime_error("original Dinosaur certificate missing");
}
void DinosaurDriver::draw_dino_activity() {
    output_->draws.push_back({background_,0,0});
    for (std::size_t i=0;i<2;++i) {
        auto& channel=animations_.channel(static_cast<dino::Character>(i));
        if (state_.feedback_frames[i]>=0) {
            channel.queue(static_cast<dino::CharacterAnimationMode>(state_.feedback_frames[i]));
            state_.feedback_frames[i]=-1;
        }
    }
    animations_.advance();
    for (const auto character:{dino::Character::Bob,dino::Character::Ellis}) {
        const auto position=dino::character_position(character,static_cast<dino::Species>(species_));
        const auto r=dino::character_source_rect(character,animations_.channel(character).frame());
        output_->draws.push_back({character==dino::Character::Bob?
            "Data/SubGameDino/bob.bmp":"Data/SubGameDino/ellis.bmp",
            position.x,position.y,Rect{r.left,r.top,r.right,r.bottom},true});
    }
    for (int pass=0; pass<2; ++pass) {
        for (std::size_t i=0;i<state_.pieces.size();++i) {
            const auto& p=state_.pieces[i];
            if ((pass==0 && p.state!=4) ||
                (pass==1 && (p.state==4 || !p.visible))) continue;
            output_->draws.push_back({pieces_[i],p.state==4?p.target_x:p.x,
                p.state==4?p.target_y:p.y,Rect{0,0,p.width,p.height},true});
        }
    }
}
void DinosaurDriver::update_print_button() {
    output_->draws.push_back({certificate_,0,0});
    const auto print=dino::update_print_button(print_button_,{
        input_.pointer_x,input_.pointer_y,input_.click_pulse,input_.pointer_button_held});
    if (print.stop_all_managed_sounds) stop_all_managed_sounds();
    if (print.hover_sound_id) sound(*print.hover_sound_id,50,2);
    if (print.visual!=dino::PrintVisual::None)
        output_->draws.push_back({print.visual==dino::PrintVisual::Pressed?
            "Data/SubGameFirework/certprintdep.bmp":"Data/SubGameFirework/certprint.bmp",
            print.overlay_x,print.overlay_y,{},true});
    if (print.draw_print_bar_before_print)
        output_->draws.push_back({"Data/ui/printbar.bmp",0,0,{},true});
    output_->print_frame=print.invoke_shared_print;
}
void DinosaurDriver::unload_dino_activity() { output_->save_and_unload=true; }
void DinosaurDriver::prepare_play_again() {
    output_->next_saved_state=static_cast<int>(game_flow::State::DinoChooserSetup);
    output_->next_replay_class=1;
}
void DinosaurDriver::stop_activity_music() { output_->audio.push_back({AudioOperation::StopBackingTrack}); }
void DinosaurDriver::play_activity_music(int index) {
    if (index!=1) throw std::runtime_error("invalid original Dinosaur music index");
    output_->audio.push_back({AudioOperation::StartBackingTrack,"Data/music/dinosaur.wav"});
}
void DinosaurDriver::mark_activity_intro_sound(int species) {
    for (auto& voice:output_->audio)
        if (voice.sound_id==dino::intro_sound_id(species)) voice.input_interruptible=true;
}
bool DinosaurDriver::unload(std::string& error) {
    initialized_=false;state_={};pieces_.clear();background_.clear();
    certificate_.clear();carried_surface_=0;output_=nullptr;
    error.clear();return true;
}
} // namespace btb::full_game
