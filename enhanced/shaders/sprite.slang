// Future GPU-native sprite path. Source colors use original magenta key;
// draw order must be preserved by dispatch/geometry submission.
struct Sprite {
    float4 rect;
    float4 uvRect;
    uint keyed;
};
[[vk::binding(0,0)]] StructuredBuffer<Sprite> sprites;
[[vk::binding(1,0)]] Texture2D<float4> artwork;
[[vk::binding(2,0)]] SamplerState pointSampler;
struct Varying {
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
    nointerpolation uint keyed : TEXCOORD1;
};
[shader("vertex")]
Varying spriteVS(uint vertex:SV_VertexID,uint instance:SV_InstanceID){
    float2 corners[6]={float2(0,0),float2(1,0),float2(0,1),float2(0,1),float2(1,0),float2(1,1)};
    Sprite s=sprites[instance];float2 c=corners[vertex];
    float2 p=s.rect.xy+c*s.rect.zw;
    Varying v;v.position=float4(p.x/320.0-1.0,p.y/240.0-1.0,0,1);
    v.uv=s.uvRect.xy+c*s.uvRect.zw;v.keyed=s.keyed;return v;
}
[shader("fragment")]
float4 spritePS(Varying v):SV_Target{
    float4 c=artwork.Sample(pointSampler,v.uv);
    if(v.keyed!=0&&all(abs(c.rgb-float3(1,0,1))<0.001))discard;
    return float4(c.rgb,1);
}
