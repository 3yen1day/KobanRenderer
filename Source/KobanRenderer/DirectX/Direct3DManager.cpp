#include "Direct3DManager.h"
#include <windows.h>
#include <fstream>
#include "../framework.h"

Direct3DManager* Direct3DManager::mInstance = nullptr;

bool Direct3DManager::Initialize(HWND hWnd, int width, int height) {
	//=====================================================
	// �t�@�N�g���[�쐬(�r�f�I �O���t�B�b�N�̐ݒ�̗񋓂�w��Ɏg�p�����I�u�W�F�N�g)
	//=====================================================
	ComPtr<IDXGIFactory> factory;
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
		return false;
	}

	//=====================================================
	//�f�o�C�X����(��Ƀ��\�[�X�쐬���Ɏg�p����I�u�W�F�N�g)
	//=====================================================
	UINT creationFlags = 0;

#ifdef _DEBUG
	// DEBUG�r���h����Direct3D�̃f�o�b�O��L���ɂ���
	// (�������d�����ׂ����G���[���킩��)
	creationFlags = !D3D11_CREATE_DEVICE_DEBUG;
#endif

	//FeatureLevel�F�r�f�I�J�[�h��DX�̃��x��
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};
	D3D_FEATURE_LEVEL featureLevel;

	//�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̍쐬
	if (FAILED(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		&mDevice,
		&featureLevel,
		&mDeviceContext)))
	{
		return false;
	}

	//=====================================================
	// �X���b�v�`�F�C���쐬(�t�����g�o�b�t�@�ɕ\���\�ȃo�b�N�o�b�t�@��������)
	//=====================================================
	DXGI_SWAP_CHAIN_DESC scDesc = {};		// �X���b�v�`�F�[���̐ݒ�f�[�^
	scDesc.BufferDesc.Width = width;						// ��ʂ̕�
	scDesc.BufferDesc.Height = height;						// ��ʂ̍���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�t�@�̌`��
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.SampleDesc.Count = 1;							// MSAA�͎g�p���Ȃ�
	scDesc.SampleDesc.Quality = 0;							// MSAA�͎g�p���Ȃ�
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�t�@�̎g�p���@ //?�o�b�N�o�b�t�@�ł͂Ȃ��āH
	scDesc.BufferCount = 2;									// �o�b�t�@�̐�
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = TRUE;									// �E�B���h�E���[�h
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//�X���b�v�`�F�C���̍쐬
	if (FAILED(factory->CreateSwapChain(mDevice.Get(), &scDesc, &mSwapChain)))
	{
		return false;
	}

	// �X���b�v�`�F�C������o�b�N�o�b�t�@���\�[�X�擾
	ComPtr<ID3D11Texture2D> backBufferPtr;
	if (FAILED(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBufferPtr))))
	{
		return false;
	}

	// �o�b�N�o�b�t�@���\�[�X�p��RTV���쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = scDesc.BufferDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	if (FAILED(mDevice->CreateRenderTargetView(backBufferPtr.Get(), &rtvDesc, &mBackBufferView)))
	{
		return false;
	}

	//=====================================================
	// �f�o�C�X�R���e�L�X�g�ɕ`��Ɋւ���ݒ���s���Ă���
	//=====================================================
	// �o�b�N�o�b�t�@��RT�Ƃ��ăZ�b�g
	// �f�o�C�X�R���e�L�X�g�̕`�����o�b�N�o�b�t�@�p�̂q�s��
	mDeviceContext->OMSetRenderTargets(1, mBackBufferView.GetAddressOf(), nullptr);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	mDeviceContext->RSSetViewports(1, &vp);

	//=====================================================
	// �V�F�[�_�[�̍쐬
	//=====================================================
	
	std::ifstream ifs(L"Shader/SpriteShader.hlsl");
	if (!ifs.is_open()) {
		return false;
	}

	//���_�V�F�[�_�[�̓ǂݍ��݁E�R���p�C��
	ComPtr<ID3DBlob> compiledVS;
	ComPtr<ID3DBlob> pErrorBlob = NULL;
	D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &compiledVS, &pErrorBlob);
	//OutputDebugStringA(static_cast<char*>(pErrorBlob->GetBufferPointer()));
	//printf((char*)pErrorBlob->GetBufferPointer());
	//MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), TEXT(""), 0);
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &compiledVS, nullptr))) {
		return false; //shader�ǂ߂ĂȂ������[�I
	}
	
	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݁E�R���p�C��
	ComPtr<ID3DBlob> compiledPS;
	if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &compiledPS, nullptr)))
	{
		return false;
	}

	//���_�V�F�[�_�[�쐬
	if (FAILED(mDevice->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &mSpriteVS)))
	{
		return false;
	}

	//�s�N�Z���V�F�[�_�[�쐬
	if (FAILED(mDevice->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &mSpritePS)))
	{
		return false;
	}

	//���_���C�A�E�g
	//1���_�̏��
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//���_�C���v�b�g���C�A�E�g�쐬
	if (FAILED(mDevice->CreateInputLayout(&layout[0], layout.size(), compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &mSpriteInputLayout)))
	{
		return false;
	}

	return true;
}