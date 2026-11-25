// Tile-binned ordered compositor. Each invocation owns exactly one
// output pixel, so overlapping sprites require neither races nor atomics.
[[vk::binding(0,0)]] StructuredBuffer<uint> words;
[[vk::binding(1,0)]] RWStructuredBuffer<uint> pixels;
struct Params {uint bgra;};
[[vk::push_constant]] ConstantBuffer<Params> params;
[shader("compute")]
[numthreads(8,8,1)]
void spriteBatch(uint3 tid:SV_DispatchThreadID){
    if(tid.x>=640||tid.y>=480)return;
    uint tile=(tid.y/32)*20+tid.x/32;
    uint first=words[4+2*tile],count=words[5+2*tile];
    uint color=0xff000000;
    for(uint n=0;n<count;n++){
        uint p=words[0]+8*words[words[1]+first+n];
        uint dx=words[p+4],dy=words[p+5],h=words[p+7]&0x7fffffff;
        if(tid.x<dx||tid.y<dy||tid.x-dx>=words[p+6]||tid.y-dy>=h)continue;
        uint c=words[words[2]+words[p]+(words[p+3]+tid.y-dy)*words[p+1]+words[p+2]+tid.x-dx]&0xffffff;
        if((words[p+7]&0x80000000)!=0&&c==0xff00ff)continue;
        color=c|0xff000000;break;
    }
    if(params.bgra!=0)color=(color&0xff00ff00)|((color&255)<<16)|((color>>16)&255);
    pixels[tid.y*640+tid.x]=color;
}
