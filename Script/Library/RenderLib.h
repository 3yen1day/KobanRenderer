#pragma once

namespace Koban {
	static class RenderLib {
	public:
		/// <summary>
		/// Shader�̐���
		/// </summary>
		/// <param name="device"></param>
		/// <param name="fileName"></param>
		/// <param name="vsName"></param>
		/// <param name="vertLayout"></param>
		/// <param name="psName"></param>
		/// <param name="vs"></param>
		/// <param name="vl"></param>
		/// <param name="ps"></param>
		static void createShader(
			ID3D11Device* device,
			const std::wstring& fileName,
			const vector<D3D11_INPUT_ELEMENT_DESC>& vertLayout,
			const std::wstring& vsName,
			const std::wstring& psName,
			ID3D11InputLayout** vl,
			ID3D11VertexShader** vs,
			ID3D11PixelShader** ps) {

			//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
			std::unique_ptr<ID3D10Blob> upCompiledShader = NULL;
			std::unique_ptr<ID3D10Blob> upErrors = NULL;
			ID3D10Blob* pCompiledShader = upCompiledShader.get();
			ID3D10Blob* pErrors = upErrors.get();
			
			//vertexShader
			auto shaderName_s = StringLib::wstr2str(vsName);

			//�u���u����vertexShader�쐬
			if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
			{
				DebugLib::error(L"hlsl�ǂݍ��ݎ��s");
			}
			if (FAILED(device->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, vs)))
			{
				DebugLib::error(L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s");
			}

			//���_�C���v�b�g���C�A�E�g���쐬
			if (FAILED(device->CreateInputLayout(vertLayout.data(), vertLayout.size(), pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), vl)))
			{
				DebugLib::error(L"�C���v�b�g���C�A�E�g�쐬���s");
			}


			//pixcelShader
			shaderName_s = StringLib::wstr2str(psName);

			//�u���u����pixcelShader�쐬
			if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
			{
				DebugLib::error(L"hlsl�ǂݍ��ݎ��s");
			}
			if (FAILED(device->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, ps)))
			{
				DebugLib::error(L"�s�N�Z���V�F�[�_�[�쐬���s");
			}
		}

		 /// <summary>
		 /// �R���X�^���g�o�b�t�@���쐬
		 /// </summary>
		 template <typename T> static void createConstantBuffer(ID3D11Device* device, ID3D11Buffer** buffer) {
			//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
			D3D11_BUFFER_DESC cb;
			cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cb.ByteWidth = sizeof(T);
			cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cb.MiscFlags = 0;
			cb.StructureByteStride = 0;
			cb.Usage = D3D11_USAGE_DYNAMIC;

			if (FAILED(device->CreateBuffer(&cb, NULL, buffer)))
			{
				DebugLib::error(L"�o�b�t�@�쐬���s");
			}
		}

		/// <summary>
		/// ���_�o�b�t�@���쐬
		/// </summary>
		 template <typename T> static void createVertexBffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<T> vertices, ID3D11Buffer** vertexBuffer) {
			 D3D11_BUFFER_DESC bd;
			 bd.Usage = D3D11_USAGE_DEFAULT;
			 bd.ByteWidth = sizeof(T) * vertices.size();
			 bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			 bd.CPUAccessFlags = 0;
			 bd.MiscFlags = 0;

			 D3D11_SUBRESOURCE_DATA InitData;
			 InitData.pSysMem = vertices.data();
			 if (FAILED(device->CreateBuffer(&bd, &InitData, vertexBuffer)))
				 DebugLib::error(L"�o�b�t�@�쐬���s");
		 }

		 /// <summary>
		 /// �C���f�b�N�X�o�b�t�@���쐬
		 /// </summary>
		 /// <param name="deviceContext"></param>
		 /// <param name="indexBuffer"></param>
		 static void createIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<int> indexis, ID3D11Buffer** indexBuffer) {
			 //�C���f�b�N�X�o�b�t�@�[���쐬
			 D3D11_BUFFER_DESC bd;
			 bd.Usage = D3D11_USAGE_DEFAULT;
			 bd.ByteWidth = sizeof(int) * indexis.size();
			 bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			 bd.CPUAccessFlags = 0;
			 bd.MiscFlags = 0;

			 D3D11_SUBRESOURCE_DATA InitData;
			 InitData.pSysMem = indexis.data();
			 if (FAILED(device->CreateBuffer(&bd, &InitData, indexBuffer)))
				 DebugLib::error(L"�o�b�t�@�쐬���s");
		 }

		/// <summary>
		/// �e�N�X�`���p�T���v���[���쐬
		/// </summary>
		 static void createSamplerState(ID3D11Device* device, ID3D11SamplerState** sampleLinear) {
			 D3D11_SAMPLER_DESC SamDesc;
			 ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
			 SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			 SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			 SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			 SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			 device->CreateSamplerState(&SamDesc, sampleLinear);
		 }

		/// <summary>
		/// �e�N�X�`�����쐬
		/// </summary>
		 static void createTexture(ID3D11Device* device, wstring filePath, ID3D11ShaderResourceView** texture) {
			 if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, filePath.c_str(), NULL, NULL, texture, NULL)))
			 {
				DebugLib::error(L"�e�N�X�`���̍쐬�Ɏ��s");
			 }
		 }
	};
}