#include "btb/full_game_park_designer.hpp"
#include "btb/retail_game_lifecycle_15.hpp"

#include <exception>
#include <fstream>
#include <string>
#include <system_error>

namespace btb::full_game {

std::filesystem::path ParkDesignerDriver::profile_filename() const {
    return save_dir_ / ("dypdata"+std::to_string(player_index_+1)+".txt");
}

bool ParkDesignerDriver::initialize(int player_index,std::string& error) {
    if(initialized()) {error="Park Designer is already initialized";return false;}
    if(player_index<0||player_index>=static_cast<int>(progress::kPlayerCount)) {
        error="Park Designer player outside five retail slots";return false;
    }
    // Source: LoadParkDesignerData uses SubGameDYP/boundareas.txt (60
    // sentinel-terminated authored polygons); never guess polygon dimensions.
    const auto bounds_file=original_data_dir_/"boundareas.txt";
    std::ifstream polygons(bounds_file);
    if(!polygons) {
        error="retail Park Designer boundareas.txt unavailable";return false;
    }
    try {
        auto parsed=park_designer::parse_bound_areas(polygons);
        park_designer::LoopState loaded{};
        const auto save_file=save_dir_/
            ("dypdata"+std::to_string(player_index+1)+".txt");
        std::error_code ec;
        const bool exists=std::filesystem::exists(save_file,ec);
        if(ec) {error="cannot inspect Park Designer save: "+ec.message();return false;}
        if(exists) {
            const auto size=std::filesystem::file_size(save_file,ec);
            if(ec||size!=park_designer::kRetailSaveBytes) {
                error="Park Designer save must be exactly 30512 retail bytes";
                return false;
            }
            std::ifstream binary(save_file,std::ios::binary);
            if(!binary) {error="cannot open original Park Designer save";return false;}
            loaded.save=park_designer::read_save(binary);
        } else {
            // Fresh original activity: 00410D10 clears the 400 records,
            // selection globals and original page counters once.
            loaded.reset_all_pending=true;
        }
        state_=std::move(loaded);
        bounds_=std::move(parsed);
        player_index_=player_index;
        awaiting_delete_all_confirmation_=false;
        error.clear();
        return true;
    } catch(const std::exception& e) {
        error=std::string("Park Designer data invalid: ")+e.what();
        return false;
    }
}

ActivityFrameOutput ParkDesignerDriver::advance(const ActivityFrameInput& input) {
    ActivityFrameOutput output{};
    if(!initialized()) {
        output.fatal_error="Park Designer run entered without native initialization";
        return output;
    }
    if(!service_) {
        output.fatal_error="Park Designer requires original native bitmap/editor/audio provider";
        return output;
    }
    if(input.confirmation_resolved&&awaiting_delete_all_confirmation_) {
        if(input.confirmation_yes)state_.reset_all_pending=true;
        awaiting_delete_all_confirmation_=false;
    }

    // The source uses 0x0044DDB0/0x0051C2E8 to interrupt editing.
    park_designer::LoopInput loop_input{};
    loop_input.managed_sound_playing=input.managed_sound_playing;
    loop_input.random_value=static_cast<std::uint32_t>(input.random_value);
    const auto frame=park_designer::advance_park_designer_loop(
        state_,loop_input,
        [&](const park_designer::SaveData& save,std::string& err) {
            return service_->render(save,*bounds_,output,err);
        },
        [&](park_designer::SaveData& save,std::string& err) {
            if(!service_->update_editor(save,*bounds_,input,output,err))
                return false;
            if(output.open_yes_no_confirmation)
                awaiting_delete_all_confirmation_=true;
            return true;
        });
    if(frame.error) {
        output.fatal_error=*frame.error;
        output.draws.clear();output.audio.clear();
        output.progress_writes.clear();
        output.save_and_unload=false;
        return output;
    }
    for(const auto& sound:frame.sound) {
        std::string error;
        if(!service_->emit_sound(sound,output,error)) {
            output.fatal_error=error.empty()
                ?"original Park Designer sound provider rejected source command"
                :error;
            output.draws.clear();output.audio.clear();
            output.progress_writes.clear();
            return output;
        }
    }
    if(frame.completed) {
        if(frame.mark_park_progress) {
            output.progress_writes.push_back({progress::Slot::ParkDesigner,1});
        }
        output.save_and_unload=true;
        if(frame.return_to_activity_select)
            output.next_outer_state=static_cast<int>(
                game_flow::State::ActivitySelectSetup);
    }
    return output;
}

bool ParkDesignerDriver::unload(std::string& error) {
    if(!initialized()){error.clear();return true;}
    std::error_code ec;
    std::filesystem::create_directories(save_dir_,ec);
    if(ec){error="cannot create Park Designer profile directory: "+ec.message();return false;}
    try {
        std::ofstream file(profile_filename(),std::ios::binary|std::ios::trunc);
        if(!file){error="cannot open Park Designer save for writing";return false;}
        if(!game_batch_15::save_park_designer(state_.save,&file)) {
            error="original SaveParkDesignerData failed to write 30512 bytes";
            return false;
        }
        file.flush();
        if(!file){error="failed to persist original 30512-byte Park Designer save";return false;}
    } catch(const std::exception& ex) {
        error=ex.what();return false;
    }
    bounds_.reset();state_={};player_index_=-1;
    awaiting_delete_all_confirmation_=false;
    error.clear();
    return true;
}

} // namespace btb::full_game
