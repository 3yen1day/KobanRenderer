#include "Direct3DManager.h"
#include <windows.h>
#include <fstream>
#include "../framework.h"

Direct3DManager* Direct3DManager::mInstance = nullptr;

bool Direct3DManager::Initialize(HWND hWnd, int width, int height) {
	//=====================================================
	// ファクトリー作成(ビデオ グラフィックの設定の列挙や指定に使用されるオブジェクト)
	//=====================================================
	ComPtr<IDXGIFactory> factory;
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
		return false;
	}

	//=====================================================
	//デバイス生成(主にリソース作成時に使用するオブジェクト)
	//=====================================================
	UINT creationFlags = 0;

#ifdef _DEBUG
	// DEBUGビルド時はDirect3Dのデバッグを有効にする
	// (すごく重いが細かいエラーがわかる)
	creationFlags = !D3D11_CREATE_DEVICE_DEBUG;
#endif

	//FeatureLevel：ビデオカードのDXのレベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};
	D3D_FEATURE_LEVEL featureLevel;

	//デバイスとデバイスコンテキストの作成
	if (FAILED(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&mDevice,
		&featureLevel,
		&mDeviceContext)))
	{
		return false;
	}

	//=====================================================
	// スワップチェイン作成(フロントバッファに表示可能なバックバッファを持つもの)
	//=====================================================
	DXGI_SWAP_CHAIN_DESC scDesc = {};		// スワップチェーンの設定データ
	scDesc.BufferDesc.Width = width;						// 画面の幅
	scDesc.BufferDesc.Height = height;						// 画面の高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バッファの形式
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.SampleDesc.Count = 1;							// MSAAは使用しない
	scDesc.SampleDesc.Quality = 0;							// MSAAは使用しない
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バッファの使用方法 //?バックバッファではなくて？
	scDesc.BufferCount = 2;									// バッファの数
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = TRUE;									// ウィンドウモード
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//スワップチェインの作成
	if (FAILED(factory->CreateSwapChain(mDevice.Get(), &scDesc, &mSwapChain)))
	{
		return false;
	}

	// スワップチェインからバックバッファリソース取得
	ComPtr<ID3D11Texture2D> backBufferPtr;
	if (FAILED(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferPtr))))
	{
		return false;
	}

	// バックバッファリソース用のRTVを作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = scDesc.BufferDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	if (FAILED(mDevice->CreateRenderTargetView(backBufferPtr.Get(), &rtvDesc, &mBackBufferView)))
	{
		return false;
	}

	//=====================================================
	// デバイスコンテキストに描画に関する設定を行っておく
	//=====================================================
	// バックバッファをRTとしてセット
	// デバイスコンテキストの描画先をバックバッファ用のＲＴに
	mDeviceContext->OMSetRenderTargets(1, mBackBufferView.GetAddressOf(), nullptr);

	// ビューポートの設定
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	mDeviceContext->RSSetViewports(1, &vp);

	//=====================================================
	// シェーダーの作成
	//=====================================================
	
	std::ifstream ifs(L"Shader/SpriteShader.hlsl");
	if (!ifs.is_open()) {
		return false;
	}

	//頂点シェーダーの読み込み・コンパイル
	ComPtr<ID3DBlob> compiledVS;
	ComPtr<ID3DBlob> pErrorBlob = NULL;
	D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &compiledVS, &pErrorBlob);
	//OutputDebugStringA(static_cast<char*>(pErrorBlob->GetBufferPointer()));
	//printf((char*)pErrorBlob->GetBufferPointer());
	//MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), TEXT(""), 0);
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &compiledVS, nullptr))) {
		return false; //shader読めてないじゃんー！
	}
	
	//ピクセルシェーダーの読み込み・コンパイル
	ComPtr<ID3DBlob> compiledPS;
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &compiledPS, nullptr)))
	{
		return false;
	}

	//頂点シェーダー作成
	if (FAILED(mDevice->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &mSpriteVS)))
	{
		return false;
	}

	//ピクセルシェーダー作成
	if (FAILED(mDevice->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &mSpritePS)))
	{
		return false;
	}

	//1頂点の情報
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//頂点インプットレイアウト作成
	if (FAILED(mDevice->CreateInputLayout(&layout[0], layout.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &mSpriteInputLayout)))
	{
		return false;
	}

	return true;
}