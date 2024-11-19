#pragma once
#include "../Core/Module.h"

namespace Koban {
	class RTTManager;
	class Camera;
	class Light;
	class Mesh;
	class GBufferToBackBuffer;
}

namespace Koban {
	/// <summary>
	/// Renderモジュール
	/// </summary>
	class Render:Module
	{
	public:
		Render(HWND* pHWnd);
		~Render();

		void awake() override;
		void update() override;
		void draw() override;
		void destroy() override;

		ID3D11Device* getDevice()
		{
			return mpDevice;
		}

		ID3D11DeviceContext* getDeviceContext()
		{
			return mpDeviceContext;
		}

		ID3D11RenderTargetView* getBackBuffer() {
			return mpBackBuffer_RTV;
		}

		RTTManager* const getRTTManager() {
			return mpRTTManager.get();
		}

		Camera* getCamera()
		{
			return mpCamera.get();
		}

		Light* getLight()
		{
			return mpLight.get();
		}

	private:
		void createObjects();

		//アプリに一つ必要
		//unique_ptrを使うと解放時に例外
		ID3D11Device* mpDevice;
		ID3D11DeviceContext* mpDeviceContext;
		IDXGISwapChain* mpSwapChain;
		ID3D11RenderTargetView* mpBackBuffer_RTV;

		std::unique_ptr<RTTManager> mpRTTManager;
		std::unique_ptr<GBufferToBackBuffer> mpGBufferToBackBuffer;
		
		/// <summary>
		/// GameObjectにする
		/// </summary>
		std::unique_ptr<Camera> mpCamera;
		std::unique_ptr<Light> mpLight;
		std::unique_ptr<Mesh> mpMesh;
	};
}
