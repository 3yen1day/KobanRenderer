using namespace std;

//ヘッダーファイルのインクルード
#include <string>
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#include "MESH.h"

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME L"OBJからスタティックメッシュ 完全版"

//
//
//
class MAIN
{
public:
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);
	HRESULT InitD3D();
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	void Loop();
	void App();
	void Render();
	void DestroyD3D();

	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;

	MESH* m_pMesh;

private:
	//モデルのリソースパス
	LPCSTR MODEL_PATH = "Resource/Chips.obj"; //LPSTRのconst版
};
