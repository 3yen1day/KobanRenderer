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
		/// materialPathからMaterialを作成する
		/// </summary>
		/// <param name="materialPath"></param>
		virtual void createMaterialFromFile(wstring materialPath) = 0;

		/// <summary>
		/// ShaderのDic。key:shaderPath, value:shader。
		/// </summary>
		std::unordered_map<std::wstring, BaseShader> mShaderDic;
		
	private:
		// 頂点カウント
		struct VERTEX_COUNT
		{
		public:
			int vtCount;
			int polyCount;
			int uvCount;
			int normCount;
		};

#pragma region フィールド
		BaseShader::MY_VERTEX* mVertexBuffer;
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
		/// shaderの頂点バッファを作成。
		/// materialのインデックスバッファを作成。
		/// </summary>
		/// <param name="fp"></param>
		/// <param name="pvCoord"></param>
		/// <param name="pvNormal"></param>
		/// <param name="pvUV"></param>
		/// <returns>VertexBuffer</returns>
		void createVtxBufAndIdxBuf(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV);

		/// <summary>
		/// IndexBufferのセット
		/// </summary>
		/// <param name="shaderlName"></param>
		/// <param name="materialName"></param>
		/// <param name="indexBuffer"></param>
		void setIndexBuffer(std::wstring materialName, const int indexBuffer[], int bufferSize);

		/// <summary>
		/// vertexBufferのセット
		/// </summary>
		void setVertexBuffer(const BaseShader::MY_VERTEX* const vertBuf, int bufferSize);
#pragma endregion

	};
}