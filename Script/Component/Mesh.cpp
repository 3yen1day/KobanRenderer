#include "Mesh.h"
#include "../Rendering/Render.h"
#include "Camera.h"
#include "Light.h"

namespace Koban {
	Mesh::Mesh()
	{
		//shader������
		//���_�C���v�b�g���C�A�E�g���`	
		vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};


		ID3DBlob* compiledShader = NULL;
		RenderUtil::createShader(
			DEVICE,
			L"Shader/Deferred.hlsl",
			layout,
			L"VS_To_Tex",
			L"PS_To_Tex",
			&mpVertexLayout,
			&mpVertexShader,
			&mpPixelShader
		);
		SAFE_RELEASE(compiledShader);

		//�R���X�^���g�o�b�t�@������
		RenderUtil::createConstantBuffer<CONSTANT_BUFFER_DEFAULT>(DEVICE, &mpConstantBuffer_Default);
		RenderUtil::createConstantBuffer<CONSTANT_BUFFER_MATERIAL>(DEVICE, &mpConstantBuffer_Material);

		//fbx�̃��[�h
		if (!FbxLoader::Load("Resource/Chips.obj", &mFbxVertexInfo)) {
			DebugUtil::error(L"�t�@�C���ǂݍ��ݎ��s");
		}

		//�o�[�e�b�N�X�o�b�t�@������
		RenderUtil::createVertexBffer<FbxLoader::Vertex>(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.vertices, &mpVertexBuffer);

		//�C���f�b�N�X�o�b�t�@������
		RenderUtil::createIndexBuffer(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.indices, &mpIndexBuffer);

		//�e�N�X�`���p�T���v���쐬
		RenderUtil::createSamplerState(DEVICE, &mpSampleLinear);

		////�e�N�X�`���쐬
		//RenderLib::createTexture(DEVICE, L"Resource\\Chips_Cover.jpg", &mpTexture);
	}

	void Mesh::draw() {
		//�g�p����V�F�[�_�[�̓o�^	
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);

		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
		D3D11_MAPPED_SUBRESOURCE pData_default;
		CONSTANT_BUFFER_DEFAULT cb_default;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Default, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_default)))//pData_default.pData��m_pConstantBuffer�̃A�h���X
		{
			cb_default.mW = getModelMatrix();
			//MVP�s���n��
			cb_default.mWVP = getMVPMatrix(cb_default.mW);
			//���C�g�̈ʒu��n��
			cb_default.vLightDir = RENDER->getLight()->getDirection();
			//���_�ʒu��n��
			cb_default.vEye = D3DXVECTOR4(RENDER->getCamera()->getEyeDir(), 0);
			if (memcpy_s(pData_default.pData, sizeof(CONSTANT_BUFFER_DEFAULT), (void*)(&cb_default), sizeof(cb_default))) {
				DebugUtil::error(L"memCopy���ɃG���[");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Default, 0);
		}

		D3D11_MAPPED_SUBRESOURCE pData_material;
		CONSTANT_BUFFER_MATERIAL cb_material;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Material, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_material)))//pData_material.pData��m_pConstantBuffer�̃A�h���X
		{
			cb_material = getMaterialVal();
			if (memcpy_s(pData_material.pData, sizeof(CONSTANT_BUFFER_MATERIAL), (void*)(&cb_material), sizeof(cb_material))) {
				DebugUtil::error(L"memCopy���ɃG���[");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Material, 0);
		}

		//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer_Default);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer_Default);
		DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &mpConstantBuffer_Material);
		DEVICE_CONTEXT->PSSetConstantBuffers(1, 1, &mpConstantBuffer_Material);

		//���_�C���v�b�g���C�A�E�g���Z�b�g
		DEVICE_CONTEXT->IASetInputLayout(mpVertexLayout);
		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g todo:���t���K�v�H
		UINT stride = sizeof(FbxLoader::Vertex);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//�C���f�b�N�X�o�b�t�@���Z�b�g todo:���t���K�v�H
		DEVICE_CONTEXT->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		//DEVICE_CONTEXT->PSSetSamplers(0, 1, &mpSampleLinear);
		//DEVICE_CONTEXT->PSSetShaderResources(0, 1, &mpTexture);
		//�v���~�e�B�u�������_�����O
		DEVICE_CONTEXT->DrawIndexed(mFbxVertexInfo.indices.size(), 0, 0);
	}

	/// <summary>
	/// Model�s����擾����
	/// </summary>
	/// <returns></returns>
	D3DXMATRIX Mesh::getModelMatrix() {
		// �X�P�[���s��
		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

		// ��]�s��
		D3DXQUATERNION quat;
		D3DXVECTOR3 vec3(0, 1, 0);
		static float angle = 0;
		static float speed = 0.00005;
		angle += speed;
		angle = std::fmod(angle, 6.28f);
		D3DXQuaternionRotationAxis(&quat, &vec3, angle);
		D3DXMATRIX matRotation;
		D3DXMatrixRotationQuaternion(&matRotation, &quat);

		//���s�ړ��s��
		auto pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

		return matScale * matRotation * matTrans;
	}

	/// <summary>
	/// MVP�s����擾����
	/// </summary>
	/// <returns></returns>
	D3DXMATRIX Mesh::getMVPMatrix(const D3DXMATRIX& modelMat) {
		//���[���h�A�J�����A�ˉe�s���n��
		auto camera = RENDER->getCamera();
		D3DXMATRIX m = modelMat * camera->getViewMat() * camera->getProjMat();
		D3DXMatrixTranspose(&m, &m); //GPU�̍s��v�Z���@��DirectX�̂��ꂪ�قȂ邽�ߓ]�u����

		return m;
	}

	/// <summary>
	/// Material�̒l���擾����itodo:�e�L�X�g�t�@�C������ǂނ悤�ɂ���j
	/// </summary>
	/// <returns></returns>
	Mesh::CONSTANT_BUFFER_MATERIAL Mesh::getMaterialVal() {
		auto cBuffer = CONSTANT_BUFFER_MATERIAL();
		cBuffer.vAmbient = D3DXVECTOR4(0.5f, 0, 0, 0);
		cBuffer.vDiffuse = D3DXVECTOR4(1, 1, 1, 0);
		cBuffer.vSpecular = D3DXVECTOR4(2, 2, 2, 0);
		return cBuffer;
	}
}