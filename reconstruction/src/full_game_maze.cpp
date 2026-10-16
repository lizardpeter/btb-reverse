#include "btb/full_game_maze.hpp"
#include "btb/maze_presentation.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace btb::full_game {
namespace {
// This bridge invokes already reconstructed 0x0041B140 player motion.
// Every service that belongs to the still-unhosted Spud / Herding actors
// fails loudly: absent world simulation must never masquerade as success.
class MazePlayerPort final : public original_motion4::IO {
public:
    MazePlayerPort(const ActivityFrameInput& in,ActivityFrameOutput& output)
        :input_(in),output_(output) {}

    original_motion4::Input player_input(bool spud_maze) override {
        if(spud_maze)throw std::logic_error("Spud Maze input in Maze driver");
        const auto bits=input_.directional_input_bits;
        return {
            (bits&0x02U)?1:(bits&0x01U)?-1:0,
            (bits&0x08U)?1:(bits&0x04U)?-1:0,
            input_.pointer_button_held,input_.click_pulse
        };
    }
    original_motion4::U32 random32() override {
        return static_cast<original_motion4::U32>(input_.random_value);
    }
    void sound(original_motion4::I32 id,
               original_motion4::I32 priority,
               original_motion4::I32 sound_class) override {
        output_.audio.push_back({
            AudioOperation::ManagedSoundId,{},id,priority,sound_class
        });
    }
    void spud_spawn_delay_reset() override {
        throw std::logic_error("Spud Maze spawn reset in Maze driver");
    }
    float npc_distance_to_player(const original_motion4::Npc&) override {
        throw std::logic_error("Maze Spud NPC is not yet hosted");
    }
    void spud_repair_completed(original_motion4::I32,
                               original_motion4::I32) override {
        throw std::logic_error("Spud Maze repair in Maze driver");
    }
    float actor_distance(float,float,float,float) override {
        throw std::logic_error("Spud/Herding actor distance in Maze driver");
    }
    original_motion4::I32 integer_angle(
        original_motion4::I32,original_motion4::I32,
        original_motion4::I32,original_motion4::I32) override {
        throw std::logic_error("Spud/Herding angle in Maze driver");
    }
    std::array<float,2> movement_delta(
        original_motion4::I32,float) override {
        throw std::logic_error("Spud/Herding movement in Maze driver");
    }
    std::vector<original_motion4::I32> shortest_path(
        const maze::ScreenGraph&,original_motion4::I32,
        original_motion4::I32) override {
        throw std::logic_error("Maze NPC shortest path is not yet hosted");
    }
    void animal_collision_recovery(herding::RetailEntityRecord32&) override {
        throw std::logic_error("Herding collision in Maze driver");
    }
    bool original_herding_boundary_collision(
        const herding::RetailEntityRecord32&) override {
        throw std::logic_error("Herding boundary in Maze driver");
    }
private:
    const ActivityFrameInput& input_;
    ActivityFrameOutput& output_;
};
}

void MazeDriver::configure_menu_state(
    const RetailMenuState& menu) noexcept {
    if((menu.variant_selection_origin==
            game_flow::State::MazePregameUpdate ||
        menu.variant_selection_origin==
            game_flow::State::PlayAgainDifficultyUpdate) &&
       menu.source_variant>=0 && menu.source_variant<=2)
        difficulty_=menu.source_variant;
}

bool MazeDriver::initialize(int player_index,std::string& error) {
    if(data_) {
        error="original Maze activity already initialized";
        return false;
    }
    if(player_index<0||player_index>=
          static_cast<int>(progress::kPlayerCount) ||
       difficulty_<0||difficulty_>2) {
        error="invalid original Maze player or difficulty";
        return false;
    }
    const auto path=install_root_/"loaddata"/"maze_nodes.txt";
    std::ifstream file(path,std::ios::binary);
    if(!file) {
        error="original loaddata/maze_nodes.txt is missing";
        return false;
    }
    try {
        std::string source{std::istreambuf_iterator<char>{file},
                           std::istreambuf_iterator<char>{}};
        auto parsed=std::make_unique<maze::Data>(maze::parse_data(source));
        // 0x0041A730 starts on Middle screen, at graph node 8,
        // with the float bit patterns 0x43770000 and 0x43858000.
        const auto& middle=parsed->screens[1].nodes;
        if(std::none_of(middle.begin(),middle.end(),
            [](const auto& node){return node.id==8;}) ||
           std::any_of(parsed->screens.begin(),parsed->screens.end(),
            [](const auto& screen){return screen.nodes.empty();}) ||
           parsed->player_speed<=0.0f ||
           parsed->timer_by_difficulty[static_cast<std::size_t>(difficulty_)]<=0) {
            error="original Maze node-8 initial state or tuning is invalid";
            return false;
        }
        original_motion4::Player p{};
        p.screen=1;
        p.node=8;
        p.x=247.0f;
        p.y=267.0f;
        p.previous_x=p.x;
        p.previous_y=p.y;
        p.facing=1;
        p.speed=parsed->player_speed;
        p.mouse_mode=false;
        for(std::size_t i=0;i<3;++i)
            p.graphs[i]=parsed->screens[i];
        player_=std::move(p);
        remaining_seconds_=
            parsed->timer_by_difficulty[static_cast<std::size_t>(difficulty_)];
        data_=std::move(parsed);
        player_index_=player_index;
        started_centiseconds_=0;
        timer_started_=false;
        intro_voice_queued_=false;
        intro_done_=false;
        lowtime_voice_queued_=false;
        exit_phase_=0;
        error.clear();
        return true;
    } catch(const std::exception& e) {
        error=std::string("original Maze data failed: ")+e.what();
        return false;
    }
}

