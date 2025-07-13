#ifndef __RESOUECES_HLSL__
#define __RESOUECES_HLSL__

#define PI 3.1416f
#define BACK_GROUND_COLOR float4(0.97, 0.71, 0, 1)

Texture2D g_texDepthStencil : register(t0); //深度ステンシル
Texture2D g_GBufDepthStencil : register(t1); //深度
Texture2D g_GBufColor : register(t2); //カラー
Texture2D g_GBufNormal : register(t3); //ノーマル
Texture2D g_GBufPosition : register(t4); //ワールド座標
Texture2D g_GBufShadowMap : register(t5); //シャドウマップ

SamplerState g_samLinear : register(s0);

cbuffer cGlobalInfo : register(b0)
{
    float4 g_vLightDir; //ライトの方向
    float4 g_vCameraPos; //カメラの位置
};

cbuffer cTransformInfo : register(b1)
{
    //row_major：DirectXとメモリレイアウトが異なるので、行優先で読むように指定
    row_major matrix g_mW; //ワールド行列
    row_major matrix g_mWVP; //ワールドから射影までの変換行列
};

cbuffer cMaterialInfo : register(b2)
{
    float4 g_Albedo = float4(0, 0, 0, 0); //アルベド
    float4 g_Roughness = float4(1, 0, 0, 0); //拡散反射(色）
    float4 g_Metalic = float4(1, 1, 1, 1); //鏡面反射
};

//バーテックスバッファー出力
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};

//ピクセルシェーダー出力
struct PS_OUTPUT
{
    float4 DepthStencil : SV_Target0;
    float4 Color : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Position : SV_Target3;
};

#endif //__RESOUECES_HLSL__