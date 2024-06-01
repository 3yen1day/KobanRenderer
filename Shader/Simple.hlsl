
//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールド、ビュー、射影の合成変換行列
};

//構造体
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float4 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Normal : NORMAL,float2 UV : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos,g_mWVP);
	return output;
}
//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color=float4(0,0.5,0,0);
	return color;
}