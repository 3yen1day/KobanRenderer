#include "Render.h"
#include "RTTManager.h"
#include "Camera.h"

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Koban::Render::mpDevice;
ID3D11DeviceContext* Koban::Render::mpDeviceContext;
IDXGISwapChain* Koban::Render::mpSwapChain;
ID3D11RenderTargetView* Koban::Render::mpBackBuffer_RTV;
std::unique_ptr<Koban::Camera> Koban::Render::mpCamera;
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

		//RenderObjectの初期化
		//mpRenderObjects = { new TestDefferdRender() };
	}

	void Render::destroy()
	{
		mpRTTManager->destroy();

		//unique_ptrはコピー不可なので、&をつけて参照型にする必要がある
		for (const auto& e : mpRenderObjects) {
			e->destroy();
		}

		//リソース所有権の破棄
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpDevice);
		SAFE_RELEASE(mpDeviceContext);
		SAFE_RELEASE(mpBackBuffer_RTV);
	}

	void Render::update() {
		//カメラ更新
		mpCamera->update();

		for (const auto& e : mpRenderObjects) {
			e->update();
		}
	}

	//シーンを画面にレンダリング
	void Render::draw()
	{
		//ディファードレンダリング
		drawDefferd();
	}

	void Render::drawDefferd() {
		//RenderObjectの更新
		//mpMesh->Render(mViewMat, mProjMat, D3DXVECTOR3(1, 1, -1), mPosition);
		for (const auto& e : mpRenderObjects) {
			e->draw();
		}

		//// 描画されたRTTをもとに絵を描く
		//// GBufferのクリアとセット
		//mpRTTManager->renderToScene();

		////画面更新（バックバッファをフロントバッファに）
		//mpSwapChain->Present(0, 0);
	}

#pragma region static関数
	bool Render::createVertexShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11VertexShader* vs) {
		//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
		std::unique_ptr<ID3D10Blob> upCompiledShader = NULL;
		std::unique_ptr<ID3D10Blob> upErrors = NULL;
		ID3D10Blob* pCompiledShader = upCompiledShader.get();
		ID3D10Blob* pErrors = upErrors.get();
		auto shaderName_s = StringLib::wstr2str(shaderName);
		
		//ブロブからバーテックスシェーダー作成
		if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return false;
		}
		if (FAILED(mpDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &vs)))
		{
			MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
			return false;
		}

		return true;
	}

	bool Render::createPixelShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11PixelShader* ps) {
		//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
		std::unique_ptr<ID3D10Blob> upCompiledShader = NULL;
		std::unique_ptr<ID3D10Blob> upErrors = NULL;
		ID3D10Blob* pCompiledShader = upCompiledShader.get();
		ID3D10Blob* pErrors = upErrors.get();
		auto shaderName_s = StringLib::wstr2str(shaderName);

		if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return false;
		}

		if (FAILED(mpDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &ps)))
		{
			MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
			return false;
		}
		return true;
	}
#pragma endregion
}