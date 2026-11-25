#pragma once
#include <cstdint>
#include <string_view>
#include <limits>
namespace btb::enhanced {
// 0 is uncapped; invalid input does not accidentally lock the game to 1 FPS.
constexpr unsigned parse_fps(std::wstring_view text) noexcept {
    if(text.empty()||text.size()>4)return 0;
    unsigned n=0;
    for(wchar_t c:text){if(c<L'0'||c>L'9')return 0;n=n*10+unsigned(c-L'0');}
    return n>=30&&n<=1000?n:0;
}
struct FramePacer {
    std::uint64_t next_ns{},period_ns{};
    constexpr explicit FramePacer(unsigned fps=0) noexcept
        :period_ns(fps?(1000000000ULL+fps-1)/fps:0){}
    constexpr bool due(std::uint64_t now) const noexcept {return !period_ns||now>=next_ns;}
    constexpr void submitted(std::uint64_t now) noexcept {
        if(!period_ns)return;
        const auto max=(std::numeric_limits<std::uint64_t>::max)();
        // No catch-up burst after resize, minimize or a slow submission.
        const auto base=(next_ns&&now>=next_ns&&now-next_ns<period_ns)?next_ns:now;
        next_ns=base>max-period_ns?max:base+period_ns;
    }
    constexpr unsigned wait_us(std::uint64_t now) const noexcept {
        if(due(now))return 250; // nonblocking acquire/fence retry
        const auto remaining=next_ns-now;
        return remaining>=2000000?2000:unsigned((remaining+999)/1000);
    }
};
}
