#pragma once
#include "RenderObject.h"

/// <summary>
/// RTTの生成、管理を行うクラス
/// </summary>
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
		void update() override;

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