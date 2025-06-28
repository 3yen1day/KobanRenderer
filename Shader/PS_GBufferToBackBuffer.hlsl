#include "Resources.inc.hlsl"

//------------------------
// Lambert
//------------------------
float4 lambert(float3 N, float3 L)
{
    //Lambert����
    //�X�y�L��������
    //�ǂ̕����ɂ��ϓ��Ɍ������˂���
    //�g�U���ˌ�
    float NL = saturate(dot(N, L));
    //����
    float4 ambient = g_Ambient;
    return ambient * NL;
}

//------------------------
// Cook-Torrance BRDF
//------------------------
float chiGGX(float v)
{
    return v > 0 ? 1 : 0;
}

float4 GGX_Distribution()
{
}



//�e�N�X�`���[���Q�Ƃ��ă����_�[�@�s�N�Z���V�F�[�_�[
float4 main(VS_OUTPUT input) : SV_Target
{
    //�e�N�X�`���[����������o��
    float3 vWPos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    float3 N = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 L = normalize(-g_vLightDir.xyz);
    float3 V = normalize(vWPos - g_vEye.xyz);
    float3 R = reflect(-L, N);
    
    return lambert(N, L);
}