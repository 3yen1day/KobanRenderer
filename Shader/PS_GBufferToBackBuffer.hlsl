#include "Resources.inc.hlsl"

//�e�N�X�`���[���Q�Ƃ��ă����_�[�@�s�N�Z���V�F�[�_�[
float4 main(VS_OUTPUT input) : SV_Target
{
	//�e�N�X�`���[����������o��
    float4 vTexColor = g_texColor.Sample(g_samLinear, input.UV);
    float3 vWorldNormal = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 vWorldPos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    float3 lightDir = normalize(g_vLightDir.xyz);
    float3 eyeDir = normalize(g_vEye.xyz);
	
	//���o�����������ƂɌv�Z
	//�����@��
    float4 ambient = g_Ambient;
	//�g�U���ˌ��@��
    float NL = saturate(dot(vWorldNormal, lightDir));
    float4 diffuse = (g_Diffuse / 2 + vTexColor / 2) * NL;
	//���ʔ��ˌ��@��
    float3 reflect = normalize(2 * NL * vWorldNormal - lightDir);
    float4 specular = pow(saturate(dot(reflect, eyeDir)), 4) * g_Specular;
	
    float4 color = diffuse + specular;
    return color;
}