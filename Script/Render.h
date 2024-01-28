#pragma once

namespace Koban {
	class RTTManager;
	class Camera;
	class RenderObject;
}

namespace Koban {
	class Render
	{
	public:
		Render(HWND* pHWnd);

		//void Awake(); //エントリーポイントで呼ぶ
		void update();
		void draw(); //毎フレ更新
		void destroy();
		void createObjects();

		HWND* mHwnd;

		static ID3D11Device* getDevice()
		{
			return mpDevice;
		}

		static ID3D11DeviceContext* getDeviceContext()
		{
			return mpDeviceContext;
		}

		static IDXGISwapChain* getSwapChain()
		{
			return mpSwapChain;
		}

		static ID3D11RenderTargetView* getBackBuffer() {
			return mpBackBuffer_RTV;
		}

		static RTTManager* getRTTManager() {
			return mpRTTManager;
		}

		static Camera* getCamera()
		{
			return mpCamera;
		}

		static bool createVertexShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11VertexShader* vs);

		static bool createPixelShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11PixelShader* ps);


	private:
		void drawDefferd();

		static ID3D11Device* mpDevice;
		static ID3D11DeviceContext* mpDeviceContext;
		static IDXGISwapChain* mpSwapChain;
		static ID3D11RenderTargetView* mpBackBuffer_RTV;

		static RTTManager* mpRTTManager;
		static Camera* mpCamera;

		vector<RenderObject*> mpRenderObjects;
	};
}
