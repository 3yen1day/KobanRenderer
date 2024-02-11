#pragma once
#include "TestShader.h"

//マテリアル構造体
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
		/// 頂点数
		/// </summary>
		uint32_t mVertNum;
		/// <summary>
		/// ポリゴン数
		/// </summary>
		uint32_t mFaceNum;
		/// <summary>
		/// 頂点バッファ
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		/// <summary>
		/// ShaderのDic。key:shaderPath, value:shader。
		/// </summary>
		std::unordered_map<std::wstring, TestShader> mShaderDic;

		/// <summary>
		/// MaterialのDic。key:shaderPath, value:そのshaderを持つMaterialのlist。
		/// shader毎に処理をするので、shader基準。
		/// </summary>
		std::unordered_map<std::wstring, std::list<TestMaterial*>*> mMaterialDic;
		/// <summary>
		/// サンプラーステート
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;

	private:
		//頂点の構造体
		struct MY_VERTEX
		{
			D3DXVECTOR3 mPos;
			D3DXVECTOR3 mNorm;
			D3DXVECTOR2 mUV;
		};

		// トランスフォーム情報
		D3DXVECTOR3 mPos;
		float mYaw, mPitch, mRoll, mScale;

		/// <summary>
		/// モデルのリソースパス
		/// </summary>
		std::wstring mModelPath;

		/// <summary>
		/// ファイルからMeshを読み込む
		/// </summary>
		/// <param name="FileName"></param>
		/// <returns></returns>
		HRESULT loadResources(std::wstring FileName);
	};
}
