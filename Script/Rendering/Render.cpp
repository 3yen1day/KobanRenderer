#include "Render.h"
#include "RTTManager.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"
#include "../Component/Mesh.h"
#include "GBufferToBackBuffer.h"

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Koban::Render::mpDevice;
ID3D11DeviceContext* Koban::Render::mpDeviceContext;
IDXGISwapChain* Koban::Render::mpSwapChain;
ID3D11RenderTargetView* Koban::Render::mpBackBuffer_RTV;
std::unique_ptr<Koban::Camera> Koban::Render::mpCamera;
std::unique_ptr<Koban::Light> Koban::Render::mpLight;
std::unique_ptr<Koban::RTTManager> Koban::Render::mpRTTManager;
std::unique_ptr<Koban::GBufferToBackBuffer> Koban::Render::mpGBufferToBackBuffer;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pHWnd">ウィンドウのハンドラ</param>
namespace Koban {
	Render::Render(HWND* pHWnd) :
		mHwnd(pHWnd)
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
		sd.OutputWindow = *mHwnd;
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

		//バックバッファ
		//バックバッファーテクスチャーを取得（既にあるので作成ではない）
		mpBackBuffer_RTV = nullptr;
		ID3D11Texture2D* pBackBuffer_Tex;
		mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
		DEVICE->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_RTV);
		SAFE_RELEASE(pBackBuffer_Tex);

		//Objectの作成
		createObjects();
	}

	Render::~Render() {};// = default; // デストラクタを非インライン化

	/// <summary>
	/// Objectの作成
	/// </summary>
	void Render::createObjects()
	{
		//RTTを作成するくん
		mpRTTManager.reset(new RTTManager());
		//Cameraの作成
		mpCamera.reset(new Camera());
		//Lightの作成
		mpLight.reset(new Light());
		//3Dモデル描画
		mpMesh.reset(new Mesh());
		//GBufferを元に描画
		mpGBufferToBackBuffer.reset(new GBufferToBackBuffer());
	}

	void Render::update() {
		mpCamera->update();
		mpRTTManager->update();
	}

	void Render::draw() {
		//GBufferへの描画
		mpMesh->draw();

		//レンダーターゲットを通常に戻す
		DEVICE_CONTEXT->OMSetRenderTargets(1, &mpBackBuffer_RTV, mpRTTManager->getDepthStensilSRV());
		//クリア
		float ClearColor[4] = { 0,1,0,1 };
		DEVICE_CONTEXT->ClearRenderTargetView(mpBackBuffer_RTV, ClearColor);

		//GBufferから描画
		mpGBufferToBackBuffer->draw();
		////画面更新（バックバッファをフロントバッファに）
		mpSwapChain->Present(0, 0);
	}

	void Render::destroy()
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