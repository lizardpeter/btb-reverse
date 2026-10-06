#include "btb/front_end_ui_data.hpp"

#include <charconv>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace btb::front_end {
namespace {

[[nodiscard]] std::string normalize_optional_token(std::string value) {
    if (value == "NULL") {
        value.clear();
    }
    return value;
}

[[nodiscard]] std::int32_t parse_int_token(
    const std::string& token,
    const char* what) {

    std::int32_t value{};
    const auto* first = token.data();
    const auto* last = first + token.size();
    const auto result = std::from_chars(first, last, value);
    if (result.ec != std::errc{} || result.ptr != last) {
        throw std::runtime_error(
            std::string("invalid generic UI integer while reading ") + what);
    }
    return value;
}

[[nodiscard]] std::string remainder_after_two_ints(
    const std::string& line) {

    std::istringstream row(line);
    std::int32_t x{};
    std::int32_t y{};
    row >> x >> y;

    std::string tail;
    std::getline(row, tail);
    const auto first = tail.find_first_not_of(" 	");
    if (first == std::string::npos) {
        return {};
    }
    return tail.substr(first);
}

} // namespace

std::vector<std::int32_t> parse_hot_area_counts(std::istream& in) {
    std::vector<std::int32_t> out;
    std::int32_t value{};

    while (in >> value) {
        if (value == -1) {
            return out;
        }
        out.push_back(value);
    }

    throw std::runtime_error(
        "NumUiHotArea stream ended before retail -1 sentinel");
}

std::vector<HotAreaScreen> parse_hot_areas(std::istream& in) {
    std::vector<HotAreaScreen> screens;

    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '') {
            line.pop_back();
        }
        if (!line.empty()) {
            screens.push_back({line,{}});
            break;
        }
    }

    if (screens.empty()) {
        throw std::runtime_error("uiHotArea stream has no initial screen header");
    }

    HotArea current_area;

    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }

        std::istringstream row(line);
        std::int32_t x{};
        std::int32_t y{};
        if (!(row >> x >> y)) {
            throw std::runtime_error(
                "uiHotArea row does not begin with two integers");
        }

        if (x == -99) {
            if (!current_area.polygon.empty()) {
                screens.back().areas.push_back(std::move(current_area));
            }
            return screens;
        }

        if (x == -2) {
            if (!current_area.polygon.empty()) {
                screens.back().areas.push_back(std::move(current_area));
                current_area = {};
            }

            auto header = remainder_after_two_ints(line);
            if (header.empty()) {
                // The retail fscanf implementation can consume the first -2
                // as a string after a preceding unlabeled -1 row, leaving the
                // second -2 and then reading this header as the next %s. The
                // line-oriented reconstruction accepts the canonical source
                // representation directly.
                while (std::getline(in, header)) {
                    if (!header.empty() && header.back() == '') {
                        header.pop_back();
                    }
                    if (!header.empty()) {
                        break;
                    }
                }
            }
            screens.push_back({std::move(header),{}});
            continue;
        }

        if (x == -1) {
            std::string label;
            row >> label;
            current_area.separator_label = std::move(label);
            screens.back().areas.push_back(std::move(current_area));
            current_area = {};
            continue;
        }

        current_area.polygon.push_back({x,y});
    }

    throw std::runtime_error(
        "uiHotArea stream ended before retail -99 sentinel");
}

std::vector<ReplacementScreen> parse_replacement_table(std::istream& in) {
    std::vector<ReplacementScreen> screens;

    std::string header;
    if (!(in >> header)) {
        throw std::runtime_error(
            "uiHotAreaReplace stream has no initial screen header");
    }
    screens.push_back({header,{}});

    std::string first_token;
    while (in >> first_token) {
        if (first_token == "-1") {
            if (!(in >> header)) {
                throw std::runtime_error(
                    "uiHotAreaReplace screen separator has no header");
            }
            if (header == "End") {
                return screens;
            }
            screens.push_back({header,{}});
            continue;
        }

        ReplacementRecord record;
        record.x = parse_int_token(first_token, "replacement x");

        if (!(in >> record.y
                 >> record.secondary_x
                 >> record.secondary_y
                 >> record.hover_bitmap_base
                 >> record.hover_sound_ids[0]
                 >> record.hover_sound_ids[1]
                 >> record.hover_sound_ids[2]
                 >> record.optional_surface
                 >> record.target_state_or_action
                 >> record.hover_frame_count
                 >> record.pressed_bitmap
                 >> record.click_sound_id
                 >> record.secondary_frame_count
                 >> record.secondary_bitmap_base)) {
            throw std::runtime_error(
                "short uiHotAreaReplace 15-field record");
        }

        record.optional_surface =
            normalize_optional_token(std::move(record.optional_surface));
        record.pressed_bitmap =
            normalize_optional_token(std::move(record.pressed_bitmap));
        record.secondary_bitmap_base =
            normalize_optional_token(std::move(record.secondary_bitmap_base));

        screens.back().records.push_back(std::move(record));
    }

    // The retail file normally ends with "-1 End"; accepting EOF after the
    // final record is useful for tests but keeps all parsed data intact.
    return screens;
}

} // namespace btb::front_end
