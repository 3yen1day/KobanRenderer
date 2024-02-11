#pragma once
#include "TestShader.h"

//�}�e���A���\����
namespace Koban {
	class TestMesh
	{
	public:
		TestMesh();
		~TestMesh();
		HRESULT init();
		HRESULT loadMaterialFromFile(std::wstring FileName);
		void Render(D3DXMATRIX& mViewMat, D3DXMATRIX& mProjMat, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

		/// <summary>
		/// ���_��
		/// </summary>
		uint32_t mVertNum;
		/// <summary>
		/// �|���S����
		/// </summary>
		uint32_t mFaceNum;
		/// <summary>
		/// ���_�o�b�t�@
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		/// <summary>
		/// Shader��Dic�Bkey:shaderPath, value:shader�B
		/// </summary>
		std::unordered_map<std::wstring, TestShader> mShaderDic;

		/// <summary>
		/// Material��Dic�Bkey:shaderPath, value:����shader������Material��list�B
		/// shader���ɏ���������̂ŁAshader��B
		/// </summary>
		std::unordered_map<std::wstring, std::list<TestMaterial*>*> mMaterialDic;
		/// <summary>
		/// �T���v���[�X�e�[�g
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;

	private:
		//���_�̍\����
		struct MY_VERTEX
		{
			D3DXVECTOR3 mPos;
			D3DXVECTOR3 mNorm;
			D3DXVECTOR2 mUV;
		};

		// �g�����X�t�H�[�����
		D3DXVECTOR3 mPos;
		float mYaw, mPitch, mRoll, mScale;

		/// <summary>
		/// ���f���̃��\�[�X�p�X
		/// </summary>
		std::wstring mModelPath;

		/// <summary>
		/// �t�@�C������Mesh��ǂݍ���
		/// </summary>
		/// <param name="FileName"></param>
		/// <returns></returns>
		HRESULT loadResources(std::wstring FileName);
	};
}
