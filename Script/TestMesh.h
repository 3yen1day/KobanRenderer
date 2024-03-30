#pragma once
#include "TestShader.h"
#include "TestMaterial.h"
#include "BaseMesh.h"

//�}�e���A���\����
namespace Koban {
	class TestMesh : public BaseMesh
	{
	public:
		TestMesh();
		~TestMesh();
		void update() override;
		void draw() override;

		/// <summary>
		/// materialPath����Material���쐬����
		/// </summary>
		/// <param name="materialPath"></param>
		void createMaterialFromFile(std::wstring FileName) override;

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
