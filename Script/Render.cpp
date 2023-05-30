#include "Render.h"

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pHWnd">ウィンドウのハンドラ</param>
Render::Render(HWND * pHWnd):
	mHwnd(pHWnd), 
	mpDevice(nullptr), 
	mpDeviceContext(nullptr), 
	mpSwapChain(nullptr),
	mpBackBuffer_TexRTV(nullptr),
	mpBackBuffer_DSTexDSV(nullptr),
	mpBackBuffer_DSTex(nullptr),
	mpMesh(nullptr)
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
	mpDevice->CreateTexture2D(&descDepth, NULL, &mpBackBuffer_DSTex);
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	mpDevice->CreateDepthStencilView(mpBackBuffer_DSTex, NULL, &mpBackBuffer_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	mpDeviceContext->OMSetRenderTargets(1, &mpBackBuffer_TexRTV, mpBackBuffer_DSTexDSV);
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

	CreateRenderObject();
}

/// <summary>
/// RenderObjectの作成
/// </summary>
void Render::CreateRenderObject()
{
	//RenderObjectの初期化
	//メッシュ作成
	mpMesh = new TestMesh;
	if (FAILED(mpMesh->Init(mpDevice, mpDeviceContext)))
	{
		//なんか例外
		return;
	}
}

void Render::Destroy()
{
	//todo:登録したポインタをデリート
	SAFE_DELETE(mpMesh);

	//リソースのデリート
	SAFE_RELEASE(mpSwapChain);
	SAFE_RELEASE(mpBackBuffer_TexRTV);
	SAFE_RELEASE(mpBackBuffer_DSTexDSV);
	SAFE_RELEASE(mpBackBuffer_DSTex);
	SAFE_RELEASE(mpDevice);
}


//シーンを画面にレンダリング
void Render::Draw()
{
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,1,1 };// クリア色作成　RGBAの順
	mpDeviceContext->ClearRenderTargetView(mpBackBuffer_TexRTV, ClearColor);//画面クリア
	mpDeviceContext->ClearDepthStencilView(mpBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア
	// ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -0.5f); //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	// プロジェクショントランスフォーム（射影変換）
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);

	//レンダリング
	//RenderObjectの更新
	mpMesh->Render(mView, mProj, D3DXVECTOR3(1, 1, -1), vEyePt);
	//画面更新（バックバッファをフロントバッファに）
	mpSwapChain->Present(0, 0);
}