#include "Resources.inc.hlsl"

//�e�N�X�`���[�Ƀ����_�[�@�s�N�Z���V�F�[�_�[
PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

	//�J���[�e�N�X�`���[�֏o�� 
    Out.vColor = g_tex.Sample(g_samLinear, input.UV) + float4(0.01, 0.01, 0.01, 0);
	
	//���[���h�@���e�N�X�`���[�֏o��
    float3 vNormal = input.WorldNormal;
    vNormal = vNormal * 0.5 + 0.5;
    Out.vNormal = normalize(float4(vNormal, 0));

	//���[���h���W�e�N�X�`���[�֏o��
    Out.vPosition = input.WorldPos;

    return Out;
}
