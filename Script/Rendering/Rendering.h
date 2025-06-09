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
	/// Render���W���[��
	/// </summary>
	class Rendering:ModuleManager
	{
	public:
		Rendering(HWND& pHWnd);
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

		GBufferToBackBuffer* const getGBufferToBackBuffer()
		{
			return mpGBufferToBackBuffer.get();
		}

		void setGlobalConstantBuffer();

	private:
		//ConstantBuffer�i���W�֌W�j�p�\����
		struct CONSTANT_BUFER_GLOBAL
		{
			D3DXVECTOR4 vLightDir;
			D3DXVECTOR4 vEye;
		};

		void createBackBuffer_RTV();

		//DX11�n
		//unique_ptr���g���Ɖ�����ɗ�O
		ID3D11Device* mpDevice;
		ID3D11DeviceContext* mpDeviceContext;
		IDXGISwapChain* mpSwapChain;
		ID3D11RenderTargetView* mpBackBuffer_RTV;

		//�C���X�^���X
		std::unique_ptr<RTTManager> mpRTTManager;
		std::unique_ptr<GBufferToBackBuffer> mpGBufferToBackBuffer;
		Camera* mpMainCamera;
		Light* mpLight;
		Mesh* mpMesh;
		ID3D11Buffer* mpConstantBuffer_Global;

		const float ClearColor[4] = { 0.97, 0.71, 0, 1 };
	};
}
