#pragma once

#include <cstdint>
#include <optional>

namespace btb::retail {

// Exact MSVC 2002 CRT srand/rand implementation in the shipped PE32.
//
// srand: 0x0042FFBA  mov eax,[esp+4]; mov [0x00447558],eax
// rand:  0x0042FFC4  state = state*0x343FD + 0x269EC3 (wrap uint32),
//                   return (state >> 16) & 0x7FFF.
//
// The actual initial .data word at 0x00447558 is 1. A call to
// original_srand() overrides it, and the original game may seed at
// startup. Never substitute std::rand(), mt19937, or a per-activity
// generator: this is a single process-global sequence shared by all
// original game subsystems. The host owns seed and serialized call order.
inline constexpr std::uint32_t kRetailInitialRandSeed=1u;
inline constexpr std::uint32_t kRetailRandMultiplier=0x343fdu;
inline constexpr std::uint32_t kRetailRandIncrement=0x269ec3u;

class OriginalRetailRandom {
public:
    constexpr OriginalRetailRandom() = default;
    explicit constexpr OriginalRetailRandom(std::uint32_t seed)
        : state_(seed) {}

    constexpr void seed(std::uint32_t new_seed) noexcept {
        state_=new_seed;
    }

    // The native Win32 startup adapter passes GetTickCount's uint32
    // return value here, preserving the exact final 0x4027FB seed.
    // This pure source model does not read a different host clock.
    constexpr void apply_original_startup_tick_count(
        std::uint32_t original_get_tick_count) noexcept {
        seed(original_get_tick_count);
    }

    [[nodiscard]] constexpr std::uint32_t state() const noexcept {
        return state_;
    }

    [[nodiscard]] constexpr std::int32_t next_rand() noexcept {
        // Unsigned arithmetic reproduces the retail IMUL/ADD 32-bit
        // wrap without undefined behavior. SAR + AND 0x7FFF is
        // equivalent to this unsigned shift for the low fifteen bits.
        state_=state_*kRetailRandMultiplier+kRetailRandIncrement;
        return static_cast<std::int32_t>((state_>>16)&0x7fffu);
    }

    [[nodiscard]] constexpr std::optional<std::int32_t> next_mod(
        std::int32_t modulus) noexcept {
        if (modulus<=0) return std::nullopt;
        return next_rand()%modulus;
    }

private:
    std::uint32_t state_{kRetailInitialRandSeed};
};

} // namespace btb::retail
