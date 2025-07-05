#include "Resources.inc.hlsl"

//�e�N�X�`���[�Ƀ����_�[�@�s�N�Z���V�F�[�_�[
PS_OUTPUT main(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

	//�J���[
    Out.Color = g_Ambient;
	
    //�[�x���
    float depth = g_texDepthStencil.Sample(g_samLinear, input.UV).r;
    Out.DepthStencil = float4(depth, 1, 0, 0);
    
	//���[���h�@���e�N�X�`���[�֏o��
    float3 Normal = input.WorldNormal;
    Normal = Normal * 0.5 + 0.5;
    Out.Normal = normalize(float4(Normal, 0));

	//���[���h���W�e�N�X�`���[�֏o��
    Out.Position = input.WorldPos;

    return Out;
}
