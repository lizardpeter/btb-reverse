#include "btb/full_game_squirrel.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace btb::full_game {
namespace {
constexpr const char* kSubgame="Data\\SubGameSquirrel\\";
void sprite(ActivityFrameOutput& out,const char* name,int x,int y,
            std::optional<Rect> rect=std::nullopt) {
    out.draws.push_back({
        std::string(kSubgame)+name,x,y,rect,true
    });
}
std::optional<Rect> as_rect(
    const std::optional<squirrel::SourceRect>& src) {
    if(!src)return std::nullopt;
    return Rect{src->left,src->top,src->right,src->bottom};
}
} // namespace

void SquirrelDriver::configure_menu_state(
    const RetailMenuState& menu) noexcept {
    if((menu.variant_selection_origin==
            game_flow::State::SquirrelPregameUpdate ||
        menu.variant_selection_origin==
            game_flow::State::PlayAgainDifficultyUpdate) &&
       menu.source_variant>=0 && menu.source_variant<=2)
        difficulty_=menu.source_variant;
}

bool SquirrelDriver::initialize(int player,std::string& error) {
    if(data_) {error="Squirrel Run already loaded";return false;}
    if(player<0||player>=static_cast<int>(progress::kPlayerCount) ||
       difficulty_<0||difficulty_>2) {
        error="invalid Squirrel Run profile or difficulty";
        return false;
    }
    std::ifstream source(root_/"Data"/"SubGameSquirrel"/"sqdata.txt");
    if(!source) {error="retail Data/SubGameSquirrel/sqdata.txt missing";return false;}
    try {
        auto candidate=std::make_unique<squirrel::Data>(
            squirrel::parse_retail_data(source));
        if(candidate->animation_step_delay<=0)
            throw std::runtime_error("invalid original Squirrel motion delay");
        squirrel::RunPlan plan{};
        for(auto& level:plan)
            for(auto& connector:level)
                connector=(random_stream().next_rand()%3);
        connectors_=squirrel::chain_run_plan(plan);
        data_=std::move(candidate);
        placed_piece_ids_.fill(-1);
        assembly_={};conveyor_={};
        assembly_.run_section=0;
        assembly_.x=-data_->initial_horizontal_alignment_offset;
        level_=0;placed_count_=0;
        placement_={squirrel::PlacementState::IdleSelect,placement_.position};
        placement_slot_=0;
        mouth_pose_=mouth_draw_frame_=1;
        sprite_draw_frame_=0;sprite_draw_visible_=true;
        previous_high_sprite_=false;
        temporary_decoy_=carried_piece_=-1;speech_={};
        animating_=false;complete_=false;level_transition_pending_=false;
        completion_stage_=0;
        intro_stage_=0;
        player_=player;
        if(!generate_choices(error) || !start_run_section(error)) {
            data_.reset();
            return false;
        }
        error.clear();
        return true;
    } catch(const std::exception& e) {
        error=e.what();
        return false;
    }
}

bool SquirrelDriver::generate_choices(std::string& error) {
    choices_={};
    if(!data_||level_<0||level_>difficulty_||
       placed_count_<0||placed_count_>=3) {
        error="retail Squirrel choice generation outside active level";
        return false;
    }
    const auto required=squirrel::required_connectors(
        connectors_,static_cast<std::size_t>(level_),
        static_cast<std::size_t>(placed_count_));
    const auto generated=squirrel::generate_conveyor_choices(
        static_cast<squirrel::Difficulty>(difficulty_),required,random_stream());
    if(!generated) {
        error="original Squirrel choice generation failed";
        return false;
    }
    choices_=generated->choices;
    if(generated->landing_connector)current_choice_connector_=*generated->landing_connector;
    return true;
}

bool SquirrelDriver::start_run_section(std::string& error) {
    const auto keys=squirrel::select_run_assembly_motion_keys(
        assembly_.run_section,static_cast<std::size_t>(level_),
        connectors_,placed_piece_ids_);
    if(!keys) {
        error="retail Squirrel motion-key selection failed for section "+
            std::to_string(assembly_.run_section);
        return false;
    }
    landing_connector_=current_choice_connector_;
    squirrel::begin_run_assembly_animation(assembly_,*data_,*keys);
    animating_=true;
    return true;
}

