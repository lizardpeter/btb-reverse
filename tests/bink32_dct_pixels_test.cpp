#include "btb/bink32_dct_pixels.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink DCT pixels test %d: %s\n",__LINE__,#x);std::abort();}}while(0)

static std::uint64_t fingerprint(
    const std::array<std::uint8_t,64>& pixels) {
    std::uint64_t hash=0xcbf29ce484222325ULL;
    for(const auto value:pixels)
        hash=(hash^value)*0x100000001b3ULL;
    return hash;
}
int main() {
    // Functional 16x64 intra/inter tables differ for AC
    // coefficients but share exactly the 16 DC multipliers.
    for(unsigned q=0;q<16;++q) {
        const auto a=bink_dct_quant_matrix(
            BinkDctQuantFamily::Intra,static_cast<std::uint8_t>(q));
        const auto b=bink_dct_quant_matrix(
            BinkDctQuantFamily::Inter,static_cast<std::uint8_t>(q));
        CHECK(a&&b&&(*a)[0]==(*b)[0]);
    }
    CHECK(!bink_dct_quant_matrix(BinkDctQuantFamily::Intra,16));
    CHECK(!bink_dct_quant_matrix(BinkDctQuantFamily::Inter,16));
    BinkDctCoefficients block{};
    block.values[0]=8;
    block.quant_selector=0;
    const auto intra_dc=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Intra);
    const auto inter_dc=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Inter);
    CHECK(intra_dc&&inter_dc);
    CHECK(std::all_of(intra_dc->begin(),intra_dc->end(),
                      [](auto v){return v==1;}));
    CHECK(*intra_dc==*inter_dc);
    block.quant_selector=15;
    const auto quant15=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Intra);
    CHECK(quant15);
    CHECK(std::all_of(quant15->begin(),quant15->end(),
                      [](auto v){return v==44;}));

    // Independent integer Bink1 butterfly and original numeric
    // quant-table reference produces these complete 8x8 pixel
    // fingerprints. Distinct quantizers and intra/inter families
    // must not be mixed up by the raster/scan permutation adapter.
    block.values.fill(0);
    block.values[1]=16;
    block.quant_selector=0;
    const auto a=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Intra);
    CHECK(a && fingerprint(*a)==0x49c22854c7dd8425ULL);
    CHECK((*a)[0]==3 && (*a)[7]==253);
    block.quant_selector=15;
    const auto b=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Intra);
    const auto c=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Inter);
    CHECK(b && c);
    CHECK(fingerprint(*b)==0xa6b7e551a01ae425ULL);
    CHECK(fingerprint(*c)==0xfb128ac1b3a3e185ULL);
    CHECK((*b)[0]==122 && (*c)[0]==130);
    CHECK(*b!=*c);

    block.values.fill(0);
    block.values[0]=8;
    block.values[1]=-16;
    block.quant_selector=0;
    const auto positive_dc_negative_ac=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Intra);
    CHECK(positive_dc_negative_ac);
    CHECK(fingerprint(*positive_dc_negative_ac)==0x932eba0746b5ef25ULL);
    const auto residual=reconstruct_bink_dct_pixels(
        block,BinkDctQuantFamily::Inter);
    CHECK(residual && fingerprint(*residual)==0x481998e34790f345ULL);

    block.quant_selector=16;
    CHECK(!reconstruct_bink_dct_pixels(block,BinkDctQuantFamily::Intra));
    block.quant_selector=0;
    block.values[1]=65536;
    CHECK(!reconstruct_bink_dct_pixels(block,BinkDctQuantFamily::Intra));
    std::puts("Original Bink1 intra/inter AC quantization and 8x8 IDCT: PASS");
}
