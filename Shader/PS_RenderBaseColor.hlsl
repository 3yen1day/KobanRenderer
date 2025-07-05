#include "Resources.inc.hlsl"

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float3 color = g_GBufColor.Sample(g_samLinear, input.UV).xyz;
    return float4(color.x, color.y, color.z, 1.0f);
}