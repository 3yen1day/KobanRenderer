#include "Rendering.h"
#include "RTTManager.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "GBufferToBackBuffer.h"
#include "DebugDraw.h"
#include "../Core/Transform.h"
#include "../Core/Scene.h"
#include "../Utillity/RenderUtil.h"
#include <d3d11.h>
#include <tchar.h>

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pHWnd">�E�B���h�E�̃n���h��</param>
namespace Koban {
	Rendering::Rendering(HWND& hwnd)
	{
		// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = WINDOW_WIDTH;
		sd.BufferDesc.Height = WINDOW_HEIGHT;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

		mpSwapChain = nullptr;
		mpDevice = nullptr;
		mpDeviceContext = nullptr;
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
			0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &mpSwapChain, &mpDevice,
			pFeatureLevel, &mpDeviceContext)))
		{
			//�Ȃ񂩗�O
			DebugUtil::error(L"�f�o�C�X�쐬���s");
			return;
		}

		//�r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.Width = WINDOW_WIDTH;
		vp.Height = WINDOW_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		mpDeviceContext->RSSetViewports(1, &vp);
		//���X�^���C�Y�ݒ�
		D3D11_RASTERIZER_DESC rdc;
		ZeroMemory(&rdc, sizeof(rdc));
		rdc.CullMode = D3D11_CULL_NONE;
		rdc.FillMode = D3D11_FILL_SOLID;
		ID3D11RasterizerState* pIr = NULL;
		mpDevice->CreateRasterizerState(&rdc, &pIr);
		mpDeviceContext->RSSetState(pIr);
		SAFE_RELEASE(pIr);

		//�o�b�N�o�b�t�@�̍쐬
		mpBackBuffer_RTV = nullptr;
		createBackBuffer_RTV();

		//�R���X�^���g�o�b�t�@�̍쐬
		RenderUtil::createConstantBuffer<CONSTANT_BUFER_GLOBAL>(mpDevice, &mpConstantBuffer_Global);

		//RTT���Ǘ�����
		mpRTTManager = std::make_unique<RTTManager>();
		//GBuffer���������`����Ǘ�����
		mpGBufferToBackBuffer = std::make_unique<GBufferToBackBuffer>();
		mpDebugDraw = std::make_unique<DebugDraw>();
	}

	Rendering::~Rendering() {};// = default; // �f�X�g���N�^���C�����C����

	void Rendering::start()
	{
		mpRTTManager->start();
		mpGBufferToBackBuffer->start();
		mpDebugDraw->start();

		SCENE->doStart<Mesh>();
	}

	void Rendering::update() {
		//�R���|�[�l���g�̍X�V
		SCENE->doUpdate<Camera>();
		std::vector<Camera*> cameras = SCENE->findComponents<Camera>();
		if (!cameras.empty()) {
			mpMainCamera = cameras[0];
		}

		std::vector<Light*> lights = SCENE->findComponents<Light>();
		if (!lights.empty()) {
			mpLight = lights[0];
		}

		SCENE->doUpdate<Mesh>();

		mpRTTManager->update();
		mpGBufferToBackBuffer->update();
	}

	void Rendering::draw() {
		//GBuffer�ւ̕`��
		SCENE->doDraw<Mesh>();

		//�����_�[�^�[�Q�b�g��ʏ�ɖ߂�
		DEVICE_CONTEXT->OMSetRenderTargets(1, &mpBackBuffer_RTV, nullptr);

		//�N���A
		DEVICE_CONTEXT->ClearRenderTargetView(mpBackBuffer_RTV, ClearColor);

		//GBuffer����`��
		mpGBufferToBackBuffer->draw();
		
		//�f�o�b�O�\��
		mpDebugDraw->draw();
	}

	void Rendering::destroy()
	{
		mpRTTManager->destroy();
		mpGBufferToBackBuffer->destroy();
		mpDebugDraw->destroy();

		//���\�[�X���L���̔j��
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpDevice);
		SAFE_RELEASE(mpDeviceContext);
		SAFE_RELEASE(mpBackBuffer_RTV);
	}

	void Rendering::createBackBuffer_RTV() {
		//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
		ID3D11Texture2D* pBackBuffer_Tex;
		mpSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer_Tex));
		mpDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_RTV);
		SAFE_RELEASE(pBackBuffer_Tex);
	}

	/// <summary>
	/// �Sshader���ʂ̃R���X�^���g�o�b�t�@���Z�b�g�ishader�o�^��ɕK���Ăԁj
	/// </summary>
	void Rendering::setGlobalConstantBuffer()
	{	
		D3D11_MAPPED_SUBRESOURCE pData_default;
		CONSTANT_BUFER_GLOBAL cb;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Global, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_default)))//pData_default.pData��m_pConstantBuffer�̃A�h���X
		{
			//���C�g�̈ʒu��n��
			auto lightDir = RENDER->getLight()->getDirection();
			cb.vLightDir = D3DXVECTOR4(lightDir.x, lightDir.y, lightDir.z, 1);
			//���_�ʒu��n��
			cb.vEyePos = D3DXVECTOR4(RENDER->getCamera()->getPostion(), 0);
			if (memcpy_s(pData_default.pData, sizeof(CONSTANT_BUFER_GLOBAL), (void*)(&cb), sizeof(cb)))
			{
				DebugUtil::error(L"memCopy���ɃG���[");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Global, 0);

			DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer_Global);
			DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer_Global);
		}
	}
}
