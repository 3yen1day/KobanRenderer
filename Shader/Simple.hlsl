
//�O���[�o��
cbuffer global
{
	matrix g_mWVP; //���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};

//�\����
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float4 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Normal : NORMAL,float2 UV : TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos,g_mWVP);
	return output;
}
//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color=float4(0,0.5,0,0);
	return color;
}