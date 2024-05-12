#include "SimpleSquare.h"
#include "Render.h"
#include "Camera.h"

namespace Koban {
	SimpleSquare::SimpleSquare()
	{
		//shader������
		//���_�C���v�b�g���C�A�E�g���`	
		vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		ID3DBlob* compiledShader = NULL;
		RenderLib::createShader(
			DEVICE,
			L"Shader//Simple.hlsl",
			layout,
			L"VS",
			L"PS",
			&m_pVertexLayout,
			&m_pVertexShader,
			&m_pPixelShader
		);
		SAFE_RELEASE(compiledShader);

		//�R���X�^���g�o�b�t�@������
		RenderLib::createConstantBuffer<D3DXMATRIX>(DEVICE, &m_pConstantBuffer);

		//�o�[�e�b�N�X�o�b�t�@������
		vector<SimpleVertex> vertices = {
			{D3DXVECTOR3(-0.5,-0.5,0),D3DXVECTOR2(0,1)},//���_1,
			{D3DXVECTOR3(-0.5,0.5,0), D3DXVECTOR2(0,0)},//���_2
			{D3DXVECTOR3(0.5,-0.5,0),D3DXVECTOR2(1,1)}, //���_3
			{D3DXVECTOR3(0.5,0.5,0),D3DXVECTOR2(1,0)}, //���_4
		};
		RenderLib::createVertexBffer<SimpleVertex>(DEVICE, DEVICE_CONTEXT, vertices, &m_pVertexBuffer);

		//�e�N�X�`���p�T���v���쐬
		RenderLib::createSamplerState(DEVICE, &m_pSampleLinear);

		//�e�N�X�`���쐬
		RenderLib::createTexture(DEVICE, L"Resource\\sprite.jpg", &m_pTexture);
	}

	void SimpleSquare::draw() {
		//�g�p����V�F�[�_�[�̓o�^	
		DEVICE_CONTEXT->VSSetShader(m_pVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(m_pPixelShader, NULL, 0);

		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^�i�ʒu�ƐF�j��n��	
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		auto pos = D3DXVECTOR3(0.0f, 0.0f, -0.5f);
		D3DXMATRIX wMat;
		D3DXMatrixTranslation(&wMat, pos.x, pos.y, pos.z);

		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = wMat * Render::getCamera()->getViewMat() * Render::getCamera()->getProjMat();
		D3DXMatrixTranspose(&m, &m); //GPU�̍s��v�Z���@��DirectX�̂��ꂪ�قȂ邽�ߓ]�u����
		cb.mWVP = m;
		//�J���[��n��
		D3DXVECTOR4 vColor(1, 0, 0, 1);
		cb.vColor = vColor;

		if (SUCCEEDED(DEVICE_CONTEXT->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))//pData.pData��m_pConstantBuffer�̃A�h���X
		{
			if (memcpy_s(pData.pData, sizeof(SIMPLESHADER_CONSTANT_BUFFER), (void*)(&cb), sizeof(cb))) {
				DebugLib::error(L"memCopy���ɃG���[");
			}
			DEVICE_CONTEXT->Unmap(m_pConstantBuffer, 0);
		}

		//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		DEVICE_CONTEXT->IASetInputLayout(m_pVertexLayout);
		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		DEVICE_CONTEXT->PSSetSamplers(0, 1, &m_pSampleLinear);
		DEVICE_CONTEXT->PSSetShaderResources(0, 1, &m_pTexture);
		//�v���~�e�B�u�������_�����O
		DEVICE_CONTEXT->Draw(4, 0);
	}
}