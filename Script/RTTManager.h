#pragma once
#include "Include/RenderInclude.h"
#include "RenderObject.h"

// NormalTextureやらを作成して、レンダーターゲットにする
// Shader側では、レンダーターゲットに出力
// 実際はSRVをShaderResourceとしても使いたい
// RTTにせっていしつつ、Resourceにも設定できるならそうする
// できないなら、二つ用意して、毎フレ入れ替える

namespace Koban {
	class RTTManager : public RenderObject {
	public:
		RTTManager();
		~RTTManager();

		void destroy() override;
	private:
		// RTT
		ID3D11Texture2D* mpColorTexture;
		ID3D11RenderTargetView* mpColor_RTV;
		ID3D11ShaderResourceView* mpColor_SRV;

		ID3D11Texture2D* mpNormalTexture;
		ID3D11RenderTargetView* mpNormal_RTV;
		ID3D11ShaderResourceView* mpNormal_SRV;

		ID3D11Texture2D* mpPositionTexture;
		ID3D11RenderTargetView* mpPosition_RTV;
		ID3D11ShaderResourceView* mpPosition_SRV;
	};
}