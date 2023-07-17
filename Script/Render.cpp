#include "Render.h"

//対象のRenderObject
#include "TestDefferdRender.h"

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Koban::Render::mpDevice;
ID3D11DeviceContext* Koban::Render::mpDeviceContext;
Koban::Camera* Koban::Render::mpCamera;
Koban::RTTManager* Koban::Render::mpRTTManager;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pHWnd">ウィンドウのハンドラ</param>
namespace Koban {
	Render::Render(HWND* pHWnd) :
		mHwnd(pHWnd),
		mpSwapChain(nullptr),
		mpBackBuffer_TexRTV(nullptr),
		mpDepthStencilView(nullptr),
		mpDephStencilTex(nullptr)
	{
		mpDevice = nullptr;
		mpDeviceContext = nullptr;
		mpRTTManager = nullptr;
		mpCamera = nullptr;

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

		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
			0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &mpSwapChain, &mpDevice,
			pFeatureLevel, &mpDeviceContext)))
		{
			//なんか例外
			return;
		}
		//各種テクスチャーと、それに付帯する各種ビューを作成

		//バックバッファーテクスチャーを取得（既にあるので作成ではない）
		ID3D11Texture2D* pBackBuffer_Tex;
		mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
		//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
		mpDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_TexRTV);
		SAFE_RELEASE(pBackBuffer_Tex);

		//デプスステンシルビュー用のテクスチャーを作成
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = WINDOW_WIDTH;
		descDepth.Height = WINDOW_HEIGHT;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		mpDevice->CreateTexture2D(&descDepth, NULL, &mpDephStencilTex);
		//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
		mpDevice->CreateDepthStencilView(mpDephStencilTex, NULL, &mpDepthStencilView);

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

		//RenderObject
		CreateObjects();
	}

	/// <summary>
	/// Objectの作成
	/// </summary>
	void Render::CreateObjects()
	{
		//Cameraの作成
		mpCamera = new Camera();

		//RTTを作成するくん
		mpRTTManager = new RTTManager();

		//RenderObjectの初期化
		mpRenderObjects = { new TestDefferdRender() };
	}

	void Render::Destroy()
	{
		mpRTTManager->destroy();
		for (const auto e : mpRenderObjects) {
			e->destroy();
		}

		//リソースのデリート
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpBackBuffer_TexRTV);
		SAFE_RELEASE(mpDepthStencilView);
		SAFE_RELEASE(mpDephStencilTex);
		SAFE_RELEASE(mpDevice);
	}

	void Render::Update() {
		mpCamera->update();

		for (const auto e : mpRenderObjects) {
			e->update();
		}
	}

	//シーンを画面にレンダリング
	void Render::Draw()
	{
		//画面クリア（実際は単色で画面を塗りつぶす処理）
		float ClearColor[4] = { 0,0,1,1 };// クリア色作成　RGBAの順
		mpDeviceContext->ClearRenderTargetView(mpBackBuffer_TexRTV, ClearColor);//画面クリア
		mpDeviceContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア
		
		//カメラの更新
		mpCamera->update();

		//レンダリング
		// GBufferのレンダリング
		//mpRTTManager->Render();

		// バックバッファをRTに設定
		// レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
		mpDeviceContext->OMSetRenderTargets(1, &mpBackBuffer_TexRTV, mpDepthStencilView);

		//RenderObjectの更新
		//todo:イベントに登録されたupdateを呼ぶ形に変更
		//mpMesh->Render(mViewMat, mProjMat, D3DXVECTOR3(1, 1, -1), mPosition);
		for (const auto e : mpRenderObjects) {
			e->draw();
		}

		//画面更新（バックバッファをフロントバッファに）
		mpSwapChain->Present(0, 0);
	}
}