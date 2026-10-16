#include "btb/full_game_spud_maze.hpp"
#include "btb/maze_presentation.hpp"
#include "btb/maze_runtime.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace btb::full_game {
namespace {
class SpudMazeInputPort final : public original_motion4::IO {
public:
    SpudMazeInputPort(const ActivityFrameInput& input,
                      ActivityFrameOutput& effects,
                      int& pilchard_spawn_delay)
        :input_(input),effects_(effects),
         pilchard_spawn_delay_(pilchard_spawn_delay) {}

    original_motion4::Input player_input(bool spud) override {
        if(!spud)
            throw std::logic_error("Maze input requested in Spud Maze");
        const auto bits=input_.directional_input_bits;
        return {
            (bits&0x02U)?1:(bits&0x01U)?-1:0,
            (bits&0x08U)?1:(bits&0x04U)?-1:0,
            input_.pointer_button_held,input_.click_pulse
        };
    }
    original_motion4::U32 random32() override {
        return static_cast<original_motion4::U32>(std::rand());
    }
    void sound(original_motion4::I32 id,
               original_motion4::I32 priority,
               original_motion4::I32 sound_class) override {
        effects_.audio.push_back({
            AudioOperation::ManagedSoundId,{},id,priority,sound_class
        });
    }
    void spud_spawn_delay_reset() override {
        // The original outer game resets Pilchard's future spawn timer
        // on a Bob screen transition. Full Pilchard updates are pending.
        pilchard_spawn_delay_=0;
    }
    float npc_distance_to_player(const original_motion4::Npc&) override {
        throw std::logic_error("Maze NPC interface in Spud Maze");
    }
    void spud_repair_completed(original_motion4::I32,
                               original_motion4::I32) override {
        // The source repair animation, not the hit detection frame,
        // decrements the activity remaining-repair count.
        repair_begun_=true;
    }
    float actor_distance(float x,float y,float tx,float ty) override {
        return std::hypot(x-tx,y-ty);
    }
    original_motion4::I32 integer_angle(
        original_motion4::I32,original_motion4::I32,
        original_motion4::I32,original_motion4::I32) override {
        throw std::logic_error("Herding angle callback in Spud Maze");
    }
    std::array<float,2> movement_delta(
        original_motion4::I32,float) override {
        throw std::logic_error("Herding movement callback in Spud Maze");
    }
    std::vector<original_motion4::I32> shortest_path(
        const maze::ScreenGraph&,original_motion4::I32,
        original_motion4::I32) override {
        throw std::logic_error("Maze NPC path callback in Spud Maze");
    }
    void animal_collision_recovery(herding::RetailEntityRecord32&) override {
        throw std::logic_error("Herding collision callback in Spud Maze");
    }
    bool original_herding_boundary_collision(
        const herding::RetailEntityRecord32&) override {
        throw std::logic_error("Herding boundary callback in Spud Maze");
    }
    [[nodiscard]] bool repair_begun() const noexcept {
        return repair_begun_;
    }
private:
    const ActivityFrameInput& input_;
    ActivityFrameOutput& effects_;
    int& pilchard_spawn_delay_;
    bool repair_begun_{};
};

std::string piece_filename(std::size_t screen,std::size_t ordinal) {
    return "Data\\SubGameSpudMaze\\"+
        std::to_string(screen+1)+"-"+std::to_string(ordinal+1)+".bmp";
}

} // namespace

void SpudMazeDriver::configure_menu_state(
    const RetailMenuState& menu) noexcept {
    if((menu.variant_selection_origin==
            game_flow::State::SpudMazePregameUpdate ||
        menu.variant_selection_origin==
            game_flow::State::PlayAgainDifficultyUpdate) &&
       menu.source_variant>=0 && menu.source_variant<=2)
        difficulty_=menu.source_variant;
}

