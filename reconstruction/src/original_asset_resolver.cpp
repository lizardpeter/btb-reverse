#include "btb/original_asset_resolver.hpp"

#include <system_error>
#include <utility>

namespace btb::full_game {
namespace {

char fold_ascii(char c) noexcept {
    return c >= 'A' && c <= 'Z' ? static_cast<char>(c + ('a'-'A')) : c;
}

bool ascii_case_equal(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (fold_ascii(a[i]) != fold_ascii(b[i])) {
            return false;
        }
    }
    return true;
}

} // namespace

std::optional<std::vector<std::string>> split_retail_asset_path(
    std::string_view input) {

    if (input.empty() || input.front() == '/' ||
        input.front() == '\\') {
        return std::nullopt;
    }

    std::vector<std::string> parts;
    std::string part;
    for (char c : input) {
        if (c == '\0' || c == ':') {
            return std::nullopt;
        }
        if (c == '/' || c == '\\') {
            if (!part.empty()) {
                if (part == "." || part == "..") {
                    return std::nullopt;
                }
                parts.push_back(std::move(part));
                part.clear();
            }
        } else {
            part.push_back(c);
        }
    }
    if (!part.empty()) {
        if (part == "." || part == "..") {
            return std::nullopt;
        }
        parts.push_back(std::move(part));
    }
    if (parts.empty()) {
        return std::nullopt;
    }
    return parts;
}

AssetResolution OriginalAssetResolver::resolve_below(
    const std::filesystem::path& root,
    const std::vector<std::string>& components,
    AssetResolutionStatus success) {

    std::error_code ec;
    auto current = std::filesystem::absolute(root, ec);
    if (ec) {
        return {AssetResolutionStatus::FileSystemError, {},
                "invalid retail asset search root: " + ec.message()};
    }

    for (std::size_t i = 0; i < components.size(); ++i) {
        const bool last = i + 1 == components.size();
        const auto direct = current / components[i];
        // Use the *non-following* status API. A missing exact spelling
        // must fall through to case-insensitive enumeration, not be treated
        // as an I/O failure. Refuse symlinks that could leave the roots.
        const auto status = std::filesystem::symlink_status(direct, ec);
        if (ec == std::errc::no_such_file_or_directory) {
            ec.clear();
        }
        if (ec) {
            return {AssetResolutionStatus::FileSystemError, {},
                    "cannot inspect source asset: " + ec.message()};
        }
        if (!std::filesystem::is_symlink(status)) {
            const bool right_type = last
                ? std::filesystem::is_regular_file(status)
                : std::filesystem::is_directory(status);
            if (right_type) {
                current = direct;
                continue;
            }
        }

        std::filesystem::path match;
        int matches = 0;
        for (std::filesystem::directory_iterator it(current, ec), end;
             !ec && it != end; it.increment(ec)) {

            if (!ascii_case_equal(
                    it->path().filename().string(), components[i])) {
                continue;
            }
            if (it->is_symlink(ec)) {
                continue;
            }
            const bool right_type = last
                ? it->is_regular_file(ec) : it->is_directory(ec);
            if (ec) {
                break;
            }
            if (!right_type) {
                continue;
            }
            match = it->path();
            ++matches;
            if (matches > 1) {
                return {AssetResolutionStatus::AmbiguousCaseFold, {},
                    "multiple source entries differ only in case: " +
                    components[i]};
            }
        }
        if (ec) {
            // Absent roots are a normal missing-asset condition; inaccessible
            // existing directories are errors rather than guessed results.
            if (ec == std::errc::no_such_file_or_directory) {
                return {AssetResolutionStatus::Missing, {},
                        "source asset directory does not exist"};
            }
            return {AssetResolutionStatus::FileSystemError, {},
                    "cannot enumerate source asset directory: " +
                    ec.message()};
        }
        if (matches == 0) {
            return {AssetResolutionStatus::Missing, {},
                    "original source asset not found: " + components[i]};
        }
        current = std::move(match);
    }

    return {success, current, {}};
}

AssetResolution OriginalAssetResolver::resolve(
    std::string_view original_filename) const {

    const auto pieces = split_retail_asset_path(original_filename);
    if (!pieces) {
        return {AssetResolutionStatus::InvalidOriginalPath, {},
                "empty, absolute, drive-qualified or escaping source path"};
    }

    auto installed = resolve_below(
        installed_root_, *pieces, AssetResolutionStatus::FoundInstalled);
    if (installed.status != AssetResolutionStatus::Missing || !disc_root_) {
        return installed;
    }

    return resolve_below(
        *disc_root_, *pieces, AssetResolutionStatus::FoundOnDisc);
}

} // namespace btb::full_game
