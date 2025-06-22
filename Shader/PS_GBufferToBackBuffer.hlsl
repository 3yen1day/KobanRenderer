#include "Resources.inc.hlsl"

//Lambert
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

float4 phong(float3 N, float3 L, float3 V, float3 R)
{
     //拡散反射（Lambert）
    float3 diffuse = lambert(N, L);
    
    //鏡面反射
    float spec = pow(saturate(dot(R, V)), g_Specular.r);
    float3 speculer = float3(1, 1, 1) * spec;
    
    //環境光
    float3 ambient = g_Ambient.rgb;
    
    return float4(ambient * diffuse + speculer, 1);

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
    
    return phong(N, L, V, R);
	
	////取り出した情報をもとに計算
	////環境光　項
 //   float4 ambient = g_Ambient;
	////拡散反射光　項
 //   float NL = saturate(dot(N, L));
 //   float4 diffuse = (g_Diffuse / 2 + ambient / 2) * NL;
	////鏡面反射光　項
 //   float3 reflect = normalize(2 * NL * N - L);
 //   float4 specular = pow(saturate(dot(reflect, eyeDir)), 4) * g_Specular;
	
 //   float4 color = diffuse + specular;
 //   return color;
}