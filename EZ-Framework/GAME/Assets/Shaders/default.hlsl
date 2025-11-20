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
    float4 currentPosition : CRT_POSITION;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.currentPosition = ConvertLocalToWorldPosition(input.position);
    output.position = ConvertWorldToClipPosition(output.currentPosition);
    output.normal = ConvertLocalNormalToWorldUniformScale(input.normal);
    output.uv = input.uv;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float ambient = 0.2f;
    
    float3 color = float3(0.2f, 0.4f, 0.8f);
    
    // Light intensity
    float3 normal = normalize(input.normal);
    float lightIntensity = max(ambient, dot(normal, normalize(lightDirection.xyz)));
    
    // Specular lighting
    float specularLight = 0.5f;
    float3 viewDirection = normalize((cameraPosition - input.currentPosition).xyz);
    float3 reflectDir = reflect(-lightDirection.xyz, normal);
    float specularAmount = pow(max(dot(viewDirection, reflectDir), 0), 8);
    float specular = specularLight * specularAmount;

    float3 diffuse = color * (lightIntensity + specular);
    
    return float4(diffuse, 1.0f);
}