#include "TestShader.h"
#include "Render.h"

namespace Koban {
	TestShader::TestShader(std::wstring path) :
		mpConstantBuffer1(nullptr)
	{
		initShader();
	}

	TestShader::~TestShader() {
		SAFE_RELEASE(mpConstantBuffer1);
	}

	///�}�e���A���̊e�v�f���V�F�[�_�[�ɓn��
	void TestShader::updateBuffer(SIMPLECONSTANT_BUFFER1 buffer) {
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

	void TestShader::initShader() {
		BaseShader::initShader();

		//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
		D3D11_BUFFER_DESC cb;

		//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER1);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &mpConstantBuffer1)))
		{
			DebugLib::error(L"�R���X�^���g�o�b�t�@�쐬���s");
			return;
		}
	}
}