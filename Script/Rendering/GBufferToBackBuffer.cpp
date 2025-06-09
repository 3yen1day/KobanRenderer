#include "GBufferToBackBuffer.h"
#include "Rendering.h"
#include "RTTManager.h"
#include "FbxLoader.h"

namespace Koban
{
	GBufferToBackBuffer::GBufferToBackBuffer()
	{
	}

	void GBufferToBackBuffer::start()
	{
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
		if (FAILED(DEVICE->CreateBuffer(&bd, &sd, &mpVertexBuffer)))
		{
			return;
		}
	}

	void GBufferToBackBuffer::update()
	{
		if (mRenderMode->update())
		{
			//レンダーモード=shaderが切り替わったので更新
			createShader();
		}
	}

	void GBufferToBackBuffer::draw()
	{
		//シェーダーのセット
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//テクスチャセット
		auto depth = RENDER->getRTTManager()->getRTTSRV(Koban::RTTManager::DEPTH);
		auto col = RENDER->getRTTManager()->getRTTSRV(Koban::RTTManager::COLOR);
		auto normal = RENDER->getRTTManager()->getRTTSRV(Koban::RTTManager::NORMAL);
		auto pos = RENDER->getRTTManager()->getRTTSRV(Koban::RTTManager::POSITION);
		DEVICE_CONTEXT->PSSetShaderResources(1, 1, &depth);
		DEVICE_CONTEXT->PSSetShaderResources(2, 1, &col);
		DEVICE_CONTEXT->PSSetShaderResources(3, 1, &normal);
		DEVICE_CONTEXT->PSSetShaderResources(4, 1, &pos);
		//コンスタントバッファの登録
		RENDER->setGlobalConstantBuffer();
		//トポロジーのセット
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//頂点バッファのセット
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//バックバッファにDraw!
		DEVICE_CONTEXT->Draw(4, 0);
	}

	void GBufferToBackBuffer::destroy()
	{
		SAFE_RELEASE(mpVertexShader);
		SAFE_RELEASE(mpPixelShader);
	}

	void GBufferToBackBuffer::createShader()
	{
		//使用するShaderの作成
		RenderUtil::createShader(
			DEVICE,
			mRenderMode->getShaderFileName_VS(),
			mRenderMode->getShaderFileName_PS(),
			&mpVertexShader,
			&mpPixelShader
		);
	}
}