ActivityFrameOutput MazeDriver::draw() const {
    ActivityFrameOutput output;
    if(!data_)return output;
    const auto current=static_cast<maze::Screen>(player_.screen);
    const auto* background=maze::surface_binding(
        maze::background_surface(current));
    if(!background) {
        output.fatal_error="missing original Maze backdrop source binding";
        return output;
    }
    output.draws.push_back({
        std::string(background->filename),0,0,std::nullopt,false
    });
    const auto sprite=maze::actor_source_rect(player_.facing,0);
    const auto where=maze::player_destination(player_.x,player_.y);
    output.draws.push_back({
        "Data\\SubGameMaze\\player.bmp",where.x,where.y,
        Rect{sprite.left,sprite.top,sprite.right,sprite.bottom},true
    });
    const int initial=data_->timer_by_difficulty[
        static_cast<std::size_t>(difficulty_)];
    const auto timer=maze::timer_source_rect(
        std::max(0,remaining_seconds_),initial);
    if(timer.right>0) {
        output.draws.push_back({
            "Data\\SubGameSpudMaze\\timer.bmp",422,430,
            Rect{timer.left,timer.top,timer.right,timer.bottom},true
        });
    }
    // Retail draws 4/8/12 target markers, distinguishing collected
    // packages with smallbox.bmp. NPC/package capture is not connected
    // yet, so only the real ghost-marker state is represented here.
    for(const auto& marker:maze::package_progress_markers(
            maze::kPackageCountByDifficulty[
                static_cast<std::size_t>(difficulty_)],0)) {
        const auto* image=maze::surface_binding(marker.surface);
        if(image)
            output.draws.push_back({
                std::string(image->filename),
                marker.destination.x,marker.destination.y,
                std::nullopt,true
            });
    }
    return output;
}

ActivityFrameOutput MazeDriver::advance(const ActivityFrameInput& input) {
    ActivityFrameOutput out=draw();
    if(!data_||out.fatal_error)return out;

    // Original 0x0041D4D0 gates active movement behind the opening
    // 0x49+difficulty narrator line. Never signal its finish by a fixed
    // fake timeout; the physical managed-sound observation is required.
    if(!intro_voice_queued_) {
        out.audio.push_back({
            AudioOperation::ManagedSoundId,{},0x49+difficulty_,90,1
        });
        intro_voice_queued_=true;
        return out;
    }
    if(!intro_done_) {
        if(input.managed_sound_playing)return out;
        intro_done_=true;
        timer_started_=true;
        started_centiseconds_=input.elapsed_centiseconds;
        return out;
    }
    if(timer_started_) {
        const int elapsed=std::max(
            0,input.elapsed_centiseconds-started_centiseconds_);
        remaining_seconds_=
            data_->timer_by_difficulty[
                static_cast<std::size_t>(difficulty_)]-elapsed/100;
    }
    if(remaining_seconds_==15&&!lowtime_voice_queued_) {
        out.audio.push_back({AudioOperation::StopManagedSounds});
        out.audio.push_back({
            AudioOperation::ManagedSoundId,{},0x53,90,1
        });
        lowtime_voice_queued_=true;
    }
    if(remaining_seconds_<0) {
        if(exit_phase_==0) {
            out.audio.push_back({AudioOperation::StopManagedSounds});
            out.audio.push_back({
                AudioOperation::ManagedSoundId,{},0x56,90,1
            });
            exit_phase_=1;
            return out;
        }
        if(exit_phase_==1) {
            if(input.managed_sound_playing)return out;
            exit_phase_=2;
            return out;
        }
        // The real 0x0041D4D0 timeout path has no Maze success flag.
        // Returning to the source Play Again state is not fabricated
        // victory/progress.
        out.save_and_unload=true;
        out.next_outer_state=
            static_cast<int>(game_flow::State::PlayAgainYesNoSetup);
        out.next_saved_state=static_cast<int>(game_flow::State::MazeInit);
        out.next_replay_class=1;
        return out;
    }
    try {
        MazePlayerPort io(input,out);
        original_motion4::update_maze_player(player_,io);
    } catch(const std::exception& e) {
        out.fatal_error=std::string("original Maze player motion: ")+e.what();
        return out;
    }
    // Source movement updates the actor BEFORE DrawMazeActivity; replace
    // the pre-input ordered draw plan with the new physical coordinates.
    auto updated=draw();
    updated.audio=std::move(out.audio);
    return updated;
}

bool MazeDriver::unload(std::string& error) {
    data_.reset();
    player_={};
    player_index_=-1;
    timer_started_=false;
    intro_done_=false;
    intro_voice_queued_=false;
    lowtime_voice_queued_=false;
    exit_phase_=0;
    error.clear();
    return true;
}

} // namespace btb::full_game
