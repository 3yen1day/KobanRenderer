#include "Resources.inc.hlsl"

//テクスチャーにレンダー　ピクセルシェーダー
PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

	//カラー
    Out.Color = g_Ambient;
	
    //深度情報
    float depth = g_texDepthStencil.Sample(g_samLinear, input.UV).r;
    Out.DepthStencil = float4(depth, 1, 0, 0);
    
	//ワールド法線テクスチャーへ出力
    float3 Normal = input.WorldNormal;
    Normal = Normal * 0.5 + 0.5;
    Out.Normal = normalize(float4(Normal, 0));

	//ワールド座標テクスチャーへ出力
    Out.Position = input.WorldPos;

    return Out;
}
