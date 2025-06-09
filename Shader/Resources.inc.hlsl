#ifndef __RESOUECES_HLSL__
#define __RESOUECES_HLSL__

Texture2D g_texDepthStencil : register(t0); //�[�x�X�e���V��
Texture2D g_texDepth : register(t1); //�[�x
Texture2D g_texColor : register(t2); //�J���[
Texture2D g_texNormal : register(t3); //�m�[�}��
Texture2D g_texPosition : register(t4); //���[���h���W

SamplerState g_samLinear : register(s0);

cbuffer cGlobalInfo : register(b0)
{
    float4 g_vLightDir; //�f�B���N�V���i�����C�g�̕���
    float4 g_vEye; //�����x�N�g��
};

cbuffer cTransformInfo : register(b1)
{
    //row_major�FDirectX�ƃ��������C�A�E�g���قȂ�̂ŁA�s�D��œǂނ悤�Ɏw��
    row_major matrix g_mW; //���[���h�s��
    row_major matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

cbuffer cMaterialInfo : register(b2)
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
    float4 vDepth : SV_Target0;
    float4 vColor : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vPosition : SV_Target3;
};

#endif //__RESOUECES_HLSL__