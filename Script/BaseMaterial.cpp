#include "BaseMaterial.h"
#include "Render.h"

namespace Koban {
	BaseMaterial::BaseMaterial() :
		mName{ L"" },
		shaderPath{ L"" },
		mFaceNum{ 0 },
		mpIndexBuffer{},
		mTextureName{ L"" },
		mpTexture{ nullptr }
	{
		//�e�N�X�`���[�p�T���v���[�쐬
		D3D11_SAMPLER_DESC SamDesc;
		ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

		SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		if FAILED(DEVICE->CreateSamplerState(&SamDesc, &mpSampleLinear))
			Koban::DebugLib::error(L"�T���v���[�X�e�[�g�̐ݒ�Ɏ��s");
	}

	BaseMaterial::~BaseMaterial() {
		SAFE_RELEASE(mpIndexBuffer);
		SAFE_RELEASE(mpTexture);
	}

	void BaseMaterial::addIndexBuffer(const int indexBuffer[], int size) const {
		//�T�C�Y�ύX���ĂԂ�����
		auto nowSize = mpIndexBuffer_Tmp.get()->size();
		mpIndexBuffer_Tmp.get()->resize(nowSize + size);
		for (size_t i = 0; i < size/3; i++)
		{
			(*mpIndexBuffer_Tmp.get())[nowSize + (i * 3)] = indexBuffer[i * 3];
			(*mpIndexBuffer_Tmp.get())[nowSize + (i * 3 + 1)] = indexBuffer[i * 3 + 1];
			(*mpIndexBuffer_Tmp.get())[nowSize + (i * 3 + 2)] = indexBuffer[i * 3 + 2];
		}
	}

	void BaseMaterial::createIndexBuffer() {
		//�C���f�b�N�X�o�b�t�@�[���쐬
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * mpIndexBuffer_Tmp.get()->size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = mpIndexBuffer_Tmp.get();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(DEVICE->CreateBuffer(&bd, &InitData, &mpIndexBuffer))) {
			Koban::DebugLib::error(L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s");
		}
		mpIndexBuffer_Tmp.reset();
	}

	void BaseMaterial::draw() {
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		DEVICE_CONTEXT->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (mpTexture != NULL)
		{
			DEVICE_CONTEXT->PSSetSamplers(0, 1, &mpSampleLinear);
			DEVICE_CONTEXT->PSSetShaderResources(0, 1, &mpTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			DEVICE_CONTEXT->PSSetShaderResources(0, 1, Nothing);
		}
		//�����_�����O
		//�v���~�e�B�u�������_�����O
		DEVICE_CONTEXT->DrawIndexed(mFaceNum * 3, 0, 0);
	}
}