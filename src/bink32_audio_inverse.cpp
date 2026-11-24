#include "btb/bink32_audio_inverse.hpp"
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstddef>
#include <numbers>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
namespace {
// Inverse (positive-exponent), UNnormalized Cooley-Tukey FFT.
// Complex<double> prevents large twiddle-roundoff from accumulating before
// the final single-precision sample conversion. No runtime FFT dependency.
void inverse_fft(std::vector<std::complex<double>>& a) {
    const std::size_t n=a.size();
    for(std::size_t i=1,j=0;i<n;++i) {
        std::size_t bit=n>>1;
        while(j&bit) {j^=bit;bit>>=1;}
        j^=bit;
        if(i<j)std::swap(a[i],a[j]);
    }
    for(std::size_t length=2;length<=n;length<<=1) {
        const double angle=2.0*std::numbers::pi_v<double>/
                           static_cast<double>(length);
        const std::complex<double> step{std::cos(angle),std::sin(angle)};
        for(std::size_t block=0;block<n;block+=length) {
            std::complex<double> twiddle{1.0,0.0};
            for(std::size_t j=0;j<length/2;++j) {
                const auto u=a[block+j];
                const auto v=a[block+j+length/2]*twiddle;
                a[block+j]=u+v;
                a[block+j+length/2]=u-v;
                twiddle*=step;
            }
        }
    }
}

[[nodiscard]] std::optional<std::vector<float>>
inverse_rdft(std::span<const float> encoded) {
    const std::size_t n=encoded.size();
    if(n<512||n>4096||(n&(n-1)))return std::nullopt;
    std::vector<std::complex<double>> bins(n);
    bins[0]={static_cast<double>(encoded[0]),0.0};
    bins[n/2]={static_cast<double>(encoded[1]),0.0};
    for(std::size_t k=1;k<n/2;++k) {
        // The Bink bitstream stores the opposite sign from the imaginary
        // component consumed by the inverse real DFT.
        const std::complex<double> value{
            static_cast<double>(encoded[2*k]),
            -static_cast<double>(encoded[2*k+1])};
        bins[k]=value;
        bins[n-k]=std::conj(value);
    }
    inverse_fft(bins);
    std::vector<float> time(n);
    for(std::size_t i=0;i<n;++i)
        time[i]=static_cast<float>(0.5*bins[i].real());
    return time;
}

[[nodiscard]] std::optional<std::vector<float>>
inverse_dct(std::span<const float> encoded) {
    const std::size_t n=encoded.size();
    if(n<512||n>4096||(n&(n-1)))return std::nullopt;
    // Standard 2N Hermitian FFT representation of the inverse DCT-III.
    // Output is [2*X0 + 2*sum(Xk cos(pi*k*(n+0.5)/N))]/N,
    // reflecting the source decoder's coeff[0] /= 0.5 before the
    // 1/N DCT-III transform scale. No O(N^2) per-frame cosine loop.
    std::vector<std::complex<double>> bins(2*n);
    bins[0]={2.0*encoded[0],0.0};
    for(std::size_t k=1;k<n;++k) {
        const double angle=std::numbers::pi_v<double>*
                           static_cast<double>(k)/(2.0*n);
        const double coeff=encoded[k];
        bins[k]={coeff*std::cos(angle),coeff*std::sin(angle)};
        bins[2*n-k]=std::conj(bins[k]);
    }
    inverse_fft(bins);
    std::vector<float> time(n);
    const double factor=1.0/static_cast<double>(n);
    for(std::size_t i=0;i<n;++i)
        time[i]=static_cast<float>(bins[i].real()*factor);
    return time;
}
}

std::optional<std::vector<std::vector<float>>>
inverse_bink1_audio_spectrum(
    const Bink1AudioSpectrumBlock& spectrum,
    const Bink1AudioLayout& layout) {
    const auto n=std::size_t(layout.frame_length);
    const auto channels=std::size_t(layout.transform_channels);
    if(n<512||n>4096||(n&(n-1))!=0 ||
       !channels||channels>2 ||
       spectrum.transform_coefficients.size()!=channels)
        return std::nullopt;

    std::vector<std::vector<float>> output;
    output.reserve(channels);
    for(const auto& coeff:spectrum.transform_coefficients) {
        if(coeff.size()!=n ||
           !std::all_of(coeff.begin(),coeff.end(),[](float sample){
               return std::isfinite(sample);
           }))
            return std::nullopt;
        auto decoded=layout.transform==BinkAudioTransform::dct
            ?inverse_dct(coeff):inverse_rdft(coeff);
        if(!decoded ||
           !std::all_of(decoded->begin(),decoded->end(),[](float sample){
               return std::isfinite(sample);
           }))
            return std::nullopt;
        output.push_back(std::move(*decoded));
    }
    return output;
}
} // namespace btb::bink32_recovered
