#include "../stdafx.h"
namespace Koban {
	void TestDefferdRender::draw() {
		//todo:↓Mesh描画の方に移す
		////RTTのクリア
		//float ClearColor[4] = { 0,0,1,1 };
		//DEVICE_CONTEXT->ClearRenderTargetView(mpColor_RTV, ClearColor);
		//DEVICE_CONTEXT->ClearRenderTargetView(mpNormal_RTV, ClearColor);
		//DEVICE_CONTEXT->ClearRenderTargetView(mpPosition_RTV, ClearColor);
		//DEVICE_CONTEXT->ClearDepthStencilView(Render::getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		////全てのテクスチャーをレンダーターゲットにセット
		//ID3D11RenderTargetView* pViews[3];
		//pViews[0] = mpColor_RTV;
		//pViews[1] = mpNormal_RTV;
		//pViews[2] = mpPosition_RTV;
		//DEVICE_CONTEXT->OMSetRenderTargets(3, pViews, Render::getDepthStencilView());//深度ステンシルビューは全てに共通の1つだけを使う
	}
}