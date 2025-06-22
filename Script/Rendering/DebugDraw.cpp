#include "DebugDraw.h"
#include "Rendering.h"
#include "../Core/Transform.h"
#include "Light.h"
#include "Camera.h"

namespace Koban
{
	void DebugDraw::start()
	{
		//shader������
		//���_�C���v�b�g���C�A�E�g���`	
		vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		//shader������
		RenderUtil::createShader(DEVICE, L"Shader/VS_Simple.hlsl", L"Shader/PS_Simple.hlsl", layout, &mpVertexLayout, &mpVertexShader, &mpPixelShader);
		//�R���X�^���g�o�b�t�@������
		RenderUtil::createConstantBuffer<RenderUtil::CONSTANT_BUFFER_TRANSFORM>(DEVICE, &mpConstantBuffer_Transform);
	}

	void DebugDraw::draw()
	{
		ID3D11Buffer* vertBuf = createBuffer();
		//�V�F�[�_�[�̃Z�b�g
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
		D3D11_MAPPED_SUBRESOURCE pData_default;
		RenderUtil::CONSTANT_BUFFER_TRANSFORM cb_default;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Transform, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_default)))
		{
			//�P�ʍs����Z�b�g
			cb_default.mW = RENDER->getLight()->getTransform()->getWorldMatrix();
			Camera* camera = RENDER->getCamera();
			//MVP�s���n��
			cb_default.mWVP = cb_default.mW * camera->getViewMat() * camera->getProjMat();
			if (memcpy_s(pData_default.pData, sizeof(RenderUtil::CONSTANT_BUFFER_TRANSFORM), (void*)(&cb_default), sizeof(cb_default)))
			{
				DebugUtil::error(L"memCopy���ɃG���[");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Transform, 0);
		}
		//�g�|���W�[�̃Z�b�g
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		//���_�o�b�t�@�̃Z�b�g
		UINT stride = sizeof(D3DXVECTOR3);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &vertBuf, &stride, &offset);
		//�R���X�^���g�o�b�t�@�̃Z�b�g
		DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &mpConstantBuffer_Transform);
		DEVICE_CONTEXT->PSSetConstantBuffers(1, 1, &mpConstantBuffer_Transform);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		DEVICE_CONTEXT->IASetInputLayout(mpVertexLayout);
		//�o�b�N�o�b�t�@��Draw!
		DEVICE_CONTEXT->Draw(2, 0);
	}

	ID3D11Buffer* DebugDraw::createBuffer()
	{
		D3DXVECTOR3 lightDir = RENDER->getLight()->getDirection();

		//�o�[�e�b�N�X�o�b�t�@�[���쐬
		D3DXVECTOR3 vertexData[] = { D3DXVECTOR3(0,0,0), lightDir};

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(D3DXVECTOR3) * 2;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = vertexData;
		ID3D11Buffer* pVertexBuffer;
		if (FAILED(DEVICE->CreateBuffer(&bd, &sd, &pVertexBuffer)))
		{
			return nullptr;
		}
		return pVertexBuffer;
	}

	void DebugDraw::destroy()
	{
		SAFE_RELEASE(mpVertexShader);
		SAFE_RELEASE(mpPixelShader);
		SAFE_RELEASE(mpConstantBuffer_Transform);
	}
}