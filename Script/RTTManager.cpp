#include "RTTManager.h"
#include "Render.h"

namespace Koban {
	RTTManager::RTTManager() :
		mpColor_Tex(nullptr),
		mpColor_RTV(nullptr),
		mpColor_SRV(nullptr),
		mpNormal_Tex(nullptr),
		mpNormal_RTV(nullptr),
		mpNormal_SRV(nullptr),
		mpPosition_Tex(nullptr),
		mpPosition_RTV(nullptr),
		mpPosition_SRV(nullptr)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));

		// ----------------------------------------
		// Shader
		// ----------------------------------------
		//�g�p����Shader�̓o�^
		Render::createVertexShader(mShaderFileName, mVertShaderName, mpVertexShader);
		Render::createPixelShader(mShaderFileName, mPixShaderName, mpPixelShader);

		// ----------------------------------------
		// Buffer
		// ----------------------------------------
		//�X�N���[���T�C�Y�̃|���S���p�@�o�[�e�b�N�X�o�b�t�@�[���쐬
		MY_VERTEX VertexData[] =
		{
			D3DXVECTOR3(-1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,1),
			D3DXVECTOR3(-1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),
			D3DXVECTOR3(1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1),
			D3DXVECTOR3(1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,0)
		};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = VertexData;
		if (FAILED(DEVICE->CreateBuffer(&bd, &sd, &mpVertexBuffer))) {
			return;
		}

		//�R���X�^���g�o�b�t�@�[�쐬�@
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(CBUFER_COORD);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &mpConstantBuffer)))
		{
			return;
		}

		// ----------------------------------------
		// RTT
		// ----------------------------------------
		//�o�b�N�o�b�t�@
		//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
		ID3D11Texture2D* pBackBuffer_Tex;
		Koban::Render::getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
		//RTV�쐬
		DEVICE->CreateRenderTargetView(pBackBuffer_Tex, NULL, &mpBackBuffer_RTV);
		SAFE_RELEASE(pBackBuffer_Tex);

		//�f�v�X�X�e���V���r���[
		texDesc.Width = WINDOW_WIDTH;
		texDesc.Height = WINDOW_HEIGHT;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpDepthStencil_Tex);
		//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
		DEVICE->CreateDepthStencilView(mpDepthStencil_Tex, NULL, &mpDepthStencil_SRV);
		//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
		DEVICE_CONTEXT->OMSetRenderTargets(1, &mpBackBuffer_RTV, mpDepthStencil_SRV);

		//�J���[�}�b�v
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//�e�N�X�`���쐬
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpColor_Tex);
		//RTV�쐬
		RTVDesc.Format = texDesc.Format;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D.MipSlice = 0;
		DEVICE->CreateRenderTargetView(mpColor_Tex, &RTVDesc, &mpColor_RTV);
		//SRV�쐬
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.Format = texDesc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		DEVICE->CreateShaderResourceView(mpColor_Tex, &SRVDesc, &mpColor_SRV);

		//�m�[�}���}�b�v	
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpNormal_Tex);

		RTVDesc.Format = texDesc.Format;
		DEVICE->CreateRenderTargetView(mpNormal_Tex, &RTVDesc, &mpNormal_RTV);

		SRVDesc.Format = texDesc.Format;
		DEVICE->CreateShaderResourceView(mpNormal_Tex, &SRVDesc, &mpNormal_SRV);

		//�|�W�V�����}�b�v
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpPosition_Tex);
		DEVICE->CreateRenderTargetView(mpPosition_Tex, &RTVDesc, &mpPosition_RTV);
		DEVICE->CreateShaderResourceView(mpPosition_Tex, &SRVDesc, &mpPosition_SRV);
	}

	void RTTManager::drawDefferd() {
		//RenderTarget��ʏ�ɖ߂�
		DEVICE_CONTEXT->OMSetRenderTargets(1, &mpBackBuffer_RTV, mpDepthStencil_SRV);
		//RTT�̃N���A
		float clearColor[4] = { 0,1,0,1 };
		DEVICE_CONTEXT->ClearRenderTargetView(mpBackBuffer_RTV, clearColor);
		DEVICE_CONTEXT->ClearDepthStencilView(mpDepthStencil_SRV, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//�V�F�[�_�[�̃Z�b�g
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//�e�N�X�`���Z�b�g
		DEVICE_CONTEXT->PSSetShaderResources(1, 1, &mpColor_SRV);
		DEVICE_CONTEXT->PSSetShaderResources(2, 1, &mpNormal_SRV);
		DEVICE_CONTEXT->PSSetShaderResources(3, 1, &mpPosition_SRV);
		//�g�|���W�[�̃Z�b�g
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//���_�o�b�t�@�̃Z�b�g
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//�o�b�N�o�b�t�@��Draw!
		DEVICE_CONTEXT->Draw(4, 0);
	}

	void RTTManager::destroy() {
		SAFE_RELEASE(mpVertexBuffer);
		SAFE_RELEASE(mpConstantBuffer);
		SAFE_RELEASE(mpVertexShader);
		SAFE_RELEASE(mpPixelShader);
		SAFE_RELEASE(mpBackBuffer_RTV);
		SAFE_RELEASE(mpDepthStencil_Tex);
		SAFE_RELEASE(mpDepthStencil_SRV);
		SAFE_RELEASE(mpColor_Tex);
		SAFE_RELEASE(mpColor_RTV);
		SAFE_RELEASE(mpColor_SRV);
		SAFE_RELEASE(mpNormal_Tex);
		SAFE_RELEASE(mpNormal_RTV);
		SAFE_RELEASE(mpNormal_SRV);
		SAFE_RELEASE(mpPosition_Tex);
		SAFE_RELEASE(mpPosition_RTV);
		SAFE_RELEASE(mpPosition_SRV);
	}

	ID3D11ShaderResourceView* const Koban::RTTManager::getRTTSRV(RTT_TYPE type) {
		switch (type)
		{
		case Koban::RTTManager::COLOR:
			return mpColor_SRV;
		case Koban::RTTManager::NORMAL:
			return mpNormal_SRV;
		case Koban::RTTManager::POSITION:
			return mpPosition_SRV;
		default:
			break;
		}
		return nullptr;
	}
}