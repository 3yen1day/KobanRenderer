#pragma once
#include "../stdafx.h"
//マテリアル構造体
namespace Koban {
	struct TestMaterial
	{
	public:
		std::wstring mName;
		std::wstring mTextureName;//テクスチャーファイル名
		std::wstring shaderPath;
		D3DXVECTOR4 mKa;//アンビエント
		D3DXVECTOR4 mKd;//ディフューズ
		D3DXVECTOR4 mKs;//スペキュラー
		uint32_t mFaceNum;//そのマテリアルであるポリゴン数
		ID3D11ShaderResourceView* mpTexture;
		/// <summary>
		/// インデックスバッファ
		/// </summary>
		ID3D11Buffer* mpIndexBuffer;

		TestMaterial() :
			mName{ L"" },
			mTextureName{ L"" },
			shaderPath{ L"" },
			mFaceNum{ 0 },
			mpTexture{ nullptr },
			mpIndexBuffer{ nullptr }
		{
		}

		~TestMaterial();
	};


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
		/// ShaderのDic
		/// </summary>
		std::unordered_map<std::wstring, TestShader> mShaderDic;

		/// <summary>
		/// MaterialのDic
		/// 構造的にはMaterialがShaderを持つ形が理想だが、Shader毎に処理を行ったほうが効率がいい二律背反。一旦分けておく。
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
