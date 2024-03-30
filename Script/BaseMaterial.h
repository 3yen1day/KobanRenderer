#pragma once
#include "RenderObject.h"

namespace Koban {

	/// <summary>
	/// Material（データのみ）
	/// </summary>
	struct BaseMaterial:public RenderObject
	{
	public:
		BaseMaterial();
		~BaseMaterial();

		void draw() override;

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

		std::wstring mName;
		std::wstring shaderPath;
		/// <summary>ポリゴン数</summary>
		uint32_t mFaceNum;
		/// <summary>テクスチャーファイル名</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;

	private:
		/// <summary>
		/// インデックスバッファ作成用バッファ
		/// </summary>
		unique_ptr<vector<int>> mpIndexBuffer_Tmp;
		/// <summary>インデックスバッファ</summary>
		ID3D11Buffer* mpIndexBuffer;
		/// <summary>
		/// サンプラーステート
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;
	};
}