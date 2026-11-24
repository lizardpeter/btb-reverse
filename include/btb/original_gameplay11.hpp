#pragma once
// Original 11 gameplay and asset-loader functions compiled out of line.
// Source models are cross-referenced against original Ghidra PE32 addresses.
// Host resources and binary ABI still need retail differential verification.
#include "btb/gameplay_data_batch_20.hpp"
namespace btb::original_gameplay11 {
using namespace btb::gameplay_data_20;

[[nodiscard]] Frame advance_golf(
    Golf& s,bool any_managed_playing);

[[nodiscard]] Frame advance_squirrel(
    Squirrel& s,bool managed_playing,U32 random_0_to_2);

[[nodiscard]] Frame advance_band(Band& s);

[[nodiscard]] std::vector<std::string> load_ui_bitmap_names(std::istream& in);

[[nodiscard]] MovieNames load_startup_completion_movie_names(
    std::istream& start,std::istream& complete);

[[nodiscard]] std::vector<std::string> load_bink_walkthroughs(
    std::istream& in,std::size_t max_records=100);

[[nodiscard]] HelpWavSlots load_help_wav_table(std::istream& in);

[[nodiscard]] CreditsLoad load_credit_slideshow_names(
    std::istream& in);

[[nodiscard]] bool save_unload_fireworks(
    ResourceState& s,const btb::fireworks::RetailGrid& grid,
    std::ostream* opened_profile_file);

[[nodiscard]] bool save_unload_band(
    ResourceState& s,const btb::bobs_band::Composition& composition,
    std::ostream* opened_profile_file,std::ostream* optional_last_file,
    I32 conductor_index);

[[nodiscard]] std::optional<I32> distance_to_spud_screen_node(
    float x,float y,std::span<const std::pair<I32,I32>> nodes,
    I32 screen,I32 local_index);
[[nodiscard]] bool verify_11_gameplay_data_functions() noexcept;
}
