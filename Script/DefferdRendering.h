#pragma once
#include "ShaderObject.h"

/// <summary>
/// GBuffer�̏���BuckBuffer�Ƀ����_�����O����
/// </summary>
namespace Koban {
	class DefferdShader : public ShaderObject
	{
	public:
		DefferdShader();
		~DefferdShader() {};
		void draw() override;
		void destroy() override;

	private:
		//vertexBuffer�p�\����
		struct MY_VERTEX
		{
			D3DXVECTOR3 vPos;
			D3DXVECTOR3 vNorm;
			D3DXVECTOR2 vTex;
		};

		//ConstantBuffer�i���W�֌W�j�p�\����
		struct CBUFER_COORD
		{
			D3DMATRIX WMat;
			D3DMATRIX wvpMat;
			D3DXVECTOR4 lightPos[MAX_Light];
			D3DXVECTOR4 eyePos;
		};

		//ConstantBuffer
		//screen�`��p���_�o�b�t�@
		std::unique_ptr<ID3D11Buffer> mpVertexBuffer;
		std::unique_ptr <ID3D11Buffer> mpConstantBuffer;

		const std::wstring mShaderFileName = L"Deferred.hlsl";
		const std::wstring mVSShaderName = L"VS_From_Tex";
		const std::wstring mPSShaderName = L"PS_From_Tex";
	};
}