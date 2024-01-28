#define MAX_LIGHT 100
#define ATTENU 2

Texture2D g_tex: register(t0);
Texture2D g_texColor: register(t1);
Texture2D g_texNormal: register(t2);
Texture2D g_texPosition: register(t3);

SamplerState g_samLinear : register(s0);

cbuffer global_0:register(b0)
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_vLightPos[MAX_LIGHT];//ポイントライト情報（ライトの位置）
	float4 g_vEye;//カメラ位置
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient=float4(0,0,0,0);//アンビエント光
	float4 g_Diffuse=float4(1,0,0,0); //拡散反射(色）
	float4 g_Specular=float4(1,1,1,1);//鏡面反射
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
	float4 vColor : SV_Target0;
	float4 vNormal : SV_Target1;
	float4 vPosition : SV_Target2;
};

//
//テクスチャーにレンダー　バーテックスシェーダー
//
VS_OUTPUT VS_To_Tex( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.WorldNormal=mul(Norm, (float3x3)g_mW);
	output.Pos=mul(Pos,g_mWVP);
	output.WorldPos = mul(Pos,g_mW);
	output.UV=UV;

	return output;
}

//
//テクスチャーにレンダー　ピクセルシェーダー
//
PS_OUTPUT PS_To_Tex( VS_OUTPUT input )
{
	PS_OUTPUT Out=(PS_OUTPUT)0;

	//カラーテクスチャーへ出力 
	Out.vColor=g_tex.Sample( g_samLinear, input.UV )+float4(0.01,0.01,0.01,0);
	
	//ワールド法線テクスチャーへ出力
	float3 vNormal=input.WorldNormal;
	vNormal=vNormal*0.5+0.5;
	Out.vNormal=normalize(float4(vNormal,0));

	//ワールド座標テクスチャーへ出力
	Out.vPosition=input.WorldPos;

	return Out;
}


//
//テクスチャーを参照してレンダー　バーテックスシェーダー
//
VS_OUTPUT VS_From_Tex( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Pos=Pos;
	output.UV=UV;

	return output;
}
//
//
//
float4 PLight(float3 vDiffuse,float3 vWorldPos,float3 vWorldNormal,float3 vLightPos,float3 vEyeVector,float3 LightColor)
{
	float3 vLightDir = vLightPos-vWorldPos;
	float Distance=length(vLightDir);
	vLightDir=normalize(vLightDir);

	float3 vDiffuseIntensity=saturate(dot(vLightDir,vWorldNormal));
	float3 vSpecularIntensity=pow(max(0, dot(vEyeVector, reflect(-vLightDir, vWorldNormal))), 2);

	float4 FinalColor;
	FinalColor.rgb=vDiffuseIntensity*(vDiffuse+LightColor) + vSpecularIntensity*g_Specular;
	FinalColor.a=1;
	//減衰	
	FinalColor*=pow(saturate(ATTENU / Distance),4);//減衰開始
	
	return FinalColor;
}
//
//テクスチャーを参照してレンダー　ピクセルシェーダー
//
float4 PS_From_Tex( VS_OUTPUT input ) : SV_Target
{
	//テクスチャーから情報を取り出す
	float4 vDiffuse=g_texColor.Sample( g_samLinear, input.UV );
	float3 vWorldNormal=g_texNormal.Sample( g_samLinear, input.UV ).xyz;
	float3 vWorldPos=g_texPosition.Sample( g_samLinear, input.UV ).xyz;
	//取り出した情報をもとにフォンシェーディングを計算
	float4 FinalColor=vDiffuse*g_Ambient;
	for(int i=0;i<MAX_LIGHT;i++)
	{
		//ここが重要！　この点を、このライトが照らしているかどうかチェック
		if(length(g_vLightPos[i]-vWorldPos)<ATTENU*2)
		{
			FinalColor+=PLight(vDiffuse,vWorldPos,vWorldNormal,g_vLightPos[i],normalize(g_vEye-vWorldPos),input.UV.yxy);
		}
	}
	return FinalColor;
}