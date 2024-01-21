#include "../stdafx.h"

namespace Koban {
	TestShader::TestShader(std::wstring path) :
		mpVertexLayout(nullptr),
		mpConstantBuffer0(nullptr),
		mpConstantBuffer1(nullptr),
		mpVertexShader(nullptr),
		mpPixelShader(nullptr),
		mShaderPath(path)
	{
		initShader();
	}

	TestShader::~TestShader() {
		SAFE_RELEASE(mpVertexLayout);
		SAFE_RELEASE(mpConstantBuffer0);
		SAFE_RELEASE(mpConstantBuffer1);
		SAFE_RELEASE(mpVertexShader);
		SAFE_RELEASE(mpPixelShader);
	}

	///�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
	void TestShader::setBuffer(SIMPLECONSTANT_BUFFER1 buffer) {
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;

		//�o�b�t�@�̒��g���X�V���邽�߂ɁAmap, unmap���g�p���遨lock, unlock�̂悤�Ȃ���
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource)))
		{
			memcpy_s(mappedSubResource.pData, mappedSubResource.RowPitch, (void*)&buffer, sizeof(SIMPLECONSTANT_BUFFER1));
			DEVICE_CONTEXT->Unmap(mpConstantBuffer1, 0);
		}
		DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &mpConstantBuffer1);
		DEVICE_CONTEXT->PSSetConstantBuffers(1, 1, &mpConstantBuffer1);
	}

	void TestShader::Render(D3DXMATRIX& mWorld, D3DXMATRIX& mViewMat, D3DXMATRIX& mProjMat,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye)
	{
		//�g�p����V�F�[�_�[�̓o�^	
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SIMPLECONSTANT_BUFFER0 sg;
			//���[���h�s���n��
			sg.mW = mWorld;
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			//���[���h�A�J�����A�ˉe�s���n��
			sg.mWVP = mWorld * mViewMat * mProjMat;
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			//���C�g�̕�����n��
			sg.mLightDir = D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);
			//���_�ʒu��n��
			sg.mEyePos = D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 0);

			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
			DEVICE_CONTEXT->Unmap(mpConstantBuffer0, 0);
		}
		//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		DEVICE_CONTEXT->IASetInputLayout(mpVertexLayout);
		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	HRESULT TestShader::initShader() {
		if (mShaderPath == L"") {
			MessageBox(0, L"shaderPath���s��", NULL, MB_OK);
			return E_FAIL;
		}
		//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
		ID3D10Blob* pCompiledShader = NULL;
		ID3D10Blob* pErrors = NULL;
		//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "VS", "vs_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pErrors);

		if (FAILED(DEVICE->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpVertexShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
			return E_FAIL;
		}
		//���_�C���v�b�g���C�A�E�g���`	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = sizeof(layout) / sizeof(layout[0]);
		//���_�C���v�b�g���C�A�E�g���쐬
		if (FAILED(DEVICE->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &mpVertexLayout)))
		{
			return E_FAIL;
		}
		//�u���u����s�N�Z���V�F�[�_�[�쐬
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "PS", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pErrors);
		if (FAILED(DEVICE->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpPixelShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pCompiledShader);

		//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER0);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &mpConstantBuffer0)))
		{
			return E_FAIL;
		}

		//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER1);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &mpConstantBuffer1)))
		{
			return E_FAIL;
		}

		return S_OK;
	}
}