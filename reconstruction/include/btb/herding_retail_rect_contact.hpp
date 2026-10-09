#pragma once

#include <cstdint>

namespace btb::herding {

struct RetailCornerRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
    friend bool operator==(const RetailCornerRect&,
                           const RetailCornerRect&) = default;
};

// Exact helper at 0x004169A0 in the 311296-byte PE32 executable:
// checks four corners of FIRST rectangle against SECOND, with
// inclusive edges; does not check corners of second rectangle.
//
// Do not replace with symmetric AABB overlap. For a large first
// rectangle containing a small second one this can return FALSE,
// even though the rectangles geometrically overlap.
[[nodiscard]] constexpr bool original_herding_rect_contact(
    RetailCornerRect first,
    RetailCornerRect second) noexcept {

    const auto contains=[&](std::int32_t x,std::int32_t y) {
        return x>=second.left && x<=second.right &&
               y>=second.top && y<=second.bottom;
    };
    return contains(first.left,first.top) ||
           contains(first.right,first.top) ||
           contains(first.left,first.bottom) ||
           contains(first.right,first.bottom);
}

// Native UpdateHerdingAnimal 0x4179BF..0x417A2E builds the FIRST
// rectangle from the current animal's integer top-left + source
// cell width/height. It builds the SECOND from Scruffty in the same
// way. Both calculations are 32-bit ADD/SUB in the original PE.
[[nodiscard]] constexpr RetailCornerRect original_herding_entity_rect(
    const RetailEntityRecord32& e) noexcept {

    const auto wrap_sub=[](std::int32_t a,std::int32_t b) {
        return std::bit_cast<std::int32_t>(
            static_cast<std::uint32_t>(a) -
            static_cast<std::uint32_t>(b));
    };
    const auto wrap_add=[](std::int32_t a,std::int32_t b) {
        return std::bit_cast<std::int32_t>(
            static_cast<std::uint32_t>(a) +
            static_cast<std::uint32_t>(b));
    };
    return {e.x,e.y,
        wrap_add(e.x,wrap_sub(e.source_right,e.source_left)),
        wrap_add(e.y,wrap_sub(e.source_bottom,e.source_top))};
}

[[nodiscard]] constexpr bool original_herding_animal_hits_scruffty(
    const RetailEntityRecord32& animal,
    const RetailEntityRecord32& scruffty) noexcept {

    return is_herd_animal(animal.entity_type()) &&
           scruffty.entity_type()==EntityType::Scruffty &&
           original_herding_rect_contact(
               original_herding_entity_rect(animal),
               original_herding_entity_rect(scruffty));
}

} // namespace btb::herding
