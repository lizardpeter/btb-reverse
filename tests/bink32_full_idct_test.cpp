#include "btb/bink32_full_idct.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink general IDCT test at line %d: %s\n",__LINE__,#x);std::abort();}} while(0)
static std::uint64_t fnv64(const BinkFullIdctPixels& pixels) {
    std::uint64_t hash=14695981039346656037ULL;
    for (const auto byte:pixels) hash=(hash^byte)*1099511628211ULL;
    return hash;
}
int main() {
    std::array<std::int16_t,64> coeff{};
    std::array<std::int32_t,64> quant{};
    quant.fill(8192);
    const auto zero=bink_idct_16x16(coeff,quant);
    CHECK(zero);
    for(const auto v:*zero)CHECK(v==0);
    // Original DC-fast-path parity and the x86 2x output replication.
    coeff[0]=256;
    const auto dc=bink_idct_16x16(coeff,quant);
    CHECK(dc);
    for(const auto v:*dc)CHECK(v==4);
    coeff[0]=-256;
    const auto negative=bink_idct_16x16(coeff,quant);
    CHECK(negative);
    for(const auto v:*negative)CHECK(v==252);
    // Nonzero vertical AC: 8x8 coefficients yield distinct vertical
    // pixels, replicated in x and y as in 0x300116CD..300116F4.
    coeff.fill(0);coeff[8]=203;
    const auto vertical=bink_idct_16x16(coeff,quant);
    CHECK(vertical);
    bool differing_rows=false;
    for(int y=0;y<16;y+=2) {
        for(int x=0;x<16;x+=2) {
            CHECK((*vertical)[y*16+x]==(*vertical)[y*16+x+1]);
            CHECK((*vertical)[y*16+x]==(*vertical)[(y+1)*16+x]);
            CHECK((*vertical)[y*16+x]==(*vertical)[(y+1)*16+x+1]);
        }
        for(int x=1;x<16;++x)CHECK((*vertical)[y*16+x]==(*vertical)[y*16]);
        if(y>=2&&(*vertical)[y*16]!=(*vertical)[0])differing_rows=true;
    }
    CHECK(differing_rows);
    CHECK(fnv64(*vertical)==0x858fd7b6ec7474a5ULL);
    // Horizontal AC: the transpose has constant columns instead.
    coeff.fill(0);coeff[1]=203;
    const auto horizontal=bink_idct_16x16(coeff,quant);
    CHECK(horizontal);
    bool differing_columns=false;
    for(int y=1;y<16;++y)
        for(int x=0;x<16;++x)CHECK((*horizontal)[y*16+x]==(*horizontal)[x]);
    for(int x=2;x<16;x+=2)if((*horizontal)[x]!=(*horizontal)[0])differing_columns=true;
    CHECK(differing_columns);
    CHECK(fnv64(*horizontal)==0xf40e309cc81233a5ULL);
    // Exercise arbitrary AC coefficients, negative and quantized values.
    for(std::size_t i=0;i<64;++i){
        coeff[i]=static_cast<std::int16_t>(int(i*79%511)-255);
        quant[i]=int((i*127)%65536)-16000;
    }
    const auto general=bink_idct_16x16(coeff,quant);
    CHECK(general);
    CHECK(fnv64(*general)==0xbb01e338c2d66441ULL);
    for(int y=0;y<16;y+=2)for(int x=0;x<16;x+=2){
        CHECK((*general)[y*16+x]==(*general)[y*16+x+1]);
        CHECK((*general)[y*16+x]==(*general)[(y+1)*16+x]);
        CHECK((*general)[y*16+x]==(*general)[(y+1)*16+x+1]);
    }
    CHECK(!bink_idct_16x16(std::span<const std::int16_t>(coeff).first(63),quant));
    CHECK(!bink_idct_16x16(coeff,std::span<const std::int32_t>(quant).first(63)));
    std::puts("Bink original general iDCT 8x8 -> doubled 16x16 block: PASS");
}
