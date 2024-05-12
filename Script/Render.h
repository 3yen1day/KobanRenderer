#pragma once

namespace Koban {
	class RTTManager;
	class Camera;
	class Light;
	class SimpleSquare;
}

namespace Koban {
	class Render
	{
	public:
		Render(HWND* pHWnd);
		~Render();

		//void Awake(); //�G���g���[�|�C���g�ŌĂ�
		//void start();
		void update(); //���t���X�V
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

		static IDXGISwapChain* getSwapChain()
		{
			return mpSwapChain;
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
		//�A�v���Ɉ�K�v
		//unique_ptr���g���Ɖ�����ɗ�O
		static ID3D11Device* mpDevice;
		static ID3D11DeviceContext* mpDeviceContext;
		static IDXGISwapChain* mpSwapChain;
		static ID3D11RenderTargetView* mpBackBuffer_RTV;

		static std::unique_ptr<RTTManager> mpRTTManager;
		static std::unique_ptr<Camera> mpCamera;
		static std::unique_ptr<Light> mpLight;

		std::unique_ptr<SimpleSquare> mpSimpleSquare;
	};
}
