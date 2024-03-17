#pragma once
namespace Koban {
	struct BaseMaterial
	{
	public:
		std::wstring mName;
		std::wstring shaderPath;
		/// <summary>�C���f�b�N�X�o�b�t�@</summary>
		vector<ID3D11Buffer> mpIndexBuffer;
		/// <summary>�|���S����</summary>
		uint32_t mFaceNum;
		/// <summary>�e�N�X�`���[�t�@�C����</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;

		void addIndexBuffer(const int indexBuffer[], int size) const;

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
	};
}