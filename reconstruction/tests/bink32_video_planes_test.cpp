#include "btb/bink32_video_planes.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink frame state line %d: %s\n",__LINE__,#x);std::abort();}}while(0)

static std::vector<BinkBlockCommand> make_fill(std::size_t count,
                                                std::uint8_t value) {
    std::vector<BinkBlockCommand> out(count);
    for(auto& cmd:out){cmd.mode=BinkBlockMode::Fill;cmd.fill=value;}
    return out;
}
int main() {
    const BinkYuvDimensions d{17,9,9,5};
    BinkYuvFrameHistory history(d);
    auto y=make_fill(6,70),u=make_fill(2,128),v=make_fill(2,130);
    const auto key=history.advance({y,u,v},true);
    CHECK(key && key->y.pixels.size()==17*9);
    CHECK(key->u.pixels.size()==9*5 && key->v.pixels.size()==9*5);
    CHECK(std::all_of(key->y.pixels.begin(),key->y.pixels.end(),
                      [](auto p){return p==70;}));
    CHECK(history.previous()!=nullptr);
    for(auto& c:y)c.mode=BinkBlockMode::Skip;
    for(auto& c:u)c.mode=BinkBlockMode::Skip;
    for(auto& c:v)c.mode=BinkBlockMode::Skip;
    y[0].mode=BinkBlockMode::Residue;
    y[0].residuals.fill(-80); // 70 - 80 wraps to 246
    const auto second=history.advance({y,u,v});
    CHECK(second);
    CHECK(second->y.pixels[0]==246 && second->y.pixels[7]==246);
    CHECK(second->y.pixels[8]==70 && second->y.pixels[8*17]==70);
    CHECK(second->u.pixels[0]==128 && second->v.pixels[0]==130);
    // Invalid V motion occurs after valid Y and U have already decoded.
    // The entire three-plane advance must remain transactional.
    v[0].mode=BinkBlockMode::Motion;
    v[0].motion_x=-200;
    const auto saved=history.previous()->y.pixels;
    CHECK(!history.advance({y,u,v}));
    CHECK(history.previous()->y.pixels==saved);
    v[0].mode=BinkBlockMode::Skip;
    for(auto& c:y)c.mode=BinkBlockMode::Skip;
    const auto third=history.advance({y,u,v});
    CHECK(third && third->y.pixels[0]==246);
    CHECK(third->y.pixels[8]==70);
    // A keyframe cannot silently reuse the preceding frame.
    CHECK(!history.advance({y,u,v},true));
    CHECK(history.previous()->y.pixels[0]==246);
    // An independent replacement keyframe resets the reference content.
    y=make_fill(6,111);u=make_fill(2,120);v=make_fill(2,140);
    const auto replacement=history.advance({y,u,v},true);
    CHECK(replacement && replacement->y.pixels[0]==111);
    history.reset();
    CHECK(!history.previous());
    for(auto& c:y)c.mode=BinkBlockMode::Skip;
    CHECK(!history.advance({y,u,v}));
    // Invalid dimensions, total-plane size cap and mismatched previous.
    const BinkYuvDimensions too_large{16384,16384,8192,8192};
    CHECK(!compose_bink_yuv_planes(too_large,{y,u,v}));
    CHECK(!compose_bink_yuv_planes({0,9,9,5},{y,u,v}));
    const BinkYuvDimensions wrong_dimensions{17,9,8,5};
    CHECK(!compose_bink_yuv_planes(wrong_dimensions,{y,u,v},&*replacement));
    std::puts("Bink three-plane frame history and rollback: PASS");
}
