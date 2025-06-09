#include "Resources.inc.hlsl"

//テクスチャーにレンダー　ピクセルシェーダー
PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

	//TODO:カラーテクスチャーへ出力（頂点カラーを使うようになったら考える）
    //Out.vColor = g_tex.Sample(g_samLinear, input.UV) + float4(0.01, 0.01, 0.01, 0);
	
    //深度情報
    float4 depth = g_texDepthStencil.Sample(g_samLinear, input.UV);
    Out.vDepth = float4(depth.r != 0, depth.g != 0, depth.b != 0, 1.0);
    //Out.vDepth = float4(1, 1, 1, 1);
    
	//ワールド法線テクスチャーへ出力
    float3 vNormal = input.WorldNormal;
    Out.vNormal = normalize(float4(vNormal, 0));

	//ワールド座標テクスチャーへ出力
    Out.vPosition = input.WorldPos;

    return Out;
}
