#include "Resources.inc.hlsl"

float4 main(VS_OUTPUT input) : SV_Target
{
	//�@�����擾���ĕ\��
    float3 normal = g_GBufNormal.Sample(g_samLinear, input.UV).xyz;
    return float4(normal.x, normal.y, normal.z, 1.0f);
    
    //�����x�N�g���Ƃ̓��ς��v�Z���ĕ\��
    //float3 normal = g_GBufNormal.Sample(g_samLinear, input.UV).xyz;
    //float3 lightDir = normalize(g_vLightDir.xyz);
    //float NdotV = saturate(dot(normal, lightDir));
    //return float4(NdotV, NdotV, NdotV, 1);
}