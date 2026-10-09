#include "btb/full_game_herding.hpp"

#include <exception>
#include <fstream>
#include <utility>

namespace btb::full_game {

bool HerdingDriver::initialize(
    int player_index,std::string& error) {

    if (initialized_) {
        error="Pets Corner already initialized; unload first";
        return false;
    }
    if (!simulation_) {
        error="Pets Corner requires original 0x4182B0 entity/AI provider";
        return false;
    }
    if (player_index < 0 ||
        player_index >= static_cast<int>(progress::kPlayerCount)) {
        error="Pets Corner original profile must be 0..4";
        return false;
    }
    if (difficulty_ < 0 || difficulty_ > 2) {
        error="Pets Corner difficulty must be retail Easy/Medium/Hard";
        return false;
    }
    std::ifstream source(directory_/"herd.txt");
    if (!source) {
        error="Pets Corner original Data/SubGame1/herd.txt missing";
        return false;
    }
    try {
        auto parsed=herding::parse_data(source);
        if (parsed.coordinate_groups.size()!=4 ||
            parsed.coordinate_groups[0].size()!=12 ||
            parsed.coordinate_groups[1].size()!=5 ||
            parsed.coordinate_groups[2].size()!=6 ||
            parsed.coordinate_groups[3].size()!=1) {
            error="Pets Corner herd.txt does not match installed retail "
                  "12/5/6/1 coordinate groups";
            return false;
        }
        if (!simulation_->initialize(parsed,difficulty_,error)) {
            return false;
        }
        data_=std::move(parsed);
        scene_={};
        player_index_=player_index;
        completion_stage_=0;
        startup_audio_emitted_=false;
        initialized_=true;
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error=exception.what();
        return false;
    }
}

ActivityFrameOutput HerdingDriver::advance(
    const ActivityFrameInput& input) {

    ActivityFrameOutput out;
    if (!initialized_) {
        out.fatal_error = "Pets Corner run entered before original setup";
        return out;
    }
    int undelivered=-1;
    std::string error;
    if (!simulation_->advance(input,scene_,undelivered,error)) {
        out.fatal_error = error.empty()
            ? "original Pets Corner entity simulation failed" : error;
        return out;
    }
    if (undelivered<0 ||
        undelivered>herding::initial_undelivered_animal_count(
            difficulty_)) {
        out.fatal_error =
            "Pets Corner provider returned an invalid retail animal count";
        return out;
    }

    auto composed=compose_original_herding_frame(scene_);
    if (composed.missing_farmer_pickles ||
        composed.rejected_unknown_entities!=0) {
        out.fatal_error =
            "Pets Corner provider omitted Pickles or supplied unmapped "
            "retail sprites; refusing to render an invented game frame";
        return out;
    }
    out.draws=std::move(composed.draws);

    if (!startup_audio_emitted_) {
        startup_audio_emitted_=true;
        out.audio.push_back({
            AudioOperation::StartBackingTrack,
            "data\\music\\petscorner.wav"
        });
        out.audio.push_back({
            AudioOperation::ManagedSoundId,{},581,50,1
        });
    }

    const auto complete=herding::herding_completion_step(
        undelivered,completion_stage_,
        input.managed_sound_playing,
        input.random_value & 1);
    completion_stage_=complete.stage;
    if (complete.action == herding::CompletionAction::PlayFinalLine) {
        out.audio.push_back({
            AudioOperation::ManagedSoundId,{},
            complete.sound_id,50,1
        });
    } else if (complete.action ==
               herding::CompletionAction::ExitToPlayAgain) {
        out.progress_writes.push_back({progress::Slot::PetsCorner,1});
        out.audio.push_back({AudioOperation::StopBackingTrack});
        out.next_saved_state =
            static_cast<int>(game_flow::State::HerdingInit);
        // Herding has Easy/Medium/Hard like Golf. Native replay class
        // still requires an executable write-site differential audit.
        // Keep the destination and completion record exact while
        // avoiding an unproven replay-class assignment.
        out.next_outer_state =
            static_cast<int>(game_flow::State::PlayAgainYesNoSetup);
        out.save_and_unload=true;
    }
    return out;
}

bool HerdingDriver::unload(std::string& error) {
    if (initialized_ && simulation_ &&
        !simulation_->unload(error)) {
        return false;
    }
    initialized_=false;
    startup_audio_emitted_=false;
    scene_={};
    completion_stage_=0;
    player_index_=-1;
    error.clear();
    return true;
}

} // namespace btb::full_game
