#include "Resources.inc.hlsl"

//�e�N�X�`���[���Q�Ƃ��ă����_�[�@���_�V�F�[�_�[
VS_OUTPUT main( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 UV : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Pos=Pos;
	output.UV=UV;

	return output;
}