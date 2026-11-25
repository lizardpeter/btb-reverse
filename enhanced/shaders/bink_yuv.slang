// GPU Bink YUV420 conversion candidate. Integer BT.601 equations mirror
// the native CPU conversion, including alpha blend over existing pixels.
[[vk::binding(0,0)]] Texture2D<uint> yPlane;
[[vk::binding(1,0)]] Texture2D<uint> uPlane;
[[vk::binding(2,0)]] Texture2D<uint> vPlane;
[[vk::binding(3,0)]] Texture2D<uint> aPlane;
[[vk::binding(4,0)]] RWTexture2D<float4> outputImage;
struct Params {uint2 size;uint2 origin;};
[[vk::push_constant]] ConstantBuffer<Params> params;
[shader("compute")]
[numthreads(8,8,1)]
void binkConvert(uint3 tid:SV_DispatchThreadID){
    if(any(tid.xy>=params.size))return;
    int2 p=int2(tid.xy);
    int y=max(0,int(yPlane.Load(int3(p,0)))-16);
    int u=int(uPlane.Load(int3(p/2,0)))-128;
    int v=int(vPlane.Load(int3(p/2,0)))-128;
    uint a=aPlane.Load(int3(p,0));
    if(a==0)return;
    int3 rgb=clamp(int3((298*y+409*v+128)>>8,(298*y-100*u-208*v+128)>>8,(298*y+516*u+128)>>8),0,255);
    uint2 dest=tid.xy+params.origin;
    float4 old=outputImage[dest];
    int3 previous=int3(round(saturate(old.rgb)*255.0));
    int3 result=(int(a)*rgb+(255-int(a))*previous+127)/255;
    outputImage[dest]=float4(float3(result)/255.0,old.a);
}
