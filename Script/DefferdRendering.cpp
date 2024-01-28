#include "../stdafx.h"
#include "DefferdRendering.h"
#include "Render.h"
#include "RTTManager.h"

namespace Koban {
	DefferdShader::DefferdShader() {
		// ----------------------------------------
		// Shader
		// ----------------------------------------
		//�g�p����Shader�̍쐬
		Render::createVertexShader(mShaderFileName, mVSShaderName, mpVertexShader);
		Render::createPixelShader(mShaderFileName, mPSShaderName, mpPixelShader);

		// ----------------------------------------
		// Buffer
		// ----------------------------------------
		//�X�N���[���T�C�Y�̃|���S���p�@�o�[�e�b�N�X�o�b�t�@�[���쐬
		MY_VERTEX VertexData[] =
		{
			D3DXVECTOR3(-1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,1),
			D3DXVECTOR3(-1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),
			D3DXVECTOR3(1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,1),
			D3DXVECTOR3(1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(1,0)
		};
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = VertexData;
		if (FAILED(DEVICE->CreateBuffer(&bd, &sd, &mpVertexBuffer))) {
			return;
		}

		//�R���X�^���g�o�b�t�@�[�쐬�@
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(CBUFER_COORD);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &mpConstantBuffer)))
		{
			return;
		}
	}
	
	void DefferdShader::updateShader() {
		auto pDepthStencil = Render::getRTTManager()->getDepthStensilSRV();
		auto pBackBuffer = Render::getBackBuffer();

		//RenderTarget��ʏ�ɖ߂�
		DEVICE_CONTEXT->OMSetRenderTargets(1, &pBackBuffer, Render::getRTTManager()->getDepthStensilSRV());
		//RTT�̃N���A
		float clearColor[4] = { 0,1,0,1 };
		DEVICE_CONTEXT->ClearRenderTargetView(pBackBuffer, clearColor);
		DEVICE_CONTEXT->ClearDepthStencilView(pDepthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

		//�V�F�[�_�[�̃Z�b�g
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//�e�N�X�`���Z�b�g
		auto col = Render::getRTTManager()->getRTTSRV(Koban::RTTManager::COLOR);
		auto normal = Render::getRTTManager()->getRTTSRV(Koban::RTTManager::NORMAL);
		auto pos = Render::getRTTManager()->getRTTSRV(Koban::RTTManager::POSITION);
		DEVICE_CONTEXT->PSSetShaderResources(1, 1, &col);
		DEVICE_CONTEXT->PSSetShaderResources(2, 1, &normal);
		DEVICE_CONTEXT->PSSetShaderResources(3, 1, &pos);
		//�g�|���W�[�̃Z�b�g
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//���_�o�b�t�@�̃Z�b�g
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//�o�b�N�o�b�t�@��Draw!
		DEVICE_CONTEXT->Draw(4, 0);
	}

	void DefferdShader::destroy() {
		ShaderObject::destroy();
		SAFE_RELEASE(mpVertexBuffer);
		SAFE_RELEASE(mpConstantBuffer);
	}
}