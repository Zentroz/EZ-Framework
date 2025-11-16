#include"Common.hlsl"

cbuffer MaterialConstantBuffer : register(b1)
{
    float test;
};

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
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
    float ambient = 0.2f;
    
    float3 color = float3(0.8f, 0.8f, 0.2f);
    
    float3 normal = normalize(input.normal);
    
    float lightIntensity = max(ambient, dot(normal, normalize(lightDirection.xyz)));
    
    float3 diffuse = color * lightIntensity;
    
    return float4(diffuse, 1.0f);
}