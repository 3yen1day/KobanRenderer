#include "Resources.inc.hlsl"

//�e�N�X�`���[�Ƀ����_�[�@�s�N�Z���V�F�[�_�[
PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

	//TODO:�J���[�e�N�X�`���[�֏o�́i���_�J���[���g���悤�ɂȂ�����l����j
    //Out.vColor = g_tex.Sample(g_samLinear, input.UV) + float4(0.01, 0.01, 0.01, 0);
	
    //�[�x���
    float4 depth = g_texDepthStencil.Sample(g_samLinear, input.UV);
    Out.vDepth = float4(depth.r != 0, depth.g != 0, depth.b != 0, 1.0);
    //Out.vDepth = float4(1, 1, 1, 1);
    
	//���[���h�@���e�N�X�`���[�֏o��
    float3 vNormal = input.WorldNormal;
    Out.vNormal = normalize(float4(vNormal, 0));

	//���[���h���W�e�N�X�`���[�֏o��
    Out.vPosition = input.WorldPos;

    return Out;
}
