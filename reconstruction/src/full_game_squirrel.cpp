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
int random_bounded(int n) {return std::rand()%n;}
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
                connector=random_bounded(3);
        connectors_=squirrel::chain_run_plan(plan);
        data_=std::move(candidate);
        placed_piece_ids_.fill(-1);
        assembly_={};
        level_=0;placed_count_=0;selected_choice_=-1;
        animating_=false;complete_=false;
        completion_stage_=0;
        player_=player;
        if(!generate_choices(error)) {
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
    const auto mix=squirrel::conveyor_mix(
        static_cast<squirrel::Difficulty>(difficulty_));
    const auto variant=squirrel::kOuterVariantPermutations[
        static_cast<std::size_t>(random_bounded(7))];
    // Retail 0x00426F30 generates two groups before randomly
    // interleaving them with repeated rand()&1 draws. Never put all
    // correct options first as a fixed or trivially guessable order.
    std::array<squirrel::ConnectorPair,4> corrects{};
    std::array<squirrel::ConnectorPair,4> decoys{};
    for(int i=0;i<mix.correct_items;++i)
        corrects[static_cast<std::size_t>(i)]=required;
    for(int i=0;i<mix.decoy_items;++i) {
        auto& pair=decoys[static_cast<std::size_t>(i)];
        do {pair.from=random_bounded(3);}
            while(pair.from==required.from);
        do {pair.to=random_bounded(3);}
            while(pair.to==required.to);
    }
    int correct_idx=0,decoy_idx=0;
    for(int i=0;i<mix.total_items();++i) {
        // The source rerolls its choice bit until there are slots left
        // in the selected group. Cap broken PRNG providers fail closed.
        bool pick_correct=false;
        bool selected=false;
        for(int attempt=0;attempt<4096;++attempt) {
            pick_correct=(std::rand()&1U)==0U;
            if(pick_correct?correct_idx<mix.correct_items:
                            decoy_idx<mix.decoy_items) {
                selected=true;
                break;
            }
        }
        if(!selected) {
            error="original Squirrel choice RNG did not select a live group";
            return false;
        }
        const auto& pair=pick_correct
            ?corrects[static_cast<std::size_t>(correct_idx++)]
            :decoys[static_cast<std::size_t>(decoy_idx++)];
        choices_[static_cast<std::size_t>(i)]={
            squirrel::encode_piece_id({
                variant[static_cast<std::size_t>(i)],
                pair.from,pair.to
            }),pick_correct,true
        };
    }
    return true;
}

