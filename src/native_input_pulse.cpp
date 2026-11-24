#include "btb/native_input_pulse.hpp"
namespace btb::full_game {
ConsumedNativeClick consume_native_click(bool& pending) noexcept {
    const bool pressed=pending;
    pending=false;
    return {pressed,pressed};
}
}
