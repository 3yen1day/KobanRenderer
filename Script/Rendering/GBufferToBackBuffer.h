#pragma once
#include "RenderMode.h"

/// <summary>
/// GBuffer�̏���BuckBuffer�Ƀ����_�����O����
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
		//vertexBuffer�p�\����
		struct MY_VERTEX
		{
			D3DXVECTOR3 vPos;
			D3DXVECTOR3 vNorm;
			D3DXVECTOR2 vTex;
		};

		//ConstantBuffer�i���W�֌W�j�p�\����
		struct CBUFER_COORD
		{
			D3DMATRIX WMat;
			D3DMATRIX wvpMat;
			D3DXVECTOR4 lightPos[MAX_Light];
			D3DXVECTOR4 eyePos;
		};

		void createShader();

		//screen�`��p���_�o�b�t�@
		ID3D11Buffer* mpVertexBuffer; //unique_ptr�ɂ���ƃf�X�g���N�g�ł̃q�[�v�̉���ŗ�O
		//ConstantBuffer
		std::unique_ptr <ID3D11Buffer> mpConstantBuffer;
		//Shader
		ID3D11VertexShader* mpVertexShader = NULL;
		ID3D11PixelShader* mpPixelShader = NULL;
		//RenderingMode
		RenderMode* mRenderMode = new RenderMode();
	};
}