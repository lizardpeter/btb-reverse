#pragma once
// Stateful Bink1 1/16-block overlap for already INVERSE-TRANSFORMED
// floating-point audio samples. Does not implement DCT/RDFT or PCM output;
// spectral coefficients must never be passed here as if time-domain audio.
#include "btb/bink32_audio_spectrum.hpp"
#include <cstddef>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
class Bink1AudioOverlap final {
public:
    explicit Bink1AudioOverlap(const Bink1AudioLayout& layout);
    // Returns ready-to-stage time-domain float samples, excluding the final
    // overlap_length samples retained for the following block. A separate
    // PCM converter must apply output format and bounds.
    // Rejects incorrect shape/nonfinite samples transactionally.
    [[nodiscard]] std::optional<std::vector<std::vector<float>>>
    process(std::span<const std::vector<float>> inverse_time_domain);
    void rewind() noexcept;
    [[nodiscard]] bool has_previous_block() const noexcept {
        return have_previous_;
    }
private:
    std::size_t length_{},overlap_{},channels_{};
    std::vector<std::vector<float>> previous_tail_{};
    bool have_previous_{};
};
} // namespace btb::bink32_recovered
