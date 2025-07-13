#include "Rendering.h"
#include "RTTManager.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "GBufferToBackBuffer.h"
#include "DebugDraw.h"
#include "../Core/Transform.h"
#include "../Core/Scene.h"
#include "../Utillity/RenderUtil.h"
#include <d3d11.h>
#include <tchar.h>

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pHWnd">ウィンドウのハンドラ</param>
namespace Koban {
	Rendering::Rendering(HWND& hwnd)
	{
		// デバイスとスワップチェーンの作成
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = WINDOW_WIDTH;
		sd.BufferDesc.Height = WINDOW_HEIGHT;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

		mpSwapChain = nullptr;
		mpDevice = nullptr;
		mpDeviceContext = nullptr;
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
			0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &mpSwapChain, &mpDevice,
			pFeatureLevel, &mpDeviceContext)))
		{
			//なんか例外
			DebugUtil::error(L"デバイス作成失敗");
			return;
		}

		//ビューポートの設定
		D3D11_VIEWPORT vp;
		vp.Width = WINDOW_WIDTH;
		vp.Height = WINDOW_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		mpDeviceContext->RSSetViewports(1, &vp);
		//ラスタライズ設定
		D3D11_RASTERIZER_DESC rdc;
		ZeroMemory(&rdc, sizeof(rdc));
		rdc.CullMode = D3D11_CULL_NONE;
		rdc.FillMode = D3D11_FILL_SOLID;
		ID3D11RasterizerState* pIr = NULL;
		mpDevice->CreateRasterizerState(&rdc, &pIr);
		mpDeviceContext->RSSetState(pIr);
		SAFE_RELEASE(pIr);

		//バックバッファの作成
		mpBackBuffer_RTV = nullptr;
		createBackBuffer_RTV();

		//コンスタントバッファの作成
		RenderUtil::createConstantBuffer<CONSTANT_BUFER_GLOBAL>(mpDevice, &mpConstantBuffer_Global);

		//RTTを管理する
		mpRTTManager = std::make_unique<RTTManager>();
		//GBufferを元した描画を管理する
		mpGBufferToBackBuffer = std::make_unique<GBufferToBackBuffer>();
		mpDebugDraw = std::make_unique<DebugDraw>();
	}

	Rendering::~Rendering() {};// = default; // デストラクタを非インライン化

	void Rendering::start()
	{
		mpRTTManager->start();
		mpGBufferToBackBuffer->start();
		mpDebugDraw->start();

		SCENE->doStart<Mesh>();
	}

	void Rendering::update() {
		//コンポーネントの更新
		SCENE->doUpdate<Camera>();
		std::vector<Camera*> cameras = SCENE->findComponents<Camera>();
		if (!cameras.empty()) {
			mpMainCamera = cameras[0];
		}

		std::vector<Light*> lights = SCENE->findComponents<Light>();
		if (!lights.empty()) {
			mpLight = lights[0];
		}

		SCENE->doUpdate<Mesh>();

		mpRTTManager->update();
		mpGBufferToBackBuffer->update();
	}

	void Rendering::draw() {
		//GBufferへの描画
		SCENE->doDraw<Mesh>();

		//レンダーターゲットを通常に戻す
		DEVICE_CONTEXT->OMSetRenderTargets(1, &mpBackBuffer_RTV, nullptr);

		//クリア
		DEVICE_CONTEXT->ClearRenderTargetView(mpBackBuffer_RTV, ClearColor);

		//GBufferから描画
		mpGBufferToBackBuffer->draw();
		
		//デバッグ表示
		mpDebugDraw->draw();
	}

	void Rendering::destroy()
	{
		mpRTTManager->destroy();
		mpGBufferToBackBuffer->destroy();
		mpDebugDraw->destroy();

		//リソース所有権の破棄
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpDevice);
		SAFE_RELEASE(mpDeviceContext);
		SAFE_RELEASE(mpBackBuffer_RTV);
	}

	void Rendering::createBackBuffer_RTV() {
		//バックバッファーテクスチャーを取得（既にあるので作成ではない）
		ID3D11Texture2D* pBackBuffer_Tex;
		mpSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer_Tex));
		mpDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_RTV);
		SAFE_RELEASE(pBackBuffer_Tex);
	}

	/// <summary>
	/// 全shader共通のコンスタントバッファをセット（shader登録後に必ず呼ぶ）
	/// </summary>
	void Rendering::setGlobalConstantBuffer()
	{	
		D3D11_MAPPED_SUBRESOURCE pData_default;
		CONSTANT_BUFER_GLOBAL cb;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Global, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_default)))//pData_default.pDataにm_pConstantBufferのアドレス
		{
			//ライトの位置を渡す
			auto lightDir = RENDER->getLight()->getDirection();
			cb.vLightDir = D3DXVECTOR4(lightDir.x, lightDir.y, lightDir.z, 1);
			//視点位置を渡す
			cb.vEyePos = D3DXVECTOR4(RENDER->getCamera()->getPostion(), 0);
			if (memcpy_s(pData_default.pData, sizeof(CONSTANT_BUFER_GLOBAL), (void*)(&cb), sizeof(cb)))
			{
				DebugUtil::error(L"memCopy時にエラー");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Global, 0);

			DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer_Global);
			DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer_Global);
		}
	}
}
