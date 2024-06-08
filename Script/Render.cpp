#include "Render.h"
#include "RTTManager.h"
#include "Camera.h"
#include "Light.h"
#include "Render3DModel.h"
#include "DefferdRendering.h"

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Koban::Render::mpDevice;
ID3D11DeviceContext* Koban::Render::mpDeviceContext;
IDXGISwapChain* Koban::Render::mpSwapChain;
ID3D11RenderTargetView* Koban::Render::mpBackBuffer_RTV;
std::unique_ptr<Koban::Camera> Koban::Render::mpCamera;
std::unique_ptr<Koban::Light> Koban::Render::mpLight;
std::unique_ptr<Koban::RTTManager> Koban::Render::mpRTTManager;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pHWnd">�E�B���h�E�̃n���h��</param>
namespace Koban {
	Render::Render(HWND* pHWnd) :
		mHwnd(pHWnd)
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
		sd.OutputWindow = *mHwnd;
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
			DebugLib::error(L"�f�o�C�X�쐬���s");
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

		//�o�b�N�o�b�t�@
		//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
		mpBackBuffer_RTV = nullptr;
		ID3D11Texture2D* pBackBuffer_Tex;
		Koban::Render::getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
		DEVICE->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_RTV);
		SAFE_RELEASE(pBackBuffer_Tex);

		//Object�̍쐬
		createObjects();
	}

	Render::~Render() = default; // �f�X�g���N�^���C�����C����

	/// <summary>
	/// Object�̍쐬
	/// </summary>
	void Render::createObjects()
	{
		//RTT���쐬���邭��
		mpRTTManager.reset(new RTTManager());
		//Camera�̍쐬
		mpCamera.reset(new Camera());
		//�V���v���Ȏl�p�`�`��
		mpRender3DModel.reset(new Render3DModel());
		//GBuffer�����ɕ`��
		mpDefferdShader.reset(new DefferdShader());
	}

	void Render::update() {
		mpCamera->update();
		mpRTTManager->update();
	}

	void Render::draw() {
		//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
		float ClearColor[4] = { 0,0,0.5,1 };// �N���A�F�쐬�@RGBA�̏�
		mpDeviceContext->ClearRenderTargetView(mpBackBuffer_RTV, ClearColor);//��ʃN���A
		
		mpRender3DModel->draw();
		mpDefferdShader->draw();
		////��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
		mpSwapChain->Present(0, 0);
	}

	void Render::destroy()
	{
		mpRTTManager->destroy();
		mpDefferdShader->destroy();

		//���\�[�X���L���̔j��
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpDevice);
		SAFE_RELEASE(mpDeviceContext);
		SAFE_RELEASE(mpBackBuffer_RTV);
	}
}