bool SpudMazeDriver::initialize(int player,std::string& error) {
    if(data_) {error="Spud Maze already initialized";return false;}
    if(player<0||player>=static_cast<int>(progress::kPlayerCount) ||
       difficulty_<0||difficulty_>2) {
        error="invalid Spud Maze player or difficulty";
        return false;
    }
    std::ifstream source(root_/"loaddata"/"spudmaze_nodes.txt");
    if(!source) {
        error="original loaddata/spudmaze_nodes.txt unavailable";
        return false;
    }
    try {
        auto loaded=std::make_unique<spud_maze::Data>(
            spud_maze::parse_data(source));
        if(std::any_of(loaded->screens.begin(),loaded->screens.end(),
            [](const auto& graph){return graph.empty();}))
            throw std::runtime_error("missing original Spud Maze screen");
        const auto first=std::find_if(
            loaded->screens[0].begin(),loaded->screens[0].end(),
            [](const auto& node){return node.index==2;});
        if(first==loaded->screens[0].end())
            throw std::runtime_error("source start node 2 unavailable");
        auto damaged=spud_maze::select_repair_damage(
            static_cast<spud_maze::Difficulty>(difficulty_),
            static_cast<int>(std::rand()%4)+1);
        original_motion4::Player actor{};
        actor.screen=0;
        actor.node=2;
        actor.x=static_cast<float>(first->x);
        actor.y=static_cast<float>(first->y);
        actor.previous_x=actor.x;
        actor.previous_y=actor.y;
        actor.facing=1;
        // The retail loader overwrites the file's 2.0f to 4.0f.
        actor.speed=loaded->retail_player_speed;
        actor.repair_tool_engaged=true;
        std::size_t ordinal=0;
        for(std::size_t screen=0;screen<5;++screen) {
            for(const auto& node:loaded->screens[screen]) {
                maze::Node n{};
                n.id=node.index;
                n.source_position={node.x,node.y+11};
                n.retail_position={node.x,node.y};
                n.direction_bits=node.direction_bits;
                n.node_type=node.node_type;
                n.links=node.links;
                actor.graphs[screen].nodes.push_back(n);
            }
            for(const auto& point:loaded->repair_points[screen]) {
                actor.repairs[screen].push_back({
                    point[0],point[1],40,
                    damaged.damaged[ordinal++]?1:0
                });
            }
        }
        if(ordinal!=spud_maze::kRepairPointCount ||
           damaged.repairs_remaining<=0)
            throw std::runtime_error("invalid original Spud Maze repairs");
        timer_=loaded->timer_values[static_cast<std::size_t>(difficulty_)];
        if(timer_<=0)
            throw std::runtime_error("invalid original Spud Maze timer");
        bob_=std::move(actor);
        data_=std::move(loaded);
        pilchard_={};
        pilchard_.spawn_wait=data_->spud_spawn_times[
            static_cast<std::size_t>(difficulty_)];
        hammer_={};
        hammer_.hammer_held=true;
        hammer_.current_screen=0;
        hammer_.bob_direction=bob_.facing;
        player_=player;
        repairs_remaining_=damaged.repairs_remaining;
        total_repairs_=damaged.repairs_remaining;
        repair_animation_frames_=0;
        repair_cycles_remaining_=0;
        timer_origin_=-1;
        spawned_pilchard_delay_=data_->spud_spawn_times[
            static_cast<std::size_t>(difficulty_)];
        exit_phase_=0;
        won_=false;
        first_audio_=false;
        warning_played_=false;
        error.clear();
        return true;
    } catch(const std::exception& e) {
        error=std::string("cannot initialize original Spud Maze: ")+e.what();
        return false;
    }
}

