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
		virtual void updateBaseConstantBuffer
		(
			const D3DXMATRIX& worldMat,
			const D3DXMATRIX& viewMat,
			const D3DXMATRIX& projMat,
			const D3DXVECTOR3& light,
			const D3DXVECTOR3& eye
		);

		void createVertexBuffer(const MY_VERTEX* const vertexBuffer, int polyNum);

		/// <summary>
		/// Material�̒ǉ�
		/// </summary>
		/// <param name="material"></param>
		void addMaterial(BaseMaterial* material);

		/// <summary>
		/// Material�̎擾
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns></returns>
		const BaseMaterial* getMaterial(std::wstring matName);

		const std::vector<BaseMaterial*> getMaterials();

	private:
		ID3D11InputLayout* mpVertexLayout;
		ID3D11Buffer* mpConstantBuffer0;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;
		/// <summary>
		/// ���_�o�b�t�@
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		/// <summary>
		/// �T���v���[�X�e�[�g
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;
		std::unordered_map<std::wstring, BaseMaterial*> mMaterialDic;
		std::wstring mShaderPath;
	};
}
