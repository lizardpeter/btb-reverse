#pragma once
// A single atomic consumption of the Win32 mouse pulse per simulation tick.
// Keeps retail activity click and native primary-input observation identical,
// instead of clearing the input before passing it to the latter.
namespace btb::full_game {
struct ConsumedNativeClick {
    bool activity_click{};
    bool primary_input_click{};
};
[[nodiscard]] ConsumedNativeClick consume_native_click(bool& pending) noexcept;
}
