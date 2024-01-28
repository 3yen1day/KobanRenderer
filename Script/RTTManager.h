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
		/// GBufferからシーンに描画
		/// </summary>
		void renderToScene();

		/// <summary>
		/// RTTのSRVを取得
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		ID3D11ShaderResourceView* const getRTTSRV(RTT_TYPE type);


	private:
		//vertexBuffer用構造体
		struct MY_VERTEX
		{
			D3DXVECTOR3 vPos;
			D3DXVECTOR3 vNorm;
			D3DXVECTOR2 vTex;
		};

		//ConstantBuffer（座標関係）用構造体
		struct CBUFER_COORD
		{
			D3DMATRIX WMat;
			D3DMATRIX wvpMat;
			D3DXVECTOR4 lightPos[MAX_Light];
			D3DXVECTOR4 eyePos;
		};

		//ConstantBuffer
		//screen描画用頂点バッファ
		ID3D11Buffer* mpVertexBuffer;
		ID3D11Buffer* mpConstantBuffer;

		//Shader
		ID3D11VertexShader* mpVertexShader;
		ID3D11PixelShader* mpPixelShader;

		// RTT, RTV
		ID3D11RenderTargetView* mpBackBuffer_RTV;

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

		wstring mShaderFileName = L"Shader/Deferred.hlsl";
		wstring mVertShaderName = L"VS_To_Tex";
		wstring mPixShaderName = L"PS_To_Tex";
	};
}