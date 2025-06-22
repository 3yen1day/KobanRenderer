#pragma once
#include "../Rendering/FbxLoader.h"
#include "../Core/Component.h"

namespace Koban {
	class Mesh : public Component
	{
	public:
		Mesh() {};
		~Mesh() {};

		void start() override ;
		void update() override;
		void draw() override ;
		void destroy() override {};
		void drawUI() override {};

		void setMeshResourcePath(const string& resourcePath);

		struct CONSTANT_BUFFER_MATERIAL
		{
			D3DXVECTOR4 vAmbient;//�A���r�G���g��
			D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
			D3DXVECTOR4 vSpecular;//���ʔ���
		};

	private:
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
		ID3D11Buffer* mpConstantBuffer_Transform;
		ID3D11Buffer* mpConstantBuffer_Material;

		FbxLoader::VertexInfo mFbxVertexInfo; // fbx���f������ǂݍ��񂾒��_���
		string mResourcePath; // ���\�[�X�p�X
	};
}