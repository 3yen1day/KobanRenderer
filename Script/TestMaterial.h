#pragma once
#include "BaseMaterial.h"

namespace Koban {
	
	
	struct TestMaterial:BaseMaterial
	{
	public:
		/// <summary>�e�N�X�`���[�t�@�C����</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;
		D3DXVECTOR4 mKa;//�A���r�G���g
		D3DXVECTOR4 mKd;//�f�B�t���[�Y
		D3DXVECTOR4 mKs;//�X�y�L�����[

		TestMaterial() :
			mTextureName{ L"" },
			mpTexture{ nullptr }
		{
		}

		~TestMaterial();
	};
}