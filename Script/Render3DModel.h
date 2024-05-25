#pragma once
namespace Koban {
	class Render3DModel
	{
	public:
		Render3DModel();
		~Render3DModel() {};
		void draw();

		//頂点の構造体
		struct SimpleVertex
		{
			D3DXVECTOR3 Pos; //位置
			D3DXVECTOR2 vTex; //テクスチャー座標
		};
		//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
		struct SIMPLESHADER_CONSTANT_BUFFER
		{
			D3DXMATRIX mWVP;
			D3DXVECTOR4 vColor;
		};

	private:
		//モデルの種類毎
		ID3D11InputLayout* m_pVertexLayout;
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11Buffer* m_pConstantBuffer;

		//モデル毎
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
		ID3D11ShaderResourceView* m_pTexture;//テクスチャー
	};
}