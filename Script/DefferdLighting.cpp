#include "../stdafx.h"

namespace Koban {
	DefferdLighting::DefferdLighting(){
		//�V�F�[�_�[�쐬
		ID3DBlob* pCompiledShader = NULL;
		ID3DBlob* pErrors = NULL;
		if (FAILED(D3DX11CompileFromFile(mShaderFileName.data(), NULL, NULL, "VS_From_Tex", "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return;
		}

		if (FAILED(DEVICE->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpVS_From_Tex)))
		{
			MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
			return;
		}
		//�s�N�Z���V�F�[�_�[�쐬
		if (FAILED(D3DX11CompileFromFile(mShaderFileName.data(), NULL, NULL, "PS_From_Tex", "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBoxA(0, (char*)pErrors->GetBufferPointer(), NULL, MB_OK);
			MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			return;
		}

		if (FAILED(DEVICE->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpPS_From_Tex)))
		{
			MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
			return;
		}
	}
	void DefferdLighting::draw() {

	}
}