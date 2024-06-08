#include "Render.h"
#include "RTTManager.h"
#include "Camera.h"
#include "Light.h"
#include "Render3DModel.h"
#include "DefferdRendering.h"

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Koban::Render::mpDevice;
ID3D11DeviceContext* Koban::Render::mpDeviceContext;
IDXGISwapChain* Koban::Render::mpSwapChain;
ID3D11RenderTargetView* Koban::Render::mpBackBuffer_RTV;
std::unique_ptr<Koban::Camera> Koban::Render::mpCamera;
std::unique_ptr<Koban::Light> Koban::Render::mpLight;
std::unique_ptr<Koban::RTTManager> Koban::Render::mpRTTManager;

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
			DebugLib::error(L"デバイス作成失敗");
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
		Koban::Render::getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
		DEVICE->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_RTV);
		SAFE_RELEASE(pBackBuffer_Tex);

		//Objectの作成
		createObjects();
	}

	Render::~Render() = default; // デストラクタを非インライン化

	/// <summary>
	/// Objectの作成
	/// </summary>
	void Render::createObjects()
	{
		//RTTを作成するくん
		mpRTTManager.reset(new RTTManager());
		//Cameraの作成
		mpCamera.reset(new Camera());
		//シンプルな四角形描画
		mpRender3DModel.reset(new Render3DModel());
		//GBufferを元に描画
		mpDefferdShader.reset(new DefferdShader());
	}

	void Render::update() {
		mpCamera->update();
		mpRTTManager->update();
	}

	void Render::draw() {
		//画面クリア（実際は単色で画面を塗りつぶす処理）
		float ClearColor[4] = { 0,0,0.5,1 };// クリア色作成　RGBAの順
		mpDeviceContext->ClearRenderTargetView(mpBackBuffer_RTV, ClearColor);//画面クリア
		
		mpRender3DModel->draw();
		mpDefferdShader->draw();
		////画面更新（バックバッファをフロントバッファに）
		mpSwapChain->Present(0, 0);
	}

	void Render::destroy()
	{
		mpRTTManager->destroy();
		mpDefferdShader->destroy();

		//リソース所有権の破棄
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpDevice);
		SAFE_RELEASE(mpDeviceContext);
		SAFE_RELEASE(mpBackBuffer_RTV);
	}
}