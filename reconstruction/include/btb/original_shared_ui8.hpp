#pragma once
#include "btb/shared_ui_native_9.hpp"
namespace btb::original_shared_ui8 {
using namespace btb::shared_game_batch_9;
[[nodiscard]] Plan load_generic_ui(UiScreen&,const btb::front_end::ReplacementScreen&);
[[nodiscard]] Plan unload_generic_ui(UiScreen&);
[[nodiscard]] Plan update_options(Options&,I32);
[[nodiscard]] Plan update_quit(Quit&,U32);
[[nodiscard]] Plan update_yes_no(YesNo&);
[[nodiscard]] Plan draw_profile_name(ProfileNamePopup&);
[[nodiscard]] std::pair<bool,Plan> update_bentley(Bentley&);
[[nodiscard]] Plan open_walkthrough(Walkthrough&,I32,
 const std::function<std::optional<std::vector<U8>>(const std::string&,std::size_t)>&);
[[nodiscard]] bool verify_8_original_shared_ui();
}
