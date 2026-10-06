#include "btb/bink_system.hpp"

namespace btb::bink {

std::optional<std::string> cd_fallback_path(
    std::string_view path,
    std::int32_t drive_index) {

    if (drive_index < 0) {
        return std::nullopt;
    }

    std::string out;
    out.reserve(3 + path.size());
    out.push_back(static_cast<char>('a' + drive_index));
    out += ":\\";
    out.append(path);
    return out;
}

OpenPlan generic_open_plan(
    std::string_view path,
    std::int32_t drive_index) {

    return {
        std::string(path),
        cd_fallback_path(path, drive_index),
        kGenericOpenFlags,
        true,
        false,
        false,
    };
}

OpenPlan global_open_plan(
    std::string_view path,
    std::int32_t drive_index) {

    return {
        std::string(path),
        cd_fallback_path(path, drive_index),
        kGlobalOpenFlags,
        true,
        true,
        true,
    };
}

} // namespace btb::bink
