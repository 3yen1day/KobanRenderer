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

		//ConstantBuffer（座標関係）用構造体
		struct CBUFER_COORD
		{
			D3DMATRIX WMat;
			D3DMATRIX wvpMat;
			D3DXVECTOR4 lightPos[MAX_Light];
			D3DXVECTOR4 eyePos;
		};

		void createShader();

		//screen描画用頂点バッファ
		ID3D11Buffer* mpVertexBuffer; //unique_ptrにするとデストラクトでのヒープの解放で例外
		//ConstantBuffer
		std::unique_ptr <ID3D11Buffer> mpConstantBuffer;
		//Shader
		ID3D11VertexShader* mpVertexShader = NULL;
		ID3D11PixelShader* mpPixelShader = NULL;
		//RenderingMode
		RenderMode* mRenderMode = new RenderMode();
	};
}