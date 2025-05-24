#ifndef __RESOUECES_HLSL__
#define __RESOUECES_HLSL__

Texture2D g_tex : register(t0);
Texture2D g_texColor : register(t1);
Texture2D g_texNormal : register(t2);
Texture2D g_texPosition : register(t3);

SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix g_mW; //���[���h�s��
    matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
    float4 g_vLightDir; //�f�B���N�V���i�����C�g�̕���
    float4 g_vEye; //�����x�N�g��
};

cbuffer global_1 : register(b1)
{
    float4 g_Ambient = float4(0, 0, 0, 0); //�A���r�G���g��
    float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 g_Specular = float4(1, 1, 1, 1); //���ʔ���
};

//�o�[�e�b�N�X�o�b�t�@�[�o��
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};

//�s�N�Z���V�F�[�_�[�o��
struct PS_OUTPUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
};

#endif //__RESOUECES_HLSL__