ActivityFrameOutput SquirrelDriver::draw() const {
    ActivityFrameOutput out;
    if(!data_)return out;
    const auto base=std::string(kSubgame);
    out.draws.push_back({
        base+"level"+std::to_string(difficulty_+1)+".bmp",0,0,
        as_rect(squirrel::level_background_source_rect(level_)),false
    });
    sprite(out,"conveyor.bmp",0,417,
        Rect{conveyor_.animation_frame*640,0,
             (conveyor_.animation_frame+1)*640,44});
    sprite(out,"body_01.bmp",
        squirrel::kLoftyBodyX,squirrel::kLoftyBodyY);
    sprite(out,"eyes.bmp",squirrel::kLoftyEyesX,
        squirrel::kLoftyEyesY,
        as_rect(squirrel::eyes_source_rect(placement_.position.x/30)));
    sprite(out,"mouth.bmp",squirrel::kLoftyMouthX,
        squirrel::kLoftyMouthY,
        as_rect(squirrel::mouth_source_rect(mouth_draw_frame_)));
    sprite(out,"loftyarmswing_8bit.bmp",
        squirrel::kLoftyArmSwingX,squirrel::kLoftyArmSwingY,
        as_rect(squirrel::arm_swing_source_rect((placement_.position.x+28)/21)));
    for(int i=0;i<placed_count_;++i) {
        const int id=placed_piece_ids_[
            static_cast<std::size_t>(level_)*4+
            static_cast<std::size_t>(i)];
        const auto& dest=squirrel::kCompletedRunPieceDestinations[
            static_cast<std::size_t>(i)];
        sprite(out,"all.bmp",dest.x,dest.y,
            as_rect(squirrel::run_piece_source_rect(id)));
    }
    if(temporary_decoy_>=0) {
        const auto& dest=squirrel::kCompletedRunPieceDestinations[
            static_cast<std::size_t>(placement_slot_)];
        sprite(out,"all.bmp",dest.x,dest.y,
            as_rect(squirrel::run_piece_source_rect(temporary_decoy_)));
    }
    // 004267B0 calls BuildSquirrelFrameRect for these connector IDs too.
    for(std::size_t i=0;i<squirrel::kConnectorDestinations.size();++i) {
        const int connector=connectors_[
            static_cast<std::size_t>(level_)][i];
        if(connector<0||connector>=3) {
            out.fatal_error="retail Squirrel connector outside 0..2";
            return out;
        }
        const int id=squirrel::kConnectorPieceIds[
            static_cast<std::size_t>(connector)];
        const auto& dest=squirrel::kConnectorDestinations[i];
        sprite(out,"all.bmp",dest.x,dest.y,
            as_rect(squirrel::run_piece_source_rect(id)));
    }
    if(level_==difficulty_) {
        if(const auto nut=squirrel::nut_pile_destination(
                connectors_[static_cast<std::size_t>(level_)][3]))
            sprite(out,"nutpile.bmp",nut->x,nut->y);
    }
    // This is the actual 8772x92 original Squirrel sprite sheet.
    // Native assembly motion uses the recovered source x/y deltas
    // (0x00514F94 and 0x00514F98). The idle/active frame is in the
    // original 102px horizontal cell layout, not the 89px conveyor atlas.
    if(const auto origin=squirrel::source_squirrel_start_position(
          connectors_[static_cast<std::size_t>(level_)][0])) {
        if(const auto rect=squirrel::source_squirrel_frame_rect(sprite_draw_frame_);
           sprite_draw_visible_ && rect)
            sprite(out,"squriel_1_86.bmp",
                origin->x+assembly_.x,origin->y+assembly_.y,
                Rect{rect->left,rect->top,rect->right,rect->bottom});
    }
    // The carried piece follows the original actor origin, not the
    // conveyor slot; the hook remains visible in the normal scene.
    sprite(out,"hook.bmp",placement_.position.x+28,0,
        as_rect(squirrel::lofty_hook_source_rect(placement_.position.y)));
    if(placement_.carrying && carried_piece_>=0)
        sprite(out,"allconveyor.bmp",placement_.position.x,
            placement_.position.y-squirrel::kConveyorHitHeight,
            as_rect(squirrel::run_piece_source_rect(carried_piece_)));
    for(std::size_t i=0;i<choices_.size();++i) {
        const auto& choice=choices_[i];
        if(!choice.active || (placement_.carrying && placement_.selected==static_cast<int>(i)))continue;
        const auto& xy=squirrel::kConveyorItemOrigins[i];
        const int x=xy.x+conveyor_.position;
        const int left=std::max(x,0),right=std::min(x+89,640);
        if(right<=left)continue;
        auto rect=as_rect(squirrel::run_piece_source_rect(choice.piece_id));
        if(!rect)continue;
        rect->left+=left-x;
        rect->right-=x+89-right;
        sprite(out,"allconveyor.bmp",left,xy.y,rect);
    }
    for(const auto& border:squirrel::kFinalBorderDraws) {
        const auto* image=squirrel::surface_binding(border.surface);
        if(image)
            out.draws.push_back({
                std::string(image->filename),border.x,border.y,
                std::nullopt,true
            });
    }
    return out;
}

