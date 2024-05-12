#include "SimpleSquare.h"
#include "Render.h"
#include "Camera.h"

namespace Koban {
	SimpleSquare::SimpleSquare()
	{
		//shader初期化
		//頂点インプットレイアウトを定義	
		vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ID3DBlob* compiledShader = NULL;
		RenderLib::createShader(
			DEVICE,
			L"Shader//Simple.hlsl",
			layout,
			L"VS",
			L"PS",
			&m_pVertexLayout,
			&m_pVertexShader,
			&m_pPixelShader
		);
		SAFE_RELEASE(compiledShader);

		//コンスタントバッファ初期化
		RenderLib::createConstantBuffer<D3DXMATRIX>(DEVICE, &m_pConstantBuffer);

		//バーテックスバッファ初期化
		vector<SimpleVertex> vertices = {
			{D3DXVECTOR3(-0.5,-0.5,0),D3DXVECTOR2(0,1)},//頂点1,
			{D3DXVECTOR3(-0.5,0.5,0), D3DXVECTOR2(0,0)},//頂点2
			{D3DXVECTOR3(0.5,-0.5,0),D3DXVECTOR2(1,1)}, //頂点3
			{D3DXVECTOR3(0.5,0.5,0),D3DXVECTOR2(1,0)}, //頂点4
		};
		RenderLib::createVertexBffer<SimpleVertex>(DEVICE, DEVICE_CONTEXT, vertices, &m_pVertexBuffer);

		//テクスチャ用サンプラ作成
		RenderLib::createSamplerState(DEVICE, &m_pSampleLinear);

		//テクスチャ作成
		RenderLib::createTexture(DEVICE, L"Resource\\sprite.jpg", &m_pTexture);
	}

	void SimpleSquare::draw() {
		//使用するシェーダーの登録	
		DEVICE_CONTEXT->VSSetShader(m_pVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(m_pPixelShader, NULL, 0);

		//シェーダーのコンスタントバッファーに各種データ（位置と色）を渡す	
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		auto pos = D3DXVECTOR3(0.0f, 0.0f, -0.5f);
		D3DXMATRIX wMat;
		D3DXMatrixTranslation(&wMat, pos.x, pos.y, pos.z);

		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = wMat * Render::getCamera()->getViewMat() * Render::getCamera()->getProjMat();
		D3DXMatrixTranspose(&m, &m); //GPUの行列計算方法とDirectXのそれが異なるため転置する
		cb.mWVP = m;
		//カラーを渡す
		D3DXVECTOR4 vColor(1, 0, 0, 1);
		cb.vColor = vColor;

		if (SUCCEEDED(DEVICE_CONTEXT->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))//pData.pDataにm_pConstantBufferのアドレス
		{
			if (memcpy_s(pData.pData, sizeof(SIMPLESHADER_CONSTANT_BUFFER), (void*)(&cb), sizeof(cb))) {
				DebugLib::error(L"memCopy時にエラー");
			}
			DEVICE_CONTEXT->Unmap(m_pConstantBuffer, 0);
		}

		//このコンスタントバッファーを使うシェーダーの登録
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		//頂点インプットレイアウトをセット
		DEVICE_CONTEXT->IASetInputLayout(m_pVertexLayout);
		//プリミティブ・トポロジーをセット
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//テクスチャーをシェーダーに渡す
		DEVICE_CONTEXT->PSSetSamplers(0, 1, &m_pSampleLinear);
		DEVICE_CONTEXT->PSSetShaderResources(0, 1, &m_pTexture);
		//プリミティブをレンダリング
		DEVICE_CONTEXT->Draw(4, 0);
	}
}