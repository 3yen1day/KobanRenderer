#pragma once
#include "RenderMode.h"

/// <summary>
/// GBufferの情報をBuckBufferにレンダリングする
/// </summary>
namespace Koban {
	class GBufferToBackBuffer
	{
	public:
		GBufferToBackBuffer();
		~GBufferToBackBuffer() {};
		void start();
		void update();
		void draw();
		void destroy();

		void setRenderMode(RenderMode::MODE mode) { mRenderMode->setMode(mode); }
		RenderMode::MODE getRenderMode() { return mRenderMode->getMode(); }

	private:
		//vertexBuffer用構造体
		struct MY_VERTEX
		{
			D3DXVECTOR3 vPos;
			D3DXVECTOR3 vNorm;
			D3DXVECTOR2 vTex;
		};

		void createShader();

		//screen描画用頂点バッファ
		ID3D11Buffer* mpVertexBuffer; //unique_ptrにするとデストラクトでのヒープの解放で例外
		//Shader
		ID3D11VertexShader* mpVertexShader = NULL;
		ID3D11PixelShader* mpPixelShader = NULL;
		//RenderingMode
		RenderMode* mRenderMode = new RenderMode();
	};
}