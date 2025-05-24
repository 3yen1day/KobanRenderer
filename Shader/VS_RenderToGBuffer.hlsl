#include "Resources.inc.hlsl"

//�e�N�X�`���[�Ƀ����_�[�@���_�V�F�[�_�[
VS_OUTPUT main(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.WorldNormal = mul(Norm, (float3x3) g_mW);
    output.Pos = mul(Pos, g_mWVP);
    output.WorldPos = mul(Pos, g_mW);
    output.UV = UV;

    return output;
}
