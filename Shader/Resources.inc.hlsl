#ifndef __RESOUECES_HLSL__
#define __RESOUECES_HLSL__

Texture2D g_texDepthStencil : register(t0); //深度ステンシル
Texture2D g_texDepth : register(t1); //深度
Texture2D g_texColor : register(t2); //カラー
Texture2D g_texNormal : register(t3); //ノーマル
Texture2D g_texPosition : register(t4); //ワールド座標

SamplerState g_samLinear : register(s0);

cbuffer cGlobalInfo : register(b0)
{
    float4 g_vLightDir; //ディレクショナルライトの方向
    float4 g_vEye; //視線ベクトル
};

cbuffer cTransformInfo : register(b1)
{
    //row_major：DirectXとメモリレイアウトが異なるので、行優先で読むように指定
    row_major matrix g_mW; //ワールド行列
    row_major matrix g_mWVP; //ワールドから射影までの変換行列
};

cbuffer cMaterialInfo : register(b2)
{
    float4 g_Ambient = float4(0, 0, 0, 0); //アンビエント光
    float4 g_Diffuse = float4(1, 0, 0, 0); //拡散反射(色）
    float4 g_Specular = float4(1, 1, 1, 1); //鏡面反射
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
    float4 vDepth : SV_Target0;
    float4 vColor : SV_Target1;
    float4 vNormal : SV_Target2;
    float4 vPosition : SV_Target3;
};

#endif //__RESOUECES_HLSL__