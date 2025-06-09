#include "Resources.inc.hlsl"

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float3 pos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    return float4(pos.x, pos.y, pos.z, 1.0f);
}