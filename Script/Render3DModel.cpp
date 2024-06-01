#include "Render3DModel.h"
#include "Render.h"
#include "Camera.h"

namespace Koban {
	Render3DModel::Render3DModel()
	{
		//shader������
		//���_�C���v�b�g���C�A�E�g���`	
		vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

		//fbx�̃��[�h
		if (!FbxLoader::Load("Resource/saikoro.fbx", &mFbxVertexInfo)) {
			DebugLib::error(L"�t�@�C���ǂݍ��ݎ��s");
		}

		//�o�[�e�b�N�X�o�b�t�@������
		RenderLib::createVertexBffer<FbxLoader::Vertex>(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.vertices, &m_pVertexBuffer);

		//�C���f�b�N�X�o�b�t�@������
		RenderLib::createIndexBuffer(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.indices, &m_pIndexBuffer);

		//�e�N�X�`���p�T���v���쐬
		RenderLib::createSamplerState(DEVICE, &m_pSampleLinear);

		//�e�N�X�`���쐬
		RenderLib::createTexture(DEVICE, L"Resource\\sprite.jpg", &m_pTexture);
	}

	void Render3DModel::draw() {
		//�g�p����V�F�[�_�[�̓o�^	
		DEVICE_CONTEXT->VSSetShader(m_pVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(m_pPixelShader, NULL, 0);

		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^�i�ʒu�ƐF�j��n��	
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;

		// �X�P�[���s��
		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

		// ��]�s��
		D3DXQUATERNION quat;
		D3DXVECTOR3 vec3(0, 1, 0);
		static float angle = 0;
		static float speed = 0.0001;
		angle += speed;
		angle = std::fmod(angle, 6.28f);
		D3DXQuaternionRotationAxis(&quat, &vec3, angle);
		D3DXMATRIX matRotation;
		D3DXMatrixRotationQuaternion(&matRotation, &quat);

		//���s�ړ��s��
		auto pos = D3DXVECTOR3(0.0f, 0.0f, -0.5f);
		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

		D3DXMATRIX wMat = matScale * matRotation * matTrans;

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
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g todo:���t���K�v�H
		UINT stride = sizeof(FbxLoader::Vertex);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		//�C���f�b�N�X�o�b�t�@���Z�b�g todo:���t���K�v�H
		DEVICE_CONTEXT->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		DEVICE_CONTEXT->PSSetSamplers(0, 1, &m_pSampleLinear);
		DEVICE_CONTEXT->PSSetShaderResources(0, 1, &m_pTexture);
		//�v���~�e�B�u�������_�����O
		DEVICE_CONTEXT->DrawIndexed(mFbxVertexInfo.indices.size(), 0, 0);
	}
}