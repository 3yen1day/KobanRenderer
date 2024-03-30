#pragma once
#include "RenderObject.h"
#include "BaseShader.h"
#include "BaseMaterial.h"

namespace Koban {
	class BaseMesh : public RenderObject
	{
	public:
		BaseMesh();
		~BaseMesh();

		void start() override;
		void update() override;

	protected:
		/// <summary>
		/// materialPath����Material���쐬����
		/// </summary>
		/// <param name="materialPath"></param>
		virtual void createMaterialFromFile(wstring materialPath) = 0;

		/// <summary>
		/// Shader��Dic�Bkey:shaderPath, value:shader�B
		/// </summary>
		std::unordered_map<std::wstring, BaseShader> mShaderDic;
		
	private:
		// ���_�J�E���g
		struct VERTEX_COUNT
		{
		public:
			int vtCount;
			int polyCount;
			int uvCount;
			int normCount;
		};

#pragma region �t�B�[���h
		BaseShader::MY_VERTEX* mVertexBuffer;
		/// <summary>
		/// ���_�o�b�t�@
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		/// <summary>
		/// �T���v���[�X�e�[�g
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;
		/// <summary>
		/// ���f���̃��\�[�X�p�X
		/// </summary>
		std::wstring mModelPath;
		/// <summary>
		/// ���_�J�E���g
		/// </summary>
		unique_ptr<VERTEX_COUNT> mVertCount;

		/// <summary>
		/// �t�@�C������Mesh��ǂݍ���
		/// </summary>
		/// <param name="FileName"></param>
		/// <returns></returns>
		HRESULT loadResources(std::wstring FileName);
#pragma endregion

#pragma region  �֐�
		/// <summary>
		/// ���_���J�E���g
		/// </summary>
		/// <param name="fp"></param>
		void loadVertCount(FILE* fp);

		/// <summary>
		/// material�����[�h����
		/// </summary>
		/// <param name="fp"></param>
		void loadMaterial(FILE* fp);

		/// <summary>
		/// ���_�ǂݍ���
		/// </summary>
		/// <param name="fp"></param>
		void loadVert(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV);
		
		/// <summary>
		/// shader�̒��_�o�b�t�@���쐬�B
		/// material�̃C���f�b�N�X�o�b�t�@���쐬�B
		/// </summary>
		/// <param name="fp"></param>
		/// <param name="pvCoord"></param>
		/// <param name="pvNormal"></param>
		/// <param name="pvUV"></param>
		/// <returns>VertexBuffer</returns>
		void createVtxBufAndIdxBuf(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV);

		/// <summary>
		/// IndexBuffer�̃Z�b�g
		/// </summary>
		/// <param name="shaderlName"></param>
		/// <param name="materialName"></param>
		/// <param name="indexBuffer"></param>
		void setIndexBuffer(std::wstring materialName, const int indexBuffer[], int bufferSize);

		/// <summary>
		/// vertexBuffer�̃Z�b�g
		/// </summary>
		void setVertexBuffer(const BaseShader::MY_VERTEX* const vertBuf, int bufferSize);
#pragma endregion

	};
}