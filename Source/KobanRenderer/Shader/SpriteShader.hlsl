//0�Ԗڂ̃e�N�X�`���X���b�g���g�p
Texture2D mainTexture : register(t0);
//0�Ԗڂ̃T���v���X���b�g���g�p
SamplerState mainSampler : register(s0);

//���_�V�F�[�_����o�͂���f�[�^
struct VSOutput {
	float4 Pos: SV_Position; //���_�̍��W
    float2 UV : TEXCOORD0; //UV���W
};

//========================================
// ���_�V�F�[�_�[
//========================================
VSOutput VS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    VSOutput Out;
    Out.Pos = pos;
    Out.UV = uv;
    return Out;
}

//========================================
// �s�N�Z���V�F�[�_�[
//========================================
float4 PS(VSOutput In) : SV_Target0
{
    float4 col = mainTexture.Sample(mainSampler, In.UV);
    return col;
}