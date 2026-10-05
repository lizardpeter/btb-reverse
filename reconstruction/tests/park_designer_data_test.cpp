#include "btb/park_designer_data.hpp"

#include <cassert>
#include <cstdint>
#include <sstream>
#include <string>

using namespace btb::park_designer;

int main() {
    static_assert(sizeof(RetailObjectRecord32) == 0x4C);
    static_assert(kObjectRecordCount == 400);
    static_assert(kTrailingStateValueCount == 28);
    static_assert(kRetailSaveBytes == 30512);

    static_assert(static_cast<int>(Season::Summer) == 0);
    static_assert(static_cast<int>(Season::Winter) == 1);
    static_assert(editor_mode_for_control(EditorControl::Pond) == EditorMode::Pond);
    static_assert(editor_mode_for_control(EditorControl::Bandstand) == EditorMode::Bandstand);
    static_assert(editor_mode_for_control(EditorControl::Decorate) == EditorMode::Decorate);
    static_assert(editor_mode_for_control(EditorControl::View) == EditorMode::View);

    // Generate the exact retail 3 x 4 x 5 polygon ordering without copying
    // proprietary source data into the test fixture.
    std::ostringstream source;
    for (std::size_t mode = 0; mode < kBoundModes; ++mode) {
        for (std::size_t category = 0; category < kBoundCategories; ++category) {
            for (std::size_t variant = 0; variant < kBoundVariants; ++variant) {
                const int base = static_cast<int>(
                    bound_polygon_flat_index(mode, category, variant) * 10);
                source << base << ' ' << base + 1 << ' '
                       << base + 2 << ' ' << base + 3 << ' '
                       << "-1 -1\n";
            }
        }
    }

    std::istringstream polygon_input(source.str());
    const auto bounds = parse_bound_areas(polygon_input);
    assert(bounds[0][0][0].vertices.size() == 2);
    assert((bounds[0][0][0].vertices[0] == Vec2i{0, 1}));
    assert((bounds[2][3][4].vertices[0] == Vec2i{590, 591}));
    assert(bounds[2][3][4].retail_stored_point_count() == 3);

    assert(bound_polygon_flat_index(0, 0, 0) == 0);
    assert(bound_polygon_flat_index(0, 3, 4) == 19);
    assert(bound_polygon_flat_index(1, 0, 0) == 20);
    assert(bound_polygon_flat_index(2, 3, 4) == 59);

    assert(record_family_for_index(0) == RecordFamily::PondPrimary);
    assert(record_family_for_index(99) == RecordFamily::PondPrimary);
    assert(record_family_for_index(100) == RecordFamily::PondAuxiliary);
    assert(record_family_for_index(199) == RecordFamily::PondAuxiliary);
    assert(record_family_for_index(200) == RecordFamily::Bandstand);
    assert(record_family_for_index(299) == RecordFamily::Bandstand);
    assert(record_family_for_index(300) == RecordFamily::Decorate);
    assert(record_family_for_index(399) == RecordFamily::Decorate);

    RetailObjectRecord32 record{};
    record.record_index = 42;
    record.bound_category = 3;
    record.bound_variant = 4;
    assert(bound_mode_for_record_index(record.record_index) == 0);
    assert(bound_polygon_index_for_record(record) == 19);

    record.record_index = 100;
    assert(bound_mode_for_record_index(record.record_index) == 1);
    assert(bound_polygon_index_for_record(record) == 39);

    record.record_index = 300;
    assert(bound_mode_for_record_index(record.record_index) == 2);
    assert(bound_polygon_index_for_record(record) == 59);

    SaveData save{};
    save.objects[0].left = 123;
    save.objects[0].top = 456;
    save.objects[0].object_code = 201;
    save.objects[0].record_index = 0;
    save.objects[0].bound_category = 2;
    save.objects[0].bound_variant = 4;
    save.objects[399].record_index = 399;
    save.trailing_state[0] = 0x11223344;
    save.trailing_state[27] = -7;

    std::ostringstream encoded(std::ios::binary);
    write_save(encoded, save);
    assert(encoded.str().size() == kRetailSaveBytes);

    std::istringstream decoded(encoded.str(), std::ios::binary);
    const auto roundtrip = read_save(decoded);
    assert(roundtrip.objects[0].left == 123);
    assert(roundtrip.objects[0].top == 456);
    assert(roundtrip.objects[0].object_code == 201);
    assert(roundtrip.objects[0].bound_category == 2);
    assert(roundtrip.objects[399].record_index == 399);
    assert(roundtrip.trailing_state[0] == 0x11223344);
    assert(roundtrip.trailing_state[27] == -7);

    auto reset = roundtrip;
    for (auto& object : reset.objects) {
        object.object_code = 42;
    }
    reset.state(TrailingStateIndex::NextPondPrimaryRecord) = 77;
    reset.state(TrailingStateIndex::NextBandstandRecord) = 222;
    reset.state(TrailingStateIndex::NextDecorateRecord) = 333;
    delete_all_objects(reset);
    for (const auto& object : reset.objects) {
        assert(object.object_code == -1);
    }
    assert(reset.state(TrailingStateIndex::NextPondPrimaryRecord) == 0);
    assert(reset.state(TrailingStateIndex::NextBandstandRecord) == 200);
    assert(reset.state(TrailingStateIndex::NextDecorateRecord) == 300);
}
