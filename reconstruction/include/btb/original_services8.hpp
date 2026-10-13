#pragma once
#include "btb/retail_service_batch_8.hpp"
namespace btb::original_services8 {
using namespace btb::retail_services;
[[nodiscard]] Plan release_inputs(Inputs&);
[[nodiscard]] Plan update_credits(Credits&,I32,bool);
[[nodiscard]] Plan decode_and_blit_bink(BinkFrame&);
[[nodiscard]] Plan advance_global_bink(GlobalBink&);
[[nodiscard]] Plan initialize_sound_manager(SoundManagerBoot&,bool,I32);
[[nodiscard]] bool sound_available(const ManagedBank&,I32);
[[nodiscard]] I32 find_sound_slot(const ManagedBank&,I32);
[[nodiscard]] AcquireResult acquire_sound(ManagedBank&,I32,I32,I32,bool);
[[nodiscard]] Plan play_managed_sound(ManagedBank&,I32,I32,I32,bool=true);
[[nodiscard]] Plan set_printer_scale(PrinterScale&,I32);
[[nodiscard]] bool verify_8_original_services();
}
