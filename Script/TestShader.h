#pragma once
#include "Include/RenderInclude.h"

namespace Koban {
	class TestShader {
	public:
		TestShader() {};
		TestShader(std::wstring path);
		~TestShader();

		struct SIMPLECONSTANT_BUFFER0
		{
			D3DXMATRIX mW;//���[���h�s��
			D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
			D3DXVECTOR4 mLightDir;//���C�g����
			D3DXVECTOR4 mEyePos;//�J�����ʒu
		};

		struct SIMPLECONSTANT_BUFFER1
		{
			D3DXVECTOR4 mAmbient;//�A���r�G���g��
			D3DXVECTOR4 mDiffuse;//�f�B�t���[�Y�F
			D3DXVECTOR4 mSpecular;//���ʔ���
		};

		HRESULT initShader();
		void setBuffer(SIMPLECONSTANT_BUFFER1 buffer);
		void render(D3DXMATRIX& mWorld, D3DXMATRIX& mView, D3DXMATRIX& mProj,
			D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	private:
		ID3D11InputLayout* mpVertexLayout;
		ID3D11Buffer* mpConstantBuffer0;
		ID3D11Buffer* mpConstantBuffer1;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;

		std::wstring mShaderPath;
	};
}
