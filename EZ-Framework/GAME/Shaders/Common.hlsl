// cbuffer GlobalDataBuffer : register(b0){
//     float4x4 ViewProjectionMatrix;
//     float4x4 InverseViewProjectionMatrix;
//     float4   CameraPosition; // w component can be used for padding
//     float4   LightDirection; // w component can be used for padding
//     float4   AmbientColor;   // w component can be used for padding
//     float4   TimeData;       // x: time, y: deltaTime, z: totalTime, w: padding
// };

cbuffer GlobalBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    float4 lightDirection;
    float4 cameraPosition;
    float time;
    float deltaTime;
};

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