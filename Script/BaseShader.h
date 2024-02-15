#pragma once
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

		HRESULT initShader();

		void setConstantBuffer(SIMPLECONSTANT_BUFFER0 data) {
			mConstantBuffer = data;
		}
		void update();

	private:
		ID3D11InputLayout* mpVertexLayout;
		ID3D11Buffer* mpConstantBuffer0;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;

		SIMPLECONSTANT_BUFFER0 mConstantBuffer;
		std::wstring mShaderPath;
	};
}
