#include "btb/bink32_pixel_reconstruction.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vector>

using namespace btb::bink32_recovered;
#define CHECK(x) do { if(!(x)) {std::fprintf(stderr,"Bink pixels test %d: %s\n",__LINE__,#x);std::abort();} }while(0)

int main() {
    constexpr std::size_t pitch=13;
    std::array<std::uint8_t,pitch*8> reference{},output{};
    std::array<std::int8_t,64> residual{};
    reference.fill(75);output.fill(99);
    for(std::size_t y=0;y<8;++y)for(std::size_t x=0;x<8;++x) {
        reference[y*pitch+x]=static_cast<std::uint8_t>((y*19+x*31)&255);
        residual[y*8+x]=static_cast<std::int8_t>(int((y*8+x)%31)-15);
    }
    CHECK(compose_bink_interframe_block8(reference,pitch,residual,output,pitch));
    for(std::size_t y=0;y<8;++y)for(std::size_t x=0;x<pitch;++x) {
        if(x<8)CHECK(output[y*pitch+x]==
            static_cast<std::uint8_t>(int(reference[y*pitch+x])+int(residual[y*8+x])));
        else CHECK(output[y*pitch+x]==99); // padding untouched
    }
    // Original r8 addition wraps rather than saturates.
    reference.fill(255);residual.fill(1);
    CHECK(compose_bink_interframe_block8(reference,pitch,residual,output,pitch));
    for(std::size_t y=0;y<8;++y)for(std::size_t x=0;x<8;++x)
        CHECK(output[y*pitch+x]==0);
    reference.fill(0);residual.fill(-1);
    CHECK(compose_bink_interframe_block8(reference,pitch,residual,output,pitch));
    for(std::size_t y=0;y<8;++y)for(std::size_t x=0;x<8;++x)
        CHECK(output[y*pitch+x]==255);

    // One in-place update: reference pixels and output are the same span.
    std::array<std::uint8_t,64> in_place{};
    std::array<std::int8_t,64> plus{};
    in_place.fill(252);plus.fill(7);
    CHECK(compose_bink_interframe_block8(in_place,8,plus,in_place,8));
    for(auto byte:in_place)CHECK(byte==3);
    const auto unchanged=in_place;
    CHECK(!compose_bink_interframe_block8(in_place,7,plus,in_place,8));
    CHECK(in_place==unchanged);
    CHECK(!compose_bink_interframe_block8(in_place,8,
        std::span<const std::int8_t>(plus).first(63),in_place,8));
    CHECK(in_place==unchanged);
    CHECK(!compose_bink_interframe_block8(in_place,
        std::numeric_limits<std::size_t>::max(),plus,in_place,8));
    CHECK(in_place==unchanged);
    CHECK(!compose_bink_interframe_block8(in_place,8,plus,in_place,9));
    CHECK(in_place==unchanged);

    std::array<std::uint8_t,64> intra{};
    for(std::size_t i=0;i<64;++i)intra[i]=static_cast<std::uint8_t>(i*3);
    output.fill(88);
    CHECK(store_bink_intra_block8(intra,output,pitch));
    for(std::size_t y=0;y<8;++y)
        for(std::size_t x=0;x<pitch;++x)
            CHECK(output[y*pitch+x] == (x<8?intra[y*8+x]:88));
    CHECK(!store_bink_intra_block8(intra,std::span<std::uint8_t>(output).first(8),pitch));
    CHECK(!store_bink_intra_block8(
        std::span<const std::uint8_t>(intra).first(63),output,pitch));

    std::puts("Bink original 0x30012940 pixel residual composition: PASS");
}
