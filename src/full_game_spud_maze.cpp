#include "btb/full_game_spud_maze.hpp"
#include "btb/maze_presentation.hpp"
#include "btb/maze_runtime.hpp"
#include "btb/original_asset_resolver.hpp"

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
                      spud_maze::PilchardNative& pilchard,retail::OriginalRetailRandom& rng,
                      original_motion4::Player& player,maze::InputModeState& mode,
                      bool& pointer_sampled)
        :input_(input),effects_(effects),pilchard_(pilchard),rng_(rng),
         player_(player),mode_(mode),pointer_sampled_(pointer_sampled) {}

    original_motion4::Input player_input(bool spud) override {
        if(!spud)
            throw std::logic_error("Maze input requested in Spud Maze");
        const auto bits=input_.directional_input_bits;
        if(!pointer_sampled_&&input_.pointer_position_valid){
            mode_.remembered_mouse_x=input_.pointer_x;
            mode_.remembered_mouse_y=input_.pointer_y;
            pointer_sampled_=true;
        }
        // 004211A0: entry mode decides THIS call. Unlike Maze,
        // Spud uses a +50 pointer-Y offset, 26-unit dead zone,
        // separate 0.1/0.4 thresholds and no three-sample debounce.
        player_.mouse_mode=mode_.mouse_mode;
        if(mode_.mouse_mode&&input_.pointer_position_valid){
            if(bits||input_.click_pulse)mode_.mouse_mode=false;
            const maze::Vec2i from{static_cast<int>(player_.x),static_cast<int>(player_.y)};
            const maze::Vec2i to{input_.pointer_x,input_.pointer_y+50};
            int dx=0,dy=0;
            if(std::hypot(double(from.x)-to.x,double(from.y)-to.y)>26.0){
                const auto angle=maze::angle_between_integer_points_degrees(from,to);
                const long double rad=angle*static_cast<long double>(0.017453530803322792f);
                const auto sn=static_cast<float>(std::sin(rad));
                const auto cs=static_cast<float>(std::cos(rad));
                if(std::fabs(sn)>0.1)dx=sn>0?1:-1;
                if(std::fabs(cs)>0.4)dy=cs>0?-1:1;
            }
            return {dx,dy,input_.pointer_button_held,input_.click_pulse};
        }
        const auto raw=maze::keyboard_direction_from_bits(bits);
        if(input_.pointer_position_valid){
            if(raw.x==0&&raw.y==0&&
                (input_.pointer_x!=mode_.remembered_mouse_x||
                 input_.pointer_y!=mode_.remembered_mouse_y))mode_.mouse_mode=true;
            mode_.remembered_mouse_x=input_.pointer_x;
            mode_.remembered_mouse_y=input_.pointer_y;
        }
        return {raw.x,-raw.y,input_.pointer_button_held,input_.click_pulse};
    }
    original_motion4::U32 random32() override {
        return static_cast<original_motion4::U32>(rng_.next_rand());
    }
    void sound(original_motion4::I32 id,
               original_motion4::I32 priority,
               original_motion4::I32 sound_class) override {
        effects_.audio.push_back({
            AudioOperation::ManagedSoundId,{},id,priority,sound_class
        });
    }
    void spud_spawn_delay_reset() override {
        // Exact 0x00423610 ResetPilchardSpawnDelay: phase 0, 500 ticks,
        // including Easy mode where the initial -1 only disables the
        // initial spawn. Moving screens restarts this independent timer.
        pilchard_.phase=0;
        pilchard_.spawn_wait=500;
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
    spud_maze::PilchardNative& pilchard_;
    retail::OriginalRetailRandom& rng_;
    original_motion4::Player& player_;
    maze::InputModeState& mode_;
    bool& pointer_sampled_;
    bool repair_begun_{};
};

std::string piece_filename(std::size_t screen,std::size_t ordinal) {
    return "Data\\SubGameSpudMaze\\"+
        std::to_string(screen+1)+"-"+std::to_string(ordinal+1)+".bmp";
}

