#pragma once
#include "RenderObject.h"

namespace Koban {

	/// <summary>
	/// Material�i�f�[�^�̂݁j
	/// </summary>
	struct BaseMaterial:public RenderObject
	{
	public:
		BaseMaterial();
		~BaseMaterial();

		void draw() override;

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

		std::wstring mName;
		std::wstring shaderPath;
		/// <summary>�|���S����</summary>
		uint32_t mFaceNum;
		/// <summary>�e�N�X�`���[�t�@�C����</summary>
		std::wstring mTextureName;
		ID3D11ShaderResourceView* mpTexture;

	private:
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�쐬�p�o�b�t�@
		/// </summary>
		unique_ptr<vector<int>> mpIndexBuffer_Tmp;
		/// <summary>�C���f�b�N�X�o�b�t�@</summary>
		ID3D11Buffer* mpIndexBuffer;
		/// <summary>
		/// �T���v���[�X�e�[�g
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;
	};
}