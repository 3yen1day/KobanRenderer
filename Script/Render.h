#pragma once
using namespace std;

#include "Camera.h"
#include "RTTManager.h"
#include "Include/RenderInclude.h"
#include "RenderObject.h"

namespace Koban {
	class Render
	{
	public:
		Render(HWND* pHWnd);

		//void Awake(); //エントリーポイントで呼ぶ
		void Update();
		void Draw(); //毎フレ更新
		void Destroy();
		void CreateObjects();

		HWND* mHwnd;
		IDXGISwapChain* mpSwapChain;
		ID3D11RenderTargetView* mpBackBuffer_TexRTV;
		ID3D11Texture2D* mpDephStencilTex;
		ID3D11DepthStencilView* mpDepthStencilView;

		static ID3D11Device* getDevice()
		{
			return mpDevice;
		}

		static ID3D11DeviceContext* getDeviceContext()
		{
			return mpDeviceContext;
		}

		static RTTManager* getRTTManager() {
			return mpRTTManager;
		}

		static Camera* getCamera()
		{
			return mpCamera;
		}

	private:
		static ID3D11Device* mpDevice;
		static ID3D11DeviceContext* mpDeviceContext;
		static RTTManager* mpRTTManager;
		static Camera* mpCamera;

		vector<RenderObject*> mpRenderObjects;
	};
}
