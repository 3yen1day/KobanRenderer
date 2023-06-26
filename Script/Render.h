#pragma once
using namespace std;

//RenderTarget
#include "TestMesh.h"
#include "Include/RenderInclude.h"

//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME L"KOBAN_RENDERER"

namespace Koban {
	class Render
	{
	public:
		Render(HWND* pHWnd);

		//void Awake(); //�G���g���[�|�C���g�ŌĂ�
		//void Update();
		void Draw(); //���t���X�V
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
		//�J�����n��Matrix�͑S���܂Ƃ߂�
		TestMesh* mpMesh;
		static ID3D11Device* mpDevice;
		static ID3D11DeviceContext* mpDeviceContext;
	};
}
