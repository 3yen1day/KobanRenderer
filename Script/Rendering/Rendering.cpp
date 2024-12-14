#include "Rendering.h"
#include "RTTManager.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "GBufferToBackBuffer.h"
#include "../Core/Scene.h"
#include <d3d11.h>

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pHWnd">ウィンドウのハンドラ</param>
namespace Koban {
	Rendering::Rendering(HWND* pHWnd)
	{
		// デバイスとスワップチェーンの作成
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = WINDOW_WIDTH;
		sd.BufferDesc.Height = WINDOW_HEIGHT;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = *pHWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

		mpSwapChain = nullptr;
		mpDevice = nullptr;
		mpDeviceContext = nullptr;
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
			0, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &mpSwapChain, &mpDevice,
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

		//バックバッファ
		//バックバッファーテクスチャーを取得（既にあるので作成ではない）
		mpBackBuffer_RTV = nullptr;
		ID3D11Texture2D* pBackBuffer_Tex;
		mpSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer_Tex));
		mpDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_RTV);
		SAFE_RELEASE(pBackBuffer_Tex);

		//RTTを管理
		mpRTTManager = std::make_unique<RTTManager>();
		//GBufferを元に描画
		mpGBufferToBackBuffer = std::make_unique<GBufferToBackBuffer>();
	}

	Rendering::~Rendering() {};// = default; // デストラクタを非インライン化

	void Rendering::start()
	{
		mpRTTManager->start();
		mpGBufferToBackBuffer->start();

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
		std::vector<Mesh*> meshes = SCENE->findComponents<Mesh>();
		if (!meshes.empty()) {
			mpMesh = meshes[0];
		}

		mpRTTManager->update();
	}

	void Rendering::draw() {
		//GBufferへの描画
		mpMesh->draw();

		//レンダーターゲットを通常に戻す
		DEVICE_CONTEXT->OMSetRenderTargets(1, &mpBackBuffer_RTV, mpRTTManager->getDepthStensilSRV());
		//クリア
		float ClearColor[4] = { 0.97, 0.71, 0, 1 };
		DEVICE_CONTEXT->ClearRenderTargetView(mpBackBuffer_RTV, ClearColor);

		//GBufferから描画
		mpGBufferToBackBuffer->draw();
	}

	void Rendering::destroy()
	{
		mpRTTManager->destroy();
		//mpRender3DModel->
		mpGBufferToBackBuffer->destroy();

		//リソース所有権の破棄
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpDevice);
		SAFE_RELEASE(mpDeviceContext);
		SAFE_RELEASE(mpBackBuffer_RTV);
	}
}