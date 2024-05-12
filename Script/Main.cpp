#include "Main.h"

//�O���[�o���ϐ�
Main* g_pMain = NULL;
//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#pragma region ���C������
//�A�v���P�[�V�����̃G���g���[�֐� 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	g_pMain = new Main();
	if (g_pMain != NULL)
	{
		if (SUCCEEDED(g_pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH,
			WINDOW_HEIGHT, APP_NAME)))
		{
			//������
			g_pMain->Awake();
			//todo:�K�v�Ȃ�Start������
			//���[�v
			g_pMain->Loop();
		}
		//�A�v���I��
		g_pMain->destroy();
		delete g_pMain;
	}
	return 0;
}


void Main::Awake() {
	mpRender.reset(new Koban::Render(&mHwnd));
}


void Main::Start() {
}

//���b�Z�[�W���[�v�ƃA�v���P�[�V���������̓����
void Main::Loop()
{

	//���b�V���쐬�@�I���

	 // ���b�Z�[�W���[�v
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	
	// start
	//mpRender->start();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�X�V
			update();
			//�`��
			draw();
		}
	}
	//�A�v���P�[�V�����̏I��
}

void Main::update()
{
	mpRender->update();
}

void Main::draw()
{
	mpRender->draw();
}

void Main::destroy()
{
}
#pragma endregion ���C������



//OS���猩���E�B���h�E�v���V�[�W���[�i���ۂ̏�����MAIN�N���X�̃v���V�[�W���[�ŏ����j
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}


//�E�B���h�E�쐬
HRESULT Main::InitWindow(HINSTANCE hInstance,
	INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	// �E�B���h�E�̒�`
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

	//�E�B���h�E�̍쐬
	mHwnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!mHwnd)
	{
		return E_FAIL;
	}
	//�E�C���h�E�̕\��
	ShowWindow(mHwnd, SW_SHOW);
	UpdateWindow(mHwnd);

	return S_OK;
}


//�E�B���h�E�v���V�[�W���[
LRESULT Main::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESC�L�[�ŏC��
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