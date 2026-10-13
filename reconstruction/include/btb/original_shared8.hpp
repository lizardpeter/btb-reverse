#pragma once
// Native out-of-line implementations of eight original PE32 UI and
// walkthrough procedures. Based on original Ghidra analysis; not
// exact original x86 ABI.
#include "btb/shared_ui_native_9.hpp"
namespace btb::original_shared8 {
using namespace btb::shared_game_batch_9;
[[nodiscard]] Plan load_generic_ui(UiScreen&,const front_end::ReplacementScreen&); // 00429150
[[nodiscard]] Plan unload_generic_ui(UiScreen&); // 00428ED0
[[nodiscard]] Plan update_options(Options&,I32); // 00429800
[[nodiscard]] Plan update_quit(Quit&,U32); // 00429AA0
[[nodiscard]] Plan update_yes_no(YesNo&); // 00429C90
[[nodiscard]] Plan draw_profile_name(ProfileNamePopup&); // 0042DB70
[[nodiscard]] std::pair<bool,Plan> update_bentley(Bentley&); // 00423BE0
using WalkthroughRead=std::function<std::optional<std::vector<U8>>(
 const std::string&,std::size_t)>;
[[nodiscard]] Plan open_walkthrough(Walkthrough&,I32,const WalkthroughRead&); // 004281D0
[[nodiscard]] bool verify_8_original_shared_ui_functions() noexcept;
}
