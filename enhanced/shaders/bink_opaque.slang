// Opaque, full-screen native Bink YUV420. Plane bytes are packed into
// 32-bit words by the host, and the output is integer packed RGBA/BGRA.
[[vk::binding(0,0)]] StructuredBuffer<uint> words;
[[vk::binding(1,0)]] RWStructuredBuffer<uint> pixels;
struct Params {uint bgra;};
[[vk::push_constant]] ConstantBuffer<Params> params;
uint byteAt(uint base,uint index){return (words[base+index/4]>>(8*(index%4)))&255;}
[shader("compute")]
[numthreads(8,8,1)]
void binkOpaque(uint3 tid:SV_DispatchThreadID){
    if(tid.x>=640||tid.y>=480)return;
    uint luma=tid.y*640+tid.x,chroma=(tid.y/2)*320+tid.x/2;
    int y=max(0,int(byteAt(words[0],luma))-16);
    int u=int(byteAt(words[1],chroma))-128;
    int v=int(byteAt(words[2],chroma))-128;
    int r=clamp((298*y+409*v+128)>>8,0,255);
    int g=clamp((298*y-100*u-208*v+128)>>8,0,255);
    int b=clamp((298*y+516*u+128)>>8,0,255);
    pixels[luma]=0xff000000|uint(g<<8)|(params.bgra!=0?uint(b|(r<<16)):uint(r|(b<<16)));
}
