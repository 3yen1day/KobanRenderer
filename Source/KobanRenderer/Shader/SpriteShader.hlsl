//0番目のテクスチャスロットを使用
Texture2D mainTexture : register(t0);
//0番目のサンプラスロットを使用
SamplerState mainSampler : register(s0);

//頂点シェーダから出力するデータ
struct VSOutput {
	float4 Pos: SV_Position; //頂点の座標
    float2 UV : TEXCOORD0; //UV座標
};

//========================================
// 頂点シェーダー
//========================================
VSOutput VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput Out;
    Out.Pos = pos;
    Out.UV = uv;
    return Out;
}

//========================================
// ピクセルシェーダー
//========================================
float4 PS(VSOutput In) : SV_Target0
{
    float4 col = mainTexture.Sample(mainSampler, In.UV);
    return col;
}