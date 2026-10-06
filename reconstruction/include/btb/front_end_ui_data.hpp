#pragma once

#include <array>
#include <cstdint>
#include <istream>
#include <string>
#include <vector>

namespace btb::front_end {

struct Point {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Point&, const Point&) = default;
};

struct HotArea {
    std::vector<Point> polygon{};
    // uiHotArea.txt may put a token after "-1 -1". The final area in a
    // screen commonly omits it because the following -2 screen sentinel is
    // sufficient to the retail fscanf parser.
    std::string separator_label{};
};

struct HotAreaScreen {
    std::string raw_header{};
    std::vector<HotArea> areas{};
};

[[nodiscard]] std::vector<std::int32_t> parse_hot_area_counts(
    std::istream& in);

[[nodiscard]] std::vector<HotAreaScreen> parse_hot_areas(
    std::istream& in);

struct ReplacementRecord {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t secondary_x{};
    std::int32_t secondary_y{};

    std::string hover_bitmap_base{};
    std::array<std::int32_t,3> hover_sound_ids{{-1,-1,-1}};

    std::string optional_surface{};
    std::int32_t target_state_or_action{-1};
    std::int32_t hover_frame_count{};

    std::string pressed_bitmap{};
    std::int32_t click_sound_id{-1};
    std::int32_t secondary_frame_count{};
    std::string secondary_bitmap_base{};
};

struct ReplacementScreen {
    std::string raw_header{};
    std::vector<ReplacementRecord> records{};
};

[[nodiscard]] std::vector<ReplacementScreen> parse_replacement_table(
    std::istream& in);

[[nodiscard]] constexpr std::int32_t available_hover_sound_count(
    const ReplacementRecord& record) noexcept {
    std::int32_t count = 0;
    for (const auto id : record.hover_sound_ids) {
        if (id != -1) {
            ++count;
        }
    }
    return count;
}

[[nodiscard]] constexpr std::int32_t selected_hover_sound_id(
    const ReplacementRecord& record,
    std::int32_t selected_index) noexcept {

    if (selected_index < 0) {
        return -1;
    }

    std::int32_t current = 0;
    for (const auto id : record.hover_sound_ids) {
        if (id == -1) {
            continue;
        }
        if (current == selected_index) {
            return id;
        }
        ++current;
    }
    return -1;
}

} // namespace btb::front_end
