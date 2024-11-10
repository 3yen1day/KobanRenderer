#include "GBufferToBackBuffer.h"
#include "Render.h"
#include "RTTManager.h"
#include "FbxLoader.h"

namespace Koban {
	GBufferToBackBuffer::GBufferToBackBuffer() {
		// ----------------------------------------
		// Shader
		// ----------------------------------------
		//使用するShaderの作成
		RenderUtil::createShader(
			DEVICE,
			L"Shader/Deferred.hlsl",
			L"VS_From_Tex",
			L"PS_From_Tex",
			&mpVertexShader,
			&mpPixelShader
		);

		// ----------------------------------------
		// Buffer
		// ----------------------------------------
		//スクリーンサイズのポリゴン用　バーテックスバッファーを作成
		MY_VERTEX VertexData[] =
		{
			D3DXVECTOR3(-1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,1),
			D3DXVECTOR3(-1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),
			D3DXVECTOR3(1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1),
			D3DXVECTOR3(1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,0)
		};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = VertexData;
		if (FAILED(DEVICE->CreateBuffer(&bd, &sd, &mpVertexBuffer))) {
			return;
		}

		//コンスタントバッファー作成　
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(CBUFER_COORD);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		auto cb_p = mpConstantBuffer.get();
		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &cb_p)))
		{
			return;
		}
	}
	
	void GBufferToBackBuffer::update() {

	}

	void GBufferToBackBuffer::draw() {
		//シェーダーのセット
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//テクスチャセット
		auto col = Render::getRTTManager()->getRTTSRV(Koban::RTTManager::COLOR);
		auto normal = Render::getRTTManager()->getRTTSRV(Koban::RTTManager::NORMAL);
		auto pos = Render::getRTTManager()->getRTTSRV(Koban::RTTManager::POSITION);
		DEVICE_CONTEXT->PSSetShaderResources(1, 1, &col);
		DEVICE_CONTEXT->PSSetShaderResources(2, 1, &normal);
		DEVICE_CONTEXT->PSSetShaderResources(3, 1, &pos);
		//トポロジーのセット
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//頂点バッファのセット
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//バックバッファにDraw!
		DEVICE_CONTEXT->Draw(4, 0);
	}

	void GBufferToBackBuffer::destroy() {
		SAFE_RELEASE(mpVertexShader);
		SAFE_RELEASE(mpPixelShader);
	}
}