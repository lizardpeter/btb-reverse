#pragma once
#include "btb/win32_frame_output.hpp"
#include <memory>
namespace btb::enhanced {
std::unique_ptr<full_game::Win32FrameOutput> make_vulkan_presenter();
}
