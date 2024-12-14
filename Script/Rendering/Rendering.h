#pragma once
#include "../Core/Module.h"

#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_win32.h"
#include "../../lib/imgui/imgui_impl_dx11.h"

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
	class Rendering:ModuleManager
	{
	public:
		Rendering(HWND* pHWnd);
		~Rendering();

		void start() override;
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

		IDXGISwapChain* getSwapChain() {
			return mpSwapChain;
		}

		RTTManager* const getRTTManager() {
			return mpRTTManager.get();
		}

		Camera* const getCamera()
		{
			return mpMainCamera;
		}

		Light* const getLight()
		{
			return mpLight;
		}

	private:
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
		Camera* mpMainCamera;
		Light* mpLight;
		Mesh* mpMesh;


		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	};
}
