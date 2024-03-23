#pragma once
namespace Koban {
	struct BaseMaterial
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

		/// <summary>
		/// インデックスバッファに追加
		/// </summary>
		/// <param name="indexBuffer"></param>
		/// <param name="size"></param>
		void addIndexBuffer(const int indexBuffer[], int size) const;

		/// <summary>
		/// インデックスバッファを作成
		/// </summary>
		void createIndexBuffer();

		BaseMaterial() :
			mName{ L"" },
			shaderPath{ L"" },
			mFaceNum{ 0 },
			mpIndexBuffer{},
			mTextureName{ L"" },
			mpTexture{ nullptr }
		{
		}

		~BaseMaterial();

	private:
		/// <summary>
		/// インデックスバッファ作成用バッファ
		/// </summary>
		unique_ptr<vector<int>> mpIndexBuffer_Tmp;
	};
}