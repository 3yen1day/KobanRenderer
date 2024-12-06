#pragma once
#include "../Rendering/FbxLoader.h"
#include "../Core/Component.h"

namespace Koban {
	class Mesh : public Component
	{
	public:
		Mesh();
		~Mesh() {};

		void start() override ;
		void update() override {};
		void draw() override ;
		void destroy() override {};

		//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
		struct CONSTANT_BUFFER_DEFAULT
		{
			D3DXMATRIX mW;//���[���h�s��
			D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
			D3DXVECTOR4 vLightDir;//���C�g�ʒu
			D3DXVECTOR4 vEye;//�J�����ʒu
		};

		struct CONSTANT_BUFFER_MATERIAL
		{
			D3DXVECTOR4 vAmbient;//�A���r�G���g��
			D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
			D3DXVECTOR4 vSpecular;//���ʔ���
		};

	private:
		D3DXMATRIX getModelMatrix();
		D3DXMATRIX getMVPMatrix(const D3DXMATRIX& modelMat);
		CONSTANT_BUFFER_MATERIAL getMaterialVal();

		//���f���̎�ޖ�
		ID3D11InputLayout* mpVertexLayout;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;

		//���f����
		ID3D11Buffer* mpVertexBuffer;
		ID3D11Buffer* mpIndexBuffer;
		ID3D11SamplerState* mpSampleLinear;//�e�N�X�`���[�̃T���v���[
		ID3D11ShaderResourceView* mpTexture;//�e�N�X�`���[
		ID3D11Buffer* mpConstantBuffer_Default;
		ID3D11Buffer* mpConstantBuffer_Material;

		FbxLoader::VertexInfo mFbxVertexInfo; // fbx���f������ǂݍ��񂾒��_���
	};
}