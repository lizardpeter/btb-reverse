#include "btb/native_movie_pixels.hpp"
#include <array>
using namespace btb;
using namespace btb::full_game;
static bink32_recovered::BinkNativeYuVaFrame frame(unsigned w,unsigned h,uint8_t y,uint8_t u,uint8_t v,uint8_t a){
    bink32_recovered::BinkNativeYuVaFrame f;f.width=w;f.height=h;
    f.planes[0].assign(w*h,y);f.planes[1].assign(((w+1)/2)*((h+1)/2),u);
    f.planes[2].assign(f.planes[1].size(),v);f.planes[3].assign(w*h,a);return f;
}
int main(){
    NativeMoviePixels stream;
    if(stream.valid()||stream.allocate(0,4)||stream.allocate(641,480)||!stream.allocate(5,3))return 1;
    const auto white=frame(5,3,235,128,128,255);
    if(!stream.write(white))return 2;
    const auto& bytes=stream.snapshot().pixels;
    for(std::size_t i=0;i<bytes.size();i+=4)
        if(bytes[i]!=255||bytes[i+1]!=255||bytes[i+2]!=255||bytes[i+3]!=0)return 3;
    const auto frozen=stream;
    if(!stream.write(frame(3,2,16,128,128,128)))return 4;
    for(unsigned y=0;y<3;++y)for(unsigned x=0;x<5;++x){
        const auto i=(y*5+x)*4;const auto expected=x<3&&y<2?127:255;
        if(bytes[i]!=expected||bytes[i+1]!=expected||bytes[i+2]!=expected)return 5;
        if(frozen.snapshot().pixels[i]!=255)return 6;
    }
    const auto before=stream.snapshot().pixels;
    if(!stream.write(frame(5,3,0,0,0,0))||before!=stream.snapshot().pixels)return 7;
    if(stream.write(frame(6,3,235,128,128,255))||before!=stream.snapshot().pixels)return 8;
    auto bad=white;bad.planes[1].clear();
    if(stream.write(bad)||before!=stream.snapshot().pixels)return 9;
    stream.reset();if(stream.valid()||!frozen.valid()||stream.write(white))return 10;
    return 0;
}