// Source loader queries each repair BMP's dimensions. Read only the
// original DIB metadata; native DirectDraw still loads the pixel data.
std::pair<int,int> repair_bitmap_dimensions(const std::filesystem::path& path){
    std::ifstream f(path,std::ios::binary);
    std::array<unsigned char,26> b{};
    if(!f.read(reinterpret_cast<char*>(b.data()),b.size())||b[0]!='B'||b[1]!='M')
        throw std::runtime_error("invalid repair BMP header: "+path.string());
    const auto le=[&](std::size_t offset){
        return std::uint32_t(b[offset])|(std::uint32_t(b[offset+1])<<8)|
            (std::uint32_t(b[offset+2])<<16)|(std::uint32_t(b[offset+3])<<24);
    };
    const auto w=static_cast<std::int32_t>(le(18)),h=static_cast<std::int32_t>(le(22));
    if(le(14)<40||w<=0||w>8192||h==0||h>8192||h< -8192)
        throw std::runtime_error("invalid repair BMP dimensions: "+path.string());
    return {w,std::abs(h)};
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
            random_stream().next_rand()%4+1);
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
            for(std::size_t i=0;i<loaded->repair_points[screen].size();++i) {
                const auto& point=loaded->repair_points[screen][i];
                const auto name=piece_filename(screen,i);
                const auto file=OriginalAssetResolver(root_).resolve(name);
                if(!file.found())throw std::runtime_error("missing original repair bitmap: "+name);
                const auto [width,height]=repair_bitmap_dimensions(file.absolute_path);
                actor.repairs[screen].push_back({
                    point[0],point[1],height,
                    damaged.damaged[ordinal++]?1:0,width
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
        input_mode_={};input_mode_.mouse_mode=false;pointer_sampled_=false;
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
        walk_animation_wait_=0;move_permitted_=false;walking_=false;
        repair_animation_frames_=0;
        repair_cycles_remaining_=0;
        timer_origin_=-1;
        spawned_pilchard_delay_=data_->spud_spawn_times[
            static_cast<std::size_t>(difficulty_)];
        exit_phase_=0;
        slide_={};bentley_={};
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

void SpudMazeDriver::append_repairs(ActivityFrameOutput& out,int screen,int offset) const {
    const auto index=static_cast<std::size_t>(screen);
    for(std::size_t i=0;i<bob_.repairs[index].size();++i) {
        if(bob_.repairs[index][i].counter<=0)continue;
        const auto& p=data_->repair_points[index][i];
        out.draws.push_back({piece_filename(index,i),p[0]+offset,p[1],std::nullopt,true});
    }
}

ActivityFrameOutput SpudMazeDriver::draw(const spud_maze::SlideFrame* slide) const {
    ActivityFrameOutput out;
    if(!data_)return out;
    if(bob_.screen<0||bob_.screen>=5) {
        out.fatal_error="Spud Maze Bob screen outside retail 0..4";
        return out;
    }
    const auto screen=static_cast<std::size_t>(bob_.screen);
    const bool sliding=slide&&!slide->complete;
    const int offset=sliding?slide->new_offset:0;
    const auto background=[](int s) {
        return "Data\\SubGameSpudMaze\\screen"+std::to_string(s+1)+".bmp";
    };
    out.draws.push_back({background(sliding?slide_.previous:bob_.screen),0,0,std::nullopt,false});
    if(sliding) {
        const auto strip=[&](const spud_maze::SlideStrip& b) {
            if(b.right>b.left)
                out.draws.push_back({background(b.screen),b.destination_x,20,
                    Rect{b.left,20,b.right,400},false});
        };
        strip(slide->left);strip(slide->right);
        append_repairs(out,slide_.previous,slide->old_offset);
    }
    append_repairs(out,bob_.screen,offset);
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
        static_cast<int>(bob_.x)-41+offset,
        static_cast<int>(bob_.y)-41,
        Rect{frame*83,row*83,(frame+1)*83,(row+1)*83},true
    });
    // Source Pilchard sheet is 6084x256: thirty-nine 156px animation
    // columns with 128px direction bands (0x004227F0).
    if(pilchard_.phase>=2 && pilchard_.screen==bob_.screen) {
        const int col=std::clamp(pilchard_.animation_frame,0,38);
        const int row=(pilchard_.roaming_direction==0||pilchard_.roaming_direction==1)?0:1;
        out.draws.push_back({
            "Data\\SubGameSpudMaze\\pilchard.bmp",
            static_cast<int>(pilchard_.x)-78+offset,
            static_cast<int>(pilchard_.y)-64,
            Rect{col*156,row*128,(col+1)*156,(row+1)*128},true
        });
    }
    if(!hammer_.hammer_held&&hammer_.hammer_dropped &&
       hammer_.hammer.screen==bob_.screen) {
        out.draws.push_back({
            "Data\\SubGameSpudMaze\\hammer.bmp",
            hammer_.hammer.x+offset,hammer_.hammer.y-30,
            std::nullopt,true
        });
    }
    if(bentley_.visible) {
        const int x=bentley_.frame*76;
        out.draws.push_back({"Data\\SubGameSpudMaze\\bentley.bmp",bentley_.x,bentley_.y,
            Rect{x,0,x+76,80},true});
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
            i<total_repairs_-repairs_remaining_
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
        out.audio.push_back({AudioOperation::StartBackingTrack,"Data/music/Skateboardfix.wav"});
        out.audio.push_back({AudioOperation::ManagedSoundId,{},
            spud_maze::kStartupVoiceSoundId,50,1,true});
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
    if((outcome!=spud_maze::ActivityOutcome::Continue&&slide_.phase==0)||exit_phase_!=0) {
        if(exit_phase_==0) {
            won_=outcome==spud_maze::ActivityOutcome::Success;
            exit_phase_=1;
        }
        if(exit_phase_==1) {
            // 00423BE0: stop repair interaction, animate original Bentley,
            // wait for actual managed voices, then allow replay teardown.
            bob_.facing=3;bob_.hazard_lock=false;
            repair_cycles_remaining_=0;repair_animation_frames_=0;
            const int bit=bentley_.phase==3&&!input.managed_sound_playing
                ?random_stream().next_rand()&1:0;
            const auto narration=spud_maze::advance_bentley(
                bentley_,!won_,input.managed_sound_playing,bit);
            if(narration.stop_managed)out.audio.push_back({AudioOperation::StopManagedSounds});
            if(narration.sound_id>=0)out.audio.push_back({
                AudioOperation::ManagedSoundId,{},narration.sound_id,50,1});
            if(narration.complete)exit_phase_=2;
            auto rendered=draw();
            // 00422400 has the separate 8-tick 0..9 narration cycle.
            static_cast<void>(spud_maze::advance_bob_walk_animation(
                bob_.animation,walk_animation_wait_,bob_.facing,false,true,input.original_windowed_display));
            rendered.audio=std::move(out.audio);
            return rendered;
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
    bool portal_captured=false;
    bool repair_ended_this_frame=false;
    std::optional<spud_maze::SlideFrame> slide_frame;
    try {
        if(slide_.phase!=0) {
            slide_frame=spud_maze::advance_slide(slide_);
            if(slide_frame->complete)bob_.transition=0;
        } else if(repair_cycles_remaining_>0) {
            // Exact active-repair branch in original 0x00422400:
            // 0x005109F0 gates every four frames; frame 9 advances
            // through 14 and wraps to 9 while 0x00514514 counts down
            // its FIVE full passes. Only the final wrap decrements the
            // damaged-track counter 0x005144C8.
            if(spud_maze::bob_animation_tick_due(repair_animation_frames_,input.original_windowed_display)) {
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
                    // The original final repair wrap writes countdown 4.
                    // Do not run the walking updater again in this same frame.
                    walk_animation_wait_=4;
                    repair_ended_this_frame=true;
                    move_permitted_=true;
                    walking_=false;
                    if(repairs_remaining_<=0)
                        throw std::logic_error(
                            "retail Spud Maze repair counter underflow");
                    --repairs_remaining_;
                }
            }
        } else {
            SpudMazeInputPort io(input,out,pilchard_,random_stream(),
                                 bob_,input_mode_,pointer_sampled_);
            const auto previous=bob_.screen;
            const int previous_animation=bob_.animation;
            const int previous_input_ticks=bob_.input_ticks;
            bob_.can_move_x=move_permitted_;
            move_permitted_=false;
            original_motion4::update_spud_maze_bob(bob_,io);
            walking_=bob_.input_ticks!=previous_input_ticks;
            // Motion4 also exposes the older free-running animation count;
            // the physical scene owns the source four-tick animation instead.
            if(!io.repair_begun())bob_.animation=previous_animation;
            if(previous!=bob_.screen) {
                slide_={};slide_.previous=previous;slide_.destination=bob_.screen;
                slide_.phase=1;portal_captured=true;
            }
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
        // 0x00423F40 draws/updates the scene (including retrieval of
        // a previously dropped hammer) BEFORE UpdateSpudMazePilchard.
        // Do not permit the newly dropped hammer to be recovered on the
        // same physical frame as Pilchard's collision.
        if(!portal_captured&&spud_maze::pickup_dropped_hammer(hammer_,bob))
            bob_.repair_tool_engaged=true;
        const auto pilot=spud_maze::update_pilchard_native(
            pilchard_,hammer_,*data_,difficulty_,bob,
            [this]{return static_cast<std::uint32_t>(random_stream().next_rand());});
        for(const auto& effect:pilot.sound_and_hammer) {
            if(effect.op==maze_spud_batch::Op::PlayManagedSound)
                out.audio.push_back({
                    AudioOperation::ManagedSoundId,{},
                    effect.argument,effect.priority,effect.arbitration
                });
        }
        if(pilot.hammer_dropped)
            bob_.repair_tool_engaged=false;
    } catch(const std::exception& e) {
        out.fatal_error=std::string("Spud Maze source Bob/Pilchard: ")+e.what();
        return out;
    }
    if(portal_captured)return out;
    auto frame=draw(slide_frame?&*slide_frame:nullptr);
    if(!repair_ended_this_frame && repair_cycles_remaining_==0 && !bob_.hazard_lock) {
        // Scene has already submitted the current sprite; update for next
        // frame, keeping the movement permission latch through transitions.
        if(spud_maze::advance_bob_walk_animation(bob_.animation,
            walk_animation_wait_,bob_.facing,walking_,false,input.original_windowed_display))move_permitted_=true;
    }
    frame.audio=std::move(out.audio);
    return frame;
}

bool SpudMazeDriver::unload(std::string& error) {
    bob_={};input_mode_={};pointer_sampled_=false;
    pilchard_={};
    hammer_={};
    data_.reset();
    player_=-1;
    repairs_remaining_=0;
    total_repairs_=0;
    repair_animation_frames_=0;
    repair_cycles_remaining_=0;
    walk_animation_wait_=0;move_permitted_=false;walking_=false;
    spawned_pilchard_delay_=0;
    timer_origin_=-1;
    exit_phase_=0;
    slide_={};bentley_={};
    won_=false;
    first_audio_=false;
    warning_played_=false;
    error.clear();
    return true;
}

} // namespace btb::full_game
