using namespace std;

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <string>
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#include"Render.h"

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME L"KOBAN_RENDERER"


class MAIN
{
public:
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	void Awake();
	void Start();
	void Loop();
	void Update();
	void Draw();
	void Destroy();

	HWND mHwnd;

private:
	Render* mpRender;
};
