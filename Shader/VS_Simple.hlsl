#include "Resources.inc.hlsl"

VS_OUTPUT main(float4 pos : POSITION)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(float4(pos.xyz, 1), g_mWVP);
    return output;
}