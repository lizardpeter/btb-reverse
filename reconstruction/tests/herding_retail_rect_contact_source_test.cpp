#include "btb/herding_retail_rect_contact.hpp"

#include <cassert>

using btb::herding::original_herding_rect_contact;
using btb::herding::RetailCornerRect;

constexpr RetailCornerRect small{4,4,6,6};
constexpr RetailCornerRect large{0,0,10,10};
constexpr RetailCornerRect outside{20,20,25,25};

// The retail x86 function is asymmetric. A has corners in B => 1.
// B's corners are NOT checked against A.
static_assert(original_herding_rect_contact(small,large));
static_assert(!original_herding_rect_contact(large,small));

// A cross-shaped overlap without FIRST rectangle corners inside
// SECOND returns false, unlike the usual intersection-of-AABBs test.
static_assert(!original_herding_rect_contact(
    {0,4,10,6},{4,0,6,10}));
static_assert(!original_herding_rect_contact(
    {4,0,6,10},{0,4,10,6}));

// Exact signed <=/>= checks include a corner touching the boundary.
static_assert(original_herding_rect_contact(
    {10,10,11,11},large));
static_assert(original_herding_rect_contact(
    {10,-3,15,0},large));
static_assert(!original_herding_rect_contact(outside,large));
static_assert(original_herding_rect_contact(large,large));

// Original 0x417A2E builds animal rectangle FIRST and Scruffty SECOND
// from the same 0x64-byte sprite record, using source-cell dimensions.
constexpr auto animal=[] {
    btb::herding::RetailEntityRecord32 e{};
    e.type=static_cast<int>(btb::herding::EntityType::Sheep);
    e.x=100;e.y=100;
    e.source_left=20;e.source_right=60;
    e.source_top=30;e.source_bottom=70;
    return e;
}();
constexpr auto scruffty=[] {
    btb::herding::RetailEntityRecord32 e{};
    e.type=static_cast<int>(btb::herding::EntityType::Scruffty);
    e.x=130;e.y=130;
    e.source_left=0;e.source_right=50;
    e.source_top=0;e.source_bottom=50;
    return e;
}();
static_assert(btb::herding::original_herding_entity_rect(animal)==
              (RetailCornerRect{100,100,140,140}));
static_assert(btb::herding::original_herding_animal_hits_scruffty(
    animal,scruffty));

constexpr auto nested_dog=[] {
    auto e=scruffty;
    e.x=140;e.y=140;
    e.source_right=20;e.source_bottom=20;
    return e;
}();
constexpr auto large_animal=[] {
    auto e=animal;
    e.source_left=0;e.source_right=100;
    e.source_top=0;e.source_bottom=100;
    return e;
}();
static_assert(!btb::herding::original_herding_animal_hits_scruffty(
    large_animal,nested_dog));

int main() {
    assert(original_herding_rect_contact(small,large));
    assert(!original_herding_rect_contact(large,small));
}
