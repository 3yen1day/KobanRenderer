#pragma once
namespace Koban {
	struct TestMaterial
	{
	public:
		std::wstring mName;
		std::wstring shaderPath;
		/// <summary>インデックスバッファ</summary>
		ID3D11Buffer* mpIndexBuffer;
		/// <summary>ポリゴン数</summary>
		uint32_t mFaceNum;

		/// <summary>テクスチャーファイル名</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;
		D3DXVECTOR4 mKa;//アンビエント
		D3DXVECTOR4 mKd;//ディフューズ
		D3DXVECTOR4 mKs;//スペキュラー

		TestMaterial() :
			mName{ L"" },
			shaderPath{ L"" },
			mTextureName{ L"" },
			mFaceNum{ 0 },
			mpTexture{ nullptr },
			mpIndexBuffer{ nullptr }
		{
		}

		~TestMaterial();
	};
}