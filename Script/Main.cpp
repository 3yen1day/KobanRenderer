#include "Main.h"
#include "Core/GlobalAccess.h"
#include "Rendering/Rendering.h"
#include "Core/Scene.h"
#include "GUI/GUI.h"

#include "../../lib/imgui/imgui.h"

//グローバル変数
Main* g_pMain = NULL;
//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma region メイン処理
//アプリケーションのエントリー関数 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	g_pMain = new Main();
	if (g_pMain != NULL)
	{
		if (SUCCEEDED(g_pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH,
			WINDOW_HEIGHT, APP_NAME)))
		{
			//初期化
			g_pMain->Awake();
			//todo:必要ならStartを入れる
			//ループ
			g_pMain->Loop();
		}
		//アプリ終了
		g_pMain->destroy();
		delete g_pMain;
	}
	return 0;
}


void Main::Awake() {
	//初期化
	mpGlobalAccess = std::make_unique<Koban::GlobalAccess>(&mHwnd);
}

//メッセージループとアプリケーション処理の入り口
void Main::Loop()
{
	 // メッセージループ
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	
	// start
	start();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//更新
			update();
			//描画
			draw();
		}
	}
	//アプリケーションの終了
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
	GA::getGUI()->update();
}

void Main::draw()
{
	RENDER->draw();
	GA::getGUI()->draw();
}

void Main::destroy()
{
	SCENE->destroy();
	RENDER->destroy();
	GA::getGUI()->destroy();
}
#pragma endregion メイン処理



//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//imguiのウィンドウプロシージャ
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}


//ウィンドウ作成
HRESULT Main::InitWindow(HINSTANCE hInstance,
	INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	// ウィンドウの定義
	WNDCLASSEX  wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

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


//ウィンドウプロシージャー
LRESULT Main::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
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