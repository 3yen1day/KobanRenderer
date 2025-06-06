#include "Resources.inc.hlsl"

float4 main(VS_OUTPUT input) : SV_Target
{
	//法線を取得して表示
    float4 normal = g_texNormal.Sample(g_samLinear, input.UV);
    return float4(normal.r, normal.g, normal.b, 1);
}