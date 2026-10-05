#pragma once

#include <array>
#include <cstdint>

namespace btb::bobs_band {

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
};

struct Box4i {
    std::int32_t a{};
    std::int32_t b{};
    std::int32_t c{};
    std::int32_t d{};
};

} // namespace btb::bobs_band
