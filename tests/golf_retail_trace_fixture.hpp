#pragma once

// Regression fixture for the complete original 00415080 shot lifecycle.
// Use the public Round input interface, not a parallel implementation.
#include "btb/golf_activity.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <stdexcept>

namespace btb::golf::trace {
inline constexpr std::array<Vec2i,3> centers{{{429,338},{359,179},{532,160}}};
inline constexpr std::uint64_t expected_frames=31252395;
inline constexpr std::uint64_t expected_shots=178178;
inline constexpr std::uint64_t expected_hash=0x5bf54240cf8eca71ull;
struct Shot {
    bool windowed;
    int aim;
    int power;
    [[nodiscard]] std::uint32_t seed() const noexcept {
        return 0x9e3779b9u*static_cast<std::uint32_t>(power)+aim+7;
    }
    [[nodiscard]] std::uint32_t counter() const noexcept {
        return static_cast<std::uint32_t>((power+aim)%10);
    }
    [[nodiscard]] Data data() const {
        Data result{};
        result.bob_position={187,330};
        for(std::size_t i=0;i<centers.size();++i)
            result.course_objects[i].position=centers[i];
        // Inject every possible accepted power through normal input.
        // Some values cannot be reached with the shipped meter speeds.
        result.power_bar_speed_by_difficulty[0]=power;
        return result;
    }
};
using Words=std::array<std::uint32_t,13>;
struct Result {
    std::uint64_t shots{};
    std::uint64_t frames{};
    std::uint64_t hash=14695981039346656037ull;
    void mix(std::uint32_t word) noexcept {
        for(int i=0;i<4;++i) {hash^=word&255u;hash*=1099511628211ull;word>>=8;}
    }
    [[nodiscard]] bool matches() const noexcept {
        return shots==expected_shots&&frames==expected_frames&&hash==expected_hash;
    }
};
template<class Begin,class Observe>
Result run(Begin begin,Observe observe) {
    Result result;
    for(int display=0;display<2;++display)
    for(int aim=0;aim<=88;++aim)
    for(int power=0;power<=1000;++power) {
        const Shot shot{display!=0,aim,power};
        Round round(shot.data(),0,shot.counter());
        retail::OriginalRetailRandom rng(shot.seed());
        (void)round.advance({.accept_aim=true,.source_mouse_aim=aim});
        (void)round.advance({.accept_power=true});
        if(round.state()!=RoundState::LaunchSetup||round.snapshot().power!=power)
            throw std::runtime_error("trace fixture could not initialize accepted shot");
        begin(shot);
        int swing_ticks=0,ticks=0;
        do {
            RoundInput input{};
            input.original_windowed_display=shot.windowed;
            input.random_stream=&rng;
            if(round.state()==RoundState::SwingAnimationDelay)
                input.swing_animation_finished=++swing_ticks==72;
            const auto f=round.advance(input);
            const Words words{
                static_cast<std::uint32_t>(f.state),
                std::bit_cast<std::uint32_t>(f.ball.position.x),
                std::bit_cast<std::uint32_t>(f.ball.position.y),
                static_cast<std::uint32_t>(f.ball.speed),
                static_cast<std::uint32_t>(f.ball.angle_degrees),
                static_cast<std::uint32_t>(f.bounce_height),
                round.flight_counter(),
                static_cast<std::uint32_t>(f.score),
                static_cast<std::uint32_t>(f.attempts_remaining),
                static_cast<std::uint32_t>(f.matched_target),
                static_cast<std::uint32_t>(f.power),
                rng.state(),
                static_cast<std::uint32_t>(f.managed_voice.value_or(-1))
            };
            observe(shot,ticks,words);
            for(auto word:words)result.mix(word);
            ++result.frames;
            if(++ticks>400)throw std::runtime_error("shot did not finish");
        }while(round.state()!=RoundState::Aim);
        ++result.shots;
    }
    return result;
}
} // namespace btb::golf::trace
