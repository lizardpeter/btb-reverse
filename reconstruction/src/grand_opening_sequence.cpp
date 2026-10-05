#include "btb/grand_opening_sequence.hpp"

#include <stdexcept>

namespace btb::grand_opening {

namespace {

template <class T>
void read_exact(std::istream& in, T& value, const char* what) {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (in.gcount() != static_cast<std::streamsize>(sizeof(value))) {
        throw std::runtime_error(
            std::string("short Grand Opening composition while reading ") + what);
    }
}

template <class T>
void write_exact(std::ostream& out, const T& value, const char* what) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    if (!out) {
        throw std::runtime_error(
            std::string("could not write Grand Opening ") + what);
    }
}

constexpr const char* kCompositionFilenames[15] = {
    "musicbob1.txt",
    "musicbob2.txt",
    "musicbob3.txt",
    "musicbob4.txt",
    "musicbob5.txt",
    "musicwendy1.txt",
    "musicwendy2.txt",
    "musicwendy3.txt",
    "musicwendy4.txt",
    "musicwendy5.txt",
    "musicfarmer1.txt",
    "musicfarmer2.txt",
    "musicfarmer3.txt",
    "musicfarmer4.txt",
    "musicfarmer5.txt",
};

} // namespace

Composition read_composition(std::istream& in) {
    Composition out;
    for (auto& row : out.cells) {
        for (auto& cell : row) {
            read_exact(in, cell, "5x24 grid cell");
        }
    }
    return out;
}

void write_composition(
    std::ostream& out,
    const Composition& composition) {
    for (const auto& row : composition.cells) {
        for (const auto cell : row) {
            write_exact(out, cell, "5x24 grid cell");
        }
    }
}

bool can_place_event(
    const Composition& composition,
    std::size_t row,
    std::size_t step,
    MachineType type) noexcept {

    if (row >= kVariationRowCount || step >= kTimelineStepCount) {
        return false;
    }

    const auto span = static_cast<std::size_t>(machine_span(type));
    if (step + span > kTimelineStepCount) {
        return false;
    }

    for (std::size_t i = 0; i < span; ++i) {
        if (composition.cells[row][step + i] != kEmptyCell) {
            return false;
        }
    }

    return true;
}

bool place_event(
    Composition& composition,
    std::size_t row,
    std::size_t step,
    MachineType type) noexcept {

    if (!can_place_event(composition, row, step, type)) {
        return false;
    }

    composition.cells[row][step] = static_cast<std::int32_t>(type);

    const auto span = static_cast<std::size_t>(machine_span(type));
    for (std::size_t i = 1; i < span; ++i) {
        composition.cells[row][step + i] = kContinuationCell;
    }

    return true;
}

std::int32_t remove_event_at(
    Composition& composition,
    std::size_t row,
    std::size_t step) noexcept {

    if (row >= kVariationRowCount || step >= kTimelineStepCount) {
        return kEmptyCell;
    }

    auto owner_step = step;
    auto value = composition.cells[row][owner_step];

    if (value == kEmptyCell) {
        return kEmptyCell;
    }

    while (value > 9 && owner_step > 0) {
        --owner_step;
        value = composition.cells[row][owner_step];
    }

    if (!is_machine_type(value)) {
        return kEmptyCell;
    }

    const auto type = static_cast<MachineType>(value);
    const auto span = static_cast<std::size_t>(machine_span(type));

    for (std::size_t i = 0;
         i < span && owner_step + i < kTimelineStepCount;
         ++i) {
        composition.cells[row][owner_step + i] = kEmptyCell;
    }

    return value;
}

const char* composition_filename(
    Conductor conductor,
    std::size_t zero_based_player_profile) noexcept {

    if (zero_based_player_profile >= kPlayerProfileCount) {
        return nullptr;
    }
    const auto index = composition_file_index(
        conductor, zero_based_player_profile);
    if (index >= 15) {
        return nullptr;
    }
    return kCompositionFilenames[index];
}

} // namespace btb::grand_opening
