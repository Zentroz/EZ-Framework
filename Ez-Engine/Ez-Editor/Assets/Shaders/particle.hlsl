#include"Common.hlsl"

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    float4 worldPosition = ConvertLocalToWorldPosition(input.position);
    output.position = ConvertWorldToClipPosition(worldPosition);
    output.normal = ConvertLocalNormalToWorldUniformScale(input.normal);
    output.uv = input.uv;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return baseColor;
}