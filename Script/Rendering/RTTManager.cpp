#include "RTTManager.h"
#include "Rendering.h"
#include "Camera.h"

namespace Koban
{
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
	}

	void RTTManager::start()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		ZeroMemory(&DSVDesc, sizeof(DSVDesc));
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));

		//------------------------
		// デプスステンシルビュー
		//------------------------
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

		DSVDesc.Format = texDesc.Format;
		DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DEVICE->CreateDepthStencilView(mpDepthStencil_Tex, &DSVDesc, &mpDepthStencilView);

		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D.MipSlice = 0;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		//------------------------
		// 深度マップ
		//------------------------
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//テクスチャ作成
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpDepth_Tex);
		//RTV作成
		RTVDesc.Format = texDesc.Format;
		DEVICE->CreateRenderTargetView(mpDepth_Tex, &RTVDesc, &mpDepth_RTV);
		//SRV作成
		SRVDesc.Format = texDesc.Format;
		DEVICE->CreateShaderResourceView(mpDepth_Tex, &SRVDesc, &mpDepth_SRV);

		//------------------------
		// カラーマップ
		//------------------------
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpColor_Tex);

		RTVDesc.Format = texDesc.Format;
		DEVICE->CreateRenderTargetView(mpColor_Tex, &RTVDesc, &mpColor_RTV);

		SRVDesc.Format = texDesc.Format;
		DEVICE->CreateShaderResourceView(mpColor_Tex, &SRVDesc, &mpColor_SRV);

		//------------------------
		// ノーマルマップ
		//------------------------
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpNormal_Tex);

		RTVDesc.Format = texDesc.Format;
		DEVICE->CreateRenderTargetView(mpNormal_Tex, &RTVDesc, &mpNormal_RTV);

		SRVDesc.Format = texDesc.Format;
		DEVICE->CreateShaderResourceView(mpNormal_Tex, &SRVDesc, &mpNormal_SRV);

		//------------------------
		// ポジションマップ
		//------------------------
		DEVICE->CreateTexture2D(&texDesc, NULL, &mpPosition_Tex);
		DEVICE->CreateRenderTargetView(mpPosition_Tex, &RTVDesc, &mpPosition_RTV);
		DEVICE->CreateShaderResourceView(mpPosition_Tex, &SRVDesc, &mpPosition_SRV);
	}

	void RTTManager::destroy()
	{
		SAFE_RELEASE(mpDepthStencil_Tex);
		SAFE_RELEASE(mpDepthStencilView);
		SAFE_RELEASE(mpDepth_Tex);
		SAFE_RELEASE(mpDepth_RTV);
		SAFE_RELEASE(mpDepth_SRV);
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

	void RTTManager::update()
	{
		//全てのテクスチャーをレンダーターゲットにセット
		//PS_OUTPUTにセットされる
		ID3D11RenderTargetView* pViews[4];
		pViews[0] = mpDepth_RTV;
		pViews[1] = mpColor_RTV;
		pViews[2] = mpNormal_RTV;
		pViews[3] = mpPosition_RTV;
		DEVICE_CONTEXT->OMSetRenderTargets(4, pViews, mpDepthStencilView);//深度ステンシルビューは全てに共通の1つだけを使う
		
		//深度ステンシルステートの設定（書き込み許可）
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		ID3D11DepthStencilState* depthState = nullptr;
		DEVICE->CreateDepthStencilState(&dsDesc, &depthState);
		DEVICE_CONTEXT->OMSetDepthStencilState(depthState, 0);// パイプラインに設定

		//クリア
		float ClearColor[4] = { 0,0,0,1 };
		DEVICE_CONTEXT->ClearRenderTargetView(mpDepth_RTV, ClearColor);
		DEVICE_CONTEXT->ClearRenderTargetView(mpColor_RTV, ClearColor);
		DEVICE_CONTEXT->ClearRenderTargetView(mpNormal_RTV, ClearColor);
		DEVICE_CONTEXT->ClearRenderTargetView(mpPosition_RTV, ClearColor);
		DEVICE_CONTEXT->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	ID3D11ShaderResourceView* const Koban::RTTManager::getRTTSRV(RTT_TYPE type)
	{
		switch (type)
		{
			case Koban::RTTManager::DEPTH:
				return mpDepth_SRV;
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