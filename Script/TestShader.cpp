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

	///マテリアルの各要素をエフェクト（シェーダー）に渡す
	void TestShader::setBuffer(SIMPLECONSTANT_BUFFER1 buffer) {
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;

		//バッファの中身を更新するために、map, unmapを使用する→lock, unlockのようなもの
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
		//使用するシェーダーの登録	
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//シェーダーのコンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SIMPLECONSTANT_BUFFER0 sg;
			//ワールド行列を渡す
			sg.mW = mWorld;
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			//ワールド、カメラ、射影行列を渡す
			sg.mWVP = mWorld * mViewMat * mProjMat;
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			//ライトの方向を渡す
			sg.mLightDir = D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);
			//視点位置を渡す
			sg.mEyePos = D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 0);

			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
			DEVICE_CONTEXT->Unmap(mpConstantBuffer0, 0);
		}
		//このコンスタントバッファーを使うシェーダーの登録
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		//頂点インプットレイアウトをセット
		DEVICE_CONTEXT->IASetInputLayout(mpVertexLayout);
		//プリミティブ・トポロジーをセット
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	HRESULT TestShader::initShader() {
		if (mShaderPath == L"") {
			MessageBox(0, L"shaderPathが不正", NULL, MB_OK);
			return E_FAIL;
		}
		//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
		ID3D10Blob* pCompiledShader = NULL;
		ID3D10Blob* pErrors = NULL;
		//ブロブからバーテックスシェーダー作成
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "VS", "vs_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pErrors);

		if (FAILED(DEVICE->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpVertexShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
			return E_FAIL;
		}
		//頂点インプットレイアウトを定義	
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = sizeof(layout) / sizeof(layout[0]);
		//頂点インプットレイアウトを作成
		if (FAILED(DEVICE->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &mpVertexLayout)))
		{
			return E_FAIL;
		}
		//ブロブからピクセルシェーダー作成
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "PS", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pErrors);
		if (FAILED(DEVICE->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpPixelShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pCompiledShader);

		//コンスタントバッファー作成　変換行列渡し用
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

		//コンスタントバッファー作成  マテリアル渡し用
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