#include "btb/bink32_audio_inverse.hpp"
#include "btb/bink32_audio_overlap.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <numbers>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do {if(!(e)){std::fprintf(stderr,"Bink1 inverse FFT test line %d: %s\n",__LINE__,#e);std::abort();}}while(0)
namespace {
[[nodiscard]] bool close(float a,double b,double tolerance=0.0002) {
    return std::abs(static_cast<double>(a)-b)<=tolerance;
}
}
int main() {
    const double pi=std::numbers::pi_v<double>;
    Bink1AudioLayout layout{};
    layout.frame_length=512;
    layout.overlap_length=32;
    layout.transform_channels=2;
    layout.transform=BinkAudioTransform::dct;
    Bink1AudioSpectrumBlock coefficients{};
    coefficients.transform_coefficients.assign(2,std::vector<float>(512,0));
    auto zero=inverse_bink1_audio_spectrum(coefficients,layout);
    CHECK(zero&&zero->size()==2&&(*zero)[0].size()==512);
    CHECK(std::all_of((*zero)[0].begin(),(*zero)[0].end(),
                      [](float x){return x==0.0f;}));
    // Source Bink DCT-III doubles DC, then scales the 512-point
    // reconstruction by 1/N. Its constant mode has no ringing.
    coefficients.transform_coefficients[0][0]=256;
    coefficients.transform_coefficients[1][0]=128;
    const auto dc=inverse_bink1_audio_spectrum(coefficients,layout);
    CHECK(dc);
    for(std::size_t i=0;i<512;++i) {
        CHECK(close((*dc)[0][i],1.0));
        CHECK(close((*dc)[1][i],0.5));
    }

    // Independent direct-cosine reference at sampled output positions.
    // Run the actual fast FFT for all 512 outputs and verify the values
    // against the O(N^2) mathematical DCT-III kernel (not itself FFT).
    std::fill(coefficients.transform_coefficients[0].begin(),
              coefficients.transform_coefficients[0].end(),0.0f);
    for(unsigned i=1;i<=19;++i)
        coefficients.transform_coefficients[0][i]=
            static_cast<float>(i%3==0?-2.5:3.25);
    const auto ac=inverse_bink1_audio_spectrum(coefficients,layout);
    CHECK(ac);
    for(unsigned n:{0U,1U,7U,63U,127U,255U,511U}) {
        double direct=0;
        for(unsigned k=1;k<=19;++k)
            direct+=(2.0/512.0)*
                coefficients.transform_coefficients[0][k]*
                std::cos(pi*static_cast<double>(k)*(n+0.5)/512.0);
        CHECK(close((*ac)[0][n],direct,0.00005));
    }

    layout.transform=BinkAudioTransform::rdft;
    layout.transform_channels=1;
    coefficients.transform_coefficients.resize(1);
    coefficients.transform_coefficients[0].assign(512,0);
    coefficients.transform_coefficients[0][0]=2.0f;
    const auto rdc=inverse_bink1_audio_spectrum(coefficients,layout);
    CHECK(rdc&&rdc->size()==1);
    for(float x:(*rdc)[0])CHECK(close(x,1.0));
    coefficients.transform_coefficients[0][0]=0;
    coefficients.transform_coefficients[0][1]=2.0f;
    const auto nyquist=inverse_bink1_audio_spectrum(coefficients,layout);
    CHECK(nyquist);
    for(unsigned i=0;i<512;++i)
        CHECK(close((*nyquist)[0][i],i%2?-1.0:1.0));
    coefficients.transform_coefficients[0][1]=0;
    coefficients.transform_coefficients[0][2]=1.0f;
    coefficients.transform_coefficients[0][3]=2.0f;
    const auto sinusoid=inverse_bink1_audio_spectrum(coefficients,layout);
    CHECK(sinusoid);
    for(unsigned n:{0U,1U,13U,64U,100U,257U,511U}) {
        const double angle=2.0*pi*n/512.0;
        // Stored odd-index imaginaries are sign-reversed by decoder.
        CHECK(close((*sinusoid)[0][n],
                    std::cos(angle)+2.0*std::sin(angle),0.00005));
    }
    // Source 1/16 overlap consumes inverse-time-domain samples; validate
    // block shape compatibility, not invented/certified PCM output.
    Bink1AudioOverlap overlap(layout);
    CHECK(overlap.process(*sinusoid));
    CHECK(overlap.has_previous_block());
    CHECK(overlap.process(*sinusoid));
    overlap.rewind();
    CHECK(!overlap.has_previous_block());

    auto corrupt=coefficients;
    corrupt.transform_coefficients[0][19]=
        std::numeric_limits<float>::quiet_NaN();
    CHECK(!inverse_bink1_audio_spectrum(corrupt,layout));
    corrupt=coefficients;
    corrupt.transform_coefficients[0].pop_back();
    CHECK(!inverse_bink1_audio_spectrum(corrupt,layout));
    auto wrong=layout;
    wrong.frame_length=513;
    CHECK(!inverse_bink1_audio_spectrum(coefficients,wrong));
    wrong=layout;
    wrong.transform_channels=3;
    CHECK(!inverse_bink1_audio_spectrum(coefficients,wrong));
    // A 4096 point stereo-RDFT case must remain bounded and avoid an
    // accidental O(N^2) inverse transform on each video frame.
    wrong=layout;wrong.frame_length=4096;wrong.overlap_length=256;
    corrupt.transform_coefficients.assign(1,std::vector<float>(4096,0));
    corrupt.transform_coefficients[0][0]=2;
    const auto large=inverse_bink1_audio_spectrum(corrupt,wrong);
    CHECK(large&&large->front().size()==4096);
    CHECK(close((*large)[0][3999],1.0));
    return 0;
}
