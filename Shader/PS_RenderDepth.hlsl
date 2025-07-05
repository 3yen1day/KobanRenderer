#include "Resources.inc.hlsl"

//テクスチャーを参照してレンダー　ピクセルシェーダー
float4 main(VS_OUTPUT input) : SV_Target
{
	//depthを取得して表示
    float depth = g_GBufDepthStencil.Sample(g_samLinear, input.UV).r;
    return float4(depth, depth, depth, 1);
}