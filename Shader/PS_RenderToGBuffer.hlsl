#include "Resources.inc.hlsl"

//テクスチャーにレンダー　ピクセルシェーダー
PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

	//カラーテクスチャーへ出力 
    Out.vColor = g_tex.Sample(g_samLinear, input.UV) + float4(0.01, 0.01, 0.01, 0);
	
	//ワールド法線テクスチャーへ出力
    float3 vNormal = input.WorldNormal;
    vNormal = vNormal * 0.5 + 0.5;
    Out.vNormal = normalize(float4(vNormal, 0));

	//ワールド座標テクスチャーへ出力
    Out.vPosition = input.WorldPos;

    return Out;
}