ActivityFrameOutput SpudMazeDriver::draw() const {
    ActivityFrameOutput out;
    if(!data_)return out;
    if(bob_.screen<0||bob_.screen>=5) {
        out.fatal_error="Spud Maze Bob screen outside retail 0..4";
        return out;
    }
    const auto screen=static_cast<std::size_t>(bob_.screen);
    out.draws.push_back({
        "Data\\SubGameSpudMaze\\screen"+
            std::to_string(screen+1)+".bmp",
        0,0,std::nullopt,false
    });
    for(std::size_t index=0;index<bob_.repairs[screen].size();++index) {
        if(bob_.repairs[screen][index].counter<=0)continue;
        const auto& p=data_->repair_points[screen][index];
        out.draws.push_back({
            piece_filename(screen,index),p[0],p[1],std::nullopt,true
        });
    }
    // Retail Repair.bmp is exactly 4980x249 (60x3 cells of 83px).
    // 0x004227F0 selects horizontal frames +30 when Bob holds the
    // hammer AND faces right/left; the three vertical bands encode
    // facing right (0), left (1) and forward/back (2).
    const int facing=bob_.facing==4?3:bob_.facing;
    const int row=facing==1?0:facing==3?1:2;
    const int raw_frame=std::clamp(bob_.animation,0,29);
    const int frame=raw_frame+
        ((facing==1||facing==3)&&bob_.repair_tool_engaged?30:0);
    out.draws.push_back({
        "Data\\SubGameSpudMaze\\Repair.bmp",
        static_cast<int>(bob_.x)-41,
        static_cast<int>(bob_.y)-41,
        Rect{frame*83,row*83,(frame+1)*83,(row+1)*83},true
    });
    // Source Pilchard sheet is 6084x256: thirty-nine 156px animation
    // columns with 128px direction bands (0x004227F0).
    if(pilchard_.phase>=2 && pilchard_.screen==bob_.screen) {
        const int col=std::clamp(pilchard_.animation_frame,0,38);
        const int row=pilchard_.roaming_direction==3?1:0;
        out.draws.push_back({
            "Data\\SubGameSpudMaze\\pilchard.bmp",
            static_cast<int>(pilchard_.x)-78,
            static_cast<int>(pilchard_.y)-64,
            Rect{col*156,row*128,(col+1)*156,(row+1)*128},true
        });
    }
    if(!hammer_.hammer_held&&hammer_.hammer_dropped &&
       hammer_.hammer.screen==bob_.screen) {
        out.draws.push_back({
            "Data\\SubGameSpudMaze\\hammer.bmp",
            hammer_.hammer.x,hammer_.hammer.y-30,
            std::nullopt,true
        });
    }
    // 0x004227F0 submits border after Bob and track repairs, then timer
    // and the original sequence of 20px hammer-icons.
    out.draws.push_back({
        "Data\\SubGameSpudMaze\\screenborder.bmp",0,0,
        std::nullopt,true
    });
    const int initial=data_->timer_values[static_cast<std::size_t>(difficulty_)];
    const int width=std::clamp(timer_,0,initial)*100/initial;
    if(width>0)
        out.draws.push_back({
            "Data\\SubGameSpudMaze\\timer.bmp",422,430,
            Rect{0,0,width,25},true
        });
    for(int i=0;i<total_repairs_;++i) {
        out.draws.push_back({
            i<repairs_remaining_
                ?"Data\\SubGameSpudMaze\\toolbarhammer.bmp"
                :"Data\\SubGameSpudMaze\\toolbarhammergrey.bmp",
            244-(total_repairs_/2)*20+(i%12)*20,
            429+(i/12)*20,std::nullopt,true
        });
    }
    return out;
}

