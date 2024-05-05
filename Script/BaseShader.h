#pragma once
#include "BaseMaterial.h"
#include "RenderObject.h"

namespace Koban {
	class BaseShader:public RenderObject {
	public:
		BaseShader() {};
		BaseShader(std::wstring path);
		~BaseShader();
		
		void update() override;
		void draw() override;

		/// <summary>
		/// 頂点情報の構造体
		/// </summary>
		struct MY_VERTEX
		{
		public:
			MY_VERTEX() {};
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

		void createVertexBuffer(const std::vector<BaseShader::MY_VERTEX> const vertexBuffer, int polyNum);

#pragma region material系
		/// <summary>
		/// Materialの追加
		/// </summary>
		/// <param name="material"></param>
		void addMaterial(BaseMaterial* material);

		void updateBaseConstantBuffer
		(
			const D3DXMATRIX& worldMat
		);

		void setIndexBuffer(std::wstring materialName, const std::vector<int> indexBuffer, int bufferSize);
		void createIndexBuffer();

#pragma endregion


	protected:
		std::unordered_map<std::wstring, BaseMaterial*> mMaterialDic;

	private:
		ID3D11InputLayout* mpVertexLayout;
		ID3D11Buffer* mpConstantBuffer0;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;
		/// <summary>
		/// 頂点バッファ
		/// </summary>
		ID3D11Buffer* mpVertexBuffer;
		std::wstring mShaderPath;
	};
}
