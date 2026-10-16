#include "btb/bink32_inverse_transform.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink iDCT fast path line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
int main() {
    BinkDctColumn dc{};
    dc[0]=64;
    const auto positive=idct_dc_only_column8(dc,4096);
    CHECK(positive);
    for(auto sample:*positive)CHECK(sample==128);
    dc[0]=-64;
    const auto negative=idct_dc_only_column8(dc,4096);
    CHECK(negative);
    for(auto sample:*negative)CHECK(sample==-128);
    dc[0]=0;
    const auto zero=idct_dc_only_column8(dc,4096);
    CHECK(zero);
    for(auto sample:*zero)CHECK(sample==0);
    dc[0]=1;
    const auto trunc=idct_dc_only_column8(dc,2047);
    CHECK(trunc);
    for(auto sample:*trunc)CHECK(sample==0);
    dc[0]=-1;
    const auto negative_round=idct_dc_only_column8(dc,2047);
    CHECK(negative_round);
    for(auto sample:*negative_round)CHECK(sample==-1);
    dc[2]=1;
    CHECK(!idct_dc_only_column8(dc,4096));
    dc[2]=0;
    CHECK(!idct_dc_only_column8(std::span<const std::int16_t>(dc).first(7),4096));
    // Overflow semantics: original x86 IMUL truncates to low 32 bits.
    dc[0]=std::numeric_limits<std::int16_t>::max();
    const auto wrap=idct_dc_only_column8(dc,std::numeric_limits<std::int32_t>::max());
    CHECK(wrap);
    // ((32767 * 2147483647) mod 2^32) interpreted as signed, then sar 11.
    const std::int64_t product=std::int64_t(dc[0])*std::numeric_limits<std::int32_t>::max();
    const auto unsigned_low=static_cast<std::uint32_t>(product);
    const auto expected=static_cast<std::int32_t>(
        std::bit_cast<std::int32_t>(unsigned_low)>>11);
    for(auto sample:*wrap)CHECK(sample==expected);
    std::puts("Bink original 0x300113CE DC-only iDCT column: PASS");
}
