#include "Render.h"
#include "RTTManager.h"
#include "Camera.h"

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

ID3D11Device* Koban::Render::mpDevice;
ID3D11DeviceContext* Koban::Render::mpDeviceContext;
IDXGISwapChain* Koban::Render::mpSwapChain;
ID3D11RenderTargetView* Koban::Render::mpBackBuffer_RTV;
std::unique_ptr<Koban::Camera> Koban::Render::mpCamera;
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

		//RenderObject�̏�����
		//mpRenderObjects = { new TestDefferdRender() };
	}

	void Render::destroy()
	{
		mpRTTManager->destroy();

		//unique_ptr�̓R�s�[�s�Ȃ̂ŁA&�����ĎQ�ƌ^�ɂ���K�v������
		for (const auto& e : mpRenderObjects) {
			e->destroy();
		}

		//���\�[�X���L���̔j��
		SAFE_RELEASE(mpSwapChain);
		SAFE_RELEASE(mpDevice);
		SAFE_RELEASE(mpDeviceContext);
		SAFE_RELEASE(mpBackBuffer_RTV);
	}

	void Render::update() {
		//�J�����X�V
		mpCamera->update();

		for (const auto& e : mpRenderObjects) {
			e->update();
		}
	}

	//�V�[������ʂɃ����_�����O
	void Render::draw()
	{
		//�f�B�t�@�[�h�����_�����O
		drawDefferd();
	}

	void Render::drawDefferd() {
		//RenderObject�̍X�V
		//mpMesh->Render(mViewMat, mProjMat, D3DXVECTOR3(1, 1, -1), mPosition);
		for (const auto& e : mpRenderObjects) {
			e->draw();
		}

		//// �`�悳�ꂽRTT�����ƂɊG��`��
		//// GBuffer�̃N���A�ƃZ�b�g
		//mpRTTManager->renderToScene();

		////��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
		//mpSwapChain->Present(0, 0);
	}

#pragma region static�֐�
	bool Render::createVertexShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11VertexShader* vs) {
		//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
		std::unique_ptr<ID3D10Blob> upCompiledShader = NULL;
		std::unique_ptr<ID3D10Blob> upErrors = NULL;
		ID3D10Blob* pCompiledShader = upCompiledShader.get();
		ID3D10Blob* pErrors = upErrors.get();
		auto shaderName_s = StringLib::wstr2str(shaderName);
		
		//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
		if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return false;
		}
		if (FAILED(mpDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &vs)))
		{
			MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
			return false;
		}

		return true;
	}

	bool Render::createPixelShader(const std::wstring& fileName, const std::wstring& shaderName, ID3D11PixelShader* ps) {
		//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
		std::unique_ptr<ID3D10Blob> upCompiledShader = NULL;
		std::unique_ptr<ID3D10Blob> upErrors = NULL;
		ID3D10Blob* pCompiledShader = upCompiledShader.get();
		ID3D10Blob* pErrors = upErrors.get();
		auto shaderName_s = StringLib::wstr2str(shaderName);

		if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return false;
		}

		if (FAILED(mpDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &ps)))
		{
			MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
			return false;
		}
		return true;
	}
#pragma endregion
}