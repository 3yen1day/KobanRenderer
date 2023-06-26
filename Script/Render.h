#pragma once
using namespace std;

//RenderTarget
#include "TestMesh.h"
#include "Include/RenderInclude.h"

//定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME L"KOBAN_RENDERER"

namespace Koban {
	class Render
	{
	public:
		Render(HWND* pHWnd);

		//void Awake(); //エントリーポイントで呼ぶ
		//void Update();
		void Draw(); //毎フレ更新
		void Destroy();
		void CreateRenderObject();

		HWND* mHwnd;
		IDXGISwapChain* mpSwapChain;
		ID3D11RenderTargetView* mpBackBuffer_TexRTV;
		ID3D11DepthStencilView* mpBackBuffer_DSTexDSV;
		ID3D11Texture2D* mpBackBuffer_DSTex;

		static ID3D11Device* getDevice()
		{
			return mpDevice;
		}

		static ID3D11DeviceContext* getDeviceContext()
		{
			return mpDeviceContext;
		}

	private:
		//カメラ系のMatrixは全部まとめる
		TestMesh* mpMesh;
		static ID3D11Device* mpDevice;
		static ID3D11DeviceContext* mpDeviceContext;
	};
}
