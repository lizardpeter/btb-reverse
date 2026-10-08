#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace btb::full_game {

// Portable adapter for retail paths such as
// Data\\SubGameOpen\\BOBINSTAND.bmp, data\\sound\\AS_BOB_01.wav,
// loaddata\\uiHotAreaReplace.txt.
//
// Win32 filesystem lookup ignores ASCII case, but on Linux the original
// installed asset tree is mixed-case. Resolve individual original path
// components against the supplied real game/CD roots; never synthesize
// alternative content, rename the native assets or overwrite them.
enum class AssetResolutionStatus {
    FoundInstalled,
    FoundOnDisc,
    Missing,
    InvalidOriginalPath,
    AmbiguousCaseFold,
    FileSystemError,
};

struct AssetResolution {
    AssetResolutionStatus status{AssetResolutionStatus::Missing};
    std::filesystem::path absolute_path{};
    std::string error{};

    [[nodiscard]] bool found() const noexcept {
        return status == AssetResolutionStatus::FoundInstalled ||
               status == AssetResolutionStatus::FoundOnDisc;
    }
};

class OriginalAssetResolver {
public:
    explicit OriginalAssetResolver(
        std::filesystem::path installed_root,
        std::optional<std::filesystem::path> disc_root = std::nullopt)
        : installed_root_(std::move(installed_root)),
          disc_root_(std::move(disc_root)) {}

    [[nodiscard]] AssetResolution resolve(
        std::string_view original_filename) const;

    [[nodiscard]] const std::filesystem::path& installed_root() const noexcept {
        return installed_root_;
    }

private:
    [[nodiscard]] static AssetResolution resolve_below(
        const std::filesystem::path& root,
        const std::vector<std::string>& components,
        AssetResolutionStatus success);

    std::filesystem::path installed_root_{};
    std::optional<std::filesystem::path> disc_root_{};
};

// Path validation is intentionally separate from finding a file. A malicious
// or malformed asset name must not escape the original game/CD root.
[[nodiscard]] std::optional<std::vector<std::string>>
split_retail_asset_path(std::string_view input);

} // namespace btb::full_game
