#include "btb/fireworks_sequence.hpp"
#include "btb/fireworks_retail_core.hpp"
#include "btb/fireworks_layout.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <sstream>
#include <string>

using namespace btb::fireworks;

int main() {
    // Retail exe SHA256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
    // Original table VA 0x00442734 (0x42734 on-disk): all 12 uint32 values 1.
    static_assert(kOriginalFireworkLengths.size() == 12);
    for (auto n : kOriginalFireworkLengths) assert(n == 1);
    static_assert(kOriginalFireworkEmpty == -1);
    static_assert(sizeof(RetailGrid) == 72);

    Sequence authored;
    assert(authored.occupied_count() == 0);
    auto raw = authored.retail_grid();
    for (auto value : raw) assert(value == -1);

    const auto invalid = static_cast<FireworkType>(12);
    assert(!authored.validate_placement(0, 0, invalid));
    assert(!authored.commit_placement(0, 0, invalid));
    assert(!authored.validate_placement(3, 0, FireworkType::RedAirbomb));
    assert(!authored.validate_placement(0, 6, FireworkType::RedAirbomb));
    assert(!authored.remove(0, 0));

    assert(authored.validate_placement(0, 5, FireworkType::MediumRed));
    assert(authored.commit_placement(0, 5, FireworkType::MediumRed));
    assert(!authored.commit_placement(0, 5, FireworkType::BlueCandle));
    assert(authored.commit_placement(1, 0, FireworkType::BlueCandle));
    raw = authored.retail_grid();
    assert(raw[5] == 2 && raw[6] == 11);
    assert(raw[4] == -1);
    assert(authored.at(0, 5) == FireworkType::MediumRed);
    assert(authored.at(1, 0) == FireworkType::BlueCandle);
    assert(authored.slots()[5] == FireworkType::MediumRed);
    assert(authored.slots()[6] == FireworkType::BlueCandle);

    // Saved game format is exactly 18 little-endian 32-bit signed integers.
    std::ostringstream saved(std::ios::binary);
    write_retail_grid(saved, authored.retail_grid());
    assert(saved.str().size() == 72);
    const auto bytes = saved.str();
    assert(static_cast<unsigned char>(bytes[20]) == 2);
    assert(static_cast<unsigned char>(bytes[24]) == 11);
    std::istringstream input(bytes, std::ios::binary);
    const RetailGrid restored_grid = read_retail_grid(input);
    assert(restored_grid == authored.retail_grid());

    Sequence restored;
    assert(restored.load_retail_grid(restored_grid));
    assert(restored.occupied_count() == 2);
    assert(restored.events_for_column(5).size() == 1);
    assert(restored.events_for_column(5)[0].type == FireworkType::MediumRed);
    assert(restored.events_for_column(0).size() == 1);
    assert(restored.events_for_column(0)[0].type == FireworkType::BlueCandle);

    auto corrupt = restored.retail_grid();
    corrupt[0] = 99;
    assert(!restored.load_retail_grid(corrupt));
    assert(restored.retail_grid() == restored_grid);
    corrupt = restored_grid;
    corrupt[17] = -2;
    assert(!restored.load_retail_grid(corrupt));
    assert(restored.retail_grid() == restored_grid);

    assert(restored.replace(0, 5, FireworkType::RedSpinner));
    assert(restored.retail_grid()[5] == 9);
    assert(restored.at(0, 5) == FireworkType::RedSpinner);
    assert(restored.remove(0, 5) == FireworkType::RedSpinner);
    assert(restored.retail_grid()[5] == -1);
    assert(!restored.remove(0, 5));
    assert(restored.occupied_count() == 1);

    for (std::size_t row = 0; row < 3; ++row) {
        for (std::size_t col = 0; col < 6; ++col) {
            if (restored.empty(row, col))
                assert(restored.commit_placement(row, col, FireworkType::RedAirbomb));
        }
    }
    assert(restored.occupied_count() == 18);
    assert(restored.full());
    for (auto value : restored.retail_grid()) assert(value >= 0 && value < 12);
    restored.clear();
    assert(restored.occupied_count() == 0);
    for (auto value : restored.retail_grid()) assert(value == -1);
}
