//頂点シェーダから出力するデータ
struct VSOutput {
	float4 Pos: SV_Position;
};

//========================================
// 頂点シェーダー
//========================================
VSOutput VS(float4 pos : POSITION)
{
    VSOutput Out;
    Out.Pos = pos;
    return Out;
}

//========================================
// ピクセルシェーダー
//========================================
float4 PS(VSOutput In) : SV_Target0
{
    return float4(0, 1, 0, 1);
}