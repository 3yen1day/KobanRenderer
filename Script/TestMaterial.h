#pragma once
namespace Koban {
	struct TestMaterial
	{
	public:
		std::wstring mName;
		std::wstring shaderPath;
		/// <summary>�C���f�b�N�X�o�b�t�@</summary>
		ID3D11Buffer* mpIndexBuffer;
		/// <summary>�|���S����</summary>
		uint32_t mFaceNum;

		/// <summary>�e�N�X�`���[�t�@�C����</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;
		D3DXVECTOR4 mKa;//�A���r�G���g
		D3DXVECTOR4 mKd;//�f�B�t���[�Y
		D3DXVECTOR4 mKs;//�X�y�L�����[

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