#include "Resources.inc.hlsl"

//Lambert
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

float4 phong(float3 N, float3 L, float3 V, float3 R)
{
     //�g�U���ˁiLambert�j
    float3 diffuse = lambert(N, L);
    
    //���ʔ���
    float spec = pow(saturate(dot(R, V)), g_Specular.r);
    float3 speculer = float3(1, 1, 1) * spec;
    
    //����
    float3 ambient = g_Ambient.rgb;
    
    return float4(ambient * diffuse + speculer, 1);

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
    
    return phong(N, L, V, R);
	
	////���o�����������ƂɌv�Z
	////�����@��
 //   float4 ambient = g_Ambient;
	////�g�U���ˌ��@��
 //   float NL = saturate(dot(N, L));
 //   float4 diffuse = (g_Diffuse / 2 + ambient / 2) * NL;
	////���ʔ��ˌ��@��
 //   float3 reflect = normalize(2 * NL * N - L);
 //   float4 specular = pow(saturate(dot(reflect, eyeDir)), 4) * g_Specular;
	
 //   float4 color = diffuse + specular;
 //   return color;
}