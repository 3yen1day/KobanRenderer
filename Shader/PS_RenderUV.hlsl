#include "Resources.inc.hlsl"

//�e�N�X�`���[���Q�Ƃ��ă����_�[�@�s�N�Z���V�F�[�_�[
float4 main(VS_OUTPUT input) : SV_Target
{
	//uv���擾���ĕ\��
    return float4(input.UV.r, input.UV.g, 0, 1);
}