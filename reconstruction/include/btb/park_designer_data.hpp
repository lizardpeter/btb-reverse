#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>

namespace btb::park_designer {

enum class Season : std::int32_t {
    Summer = 0,
    Winter = 1,
};

enum class EditorMode : std::int32_t {
    Pond = 0,
    Bandstand = 1,
    Decorate = 2,
    View = 3,
};

enum class EditorControl : std::int32_t {
    ScrollUp = 5,
    ScrollDown = 6,
    Pond = 7,
    Bandstand = 8,
    Decorate = 9,
    View = 10,
    DeleteOrDeleteAll = 11,
};

[[nodiscard]] constexpr EditorMode editor_mode_for_control(
    EditorControl control) noexcept {
    return static_cast<EditorMode>(
        static_cast<std::int32_t>(control)
        - static_cast<std::int32_t>(EditorControl::Pond));
}

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

struct BoundPolygon {
    std::vector<Vec2i> vertices;

    // Retail stores the -1,-1 sentinel in the 30-point backing slot and keeps
    // a count that includes it. PointInPolygon receives count-1.
    [[nodiscard]] std::size_t retail_stored_point_count() const noexcept {
        return vertices.size() + 1;
    }
};

inline constexpr std::size_t kBoundModes = 3;
inline constexpr std::size_t kBoundCategories = 4;
inline constexpr std::size_t kBoundVariants = 5;
inline constexpr std::size_t kBoundPolygonCount =
    kBoundModes * kBoundCategories * kBoundVariants;
inline constexpr std::size_t kRetailPolygonPointSlots = 30;

using BoundAreas = std::array<
    std::array<std::array<BoundPolygon, kBoundVariants>, kBoundCategories>,
    kBoundModes>;

[[nodiscard]] constexpr std::size_t bound_polygon_flat_index(
    std::size_t mode,
    std::size_t category,
    std::size_t variant) noexcept {
    return mode * (kBoundCategories * kBoundVariants)
         + category * kBoundVariants
         + variant;
}

BoundAreas parse_bound_areas(std::istream& in);

// Exact 32-bit retail Park Designer object record. Pointer fields stay uint32_t
// so the structure remains byte-identical when built on a 64-bit host.
struct RetailObjectRecord32 {
    std::int32_t left{};                // +0x00
    std::int32_t top{};                 // +0x04
    std::int32_t right{};               // +0x08
    std::int32_t bottom{};              // +0x0C
    std::uint32_t surface_ptr32{};      // +0x10
    std::int32_t unknown_14{};          // +0x14
    std::int32_t unknown_18{};          // +0x18
    std::int32_t object_code{};         // +0x1C
    std::int32_t object_subcode{};      // +0x20
    std::int32_t enabled_or_active{};   // +0x24
    std::int32_t sprite_width{};        // +0x28
    std::int32_t sprite_height{};       // +0x2C
    std::int32_t unknown_30{};          // +0x30
    std::int32_t record_index{};        // +0x34
    std::int32_t bound_category{};      // +0x38
    std::int32_t bound_variant{};       // +0x3C
    std::int32_t unknown_40{};          // +0x40
    std::int32_t unknown_44{};          // +0x44
    std::int32_t unknown_48{};          // +0x48
};

static_assert(sizeof(RetailObjectRecord32) == 0x4C);
static_assert(offsetof(RetailObjectRecord32, object_code) == 0x1C);
static_assert(offsetof(RetailObjectRecord32, record_index) == 0x34);
static_assert(offsetof(RetailObjectRecord32, bound_category) == 0x38);
static_assert(offsetof(RetailObjectRecord32, bound_variant) == 0x3C);

enum class RecordFamily : std::int32_t {
    PondPrimary,
    PondAuxiliary,
    Bandstand,
    Decorate,
};

[[nodiscard]] constexpr RecordFamily record_family_for_index(
    std::int32_t record_index) noexcept {
    return record_index < 100 ? RecordFamily::PondPrimary
         : record_index < 200 ? RecordFamily::PondAuxiliary
         : record_index < 300 ? RecordFamily::Bandstand
                              : RecordFamily::Decorate;
}

inline constexpr std::size_t kObjectRecordCount = 400;
inline constexpr std::size_t kTrailingStateValueCount = 28;
inline constexpr std::size_t kRetailSaveBytes =
    kObjectRecordCount * sizeof(RetailObjectRecord32)
    + kTrailingStateValueCount * sizeof(std::int32_t);

enum class TrailingStateIndex : std::size_t {
    PondSelectedPrimary = 0,       // global 0x00507A28
    Unknown507E38 = 1,
    PondSpecialRecord = 2,        // global 0x005079FC
    Unknown507E3C = 3,
    PondLinkedCount = 4,          // global 0x00507A40
    SelectedObjectRecord = 5,     // global 0x00507C54
    PondLinkedRecord0 = 6,        // 0x00507A2C
    PondLinkedRecord1 = 7,
    PondLinkedRecord2 = 8,
    PondLinkedRecord3 = 9,
    Unknown507B5C = 10,
    PondLinkedRecord4 = 11,
    ModePage0 = 12,               // 0x00507AE4
    ModePage1 = 13,
    ModePage2 = 14,
    Unknown507AF0 = 15,
    Unknown507AF4 = 16,
    Unknown508BF4 = 17,
    Unknown508BF8 = 18,
    Unknown508BFC = 19,
    Unknown508C00 = 20,
    Unknown508C04 = 21,
    NextPondPrimaryRecord = 22,   // global 0x00509340
    NextDecorateRecord = 23,      // global 0x00441DC8
    NextBandstandRecord = 24,     // global 0x00441DCC
    Unknown441DD0 = 25,
    Unknown441DD4 = 26,
    Season = 27,                  // global 0x00509344
};

struct SaveData {
    std::array<RetailObjectRecord32, kObjectRecordCount> objects{};
    std::array<std::int32_t, kTrailingStateValueCount> trailing_state{};

    [[nodiscard]] std::int32_t& state(TrailingStateIndex index) noexcept {
        return trailing_state[static_cast<std::size_t>(index)];
    }
    [[nodiscard]] const std::int32_t& state(TrailingStateIndex index) const noexcept {
        return trailing_state[static_cast<std::size_t>(index)];
    }
};

// Exact persistent portion of the confirmed Delete All path.
void delete_all_objects(SaveData& data) noexcept;

SaveData read_save(std::istream& in);
void write_save(std::ostream& out, const SaveData& data);

// The collision polygon family is selected from the record slot range itself.
[[nodiscard]] constexpr std::size_t bound_mode_for_record_index(
    std::int32_t record_index) noexcept {
    return record_index < 100 ? 0U
         : record_index < 300 ? 1U
                              : 2U;
}

[[nodiscard]] constexpr std::size_t bound_polygon_index_for_record(
    const RetailObjectRecord32& record) noexcept {
    return bound_polygon_flat_index(
        bound_mode_for_record_index(record.record_index),
        static_cast<std::size_t>(record.bound_category),
        static_cast<std::size_t>(record.bound_variant));
}

} // namespace btb::park_designer
