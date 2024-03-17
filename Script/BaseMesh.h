#pragma once
#include "RenderObject.h"
#include "BaseShader.h"
#include "BaseMaterial.h"

namespace Koban {
	class BaseMesh :RenderObject
	{
	public:
		BaseMesh();
		~BaseMesh();

		void start() override;
		void draw() override;

	protected:
		/// <summary>
		/// materialPathからMaterialを作成する
		/// </summary>
		/// <param name="materialPath"></param>
		virtual void createMaterialFromFile(wstring materialPath) = 0;
		
	private:
		//頂点の構造体
		struct MY_VERTEX
		{
			D3DXVECTOR3 mPos;
			D3DXVECTOR3 mNorm;
			D3DXVECTOR2 mUV;
		};

		// 頂点カウント
		struct VERTEX_COUNT
		{
			int vtCount;
			int polyCount;
			int uvCount;
			int normCount;
		};

#pragma region フィールド
		/// <summary>
		/// 頂点バッファ
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		/// <summary>
		/// サンプラーステート
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;
		/// <summary>
		/// モデルのリソースパス
		/// </summary>
		std::wstring mModelPath;
		/// <summary>
		/// ShaderのDic。key:shaderPath, value:shader。
		/// </summary>
		std::unordered_map<std::wstring, BaseShader> mShaderDic;
		/// <summary>
		/// 頂点カウント
		/// </summary>
		unique_ptr<VERTEX_COUNT> mVertCount;

		/// <summary>
		/// ファイルからMeshを読み込む
		/// </summary>
		/// <param name="FileName"></param>
		/// <returns></returns>
		HRESULT loadResources(std::wstring FileName);
#pragma endregion

#pragma region  関数
		/// <summary>
		/// 頂点をカウント
		/// </summary>
		/// <param name="fp"></param>
		void loadVertCount(FILE* fp);

		/// <summary>
		/// materialをロードする
		/// </summary>
		/// <param name="fp"></param>
		void loadMaterial(FILE* fp);

		/// <summary>
		/// 頂点読み込み
		/// </summary>
		/// <param name="fp"></param>
		void loadVert(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV);

		/// <summary>
		/// IndexBufferのセット
		/// </summary>
		/// <param name="shaderlName"></param>
		/// <param name="materialName"></param>
		/// <param name="indexBuffer"></param>
		void setIndexBuffer(std::wstring materialName, const int indexBuffer[], int bufferSize);
		
		/// <summary>
		/// shaderの頂点バッファを作成。
		/// materialのインデックスバッファを作成。
		/// </summary>
		/// <param name="fp"></param>
		/// <param name="pvCoord"></param>
		/// <param name="pvNormal"></param>
		/// <param name="pvUV"></param>
		/// <returns>VertexBuffer</returns>
		MY_VERTEX* createVtxBufAndIdxBuf(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV);
#pragma endregion

	};
}