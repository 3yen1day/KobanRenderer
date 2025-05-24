#include "Resources.inc.hlsl"

//テクスチャーを参照してレンダー　ピクセルシェーダー
float4 main(VS_OUTPUT input) : SV_Target
{
	//テクスチャーから情報を取り出す
    float4 vTexColor = g_texColor.Sample(g_samLinear, input.UV);
    float3 vWorldNormal = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 vWorldPos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    float3 lightDir = normalize(g_vLightDir.xyz);
    float3 eyeDir = normalize(g_vEye.xyz);
	
	//取り出した情報をもとに計算
	//環境光　項
    float4 ambient = g_Ambient;
	//拡散反射光　項
    float NL = saturate(dot(vWorldNormal, lightDir));
    float4 diffuse = (g_Diffuse / 2 + vTexColor / 2) * NL;
	//鏡面反射光　項
    float3 reflect = normalize(2 * NL * vWorldNormal - lightDir);
    float4 specular = pow(saturate(dot(reflect, eyeDir)), 4) * g_Specular;
	
    float4 color = diffuse + specular;
    return color;
}