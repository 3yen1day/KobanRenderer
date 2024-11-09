#pragma once
#include "FbxLoader.h"
namespace Koban {
	class Render3DModel
	{
	public:
		Render3DModel();
		~Render3DModel() {};

		void draw();

		//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
		struct CONSTANT_BUFFER_DEFAULT
		{
			D3DXMATRIX mW;//ワールド行列
			D3DXMATRIX mWVP;//ワールドから射影までの変換行列
			D3DXVECTOR4 vLightDir;//ライト位置
			D3DXVECTOR4 vEye;//カメラ位置
		};

		struct CONSTANT_BUFFER_MATERIAL
		{
			D3DXVECTOR4 vAmbient;//アンビエント光
			D3DXVECTOR4 vDiffuse;//ディフューズ色
			D3DXVECTOR4 vSpecular;//鏡面反射
		};

	private:
		D3DXMATRIX getModelMatrix();
		D3DXMATRIX getMVPMatrix(const D3DXMATRIX& modelMat);
		CONSTANT_BUFFER_MATERIAL getMaterialVal();

		//モデルの種類毎
		ID3D11InputLayout* mpVertexLayout;
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;

		//モデル毎
		ID3D11Buffer* mpVertexBuffer;
		ID3D11Buffer* mpIndexBuffer;
		ID3D11SamplerState* mpSampleLinear;//テクスチャーのサンプラー
		ID3D11ShaderResourceView* mpTexture;//テクスチャー
		ID3D11Buffer* mpConstantBuffer_Default;
		ID3D11Buffer* mpConstantBuffer_Material;

		FbxLoader::VertexInfo mFbxVertexInfo; // fbxモデルから読み込んだ頂点情報
	};
}