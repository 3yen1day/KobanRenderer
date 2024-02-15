#pragma once
#include "BaseMaterial.h"

namespace Koban {
	
	
	struct TestMaterial:BaseMaterial
	{
	public:
		/// <summary>テクスチャーファイル名</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;
		D3DXVECTOR4 mKa;//アンビエント
		D3DXVECTOR4 mKd;//ディフューズ
		D3DXVECTOR4 mKs;//スペキュラー

		TestMaterial() :
			mTextureName{ L"" },
			mpTexture{ nullptr }
		{
		}

		~TestMaterial();
	};
}