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

int main() {
    assert(original_herding_rect_contact(small,large));
    assert(!original_herding_rect_contact(large,small));
}
