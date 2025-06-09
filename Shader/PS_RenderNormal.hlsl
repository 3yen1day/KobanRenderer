#include "Resources.inc.hlsl"

float4 main(VS_OUTPUT input) : SV_Target
{
	//�@�����擾���ĕ\��
    float3 normal = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float NdotV = saturate(dot(normal, g_vEye.xyz));
    return float4(NdotV, NdotV, NdotV, 1);
}