#pragma once
#include "TestShader.h"
#include "TestMaterial.h"
#include "BaseMesh.h"

//�}�e���A���\����
namespace Koban {
	class TestMesh:BaseMesh
	{
	public:
		TestMesh();
		~TestMesh();
		void createMaterialFromFile(std::wstring FileName) override;
		void Render(D3DXMATRIX& mViewMat, D3DXMATRIX& mProjMat, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	private:
		//���_�̍\����
		struct MY_VERTEX
		{
			D3DXVECTOR3 mPos;
			D3DXVECTOR3 mNorm;
			D3DXVECTOR2 mUV;
		};

		// �g�����X�t�H�[�����(todo:�R���|�[�l���g�ɂ���)
		D3DXVECTOR3 mPos;
		float mYaw, mPitch, mRoll, mScale;

		/// <summary>
		/// ���f���̃��\�[�X�p�X
		/// </summary>
		std::wstring mModelPath;
	};
}
