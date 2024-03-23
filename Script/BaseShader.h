#pragma once
#include "BaseMaterial.h"

namespace Koban {
	class BaseShader {
	public:
		BaseShader() {};
		BaseShader(std::wstring path);
		~BaseShader();

		/// <summary>
		/// 頂点情報の構造体
		/// </summary>
		struct MY_VERTEX
		{
			D3DXVECTOR3 mPos;
			D3DXVECTOR3 mNorm;
			D3DXVECTOR2 mUV;
		};

		/// <summary>
		/// 基本コンスタントバッファの構造体
		/// </summary>
		struct SIMPLECONSTANT_BUFFER0
		{
			D3DXMATRIX mW;//ワールド行列
			D3DXMATRIX mWVP;//ワールドから射影までの変換行列
			D3DXVECTOR4 mLightDir;//ライト方向
			D3DXVECTOR4 mEyePos;//カメラ位置
		};

		virtual void initShader();
		virtual void update();
		virtual void updateBaseConstantBuffer
		(
			const D3DXMATRIX& worldMat,
			const D3DXMATRIX& viewMat,
			const D3DXMATRIX& projMat,
			const D3DXVECTOR3& light,
			const D3DXVECTOR3& eye
		);

		void createVertexBuffer(const MY_VERTEX* const vertexBuffer, int polyNum);

		/// <summary>
		/// Materialの追加
		/// </summary>
		/// <param name="material"></param>
		void addMaterial(BaseMaterial* material);

		/// <summary>
		/// Materialの取得
		/// </summary>
		/// <param name="fileName"></param>
		/// <returns></returns>
		const BaseMaterial* getMaterial(std::wstring matName);

		const std::vector<BaseMaterial*> getMaterials();

	private:
		ID3D11InputLayout* mpVertexLayout;
		ID3D11Buffer* mpConstantBuffer0;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;
		/// <summary>
		/// 頂点バッファ
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		/// <summary>
		/// サンプラーステート
		/// </summary>
		ID3D11SamplerState* mpSampleLinear;
		std::unordered_map<std::wstring, BaseMaterial*> mMaterialDic;
		std::wstring mShaderPath;
	};
}
