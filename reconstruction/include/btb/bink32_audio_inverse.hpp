#pragma once
// Native Bink1 audio spectral synthesis. Independent of FFTW, FFmpeg and
// any original RAD libraries. FFT-backed O(N log N) inverse DCT-III/RDFT.
//
// This is a candidate mathematical reconstruction using Bink1's inverse
// transform normalization and spectrum layout. Until actual BIK PCM
// differential tests pass, it is NOT admitted to the runtime audio path.
#include "btb/bink32_audio_spectrum.hpp"
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
// Bink DCT: inverse DCT-III of N real coefficient bins, double DC
// coefficient before applying the 1/N scale convention.
// Bink RDFT: packed C[0], C[N/2], then interleaved real/imaginary
// coeffs. The encoded odd-index imaginary signs are reversed and the
// unnormalized real inverse DFT is scaled by 0.5.
[[nodiscard]] std::optional<std::vector<std::vector<float>>>
inverse_bink1_audio_spectrum(
    const Bink1AudioSpectrumBlock& decoded,
    const Bink1AudioLayout& layout);

// A small, deterministic, portable complex IFFT implementation is
// private to this module, not an external runtime dependency.
} // namespace btb::bink32_recovered
