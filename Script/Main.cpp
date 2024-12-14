#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_win32.h"
#include "../lib/imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "Main.h"
#include "Core/GlobalAccess.h"
#include "Rendering/Rendering.h"
#include "Core/Scene.h"
#include "GUI/GUI.h"

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//グローバル変数
Main* g_pMain = NULL;
UINT g_ResizeWidth = 0, g_ResizeHeight = 0;

// Main code
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	g_pMain = new Main();

	if (SUCCEEDED(g_pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
	{
		// メッセージループ
		MSG msg = { 0 };
		ZeroMemory(&msg, sizeof(msg));

		g_pMain->awake();
		g_pMain->start();
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					break;
			}

			//ウィンドウサイズの変更（TODO：上手くいかないので、あとで直す）
			if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
			{
				RENDER->resizeWindow(g_ResizeWidth, g_ResizeHeight);
				g_ResizeWidth = g_ResizeHeight = 0;
			}

			//更新
			g_pMain->update();
			//描画
			g_pMain->draw();
		}
		//アプリケーションの終了
		g_pMain->destroy();
	}
	delete g_pMain;
	return 0;
}

//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}


//ウィンドウ作成
HRESULT Main::InitWindow(HINSTANCE hInstance,
	INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	// ウィンドウの定義
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ::WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc); //todo:wc解放しないといけないらしい

	//ウィンドウの作成
	mHwnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!mHwnd)
	{
		return E_FAIL;
	}
	//ウインドウの表示
	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);

	return S_OK;
}

LRESULT Main::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESCキーで修了
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void Main::awake() {
	//初期化
	mpGlobalAccess = std::make_unique<Koban::GlobalAccess>(&mHwnd);
}

void Main::start()
{
	SCENE->start();
	RENDER->start();
	GA::getGUI()->start();
}

void Main::update()
{
	RENDER->update();
}

void Main::draw()
{
	RENDER->draw();
	GA::getGUI()->draw();
	HRESULT hr = RENDER->getSwapChain()->Present(1, 0); // Present with vsync
}

void Main::destroy()
{
	SCENE->destroy();
	RENDER->destroy();
	GA::getGUI()->destroy();

	DestroyWindow(mHwnd);
	//::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}