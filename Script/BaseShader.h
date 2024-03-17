#pragma once

namespace Koban {
	class BaseMaterial;
}

namespace Koban {
	class BaseShader {
	public:
		BaseShader() {};
		BaseShader(std::wstring path);
		~BaseShader();

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

	private:
		ID3D11InputLayout* mpVertexLayout;
		ID3D11Buffer* mpConstantBuffer0;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;

		std::wstring mShaderPath;
		std::unordered_map<std::wstring, unique_ptr<BaseMaterial>> mMaterialDic;
	};
}
