#pragma once

namespace Koban {
	class RTTManager;
	class Camera;
	class Light;
	class Render3DModel;
	class DefferdShader;
}

namespace Koban {
	class Render
	{
	public:
		Render(HWND* pHWnd);
		~Render();

		//void Awake(); //エントリーポイントで呼ぶ
		//void start();
		void update(); //毎フレ更新
		void draw();
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

		static ID3D11RenderTargetView* getBackBuffer() {
			return mpBackBuffer_RTV;
		}

		static RTTManager* const getRTTManager() {
			return mpRTTManager.get();
		}

		static Camera* getCamera()
		{
			return mpCamera.get();
		}

		static Light* getLight()
		{
			return mpLight.get();
		}

		static bool createVertexShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11VertexShader* vs);

		static bool createPixelShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11PixelShader* ps);


	private:
		//アプリに一つ必要
		//unique_ptrを使うと解放時に例外
		static ID3D11Device* mpDevice;
		static ID3D11DeviceContext* mpDeviceContext;
		static IDXGISwapChain* mpSwapChain;
		static ID3D11RenderTargetView* mpBackBuffer_RTV;

		static std::unique_ptr<RTTManager> mpRTTManager;
		static std::unique_ptr<Camera> mpCamera;
		static std::unique_ptr<Light> mpLight;

		std::unique_ptr<DefferdShader> mpDefferdShader;
		std::unique_ptr<Render3DModel> mpRender3DModel;
	};
}
