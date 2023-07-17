#include "RTTManager.h"
#include "Render.h"

namespace Koban {
	RTTManager::RTTManager() :
		mpColorTexture(nullptr),
		mpColor_RTV(nullptr),
		mpColor_SRV(nullptr),
		mpNormalTexture(nullptr),
		mpNormal_RTV(nullptr),
		mpNormal_SRV(nullptr),
		mpPositionTexture(nullptr),
		mpPosition_RTV(nullptr),
		mpPosition_SRV(nullptr)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));

		//カラーマップ
		//テクスチャ作成
		desc.Width = WINDOW_WIDTH;
		desc.Height = WINDOW_HEIGHT;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		Koban::Render::getDevice()->CreateTexture2D(&desc, NULL, &mpColorTexture);
		//RTV作成
		RTVDesc.Format = desc.Format;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D.MipSlice = 0;
		Koban::Render::getDevice()->CreateRenderTargetView(mpColorTexture, &RTVDesc, &mpColor_RTV);
		//SRV作成
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		Koban::Render::getDevice()->CreateShaderResourceView(mpColorTexture, &SRVDesc, &mpColor_SRV);

		//ノーマルマップ	
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Koban::Render::getDevice()->CreateTexture2D(&desc, NULL, &mpNormalTexture);

		RTVDesc.Format = desc.Format;
		Koban::Render::getDevice()->CreateRenderTargetView(mpNormalTexture, &RTVDesc, &mpNormal_RTV);

		SRVDesc.Format = desc.Format;
		Koban::Render::getDevice()->CreateShaderResourceView(mpNormalTexture, &SRVDesc, &mpNormal_SRV);

		//ポジションマップ用テクスチャーとそのレンダーターゲットビュー、シェーダーリソースビューの作成	
		Koban::Render::getDevice()->CreateTexture2D(&desc, NULL, &mpPositionTexture);
		Koban::Render::getDevice()->CreateRenderTargetView(mpPositionTexture, &RTVDesc, &mpPosition_RTV);
		Koban::Render::getDevice()->CreateShaderResourceView(mpPositionTexture, &SRVDesc, &mpPosition_SRV);
	}

	void RTTManager::destroy() {
		SAFE_RELEASE(mpColorTexture);
		SAFE_RELEASE(mpColor_RTV);
		SAFE_RELEASE(mpColor_SRV);
		SAFE_RELEASE(mpNormalTexture);
		SAFE_RELEASE(mpNormal_RTV);
		SAFE_RELEASE(mpNormal_SRV);
		SAFE_RELEASE(mpPositionTexture);
		SAFE_RELEASE(mpPosition_RTV);
		SAFE_RELEASE(mpPosition_SRV);
	}

}