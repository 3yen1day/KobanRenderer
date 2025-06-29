#include "Resources.inc.hlsl"

// reference
// https://de45xmedrsdbp.cloudfront.net/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf

//------------------------
// D項（マイクロファセット法線分布関数）
//------------------------
float Distribution_GGX(float3 n, float3 h, float roughness)
{
    float alpha2 = pow(roughness, 4.0);
    float nh2 = pow(dot(n, h), 2.0);
    return alpha2 / (PI * pow(nh2 * (alpha2 - 1.0) + 1.0, 2.0));
}

//------------------------
// G項（幾何減衰項）
//------------------------
float GeometryTerm_GGX(float3 v, float3 n, float3 l, float roughness)
{
    float k = pow(roughness + 1.0, 2.0) / 8.0;
    float ln = dot(l, n);
    float vn = dot(v, n);
    return (ln / (ln * (1. - k) + k)) * (vn / (vn * (1. - k) + k));
}

//------------------------
// F項（フレネル項）
//------------------------
float Fresnel_Schlick(float3 v, float3 h, float refractive)
{
    float f0 = refractive;
    float vh = dot(v, h);
    return f0 + pow(1.0 - vh, 5.0) * (1.0 - f0);
}

//------------------------
// Lambartの余弦則
//------------------------
float lambert(float3 N, float3 L)
{
    return saturate(dot(N, L));
}

float3 directionalLight(float3 baseColor, float3 n, float3 l, float3 v, float roughness, float metalness)
{
    //TODO:ライト分加算する必要がある
    float3 lightColor = float3(1, 1, 1);
    float3 h = normalize(l + v); //ハーフベクトル
    
    //Specular BRDFの計算
    float D = Distribution_GGX(n, h, roughness);
    float G = GeometryTerm_GGX(v, n, l, roughness);
    float F = Fresnel_Schlick(v, h, metalness);
    float3 specularBRDF = D * G * F / (4 * dot(l, n) * dot(v, n)); //cookTorranceBRDF
    
    //Diffuse BRDFの計算
    float3 diffuseColor = baseColor * (1.0 - metalness);
    float3 diffuseBRDF = diffuseColor / PI; //拡散反射BRDF
    
    //BRDFの合計
    float kd = 1-F;
    float3 brdf = kd * diffuseBRDF + specularBRDF; //拡散反射と鏡面反射の合計
    
    //ライトの色*Lambartの余弦則*BRDF
    float Li = lambert(n, l);
    return lightColor * Li * brdf;
}

//テクスチャーを参照してレンダー　ピクセルシェーダー
float4 main(VS_OUTPUT input) : SV_Target
{
    //テクスチャーから情報を取り出す
    float3 baseCol = g_Ambient.xyz;
    float3 N = g_texNormal.Sample(g_samLinear, input.UV).xyz;
    float3 L = normalize(g_vLightDir.xyz);
    float3 pos = g_texPosition.Sample(g_samLinear, input.UV).xyz;
    float3 V = normalize(pos - g_vEye.xyz);
    
    return float4(directionalLight(baseCol, N, L, V, 0.5, 0.8), 1);
}