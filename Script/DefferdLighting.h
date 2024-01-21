#pragma once
#include "../stdafx.h"

/// <summary>
/// GBuffer‚Ìî•ñ‚ğBuckBuffer‚ÉƒŒƒ“ƒ_ƒŠƒ“ƒO‚·‚é
/// </summary>
namespace Koban {
	class DefferdLighting : public RenderObject
	{
	public:
		DefferdLighting();
		~DefferdLighting() {};
		void draw() override ;

	private:
		ID3D11VertexShader* mpVS_From_Tex;
		ID3D11PixelShader* mpPS_From_Tex;
		const std::wstring mShaderFileName = L"Deferred.hlsl";
		const std::wstring mVSShaderName = L"VS_From_Tex";
		const std::wstring mPSShaderName = L"PS_From_Tex";
	};
}