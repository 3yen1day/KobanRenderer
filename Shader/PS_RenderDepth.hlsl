#include "Resources.inc.hlsl"

//�e�N�X�`���[���Q�Ƃ��ă����_�[�@�s�N�Z���V�F�[�_�[
float4 main(VS_OUTPUT input) : SV_Target
{
	//depth���擾���ĕ\��
    float depth = g_GBufDepthStencil.Sample(g_samLinear, input.UV).r;
    return float4(depth, depth, depth, 1);
}