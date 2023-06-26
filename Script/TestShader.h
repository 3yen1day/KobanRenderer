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
			D3DXMATRIX mW;//ワールド行列
			D3DXMATRIX mWVP;//ワールドから射影までの変換行列
			D3DXVECTOR4 mLightDir;//ライト方向
			D3DXVECTOR4 mEyePos;//カメラ位置
		};

		struct SIMPLECONSTANT_BUFFER1
		{
			D3DXVECTOR4 mAmbient;//アンビエント光
			D3DXVECTOR4 mDiffuse;//ディフューズ色
			D3DXVECTOR4 mSpecular;//鏡面反射
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
