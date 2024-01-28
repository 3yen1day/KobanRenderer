#pragma once
#include "RenderObject.h"

// NormalTextureやらを作成して、レンダーターゲットにする
// Shader側では、レンダーターゲットに出力
// 実際はSRVをShaderResourceとしても使いたい
// RTTにせっていしつつ、Resourceにも設定できるならそうする
// できないなら、二つ用意して、毎フレ入れ替える

namespace Koban {
	class RTTManager : public RenderObject {
	public:
		enum RTT_TYPE
		{
			COLOR,
			NORMAL,
			POSITION
		};

		RTTManager();
		~RTTManager() {};

		void destroy() override;

		/// <summary>
		/// RTTを元にシーンへ描画
		/// </summary>
		void renderToScene();

		/// <summary>
		/// RTTのSRVを取得
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		ID3D11ShaderResourceView* const getRTTSRV(RTT_TYPE type);

		ID3D11DepthStencilView* const getDepthStensilSRV() { return mpDepthStencil_SRV; }

	private:
		// RTT, RTV
		ID3D11Texture2D* mpDepthStencil_Tex;
		ID3D11DepthStencilView* mpDepthStencil_SRV;

		ID3D11Texture2D* mpColor_Tex;
		ID3D11RenderTargetView* mpColor_RTV;
		ID3D11ShaderResourceView* mpColor_SRV;

		ID3D11Texture2D* mpNormal_Tex;
		ID3D11RenderTargetView* mpNormal_RTV;
		ID3D11ShaderResourceView* mpNormal_SRV;

		ID3D11Texture2D* mpPosition_Tex;
		ID3D11RenderTargetView* mpPosition_RTV;
		ID3D11ShaderResourceView* mpPosition_SRV;
	};
}