#include "Render.h"

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="pHWnd">�E�B���h�E�̃n���h��</param>
Render::Render(HWND * pHWnd):
	mHwnd(pHWnd), 
	mpDevice(nullptr), 
	mpDeviceContext(nullptr), 
	mpSwapChain(nullptr),
	mpBackBuffer_TexRTV(nullptr),
	mpBackBuffer_DSTexDSV(nullptr),
	mpBackBuffer_DSTex(nullptr),
	mpMesh(nullptr)
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

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &mpSwapChain, &mpDevice,
		pFeatureLevel, &mpDeviceContext)))
	{
		//�Ȃ񂩗�O
		return;
	}
	//�e��e�N�X�`���[�ƁA����ɕt�т���e��r���[���쐬

	//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
	ID3D11Texture2D* pBackBuffer_Tex;
	mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//���̃e�N�X�`���[�ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬
	mpDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//�f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	mpDevice->CreateTexture2D(&descDepth, NULL, &mpBackBuffer_DSTex);
	//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	mpDevice->CreateDepthStencilView(mpBackBuffer_DSTex, NULL, &mpBackBuffer_DSTexDSV);

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	mpDeviceContext->OMSetRenderTargets(1, &mpBackBuffer_TexRTV, mpBackBuffer_DSTexDSV);
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

	CreateRenderObject();
}

/// <summary>
/// RenderObject�̍쐬
/// </summary>
void Render::CreateRenderObject()
{
	//RenderObject�̏�����
	//���b�V���쐬
	mpMesh = new TestMesh;
	if (FAILED(mpMesh->Init(mpDevice, mpDeviceContext)))
	{
		//�Ȃ񂩗�O
		return;
	}
}

void Render::Destroy()
{
	//todo:�o�^�����|�C���^���f���[�g
	SAFE_DELETE(mpMesh);

	//���\�[�X�̃f���[�g
	SAFE_RELEASE(mpSwapChain);
	SAFE_RELEASE(mpBackBuffer_TexRTV);
	SAFE_RELEASE(mpBackBuffer_DSTexDSV);
	SAFE_RELEASE(mpBackBuffer_DSTex);
	SAFE_RELEASE(mpDevice);
}


//�V�[������ʂɃ����_�����O
void Render::Draw()
{
	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	float ClearColor[4] = { 0,0,1,1 };// �N���A�F�쐬�@RGBA�̏�
	mpDeviceContext->ClearRenderTargetView(mpBackBuffer_TexRTV, ClearColor);//��ʃN���A
	mpDeviceContext->ClearDepthStencilView(mpBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A
	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -0.5f); //�J�����i���_�j�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);

	//�����_�����O
	//RenderObject�̍X�V
	mpMesh->Render(mView, mProj, D3DXVECTOR3(1, 1, -1), vEyePt);
	//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
	mpSwapChain->Present(0, 0);
}