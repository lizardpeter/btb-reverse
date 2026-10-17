#include "btb/bink32_audio_overlap.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do{if(!(e)){std::fprintf(stderr,"Bink1 overlap test %d: %s\n",__LINE__,#e);std::abort();}}while(0)
int main(){
    Bink1AudioLayout layout{};
    layout.frame_length=512;
    layout.overlap_length=32;
    layout.transform_channels=2;
    Bink1AudioOverlap stereo(layout);
    CHECK(!stereo.has_previous_block());
    std::vector<std::vector<float>> block(2,std::vector<float>(512,0));
    for(std::size_t ch=0;ch<2;++ch) {
        std::fill(block[ch].begin()+480,block[ch].end(),
                  ch==0?100.0f:200.0f);
    }
    const auto first=stereo.process(block);
    CHECK(first&&first->size()==2&&(*first)[0].size()==480);
    CHECK((*first)[0][0]==0&&(*first)[1][0]==0);
    CHECK(stereo.has_previous_block());
    block[0].assign(512,1000.0f);
    block[1].assign(512,2000.0f);
    const auto second=stereo.process(block);
    CHECK(second&&(*second)[0][0]==100.0f);
    // Stereo Bink1 uses ch-offset sample weighting across 64 slots;
    // the right channel begins with one sixty-fourth new samples.
    CHECK((*second)[1][0]==(200.0f*63.0f+2000.0f)/64.0f);
    CHECK((*second)[0][1]==(100.0f*62.0f+1000.0f*2.0f)/64.0f);
    CHECK((*second)[0][32]==1000.0f);
    CHECK((*second)[1].size()==480);
    // Invalid input never consumes the cached predecessor overlap.
    auto malformed=block;
    malformed[0].pop_back();
    CHECK(!stereo.process(malformed));
    malformed=block;
    malformed[1][7]=std::numeric_limits<float>::quiet_NaN();
    CHECK(!stereo.process(malformed));
    const auto third=stereo.process(block);
    CHECK(third&&(*third)[0][0]==1000.0f&&
          (*third)[1][0]==2000.0f);
    stereo.rewind();
    CHECK(!stereo.has_previous_block());
    const auto restarted=stereo.process(block);
    CHECK(restarted&&(*restarted)[0][0]==1000.0f);
    // Interleaved RDFT stereo is ONE transform channel and therefore
    // must use mono-spectral overlap weighting, not stereo-DCT weighting.
    layout.transform_channels=1;
    Bink1AudioOverlap rdft(layout);
    std::vector<std::vector<float>> pair(1,std::vector<float>(512,0));
    std::fill(pair[0].begin()+480,pair[0].end(),64.0f);
    CHECK(rdft.process(pair));
    pair[0].assign(512,128.0f);
    const auto next=rdft.process(pair);
    CHECK(next&&(*next)[0][0]==64.0f);
    CHECK((*next)[0][1]==(64.0f*31+128.0f)/32.0f);
    layout.overlap_length=31;
    Bink1AudioOverlap invalid(layout);
    CHECK(!invalid.process(pair));
    return 0;
}
