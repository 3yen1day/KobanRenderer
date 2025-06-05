#include "Resources.inc.hlsl"

//�e�N�X�`���[���Q�Ƃ��ă����_�[�@�s�N�Z���V�F�[�_�[
float4 main(VS_OUTPUT input) : SV_Target
{
	//depth���擾���ĕ\��
    float4 depth = g_texDepth.Sample(g_samLinear, input.UV);
    if (depth.r != 0)
        return float4(1, 0, 0, 1);
    return float4(0, 0, 0, 1);
    // return float4(depth.r, depth.g, depth.b, 1);
}