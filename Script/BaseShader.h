#pragma once
#include "BaseMaterial.h"

namespace Koban {
	class BaseShader {
	public:
		BaseShader() {};
		BaseShader(std::wstring path);
		~BaseShader();

		/// <summary>
		/// ���_���̍\����
		/// </summary>
		struct MY_VERTEX
		{
			D3DXVECTOR3 mPos;
			D3DXVECTOR3 mNorm;
			D3DXVECTOR2 mUV;
		};

		/// <summary>
		/// ��{�R���X�^���g�o�b�t�@�̍\����
		/// </summary>
		struct SIMPLECONSTANT_BUFFER0
		{
			D3DXMATRIX mW;//���[���h�s��
			D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
			D3DXVECTOR4 mLightDir;//���C�g����
			D3DXVECTOR4 mEyePos;//�J�����ʒu
		};

		virtual void initShader();
		virtual void update();
		virtual void draw();

		void createVertexBuffer(const MY_VERTEX* const vertexBuffer, int polyNum);

#pragma region material�n
		/// <summary>
		/// Material�̒ǉ�
		/// </summary>
		/// <param name="material"></param>
		void addMaterial(BaseMaterial* material);

		void updateBaseConstantBuffer
		(
			const D3DXMATRIX& worldMat
		);

		void setIndexBuffer(std::wstring materialName, const int indexBuffer[], int bufferSize);
		void createIndexBuffer();

#pragma endregion


	protected:
		std::unordered_map<std::wstring, BaseMaterial*> mMaterialDic;

	private:
		ID3D11InputLayout* mpVertexLayout;
		ID3D11Buffer* mpConstantBuffer0;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;
		/// <summary>
		/// ���_�o�b�t�@
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		std::wstring mShaderPath;
	};
}
