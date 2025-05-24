#include "Resources.inc.hlsl"

//
//�e�N�X�`���[�Ƀ����_�[�@�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS_To_Tex( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.WorldNormal=mul(Norm, (float3x3)g_mW);
	output.Pos=mul(Pos,g_mWVP);
	output.WorldPos = mul(Pos,g_mW);
	output.UV=UV;

	return output;
}

//
//�e�N�X�`���[�Ƀ����_�[�@�s�N�Z���V�F�[�_�[
//
PS_OUTPUT PS_To_Tex( VS_OUTPUT input )
{
	PS_OUTPUT Out=(PS_OUTPUT)0;

	//�J���[�e�N�X�`���[�֏o�� 
	Out.vColor=g_tex.Sample( g_samLinear, input.UV )+float4(0.01,0.01,0.01,0);
	
	//���[���h�@���e�N�X�`���[�֏o��
	float3 vNormal=input.WorldNormal;
	vNormal=vNormal*0.5+0.5;
	Out.vNormal=normalize(float4(vNormal,0));

	//���[���h���W�e�N�X�`���[�֏o��
	Out.vPosition=input.WorldPos;

	return Out;
}


//
//�e�N�X�`���[���Q�Ƃ��ă����_�[�@�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS_From_Tex( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Pos=Pos;
	output.UV=UV;

	return output;
}

//
//�e�N�X�`���[���Q�Ƃ��ă����_�[�@�s�N�Z���V�F�[�_�[
//
float4 PS_From_Tex( VS_OUTPUT input ) : SV_Target
{
	//�e�N�X�`���[����������o��
	float4 vTexColor=g_texColor.Sample( g_samLinear, input.UV );
	float3 vWorldNormal=g_texNormal.Sample( g_samLinear, input.UV ).xyz;
	float3 vWorldPos=g_texPosition.Sample( g_samLinear, input.UV ).xyz;
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