ActivityFrameOutput SquirrelDriver::draw() const {
    ActivityFrameOutput out;
    if(!data_)return out;
    const auto base=std::string(kSubgame);
    out.draws.push_back({
        base+"level"+std::to_string(level_+1)+".bmp",0,0,
        std::nullopt,false
    });
    sprite(out,"conveyor.bmp",0,417,
        Rect{0,0,640,44});
    sprite(out,"body_01.bmp",
        squirrel::kLoftyBodyX,squirrel::kLoftyBodyY);
    sprite(out,"eyes.bmp",squirrel::kLoftyEyesX,
        squirrel::kLoftyEyesY,
        Rect{0,0,squirrel::kEyesFrameWidth,
                  squirrel::kEyesFrameHeight});
    sprite(out,"mouth.bmp",squirrel::kLoftyMouthX,
        squirrel::kLoftyMouthY,
        Rect{0,0,squirrel::kMouthFrameWidth,
                  squirrel::kMouthFrameHeight});
    sprite(out,"loftyarmswing_8bit.bmp",
        squirrel::kLoftyArmSwingX,squirrel::kLoftyArmSwingY,
        Rect{0,0,squirrel::kArmSwingFrameWidth,
                  squirrel::kArmSwingFrameHeight});
    for(int i=0;i<placed_count_;++i) {
        const int id=placed_piece_ids_[
            static_cast<std::size_t>(level_)*4+
            static_cast<std::size_t>(i)];
        const auto& dest=squirrel::kCompletedRunPieceDestinations[
            static_cast<std::size_t>(i)];
        sprite(out,"all.bmp",dest.x,dest.y,
            as_rect(squirrel::run_piece_source_rect(id)));
    }
    for(std::size_t i=0;i<choices_.size();++i) {
        const auto& choice=choices_[i];
        if(!choice.active)continue;
        const auto& xy=squirrel::kConveyorItemOrigins[i];
        sprite(out,"allconveyor.bmp",xy.x,xy.y,
            as_rect(squirrel::run_piece_source_rect(choice.piece_id)));
    }
    if(animating_) {
        // The retail squirrel/run animation is a 95-tick four-phase
        // sequence, keyed from the actual selected connector IDs.
        // Its dedicated squirrel sprite runtime still needs source
        // frame-table closure, so only the authored run geometry is
        // presented here, not a fabricated animal frame.
        if(assembly_.sprite_mode>=0) {
            sprite(out,"hook.bmp",squirrel::kLoftyArmSwingX,0);
        }
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
    auto out=draw();
    if(!data_)return out;
    if(complete_) {
        const auto state=squirrel::completion_step(
            completion_stage_,input.managed_sound_playing,
            random_bounded(3));
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
    if(animating_) {
        const auto update=squirrel::tick_run_assembly_animation(
            assembly_,*data_);
        if(update.cycle_completed) {
            animating_=false;
            ++placed_count_;
            if(placed_count_>=3) {
                if(level_==difficulty_) {
                    complete_=true;
                    completion_stage_=0;
                } else {
                    ++level_;placed_count_=0;
                }
            }
            if(!complete_) {
                std::string error;
                if(!generate_choices(error))out.fatal_error=error;
            }
        }
        {auto frame=draw();frame.audio=std::move(out.audio);frame.fatal_error=std::move(out.fatal_error);return frame;}
    }
    if(!input.click_pulse)return out;
    for(std::size_t i=0;i<choices_.size();++i) {
        const auto& target=squirrel::kConveyorItemOrigins[i];
        if(input.pointer_x<target.x ||
           input.pointer_x>=target.x+squirrel::kConveyorHitWidth ||
           input.pointer_y<target.y ||
           input.pointer_y>=target.y+squirrel::kConveyorHitHeight)
            continue;
        auto& chosen=choices_[i];
        if(!chosen.active)return out;
        if(!chosen.correct) {
            // Decoys are not counted as built track or source victories.
            out.audio.push_back({
                AudioOperation::ManagedSoundId,{},
                squirrel::decoy_lofty_feedback_sound_id(
                    random_bounded(2)),50,1
            });
            return out;
        }
        const auto storage=static_cast<std::size_t>(level_)*4+
                           static_cast<std::size_t>(placed_count_);
        placed_piece_ids_[storage]=chosen.piece_id;
        const int section=placed_count_*2+1;
        const auto keys=squirrel::select_run_assembly_motion_keys(
            section,static_cast<std::size_t>(level_),
            connectors_,placed_piece_ids_);
        if(!keys) {
            out.fatal_error="retail Squirrel motion-key selection failed";
            return out;
        }
        assembly_={};
        assembly_.run_section=section;
        squirrel::begin_run_assembly_animation(assembly_,*data_,*keys);
        animating_=true;
        selected_choice_=static_cast<int>(i);
        chosen.active=false;
        out.audio.push_back({
            AudioOperation::ManagedSoundId,{},
            squirrel::correct_placement_feedback_sound_id(
                random_bounded(7)),50,1
        });
        return out;
    }
    return out;
}

bool SquirrelDriver::unload(std::string& error) {
    data_.reset();player_=-1;
    connectors_={};placed_piece_ids_={};
    choices_={};assembly_={};
    level_=0;placed_count_=0;selected_choice_=-1;
    animating_=false;complete_=false;
    completion_stage_=0;
    error.clear();
    return true;
}

} // namespace btb::full_game
