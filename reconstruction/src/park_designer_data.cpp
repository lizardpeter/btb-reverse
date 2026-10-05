#include "btb/park_designer_data.hpp"

#include <cstring>
#include <stdexcept>

namespace btb::park_designer {
namespace {

template <class T>
void read_exact(std::istream& in, T& value, const char* what) {
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (in.gcount() != static_cast<std::streamsize>(sizeof(value))) {
        throw std::runtime_error(std::string("short Park Designer save while reading ") + what);
    }
}

template <class T>
void write_exact(std::ostream& out, const T& value, const char* what) {
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    if (!out) {
        throw std::runtime_error(std::string("could not write Park Designer ") + what);
    }
}

} // namespace

BoundAreas parse_bound_areas(std::istream& in) {
    BoundAreas result{};

    for (std::size_t mode = 0; mode < kBoundModes; ++mode) {
        for (std::size_t category = 0; category < kBoundCategories; ++category) {
            for (std::size_t variant = 0; variant < kBoundVariants; ++variant) {
                auto& polygon = result[mode][category][variant];

                for (;;) {
                    Vec2i point{};
                    if (!(in >> point.x >> point.y)) {
                        throw std::runtime_error(
                            "boundareas.txt ended before all 60 polygons were read");
                    }
                    if (point.x == -1) {
                        // Retail tests only X for -1 after storing the pair.
                        break;
                    }
                    if (polygon.vertices.size() + 1 >= kRetailPolygonPointSlots) {
                        throw std::runtime_error(
                            "Park Designer polygon exceeds retail 30-point slot");
                    }
                    polygon.vertices.push_back(point);
                }
            }
        }
    }

    // Retail expects exactly the fixed 60-polygon corpus.
    std::int32_t unexpected = 0;
    if (in >> unexpected) {
        throw std::runtime_error("boundareas.txt contains data after polygon 60");
    }

    return result;
}

SaveData read_save(std::istream& in) {
    SaveData data;

    for (auto& object : data.objects) {
        read_exact(in, object, "0x4C object record");
    }
    for (auto& value : data.trailing_state) {
        read_exact(in, value, "trailing state value");
    }
    return data;
}

void write_save(std::ostream& out, const SaveData& data) {
    for (const auto& object : data.objects) {
        write_exact(out, object, "0x4C object record");
    }
    for (const auto value : data.trailing_state) {
        write_exact(out, value, "trailing state value");
    }
}

void clear_pond_primary_objects(
    SaveData& data,
    PondClearSelector selector) noexcept {

    for (std::size_t i = 0; i < 100; ++i) {
        auto& object = data.objects[i];
        switch (selector) {
            case PondClearSelector::AllPrimary:
                object.object_code = -1;
                data.state(TrailingStateIndex::PondSelectedPrimary) = -1;
                break;

            case PondClearSelector::ObjectCode7:
                if (object.object_code == 7) {
                    object.object_code = -1;
                    data.state(TrailingStateIndex::PondSpecialRecord) = -1;
                }
                break;

            case PondClearSelector::Non7CodeBelow100:
                if (object.object_code != 7 &&
                    object.object_code >= 0 &&
                    object.object_code < 100) {
                    object.object_code = -1;
                }
                break;
        }
    }
}

void clear_bandstand_objects(
    SaveData& data,
    BandstandClearSelector selector) noexcept {

    for (std::size_t i = 200; i < 300; ++i) {
        auto& object = data.objects[i];
        const auto category = object.bound_category;

        if (selector == BandstandClearSelector::Categories0To2) {
            if (category >= 0 && category <= 2) {
                object.object_code = -1;
            }
        } else if (selector == BandstandClearSelector::Category2Only) {
            if (category == 2) {
                object.object_code = -1;
            }
        }
    }
}

void delete_all_objects(SaveData& data) noexcept {
    for (auto& object : data.objects) {
        object.object_code = -1;
    }

    data.state(TrailingStateIndex::NextPondPrimaryRecord) = 0;
    data.state(TrailingStateIndex::NextBandstandRecord) = 200;
    data.state(TrailingStateIndex::NextDecorateRecord) = 300;
}

} // namespace btb::park_designer
