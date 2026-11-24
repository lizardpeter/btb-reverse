#include "btb/full_game_maze.hpp"
#include "btb/maze_presentation.hpp"
#include "btb/maze_runtime.hpp"
#include "btb/maze_controller.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
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
    MazePlayerPort(const ActivityFrameInput& in,ActivityFrameOutput& output,
                   original_motion4::Player& player,
                   maze::InputModeState& mode,
                   maze::DirectionalDebounce& debounce,
                   bool& pointer_sampled,retail::OriginalRetailRandom& rng)
        :input_(in),output_(output),player_(player),mode_(mode),
         debounce_(debounce),pointer_sampled_(pointer_sampled),rng_(rng) {}

    original_motion4::Input player_input(bool spud_maze) override {
        if(spud_maze)throw std::logic_error("Spud Maze input in Maze driver");
        // Original 0x0041AF60 starts in keyboard mode. The first native
        // sample captures the source cursor instead of spuriously
        // interpreting the default (0,0) as a user mouse move.
        if(!pointer_sampled_) {
            mode_.remembered_mouse_x=input_.pointer_x;
            mode_.remembered_mouse_y=input_.pointer_y;
            pointer_sampled_=true;
        }
        const auto resolved=maze::compute_player_input_direction(
            mode_,debounce_,{
                player_.x,player_.y,
                input_.pointer_x,input_.pointer_y,
                input_.directional_input_bits
            });
        // Capture logic in 0x0041B140 uses the entry mode for THIS frame.
        // Mode switches become active for the following call only.
        player_.mouse_mode=resolved.path==maze::InputPath::Mouse;
        return {
            // 0041AF60 returns +1 for up; 0041B140 subtracts Y.
            resolved.intent.x,-resolved.intent.y,
            input_.pointer_button_held,input_.click_pulse
        };
    }
    original_motion4::U32 random32() override {
        // Original 0x0042FFC4 dispatches the shared CRT rand stream.
        return static_cast<original_motion4::U32>(rng_.next_rand());
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
    float npc_distance_to_player(const original_motion4::Npc& npc) override {
        // 0041CCB0 truncates all four coordinates before distance.
        return std::hypot(static_cast<float>(static_cast<int>(npc.position.x)-static_cast<int>(player_.x)),
                          static_cast<float>(static_cast<int>(npc.position.y)-static_cast<int>(player_.y)));
    }
    void spud_repair_completed(original_motion4::I32,
                               original_motion4::I32) override {
        throw std::logic_error("Spud Maze repair in Maze driver");
    }
    float actor_distance(float x,float y,float tx,float ty) override {
        return std::hypot(x-tx,y-ty);
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
        const maze::ScreenGraph& graph,original_motion4::I32 from,
        original_motion4::I32 to) override {
        auto route=maze::find_shortest_path_retail(graph,from,to);
        if(!route)
            throw std::runtime_error("original Maze Spud path unreachable");
        return route->nodes;
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
    original_motion4::Player& player_;
    maze::InputModeState& mode_;
    maze::DirectionalDebounce& debounce_;
    bool& pointer_sampled_;
    retail::OriginalRetailRandom& rng_;
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
        // Source 0x0041A730 samples original CRT rand()%4 repeatedly for
        // each screen until every active package has a unique location.
        // The original executable uses the CRT's live PRNG state.
        auto allocated=maze::allocate_source_boxes(difficulty_,[this] {
            return static_cast<unsigned>(random_stream().next_rand());
        });
        player_=std::move(p);
        boxes_=std::move(allocated);
        npc_={};
        npc_.screen=1;
        npc_.player_screen=1;
        npc_.difficulty=difficulty_;
        npc_.spawn_interval=maze::kSourceNpcSpawnFrames[
            static_cast<std::size_t>(difficulty_)];
        npc_.spawn_wait=npc_.spawn_interval;
        npc_.offscreen_wait=npc_.spawn_interval;
        npc_.graphs=parsed->screens;
        npc_.packages=boxes_.screens;
        npc_.has_box_target_nodes=true;
        for(std::size_t screen=0;screen<3;++screen)
            for(std::size_t location=0;location<4;++location)
                npc_.package_target_nodes[screen][location]=
                    parsed->box_locations[screen][location].node_id;
        input_mode_={};
        input_mode_.mouse_mode=false; // 0x00512124 initial 0
        mouse_debounce_={};
        pointer_sampled_=false;
        transition_={};transition_.phase=0;
        drive_animation_frame_=0;drive_animation_wait_=0;
        pickup_animation_frame_=0;
        pickup_animation_wait_=0;
        pickup_voice_sequence_=0;
        won_=false;
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

void MazeDriver::append_boxes(ActivityFrameOutput& output,int screen,int offset) const {
    const auto screen_index=static_cast<std::size_t>(screen);
    for(const auto& slot:boxes_.screens[screen_index]) {
        if(slot.node_id<0)continue;
        if(slot.status==maze::kSpudAvailablePackageStatus ||
           slot.status==maze::kSpudDroppedPackageStatus) {
            int x{},y{};
            if(slot.status==maze::kSpudAvailablePackageStatus) {
                if(slot.node_id>=4) {
                    output.fatal_error="Maze box location outside retail 0..3";
                    return;
                }
                const auto& point=data_->box_locations[screen_index][
                    static_cast<std::size_t>(slot.node_id)];
                x=point.position.x;
                y=point.position.y;
            } else {
                x=slot.dropped_x;
                y=slot.dropped_y;
            }
            output.draws.push_back({
                "Data\\SubGameMaze\\box.bmp",x-8+offset,y-8,
                std::nullopt,true
            });
        }
    }
}

void MazeDriver::append_spud(ActivityFrameOutput& output,int screen,int offset) const {
    // Retail DrawMazeActivity shows the live Spud sprite only after NPC
    // phase 1 and only on the player's screen; hidden phases are not
    // substituted with a fake actor standing on the map.
    if(static_cast<int>(npc_.phase)>1 && npc_.screen==screen) {
        const auto spud_frame=maze::actor_source_rect(
            npc_.animation_frame,npc_.current_direction);
        const auto spud=maze::spud_destination(
            npc_.position.x,npc_.position.y,offset);
        output.draws.push_back({
            "Data\\SubGameMaze\\spud.bmp",spud.x,spud.y,
            Rect{spud_frame.left,spud_frame.top,
                 spud_frame.right,spud_frame.bottom},true
        });
    }
}

void MazeDriver::append_hud(ActivityFrameOutput& output) const {
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
    // Source HUD is derived from the same real package bank, not an
    // independent invented counter.
    for(const auto& marker:maze::package_progress_markers(
            boxes_.total,boxes_.total-boxes_.remaining)) {
        const auto* image=maze::surface_binding(marker.surface);
        if(image)
            output.draws.push_back({
                std::string(image->filename),
                marker.destination.x,marker.destination.y,
                std::nullopt,true
            });
    }
}

ActivityFrameOutput MazeDriver::draw() const {
    ActivityFrameOutput output;
    if(!data_)return output;
    const auto current=static_cast<maze::Screen>(player_.screen);
    const auto* background=maze::surface_binding(maze::background_surface(current));
    output.draws.push_back({std::string(background->filename),0,0,std::nullopt,false});
    append_boxes(output,player_.screen,0);
    if(output.fatal_error)return output;
    const auto sprite=maze::actor_source_rect(
        player_.frozen?pickup_animation_frame_:drive_animation_frame_,player_.facing);
    const auto where=maze::player_destination(player_.x,player_.y);
    output.draws.push_back({
        "Data\\SubGameMaze\\player.bmp",where.x,where.y,
        Rect{sprite.left,sprite.top,sprite.right,sprite.bottom},true
    });
    append_spud(output,player_.screen,0);
    append_hud(output);
    return output;
}

ActivityFrameOutput MazeDriver::draw_transition(const maze::ScreenTransitionFrame& f) const {
    if(f.completed)return draw();
    ActivityFrameOutput output;
    const auto* background=maze::surface_binding(
        maze::background_surface(transition_.previous_screen));
    // 0041C710 preserves the previous frame border and HUD, then
    // draws the two moving strips inside the 600x380 playfield.
    output.draws.push_back({std::string(background->filename),0,0,std::nullopt,false});
    append_hud(output);
    const auto strip=[&](const maze::TransitionBlit& b) {
        if(b.source.right<=b.source.left)return;
        const auto* binding=maze::surface_binding(maze::background_surface(b.screen));
        output.draws.push_back({std::string(binding->filename),b.destination_x,b.destination_y,
            Rect{b.source.left,b.source.top,b.source.right,b.source.bottom},false});
    };
    strip(f.left_blit);strip(f.right_blit);
    // 0041C110: new-screen boxes first, old-screen Spud and boxes next.
    // The player is hidden until the slide finishes.
    append_boxes(output,static_cast<int>(transition_.destination_screen),f.actor_offset_a);
    append_spud(output,static_cast<int>(transition_.previous_screen),f.actor_offset_b);
    append_boxes(output,static_cast<int>(transition_.previous_screen),f.actor_offset_b);
    return output;
}

ActivityFrameOutput MazeDriver::advance(const ActivityFrameInput& input) {
    ActivityFrameOutput out=draw();
    if(!data_||out.fatal_error)return out;

    // Original 0x0041D4D0 gates active movement behind the opening
    // 0x49+difficulty narrator line. Never signal its finish by a fixed
    // fake timeout; the physical managed-sound observation is required.
    // 0041D4D0 checks the independently owned engine buffer and starts
    // it looping if stopped, even during the opening narrator gate.
    // The original disc spells the filename with a literal space.
    if(exit_phase_==0 && maze::classify_end_condition(boxes_.remaining,
           remaining_seconds_,transition_.phase)==maze::EndCondition::None)
        out.audio.push_back({AudioOperation::EnsureActivityLoop,
                            "Data/SubGameMaze/lofty engine.wav"});
    if(!intro_voice_queued_) {
        // 0041D4D0 invokes PlayActivityMusicByIndex(5) before the
        // interruptible opening narrator. The original WAV exists on disc.
        out.audio.push_back({AudioOperation::StartBackingTrack,"Data/music/maze.wav"});
        out.audio.push_back({AudioOperation::StopManagedSounds});
        out.audio.push_back({
            AudioOperation::ManagedSoundId,{},0x49+difficulty_,90,1,true
        });
        intro_voice_queued_=true;
        return out;
    }
    if(!intro_done_) {
        const bool opening_voice=input.maze_intro_voice_playing
            ?(*input.maze_intro_voice_playing)[static_cast<std::size_t>(difficulty_)]
            :input.managed_sound_playing;
        if(opening_voice)return out;
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
    if(maze::classify_end_condition(boxes_.remaining,remaining_seconds_,
            transition_.phase)!=maze::EndCondition::None || exit_phase_!=0) {
        if(exit_phase_==0) {
            won_=boxes_.remaining<=0;
            out.audio.push_back({AudioOperation::StopManagedSounds});
            // Source 0x0041D4D0: 0x54 (fast win), 0x55 (late win),
            // 0x56 (timeout) chosen before the voice-drain gate.
            const int voice=won_ ?
                (remaining_seconds_<16?0x55:0x54) : 0x56;
            out.audio.push_back({
                AudioOperation::ManagedSoundId,{},voice,90,1
            });
            exit_phase_=1;
            return out;
        }
        if(exit_phase_==1) {
            if(input.managed_sound_playing)return out;
            exit_phase_=2;
            return out;
        }
        // The source completion flag is granted only after feedback
        // drains. Timeout writes no progress at all.
        if(won_)
            out.progress_writes.push_back({progress::Slot::Maze,1});
        out.save_and_unload=true;
        out.next_outer_state=
            static_cast<int>(game_flow::State::PlayAgainYesNoSetup);
        out.next_saved_state=static_cast<int>(game_flow::State::MazeInit);
        out.next_replay_class=1;
        return out;
    }
    bool began_transition=false;
    std::optional<maze::ScreenTransitionFrame> slide;
    ActivityFrameOutput rendered;
    try {
        if(transition_.phase!=0) {
            // 0041D4D0 suppresses player input while the slide advances.
            slide=maze::advance_screen_transition(transition_);
            if(slide->completed)player_.transition=0;
        } else if(player_.frozen) {
            // Source 0x0041C110 holds the player while the pickup
            // animation advances from row 3 through row 7.
            if(--pickup_animation_wait_<1) {
                ++pickup_animation_frame_;
                pickup_animation_wait_=20;
                if(pickup_animation_frame_>7) {
                    player_.frozen=false;
                    pickup_animation_frame_=0;
                    drive_animation_frame_=0;drive_animation_wait_=20;
                    const int voice=pickup_voice_sequence_<6
                        ? 0x4d+pickup_voice_sequence_
                        : 0x51+(random_stream().next_rand()&1);
                    ++pickup_voice_sequence_;
                    out.audio.push_back({
                        AudioOperation::ManagedSoundId,{},voice,10,1
                    });
                }
            }
        } else {
            MazePlayerPort io(input,out,player_,input_mode_,
                              mouse_debounce_,pointer_sampled_,random_stream());
            const auto previous_screen=player_.screen;
            original_motion4::update_maze_player(player_,io);
            if(player_.screen!=previous_screen) {
                transition_={};
                transition_.previous_screen=static_cast<maze::Screen>(previous_screen);
                transition_.destination_screen=static_cast<maze::Screen>(player_.screen);
                began_transition=true;
            } else if(player_.x!=player_.previous_x||player_.y!=player_.previous_y) {
                // 0041C110: three driving columns, ten draw ticks each.
                if(--drive_animation_wait_<1) {
                    drive_animation_frame_=(drive_animation_frame_+1)%3;
                    drive_animation_wait_=10;
                }
            }
            // Original 0x0041B140 scans current screen's four boxes,
            // truncating player coordinates, radius strictly <46.
            if(!began_transition&&maze::collect_source_box(
                    boxes_,*data_,player_.screen,player_.x,player_.y)) {
                player_.frozen=true;
                pickup_animation_frame_=3;
                pickup_animation_wait_=0;
            }
        }
        // 0041D4D0 draws BEFORE updating the NPC. Keep the displayed
        // frame and the subsequent AI state separate, especially at pickup.
        if(!began_transition) {
            rendered=slide?draw_transition(*slide):draw();
            const int visible_screen=slide&&!slide->completed
                ?static_cast<int>(transition_.previous_screen):player_.screen;
            if(static_cast<int>(npc_.phase)>1 && npc_.screen==visible_screen)
                maze::advance_spud_sprite_animation(npc_.animation_frame,
                    npc_.animation_countdown,npc_.carry_active!=0);
        }
        // The original 0x0041D4D0 calls UpdateMazeSpudNPC after the
        // draw every active frame, including frozen pickups.
        // One shared package bank is authoritative for player and Spud.
        if(npc_.player_screen!=player_.screen){
            // 0041B140 reloads the offscreen timer on each portal.
            npc_.offscreen_wait=npc_.spawn_interval;
            if(npc_.phase==maze::SpudNpcPhase::SpawnWait)
                npc_.spawn_wait=npc_.spawn_interval;
        }
        npc_.player_screen=player_.screen;
        npc_.player_node=player_.node;
        const auto& player_graph=player_.graphs[static_cast<std::size_t>(player_.screen)];
        for(const auto& node:player_graph.nodes)
            if(node.id==player_.node){npc_.player_links=node.links;break;}
        npc_.packages=boxes_.screens;
        MazePlayerPort spud_io(input,out,player_,input_mode_,
                               mouse_debounce_,pointer_sampled_,random_stream());
        original_motion4::update_maze_spud_npc(npc_,spud_io);
        boxes_.screens=npc_.packages;
    } catch(const std::exception& e) {
        out.fatal_error=std::string("original Maze player motion: ")+e.what();
        return out;
    }
    // Source movement updates the actor BEFORE DrawMazeActivity; replace
    // the pre-input ordered draw plan with the new physical coordinates.
    // The portal-capture frame retains its prior presentation; the
    // next dispatch starts 0041C710. No input is accepted during sliding.
    if(began_transition)return out;
    rendered.audio=std::move(out.audio);
    return rendered;
}

bool MazeDriver::unload(std::string& error) {
    data_.reset();
    player_={};
    npc_={};
    boxes_={};
    input_mode_={};
    mouse_debounce_={};
    pointer_sampled_=false;
    transition_={};transition_.phase=0;
    drive_animation_frame_=0;drive_animation_wait_=0;
    pickup_animation_frame_=0;
    pickup_animation_wait_=0;
    pickup_voice_sequence_=0;
    won_=false;
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