ActivityFrameOutput SquirrelDriver::advance(
    const ActivityFrameInput& input) {
    auto out=advance_gameplay(input);
    if(!data_ || out.fatal_error || complete_ || out.save_and_unload)return out;
    // 004279F0: music index 9, followed by 643,632,644.
    // The two later voices wait for the original managed bank to drain.
    if(intro_stage_==0) {
        out.audio.push_back({AudioOperation::StartBackingTrack,"Data/music/squirrelrun.wav"});
        out.audio.push_back({AudioOperation::ManagedSoundId,{},643,50,1,true});
        intro_stage_=1;
    } else if(intro_stage_<3 && !input.managed_sound_playing && out.audio.empty()) {
        out.audio.push_back({AudioOperation::ManagedSoundId,{},intro_stage_==1?632:644,50,1,true});
        ++intro_stage_;
    }
    return out;
}

ActivityFrameOutput SquirrelDriver::advance_gameplay(
    const ActivityFrameInput& input) {
    if(!data_)return {};
    // 004279F0 calls 00427320 BEFORE scene composition and input.
    // The recovered normal-speed branch moves 5px per tick, regenerates
    // once when outgoing offset exceeds 500, and returns from -420.
    const bool transition_frame=level_transition_pending_;
    if(transition_frame) {
        // 004273F0 writes the transition latch after the preceding draw.
        // 004279F0 dispatches 00425780 on the NEXT frame, before the scene.
        // 00446A44 is initialized to 1, selecting the direct transition.
        const auto old_origin=squirrel::source_squirrel_start_position(
            connectors_[static_cast<std::size_t>(level_)][0]);
        const int previous_y=old_origin->y+assembly_.y;
        ++level_;placed_count_=0;
        assembly_.state=squirrel::RunAssemblyAnimationState::Idle;
        const auto next_origin=squirrel::source_squirrel_start_position(
            connectors_[static_cast<std::size_t>(level_)][0]);
        assembly_.x=46-next_origin->x;
        assembly_.y=previous_y-next_origin->y;
        assembly_.run_section=1;
        conveyor_.phase=1;
        level_transition_pending_=false;
    }
    if(!transition_frame && !complete_ && conveyor_.phase!=0) {
        const int prior=conveyor_.phase;
        retail_activity::update_squirrel_conveyor(conveyor_,false,nullptr);
        if(prior==1 && conveyor_.phase==2) {
            std::string error;
            if(!generate_choices(error)) {
                ActivityFrameOutput failed;failed.fatal_error=error;return failed;
            }
        }
    }
    const bool speech_was_active=speech_.sound_id!=-1;
    if(speech_was_active) {
        const auto index=static_cast<std::size_t>(speech_.sound_id-633);
        const bool playing=input.squirrel_lofty_voice_playing
            ?(*input.squirrel_lofty_voice_playing)[index]
            :input.managed_sound_playing;
        speech_.advance(playing,random_stream());
    }
    mouth_draw_frame_=speech_was_active?speech_.mouth_frame:mouth_pose_;

    // 004267B0 updates the assembly before its live squirrel blit.
    // Keep the active draw even when state becomes idle on this tick.
    std::string animation_error;
    if(!complete_ && !animating_ && assembly_.run_section<=placed_count_*2 &&
       assembly_.run_section<=6) {
        if(!start_run_section(animation_error)) {
            ActivityFrameOutput failed;failed.fatal_error=animation_error;return failed;
        }
    }
    if(animating_) {
        const auto prior=assembly_.state;
        const auto tick=squirrel::tick_run_assembly_animation(assembly_,*data_);
        if((prior==squirrel::RunAssemblyAnimationState::SecondFirstThreeSubsteps ||
            prior==squirrel::RunAssemblyAnimationState::RetainedTransition) &&
            assembly_.state==squirrel::RunAssemblyAnimationState::SecondLastFourSubsteps &&
            previous_high_sprite_ && landing_connector_>=0 && landing_connector_<3) {
            // 004265C7..004265E9: high atlas transition restores absolute
            // Y before the first D-profile delta.
            constexpr std::array<int,3> target_y{237,205,173};
            const auto origin=squirrel::source_squirrel_start_position(
                connectors_[static_cast<std::size_t>(level_)][0]);
            assembly_.y=target_y[static_cast<std::size_t>(landing_connector_)]-origin->y+tick.delta.y;
        }
        sprite_draw_frame_=squirrel::active_assembly_sprite_frame(assembly_);
        previous_high_sprite_=sprite_draw_frame_>=63;
        sprite_draw_visible_=squirrel::assembly_sprite_visible(assembly_,sprite_draw_frame_,
            static_cast<std::size_t>(level_),placed_piece_ids_);
        if(tick.cycle_completed)animating_=false;
    } else {
        sprite_draw_frame_=squirrel::tick_assembly_idle_sprite(assembly_,
            data_->animation_step_delay,[&]{return random_stream().next_rand();});
        sprite_draw_visible_=sprite_draw_frame_>=0&&sprite_draw_frame_<86;
    }
    auto out=draw();
    if(complete_) {
        const auto state=squirrel::completion_step(
            completion_stage_,input.managed_sound_playing,
            completion_stage_==0?random_stream().next_rand()%3:0);
        completion_stage_=state.stage;
        if(state.action==squirrel::CompletionAction::PlayFinalWendyLine) {
            out.audio.push_back({AudioOperation::StopManagedSounds});
            out.audio.push_back({
                AudioOperation::ManagedSoundId,{},
                state.sound_id,50,1
            });
        } else if(state.action==squirrel::CompletionAction::ExitToPlayAgain) {
            out.progress_writes.push_back({
                progress::Slot::SquirrelRun,1
            });
            out.shared_completion_code=5;
            out.save_and_unload=true;
            out.next_outer_state=
                static_cast<int>(game_flow::State::PlayAgainYesNoSetup);
            out.next_saved_state=
                static_cast<int>(game_flow::State::SquirrelInit);
            out.next_ui_context=0x28;
            out.next_replay_class=1;
        }
        return out;
    }
    if(transition_frame)return out; // source skips placement in transition branch
    if(placement_.state!=squirrel::PlacementState::IdleSelect) {
        if(placement_.state==squirrel::PlacementState::ReturnAfterDecoy ||
           placement_.state==squirrel::PlacementState::ReturnDecoyToConveyor)
            mouth_pose_=2;
        const int selected=placement_.selected;
        const auto event=squirrel::advance_placement_motion(placement_,placement_slot_);
        auto& choice=choices_[static_cast<std::size_t>(selected)];
        using E=squirrel::PlacementEvent;
        if(event==E::PickedUp) {
            carried_piece_=choice.piece_id;
        } else if(event==E::CorrectDropped) {
            mouth_pose_=1;
            const auto storage=static_cast<std::size_t>(level_)*4+
                               static_cast<std::size_t>(placed_count_);
            placed_piece_ids_[storage]=carried_piece_;
            ++placed_count_;
            // The next scene update starts the next available section.
            choice.active=false;
            carried_piece_=-1;
            if(placed_count_<3)conveyor_.phase=1;
            const int feedback=random_stream().next_rand()%7;
            const int sound=squirrel::correct_placement_feedback_sound_id(feedback);
            speech_.begin(sound,random_stream());
            out.audio.push_back({AudioOperation::ManagedSoundId,{},sound,50,1});
        } else if(event==E::DecoyDropped) {
            temporary_decoy_=carried_piece_;
            carried_piece_=-1;
            choice.active=false;
            // 004273F0 first draws rand()%5; every branch then draws its
            // second random value. Preserve the raw decoy comparison word.
            const auto required=squirrel::required_connectors(
                connectors_,static_cast<std::size_t>(level_),
                static_cast<std::size_t>(placement_slot_));
            const bool generic=random_stream().next_rand()%5==0;
            const int random=random_stream().next_rand();
            int sound=0;
            if(generic || required.from==choice.feedback_comparison)
                sound=squirrel::decoy_lofty_feedback_sound_id(random%2);
            else
                sound=squirrel::decoy_direction_feedback_sound_id(
                    required.from,choice.feedback_comparison,
                    random%(required.from<choice.feedback_comparison?3:4));
            speech_.begin(sound,random_stream());
            out.audio.push_back({AudioOperation::ManagedSoundId,{},sound,50,1});
        } else if(event==E::DecoyRetrieved) {
            carried_piece_=temporary_decoy_;
            temporary_decoy_=-1;
        } else if(event==E::DecoyReturned) {
            choice.active=true;
            carried_piece_=-1;
        }
        return out;
    }
    // 004273F0 gates selection on idle Lofty + stationary conveyor +
    // run_section < 6, not on whether the squirrel itself is animating.
    // This permits the player to choose during the initial approach.
    if(conveyor_.phase!=0)return out;
    if(placed_count_>=3) {
        if(!squirrel::run_reached_level_end(assembly_.run_section))return out;
        if(level_==difficulty_) {
            complete_=true;completion_stage_=0;
            out.audio.push_back({AudioOperation::StopManagedSounds});
            out.audio.push_back({AudioOperation::ManagedSoundId,{},
                squirrel::level_finished_feedback_sound_id(random_stream().next_rand()%4),50,1});
        } else {
            level_transition_pending_=true;
        }
        return out;
    }
    if(assembly_.run_section>=6 || !input.click_pulse)return out;
    for(std::size_t i=0;i<choices_.size();++i) {
        const auto& target=squirrel::kConveyorItemOrigins[i];
        if(input.pointer_x<=target.x ||
           input.pointer_x>=target.x+squirrel::kConveyorHitWidth ||
           input.pointer_y<=target.y ||
           input.pointer_y>=target.y+squirrel::kConveyorHitHeight)
            continue;
        const auto& chosen=choices_[i];
        if(!chosen.active)return out;
        placement_slot_=placed_count_;
        mouth_pose_=0;
        (void)squirrel::select_placement(placement_,static_cast<int>(i),chosen.correct);
        return out;
    }
    return out;
}

bool SquirrelDriver::unload(std::string& error) {
    data_.reset();player_=-1;
    connectors_={};placed_piece_ids_={};
    choices_={};assembly_={};conveyor_={};
    level_=0;placed_count_=0;
    placement_={squirrel::PlacementState::IdleSelect,placement_.position};
    placement_slot_=0;
    mouth_pose_=mouth_draw_frame_=1;
    sprite_draw_frame_=0;sprite_draw_visible_=true;previous_high_sprite_=false;
    temporary_decoy_=carried_piece_=-1;speech_={};
    animating_=false;complete_=false;level_transition_pending_=false;
    completion_stage_=0;
    intro_stage_=0;
    error.clear();
    return true;
}

} // namespace btb::full_game
