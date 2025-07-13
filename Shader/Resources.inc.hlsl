#ifndef __RESOUECES_HLSL__
#define __RESOUECES_HLSL__

#define PI 3.1416f
#define BACK_GROUND_COLOR float4(0.97, 0.71, 0, 1)

Texture2D g_texDepthStencil : register(t0); //�[�x�X�e���V��
Texture2D g_GBufDepthStencil : register(t1); //�[�x
Texture2D g_GBufColor : register(t2); //�J���[
Texture2D g_GBufNormal : register(t3); //�m�[�}��
Texture2D g_GBufPosition : register(t4); //���[���h���W
Texture2D g_GBufShadowMap : register(t5); //�V���h�E�}�b�v

SamplerState g_samLinear : register(s0);

cbuffer cGlobalInfo : register(b0)
{
    float4 g_vLightDir; //���C�g�̕���
    float4 g_vCameraPos; //�J�����̈ʒu
};

cbuffer cTransformInfo : register(b1)
{
    //row_major�FDirectX�ƃ��������C�A�E�g���قȂ�̂ŁA�s�D��œǂނ悤�Ɏw��
    row_major matrix g_mW; //���[���h�s��
    row_major matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

cbuffer cMaterialInfo : register(b2)
{
    float4 g_Albedo = float4(0, 0, 0, 0); //�A���x�h
    float4 g_Roughness = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 g_Metalic = float4(1, 1, 1, 1); //���ʔ���
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
    float4 DepthStencil : SV_Target0;
    float4 Color : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Position : SV_Target3;
};

#endif //__RESOUECES_HLSL__