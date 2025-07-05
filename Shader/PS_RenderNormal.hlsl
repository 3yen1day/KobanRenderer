#include "Resources.inc.hlsl"

float4 main(VS_OUTPUT input) : SV_Target
{
	//法線を取得して表示
    float3 normal = g_GBufNormal.Sample(g_samLinear, input.UV).xyz;
    return float4(normal.x, normal.y, normal.z, 1.0f);
    
    //視線ベクトルとの内積を計算して表示
    //float3 normal = g_GBufNormal.Sample(g_samLinear, input.UV).xyz;
    //float3 lightDir = normalize(g_vLightDir.xyz);
    //float NdotV = saturate(dot(normal, lightDir));
    //return float4(NdotV, NdotV, NdotV, 1);
}