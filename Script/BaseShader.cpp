#include "BaseMaterial.h"
#include"BaseShader.h"
#include "Render.h"
#include "Camera.h"
#include "Light.h"

namespace Koban {
	BaseShader::BaseShader(std::wstring path) :
		mpVertexLayout(nullptr),
		mpConstantBuffer0(nullptr),
		mpVertexShader(nullptr),
		mpPixelShader(nullptr),
		mShaderPath(path)
	{
		initShader();
	}

	BaseShader::~BaseShader() {
		mMaterialDic.clear();
		SAFE_RELEASE(mpVertexLayout);
		SAFE_RELEASE(mpConstantBuffer0);
		SAFE_RELEASE(mpVertexShader);
		SAFE_RELEASE(mpPixelShader);
	}

	void BaseShader::initShader() {
		if (mShaderPath == L"") {
			DebugLib::error(L"shaderPath���s��");
			return;
		}
		//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
		ID3D10Blob* pCompiledShader = NULL;
		ID3D10Blob* pErrors = NULL;
		//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "VS", "vs_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			DebugLib::error(L"hlsl�ǂݍ��ݎ��s");
			return;
		}
		SAFE_RELEASE(pErrors);

		if (FAILED(DEVICE->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpVertexShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			DebugLib::error(L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s");
			return;
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
			DebugLib::error(L"�C���v�b�g���C�A�E�g�쐬���s");
			return;
		}
		//�u���u����s�N�Z���V�F�[�_�[�쐬
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "PS", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			DebugLib::error(L"hlsl�ǂݍ��ݎ��s");
			return;
		}
		SAFE_RELEASE(pErrors);
		if (FAILED(DEVICE->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpPixelShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			DebugLib::error(L"�s�N�Z���V�F�[�_�[�쐬���s");
			return;
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
			DebugLib::error(L"�o�b�t�@�쐬���s");
			return;
		}

		//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
		/*cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER1);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &mpConstantBuffer1)))
		{
			return E_FAIL;
		}*/
	}

	void BaseShader::update() {
		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//�g�p����V�F�[�_�[�̓o�^	
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//�V�F�[�_�[�ɃR���X�^���g�o�b�t�@��o�^
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		//���_�C���v�b�g���C�A�E�g���Z�b�g
		DEVICE_CONTEXT->IASetInputLayout(mpVertexLayout);
		//�v���~�e�B�u�E�g�|���W�[���Z�b�g
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//material����update����
		for (auto& mat : mMaterialDic)
		{
			mat.second->update();
		}
	}

	void BaseShader::draw() {
		//material����update����
		for (auto& mat : mMaterialDic)
		{
			mat.second->draw();
		}
	}

	void BaseShader::updateBaseConstantBuffer
	(
		const D3DXMATRIX& worldMat
	) 
	{
		auto eye = Render::getCamera()->getPostion();
		auto viewMat = Render::getCamera()->getViewMat();
		auto projMat = Render::getCamera()->getProjMat();
		auto light = Render::getLight()->getDirection();

		SIMPLECONSTANT_BUFFER0 sg;
		//���[���h�s���n��
		sg.mW = worldMat;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);
		//���[���h�A�J�����A�ˉe�s���n��
		sg.mWVP = worldMat * viewMat * projMat;
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
		//���C�g�̕�����n��
		sg.mLightDir = D3DXVECTOR4(light.x, light.y, light.z, 0.0f);
		//���_�ʒu��n��
		sg.mEyePos = D3DXVECTOR4(eye.x, eye.y, eye.z, 0);

		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
			DEVICE_CONTEXT->Unmap(mpConstantBuffer0, 0);
		}
	}

	void BaseShader::addMaterial(BaseMaterial* material) {
		mMaterialDic[material->mName] = material;

		char c[110] = { 0 };
		wcstombs(c, material->mTextureName.data(), material->mTextureName.length());
		//�e�N�X�`���[���쐬
		if (FAILED(D3DX11CreateShaderResourceViewFromFileA(DEVICE, c, NULL, NULL, &(material->mpTexture), NULL)))
		{
			Koban::DebugLib::error(L"�e�N�X�`���̍쐬�Ɏ��s");
		}
	}

	void BaseShader::setIndexBuffer(std::wstring materialName, const std::vector<int> indexBuffer, int bufferSize) {
		if (materialName == L"" || bufferSize == 0)
			return;

		if (mMaterialDic.contains(materialName)) {
			mMaterialDic[materialName]->addIndexBuffer(indexBuffer, bufferSize);
		}
	}

	void BaseShader::createIndexBuffer() {
		for (auto& mat : mMaterialDic) {
			mat.second->createIndexBuffer();
		}
	}

	void BaseShader::createVertexBuffer(const std::vector<BaseShader::MY_VERTEX> const vertexBuffer, int polyNum) {
		//�o�[�e�b�N�X�o�b�t�@�[���쐬
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * polyNum * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertexBuffer.data();

		if (FAILED(DEVICE->CreateBuffer(&bd, &InitData, &mpVertexBuffer)))
			Koban::DebugLib::error(L"�o�b�t�@�̍쐬�Ɏ��s");
	}
}