ActivityFrameOutput SpudMazeDriver::advance(
    const ActivityFrameInput& input) {
    ActivityFrameOutput out=draw();
    if(!data_||out.fatal_error)return out;
    if(!first_audio_) {
        out.audio.push_back({AudioOperation::ManagedSoundId,{},
            spud_maze::kStartupVoiceSoundId,50,1});
        first_audio_=true;
    }
    if(timer_origin_<0)timer_origin_=input.elapsed_centiseconds;
    const int elapsed=std::max(0,
        input.elapsed_centiseconds-timer_origin_);
    timer_=data_->timer_values[static_cast<std::size_t>(difficulty_)]-
           elapsed/100;
    if(timer_==15&&!warning_played_) {
        out.audio.push_back({AudioOperation::StopManagedSounds});
        out.audio.push_back({AudioOperation::ManagedSoundId,{},
            spud_maze::kLowTimerWarningSoundId,90,1});
        warning_played_=true;
    }
    const auto outcome=spud_maze::activity_outcome(repairs_remaining_,timer_);
    if(outcome!=spud_maze::ActivityOutcome::Continue) {
        if(exit_phase_==0) {
            won_=outcome==spud_maze::ActivityOutcome::Success;
            exit_phase_=1;
            return out;
        }
        if(exit_phase_==1) {
            if(input.managed_sound_playing)return out;
            exit_phase_=2;
            return out;
        }
        if(won_)
            out.progress_writes.push_back({progress::Slot::SpudMaze,1});
        out.save_and_unload=true;
        out.next_outer_state=
            static_cast<int>(game_flow::State::PlayAgainYesNoSetup);
        out.next_saved_state=
            static_cast<int>(game_flow::State::SpudMazeInit);
        out.next_replay_class=1;
        return out;
    }
    try {
        if(repair_cycles_remaining_>0) {
            // Exact active-repair branch in original 0x00422400:
            // 0x005109F0 gates every four frames; frame 9 advances
            // through 14 and wraps to 9 while 0x00514514 counts down
            // its FIVE full passes. Only the final wrap decrements the
            // damaged-track counter 0x005144C8.
            if(--repair_animation_frames_<1) {
                repair_animation_frames_=4;
                if(bob_.animation<14) {
                    ++bob_.animation;
                } else if(--repair_cycles_remaining_>0) {
                    bob_.animation=9;
                } else {
                    bob_.hazard_lock=false;
                    bob_.animation=0;
                    repair_animation_frames_=0;
                    repair_cycles_remaining_=0;
                    if(repairs_remaining_<=0)
                        throw std::logic_error(
                            "retail Spud Maze repair counter underflow");
                    --repairs_remaining_;
                }
            }
        } else {
            SpudMazeInputPort io(input,out,spawned_pilchard_delay_);
            original_motion4::update_spud_maze_bob(bob_,io);
            if(io.repair_begun()) {
                repair_animation_frames_=4;
                repair_cycles_remaining_=5;
                bob_.animation=9;
            }
        }
        // Retail 0x00423F40 calls the Pilchard source phase machine
        // after updating Bob and drawing the active scene. Maintain a
        // single live collision state for hammer possession/cooldown.
        const spud_maze::PilchardBob bob{
            bob_.screen,bob_.facing,bob_.x,bob_.y,
            repair_cycles_remaining_>0
        };
        const auto pilot=spud_maze::update_pilchard_native(
            pilchard_,hammer_,*data_,difficulty_,bob,
            []{return static_cast<std::uint32_t>(std::rand());});
        for(const auto& effect:pilot.sound_and_hammer) {
            if(effect.op==maze_spud_batch::Op::PlayManagedSound)
                out.audio.push_back({
                    AudioOperation::ManagedSoundId,{},
                    effect.argument,effect.priority,effect.arbitration
                });
        }
        if(pilot.hammer_dropped)
            bob_.repair_tool_engaged=false;
        if(spud_maze::pickup_dropped_hammer(hammer_,bob))
            bob_.repair_tool_engaged=true;
    } catch(const std::exception& e) {
        out.fatal_error=std::string("Spud Maze source Bob/Pilchard: ")+e.what();
        return out;
    }
    auto frame=draw();
    frame.audio=std::move(out.audio);
    return frame;
}

bool SpudMazeDriver::unload(std::string& error) {
    bob_={};
    pilchard_={};
    hammer_={};
    data_.reset();
    player_=-1;
    repairs_remaining_=0;
    total_repairs_=0;
    repair_animation_frames_=0;
    repair_cycles_remaining_=0;
    spawned_pilchard_delay_=0;
    timer_origin_=-1;
    exit_phase_=0;
    won_=false;
    first_audio_=false;
    warning_played_=false;
    error.clear();
    return true;
}

} // namespace btb::full_game
