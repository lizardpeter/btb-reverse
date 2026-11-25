#include "btb/full_game_runtime.hpp"

#include <array>
#include <exception>
#include <fstream>
#include <string>
#include <utility>
#include <system_error>

namespace btb::full_game {

void GameRoot::update_original_profile_screen(profiles::ProfileScreenState& state,
    const profiles::ProfileScreenInput& input,profiles::ProfileScreenServices& services) {
    // The same selector object survives returning from Activity Select.
    // Its last outer_state was 4; re-entering at 1 must re-arm the
    // original state-change detection and discard old selection latches.
    if(state.outer_state!=static_cast<int>(game_flow::State::PlayerProfileAndNameEntry))
        state={};
    const int prior=state.outer_state;
    profiles::update_player_profile_screen(state,input,globals_.profile_metadata,
        globals_.player_progress,services);
    if(state.outer_state!=prior) {
        if(state.selected_profile>=0)
            static_cast<void>(select_profile(state.selected_profile));
        globals_.dispatcher.current_state=state.outer_state;
    }
}

bool GameRoot::load_profile_files(
    const std::filesystem::path& directory,
    std::string& error) {

    if (initialized_activity_) {
        error = "cannot replace player records while an activity is running";
        return false;
    }

    // Stage every file before changing the live player or unlock state.
    // Retail stores playerinfo.txt separately from five ASCII progress
    // records, each with exactly 100 signed int32 values.
    profiles::ProfileTable metadata{};
    std::array<progress::Record, progress::kPlayerCount> records{};
    try {
        // An interrupted multi-file replacement may leave mixed generations.
        // Fail closed instead of silently loading or overwriting that state.
        if(std::filesystem::exists(directory/".btb-profile-write")) {
            error="unfinished native profile write; inspect .btb-profile-write before loading";
            return false;
        }
        const auto info_file = directory / "playerinfo.txt";
        if (std::filesystem::exists(info_file)) {
            std::ifstream in(info_file, std::ios::binary);
            if (!in) {
                error = "cannot open original playerinfo.txt";
                return false;
            }
            metadata = profiles::read_playerinfo(in);
        }

        for (std::size_t i = 0; i < progress::kPlayerCount; ++i) {
            const auto filename =
                "player" + std::to_string(i + 1) + ".txt";
            const auto source = directory / filename;
            if (!std::filesystem::exists(source)) {
                // 0x0042D270 writes -1 to the first progress word when
                // playerN.txt is absent; zero incorrectly means an existing
                // first-run save. The remaining 99 words stay zero.
                records[i].values[0] = -1;
                continue;
            }

            std::ifstream in(source, std::ios::binary);
            if (!in) {
                error = "cannot open original " + filename;
                return false;
            }
            records[i] = progress::read_record(in);
        }
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }

    // Do not silently keep old profile state after loading new game data.
    globals_.profile_metadata = std::move(metadata);
    globals_.player_progress = std::move(records);
    globals_.active_profile.reset();
    name_entry_.reset();
    name_entry_blue_font_.clear();
    name_entry_phase_ = 0;
    globals_.finale_gate = {};
    profile_directory_ = directory;
    profile_screen_state_={};
    activity_select_initialized_ = false;
    generic_front_end_initialized_.fill(false);
    error.clear();
    return true;
}

bool GameRoot::save_profile_files(
    const std::filesystem::path& directory,
    std::string& error) const {

    namespace fs=std::filesystem;
    // Compatibility safeguard for the native host, not a claimed retail
    // transaction protocol. The six original files retain their exact
    // format. Stage and close ALL six before touching the existing saves.
    const auto transaction=directory/".btb-profile-write";
    std::array<fs::path,6> names{};
    names[0]="playerinfo.txt";
    for(std::size_t i=1;i<names.size();++i)
        names[i]="player"+std::to_string(i)+".txt";
    std::array<bool,6> backed_up{},installed{};
    bool owned=false;
    try {
        fs::create_directories(directory);
        if(!fs::create_directory(transaction)) {
            error="unfinished or concurrent native profile write: "+transaction.string();
            return false;
        }
        owned=true;
        // Do not replace directories, special files or symbolic links.
        // Validate every destination before serializing or moving anything.
        for(const auto& name:names) {
            const auto status=fs::symlink_status(directory/name);
            if(fs::exists(status)&&!fs::is_regular_file(status))
                throw std::runtime_error("not a regular profile file: "+name.string());
        }
        for(std::size_t i=0;i<names.size();++i) {
            std::ofstream out(transaction/names[i],std::ios::binary|std::ios::trunc);
            if(!out)throw std::runtime_error("cannot stage "+names[i].string());
            if(i==0)profiles::write_playerinfo(out,globals_.profile_metadata);
            else progress::write_record(out,globals_.player_progress[i-1]);
            out.close();
            if(!out)throw std::runtime_error("cannot finish staging "+names[i].string());
        }
        for(std::size_t i=0;i<names.size();++i) {
            const auto destination=directory/names[i];
            if(fs::exists(destination)) {
                fs::rename(destination,transaction/(names[i].string()+".old"));
                backed_up[i]=true;
            }
            fs::rename(transaction/names[i],destination);
            installed[i]=true;
        }
    } catch(const std::exception& e) {
        error=e.what();
        if(owned) {
            bool rollback_ok=true;
            for(std::size_t i=names.size();i-- >0;) {
                std::error_code ec;
                if(installed[i]) {
                    fs::remove(directory/names[i],ec);
                    if(ec){rollback_ok=false;continue;}
                }
                if(backed_up[i]) {
                    fs::rename(transaction/(names[i].string()+".old"),directory/names[i],ec);
                    if(ec)rollback_ok=false;
                }
            }
            if(rollback_ok) {
                std::error_code ec;
                fs::remove_all(transaction,ec);
                if(ec)error+="; staging cleanup failed";
            }else{
                error+="; rollback incomplete; preserve "+transaction.string();
            }
        }
        return false;
    }
    // The new files are all installed. Failure to delete the backups is
    // reported, rather than pretending a new write can safely overwrite them.
    std::error_code ec;
    fs::remove_all(transaction,ec);
    if(ec) {
        error="profile files installed but staging cleanup failed: "+ec.message();
        return false;
    }
    error.clear();
    return true;
}

} // namespace btb::full_game
