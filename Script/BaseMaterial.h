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
		/// <summary>�e�N�X�`���[�t�@�C����</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;

		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�ɒǉ�
		/// </summary>
		/// <param name="indexBuffer"></param>
		/// <param name="size"></param>
		void addIndexBuffer(const int indexBuffer[], int size) const;

		/// <summary>
		/// �C���f�b�N�X�o�b�t�@���쐬
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
		/// �C���f�b�N�X�o�b�t�@�쐬�p�o�b�t�@
		/// </summary>
		unique_ptr<vector<int>> mpIndexBuffer_Tmp;
	};
}