#pragma once
#include "../Rendering/FbxLoader.h"
#include "../Core/Component.h"

namespace Koban {
	class Mesh : public Component
	{
	public:
		Mesh() {};
		~Mesh() {};

		void start() override ;
		void update() override;
		void draw() override ;
		void destroy() override {};
		void drawUI() override {};

		void setMeshResourcePath(const string& resourcePath);

		struct CONSTANT_BUFFER_MATERIAL
		{
			D3DXVECTOR4 vAmbient;//アンビエント光
			D3DXVECTOR4 vDiffuse;//ディフューズ色
			D3DXVECTOR4 vSpecular;//鏡面反射
		};

	private:
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
		ID3D11Buffer* mpConstantBuffer_Transform;
		ID3D11Buffer* mpConstantBuffer_Material;

		FbxLoader::VertexInfo mFbxVertexInfo; // fbxモデルから読み込んだ頂点情報
		string mResourcePath; // リソースパス
	};
}