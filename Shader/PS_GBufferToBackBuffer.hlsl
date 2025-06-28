#include "Resources.inc.hlsl"

//------------------------
// Lambert
//------------------------
float4 lambert(float3 N, float3 L)
{
    //Lambert反射
    //スペキュラ無し
    //どの方向にも均等に光が反射する
    //拡散反射光
    float NL = saturate(dot(N, L));
    //環境光
    float4 ambient = g_Ambient;
    return ambient * NL;
}

//------------------------
// Cook-Torrance BRDF
//------------------------
float chiGGX(float v)
{
    return v > 0 ? 1 : 0;
}

float4 GGX_Distribution()
{
}



//テクスチャーを参照してレンダー　ピクセルシェーダー
float4 main(VS_OUTPUT input) : SV_Target
{
    //テクスチャーから情報を取り出す
    float3 vWPos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    float3 N = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 L = normalize(-g_vLightDir.xyz);
    float3 V = normalize(vWPos - g_vEye.xyz);
    float3 R = reflect(-L, N);
    
    return lambert(N, L);
}