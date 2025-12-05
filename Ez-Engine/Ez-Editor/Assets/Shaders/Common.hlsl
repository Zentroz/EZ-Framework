cbuffer GlobalBuffer : register(b0)
{
    matrix view;
    matrix projection;
    float4 lightDirection;
    float4 cameraPosition;
    float4 timeData;
};

cbuffer PerObjectBuffer : register(b1)
{
    matrix model;
    float4 baseColor;
};

float GetTotalTime()
{
    return timeData.x;
}

float GetDeltaTime()
{
    return timeData.y;
}

float4 ConvertLocalToWorldPosition(float3 position)
{
    return mul(float4(position, 1.0f), model);
}

float4 ConvertWorldToClipPosition(float4 worldPosition)
{
    float4 viewPos = mul(worldPosition, view);
    return mul(viewPos, projection);
}

float3 ConvertLocalNormalToWorldNonUniformScale(float3 normal)
{
    // TODO: To be implemented
    return float3(0, 0, 0);
}

float3 ConvertLocalNormalToWorldUniformScale(float3 normal)
{
    float3 worldNormal = mul(normal, (float3x3) model);
    worldNormal = normalize(worldNormal);

    return worldNormal;
}