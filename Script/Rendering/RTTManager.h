#pragma once

/// <summary>
/// RTTの生成、管理を行うクラス
/// </summary>
namespace Koban {
	class RTTManager{
	public:
		enum RTT_TYPE
		{
			DEPTH,
			COLOR,
			NORMAL,
			POSITION
		};

		RTTManager();
		~RTTManager() {};

		void start();
		void update();
		void destroy();

		/// <summary>
		/// RTTのSRVを取得
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		ID3D11ShaderResourceView* const getRTTSRV(RTT_TYPE type);

		ID3D11DepthStencilView* const getDepthStensilView() { return mpDepthStencilView; }

	private:
		// RTT, RTV
		ID3D11Texture2D* mpDepthStencil_Tex;
		ID3D11DepthStencilView* mpDepthStencilView;

		ID3D11Texture2D* mpDepth_Tex;
		ID3D11RenderTargetView* mpDepth_RTV;
		ID3D11ShaderResourceView* mpDepth_SRV;

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