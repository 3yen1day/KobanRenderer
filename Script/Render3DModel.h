#pragma once
#include "FbxLoader.h"
namespace Koban {
	class Render3DModel
	{
	public:
		Render3DModel();
		~Render3DModel() {};

		void draw();

		//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
		struct SIMPLESHADER_CONSTANT_BUFFER
		{
			D3DXMATRIX mWVP;
			D3DXVECTOR4 vColor;
		};

	private:
		//���f���̎�ޖ�
		ID3D11InputLayout* m_pVertexLayout;
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11Buffer* m_pConstantBuffer;

		//���f����
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
		ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
		ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[

		FbxLoader::VertexInfo mFbxVertexInfo; // fbx���f������ǂݍ��񂾒��_���
	};
}