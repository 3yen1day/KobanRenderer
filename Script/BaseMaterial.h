#pragma once
namespace Koban {
	struct BaseMaterial
	{
	public:
		std::wstring mName;
		std::wstring shaderPath;
		/// <summary>�C���f�b�N�X�o�b�t�@</summary>
		ID3D11Buffer* mpIndexBuffer;
		/// <summary>�|���S����</summary>
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