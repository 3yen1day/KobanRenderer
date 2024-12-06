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
	/// Render���W���[��
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
		//�A�v���Ɉ�K�v
		//unique_ptr���g���Ɖ�����ɗ�O
		ID3D11Device* mpDevice;
		ID3D11DeviceContext* mpDeviceContext;
		IDXGISwapChain* mpSwapChain;
		ID3D11RenderTargetView* mpBackBuffer_RTV;

		std::unique_ptr<RTTManager> mpRTTManager;
		std::unique_ptr<GBufferToBackBuffer> mpGBufferToBackBuffer;
		
		/// <summary>
		/// GameObject�ɂ���
		/// </summary>
		Camera* mpMainCamera;
		Light* mpLight;
		Mesh* mpMesh;
	};
}
