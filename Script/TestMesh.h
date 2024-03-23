#pragma once
#include "TestShader.h"
#include "TestMaterial.h"
#include "BaseMesh.h"

//マテリアル構造体
namespace Koban {
	class TestMesh:BaseMesh
	{
	public:
		TestMesh();
		~TestMesh();
		void createMaterialFromFile(std::wstring FileName) override;
		void Render(D3DXMATRIX& mViewMat, D3DXMATRIX& mProjMat, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	private:
		//頂点の構造体
		struct MY_VERTEX
		{
			D3DXVECTOR3 mPos;
			D3DXVECTOR3 mNorm;
			D3DXVECTOR2 mUV;
		};

		// トランスフォーム情報(todo:コンポーネントにする)
		D3DXVECTOR3 mPos;
		float mYaw, mPitch, mRoll, mScale;

		/// <summary>
		/// モデルのリソースパス
		/// </summary>
		std::wstring mModelPath;
	};
}
