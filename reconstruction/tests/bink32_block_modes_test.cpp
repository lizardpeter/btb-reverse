#include "btb/bink32_block_modes.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(cond) do { if(!(cond)) { std::fprintf(stderr,"mode test line %d: %s\n",__LINE__,#cond); std::abort(); } } while(false)

int main() {
    // 4x4 grid of 8x8 cells with two 16x16 scaled blocks.
    std::array<BinkBlockCommand,16> cmds{};
    for(auto& c:cmds){c.mode=BinkBlockMode::Fill;c.fill=10;}
    cmds[0].mode=BinkBlockMode::Scaled;
    cmds[0].scaled_submode=BinkBlockMode::Raw;
    cmds[1].mode=cmds[4].mode=cmds[5].mode=BinkBlockMode::Covered;
    for(std::size_t y=0;y<8;++y)
        for(std::size_t x=0;x<8;++x)
            cmds[0].samples[y*8+x]=static_cast<std::uint8_t>(x+y*8);
    cmds[2].mode=BinkBlockMode::Raw;
    cmds[2].samples.fill(31);
    cmds[3].mode=BinkBlockMode::Intra;
    cmds[3].samples.fill(99);
    cmds[6].mode=BinkBlockMode::Pattern;
    cmds[6].pattern_colors={4,200};
    cmds[6].pattern_rows.fill(0xaa);
    cmds[7].mode=BinkBlockMode::Run;
    cmds[7].samples.fill(55);
    cmds[8].mode=BinkBlockMode::Scaled;
    cmds[8].scaled_submode=BinkBlockMode::Fill;
    cmds[8].fill=222;
    cmds[9].mode=cmds[12].mode=cmds[13].mode=BinkBlockMode::Covered;
    cmds[10].mode=BinkBlockMode::Skip;
    cmds[11].mode=BinkBlockMode::Motion;
    cmds[11].motion_x=-8;
    cmds[14].mode=BinkBlockMode::Residue;
    cmds[14].residuals.fill(-15);
    cmds[15].mode=BinkBlockMode::Inter;
    cmds[15].residuals.fill(18);
    std::vector<std::uint8_t> previous(32*32,250);
    const auto p=compose_bink_mode_plane(32,32,cmds,previous);
    CHECK(p && p->pixels.size()==1024);
    CHECK(p->pixels[0]==0 && p->pixels[1]==0);
    CHECK(p->pixels[2]==1 && p->pixels[32]==0);
    CHECK(p->pixels[8*32+8]==36);
    CHECK(p->pixels[16]==31 && p->pixels[24]==99);
    CHECK(p->pixels[8*32+16]==4 && p->pixels[8*32+17]==200);
    CHECK(p->pixels[8*32+24]==55);
    CHECK(p->pixels[16*32]==222 && p->pixels[16*32+8]==222);
    CHECK(p->pixels[16*32+16]==250 && p->pixels[16*32+24]==250);
    CHECK(p->pixels[24*32+16]==235);
    CHECK(p->pixels[24*32+24]==12); // 250+18 wraps
    CHECK(p->pixels[24*32]==222 && p->pixels[31*32+15]==222);
    auto bad=cmds;
    bad[1].mode=BinkBlockMode::Fill;
    CHECK(!compose_bink_mode_plane(32,32,bad,previous));
    bad=cmds;
    bad[0].scaled_submode=BinkBlockMode::Skip;
    CHECK(!compose_bink_mode_plane(32,32,bad,previous));
    bad=cmds;
    bad[11].motion_x=-200;
    CHECK(!compose_bink_mode_plane(32,32,bad,previous));
    CHECK(!compose_bink_mode_plane(32,32,cmds));
    CHECK(!compose_bink_mode_plane(32,32,cmds,
          std::span<const std::uint8_t>(previous).first(10)));
    CHECK(!compose_bink_mode_plane(32,0,cmds,previous));
    CHECK(!compose_bink_mode_plane(16384,16384,cmds));
    CHECK(!compose_bink_mode_plane(32,32,
          std::span<const BinkBlockCommand>(cmds).first(15),previous));
    bad=cmds;
    bad[2].mode=static_cast<BinkBlockMode>(11);
    CHECK(!compose_bink_mode_plane(32,32,bad,previous));
    // Cropped 9x2 source surface checks clipped rightmost tile.
    std::array<BinkBlockCommand,2> clipped{};
    clipped[0].mode=BinkBlockMode::Fill;clipped[0].fill=1;
    clipped[1].mode=BinkBlockMode::Pattern;
    clipped[1].pattern_colors={30,70};
    clipped[1].pattern_rows.fill(1);
    const auto small=compose_bink_mode_plane(9,2,clipped);
    CHECK(small && small->pixels.size()==18);
    CHECK(small->pixels[7]==1 && small->pixels[8]==70);
    CHECK(small->pixels[17]==70);
    std::puts("Native Bink 8x8/16x16 block-mode compositor: PASS");
}
