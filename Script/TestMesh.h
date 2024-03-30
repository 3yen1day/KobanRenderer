#pragma once
#include "TestShader.h"
#include "TestMaterial.h"
#include "BaseMesh.h"

//マテリアル構造体
namespace Koban {
	class TestMesh : public BaseMesh
	{
	public:
		TestMesh();
		~TestMesh();
		void update() override;
		void draw() override;

		/// <summary>
		/// materialPathからMaterialを作成する
		/// </summary>
		/// <param name="materialPath"></param>
		void createMaterialFromFile(std::wstring FileName) override;

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
