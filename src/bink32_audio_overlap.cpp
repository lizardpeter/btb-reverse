#include "btb/bink32_audio_overlap.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
Bink1AudioOverlap::Bink1AudioOverlap(const Bink1AudioLayout& layout)
    : length_(layout.frame_length),overlap_(layout.overlap_length),
      channels_(layout.transform_channels) {
    if(length_<512||length_>4096 || !overlap_ ||
       overlap_!=length_/16 || channels_<1||channels_>2) {
        length_=overlap_=channels_=0;
        return;
    }
    previous_tail_.resize(channels_,std::vector<float>(overlap_,0));
}

std::optional<std::vector<std::vector<float>>>
Bink1AudioOverlap::process(
    std::span<const std::vector<float>> inverse_time_domain) {
    if(!channels_ || inverse_time_domain.size()!=channels_)
        return std::nullopt;
    for(const auto& channel:inverse_time_domain) {
        if(channel.size()!=length_ ||
           !std::all_of(channel.begin(),channel.end(),[](float value){
               return std::isfinite(value);
           }))
            return std::nullopt;
    }
    std::vector<std::vector<float>> output{};
    output.reserve(channels_);
    std::vector<std::vector<float>> next_tail{};
    next_tail.reserve(channels_);
    const std::size_t ready=length_-overlap_;
    const float denominator=static_cast<float>(overlap_*channels_);
    for(std::size_t c=0;c<channels_;++c) {
        const auto& samples=inverse_time_domain[c];
        std::vector<float> channel(samples.begin(),samples.begin()+ready);
        if(have_previous_) {
            for(std::size_t i=0;i<overlap_;++i) {
                const float new_weight=static_cast<float>(
                    i*channels_+c);
                const float old_weight=denominator-new_weight;
                // Retain the 2002 Bink time-domain cross-fade convention
                // across mono, stereo DCT and interleaved stereo RDFT.
                channel[i]=(previous_tail_[c][i]*old_weight+
                            samples[i]*new_weight)/denominator;
            }
        }
        next_tail.emplace_back(samples.begin()+ready,samples.end());
        output.push_back(std::move(channel));
    }
    previous_tail_=std::move(next_tail);
    have_previous_=true;
    return output;
}

void Bink1AudioOverlap::rewind() noexcept {
    have_previous_=false;
    for(auto& channel:previous_tail_)
        std::fill(channel.begin(),channel.end(),0.0f);
}
} // namespace btb::bink32_recovered
