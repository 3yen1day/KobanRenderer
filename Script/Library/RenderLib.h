#pragma once

namespace Koban {
	static class RenderLib {
	public:
		/// <summary>
		/// Shaderの生成
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
			const std::wstring& vsName,
			const std::wstring& psName,
			ID3D11VertexShader** vs,
			ID3D11PixelShader** ps) 
		{
			//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
			std::unique_ptr<ID3D10Blob> upCompiledShader = NULL;
			std::unique_ptr<ID3D10Blob> upErrors = NULL;
			ID3D10Blob* pCompiledShader = upCompiledShader.get();
			ID3D10Blob* pErrors = upErrors.get();

			//vertexShader
			createVertexShader(device, fileName, vsName, vs, &pCompiledShader, &pErrors);

			//pixcelShader
			createPixcelShader(device, fileName, psName, ps, &pCompiledShader, &pErrors);
		}

		/// <summary>
		/// Shaderの生成
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
			ID3D11PixelShader** ps) 
		{
			//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
			std::unique_ptr<ID3D10Blob> upCompiledShader = NULL;
			std::unique_ptr<ID3D10Blob> upErrors = NULL;
			ID3D10Blob* pCompiledShader = upCompiledShader.get();
			ID3D10Blob* pErrors = upErrors.get();

			//vertexShader
			createVertexShader(device, fileName, vsName, vs, &pCompiledShader, &pErrors);

			//頂点インプットレイアウトを作成
			if (FAILED(device->CreateInputLayout(vertLayout.data(), vertLayout.size(), pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), vl)))
			{
				string info = "インプットレイアウト作成失敗";
				string error = (char*)pErrors->GetBufferPointer();
				DebugLib::error(info + error);
			}

			//pixcelShader
			createPixcelShader(device, fileName, psName, ps, &pCompiledShader, &pErrors);
		}

		 /// <summary>
		 /// コンスタントバッファを作成
		 /// </summary>
		 template <typename T> static void createConstantBuffer(ID3D11Device* device, ID3D11Buffer** buffer) {
			//コンスタントバッファー作成　ここでは変換行列渡し用
			D3D11_BUFFER_DESC cb;
			cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cb.ByteWidth = sizeof(T);
			cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cb.MiscFlags = 0;
			cb.Usage = D3D11_USAGE_DYNAMIC;

			if (FAILED(device->CreateBuffer(&cb, NULL, buffer)))
			{
				DebugLib::error(L"バッファ作成失敗");
			}
		}

		/// <summary>
		/// 頂点バッファを作成
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
				 DebugLib::error(L"バッファ作成失敗");
		 }

		 /// <summary>
		 /// インデックスバッファを作成
		 /// </summary>
		 /// <param name="deviceContext"></param>
		 /// <param name="indexBuffer"></param>
		 static void createIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<int> indexis, ID3D11Buffer** indexBuffer) {
			 //インデックスバッファーを作成
			 D3D11_BUFFER_DESC bd;
			 bd.Usage = D3D11_USAGE_DEFAULT;
			 bd.ByteWidth = sizeof(int) * indexis.size();
			 bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			 bd.CPUAccessFlags = 0;
			 bd.MiscFlags = 0;

			 D3D11_SUBRESOURCE_DATA InitData;
			 InitData.pSysMem = indexis.data();
			 if (FAILED(device->CreateBuffer(&bd, &InitData, indexBuffer)))
				 DebugLib::error(L"バッファ作成失敗");
		 }

		/// <summary>
		/// テクスチャ用サンプラーを作成
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
		/// テクスチャを作成
		/// </summary>
		 static void createTexture(ID3D11Device* device, wstring filePath, ID3D11ShaderResourceView** texture) {
			 if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, filePath.c_str(), NULL, NULL, texture, NULL)))
			 {
				DebugLib::error(L"テクスチャの作成に失敗");
			 }
		 }

		 /// <summary>
		 /// 頂点シェーダ作成
		 /// </summary>
		 /// <param name="device"></param>
		 /// <param name="fileName"></param>
		 /// <param name="vsName"></param>
		 /// <param name="vs"></param>
		 /// <param name="pCompiledShader"></param>
		 /// <param name="pErrors"></param>
		 static void createVertexShader(
			 ID3D11Device* device,
			 const std::wstring& fileName,
			 const std::wstring& vsName,
			 ID3D11VertexShader** vs,
			 ID3D10Blob** pCompiledShader,
			 ID3D10Blob** pErrors)
		 {
			 //vertexShader
			 auto shaderName_s = StringLib::wstr2str(vsName);

			 //ブロブからvertexShader作成
			 if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "vs_5_0", 0, 0, NULL, pCompiledShader, pErrors, NULL)))
			 {
				 string info = "hlsl読み込み失敗";
				 string error = (char*)(*pErrors)->GetBufferPointer();
				 DebugLib::error(info + error);
			 }
			 if (FAILED(device->CreateVertexShader((*pCompiledShader)->GetBufferPointer(), (*pCompiledShader)->GetBufferSize(), NULL, vs)))
			 {
				 string info = "バーテックスシェーダー作成失敗";
				 string error = (char*)(*pErrors)->GetBufferPointer();
				 DebugLib::error(info + error);
			 }
		 }

		 /// <summary>
		 /// ピクセルシェーダー作成
		 /// </summary>
		 /// <param name="device"></param>
		 /// <param name="fileName"></param>
		 /// <param name="psName"></param>
		 /// <param name="ps"></param>
		 /// <param name="pCompiledShader"></param>
		 /// <param name="pErrors"></param>
		 static void createPixcelShader(
			 ID3D11Device* device,
			 const std::wstring& fileName,
			 const std::wstring& psName,
			 ID3D11PixelShader** ps,
			 ID3D10Blob** pCompiledShader,
			 ID3D10Blob** pErrors)
		 {
			 //vertexShader
			 auto shaderName_s = StringLib::wstr2str(psName);

			 //ブロブからpixcelShader作成
			 if (FAILED(D3DX11CompileFromFile(fileName.data(), NULL, NULL, shaderName_s->data(), "ps_5_0", 0, 0, NULL, pCompiledShader, pErrors, NULL)))
			 {
				 string info = "hlsl読み込み失敗";
				 string error = (char*)(*pErrors)->GetBufferPointer();
				 DebugLib::error(info + error);
			 }
			 if (FAILED(device->CreatePixelShader((*pCompiledShader)->GetBufferPointer(), (*pCompiledShader)->GetBufferSize(), NULL, ps)))
			 {
				 string info = "ピクセルシェーダー作成失敗";
				 string error = (char*)(*pErrors)->GetBufferPointer();
				 DebugLib::error(info + error);
			 }
		 }
	};
}