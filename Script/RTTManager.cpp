#include "RTTManager.h"
#include "Render.h"
#include "Camera.h"

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
		// RTT
		// ----------------------------------------
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
		auto backBuffer = Render::getBackBuffer();
		DEVICE_CONTEXT->OMSetRenderTargets(1, &backBuffer, mpDepthStencil_SRV);

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

	void RTTManager::destroy() {
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