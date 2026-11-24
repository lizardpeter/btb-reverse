#include "btb/player_progress.hpp"

#include <stdexcept>

namespace btb::progress {

Record read_record(std::istream& in) {
    Record record;
    for (auto& value : record.values) {
        if (!(in >> value)) {
            throw std::runtime_error("short player progress record");
        }
    }
    return record;
}

void write_record(std::ostream& out, const Record& record) {
    for (const auto value : record.values) {
        out << value << ' ';
        if (!out) {
            throw std::runtime_error("could not write player progress record");
        }
    }
}

} // namespace btb::progress
