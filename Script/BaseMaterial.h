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

		BaseMaterial() :
			mName{ L"" },
			shaderPath{ L"" },
			mFaceNum{ 0 },
			mpIndexBuffer{ nullptr }
		{
		}

		~BaseMaterial();
	};
}