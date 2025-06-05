#include "Resources.inc.hlsl"

//テクスチャーを参照してレンダー　ピクセルシェーダー
float4 main(VS_OUTPUT input) : SV_Target
{
	//depthを取得して表示
    float4 depth = g_texDepth.Sample(g_samLinear, input.UV);
    if (depth.r != 0)
        return float4(1, 0, 0, 1);
    return float4(0, 0, 0, 1);
    // return float4(depth.r, depth.g, depth.b, 1);
}