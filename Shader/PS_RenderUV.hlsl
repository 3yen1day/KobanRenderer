#include "Resources.inc.hlsl"

//テクスチャーを参照してレンダー　ピクセルシェーダー
float4 main(VS_OUTPUT input) : SV_Target
{
	//uvを取得して表示
    return float4(input.UV.r, input.UV.g, 0, 1);
}