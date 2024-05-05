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
			DebugLib::error(L"shaderPathが不正");
			return;
		}
		//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
		ID3D10Blob* pCompiledShader = NULL;
		ID3D10Blob* pErrors = NULL;
		//ブロブからバーテックスシェーダー作成
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "VS", "vs_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			DebugLib::error(L"hlsl読み込み失敗");
			return;
		}
		SAFE_RELEASE(pErrors);

		if (FAILED(DEVICE->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpVertexShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			DebugLib::error(L"バーテックスシェーダー作成失敗");
			return;
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
			DebugLib::error(L"インプットレイアウト作成失敗");
			return;
		}
		//ブロブからピクセルシェーダー作成
		if (FAILED(D3DX11CompileFromFile(mShaderPath.data(), NULL, NULL, "PS", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			DebugLib::error(L"hlsl読み込み失敗");
			return;
		}
		SAFE_RELEASE(pErrors);
		if (FAILED(DEVICE->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &mpPixelShader)))
		{
			SAFE_RELEASE(pCompiledShader);
			DebugLib::error(L"ピクセルシェーダー作成失敗");
			return;
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
			DebugLib::error(L"バッファ作成失敗");
			return;
		}

		//コンスタントバッファー作成  マテリアル渡し用
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
		//バーテックスバッファーをセット
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//使用するシェーダーの登録	
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);
		//シェーダーにコンスタントバッファを登録
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer0);
		//頂点インプットレイアウトをセット
		DEVICE_CONTEXT->IASetInputLayout(mpVertexLayout);
		//プリミティブ・トポロジーをセット
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//material毎にupdateする
		for (auto& mat : mMaterialDic)
		{
			mat.second->update();
		}
	}

	void BaseShader::draw() {
		//material毎にupdateする
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
		//ワールド行列を渡す
		sg.mW = worldMat;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);
		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = worldMat * viewMat * projMat;
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
		//ライトの方向を渡す
		sg.mLightDir = D3DXVECTOR4(light.x, light.y, light.z, 0.0f);
		//視点位置を渡す
		sg.mEyePos = D3DXVECTOR4(eye.x, eye.y, eye.z, 0);

		//シェーダーのコンスタントバッファーに各種データを渡す
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
		//テクスチャーを作成
		if (FAILED(D3DX11CreateShaderResourceViewFromFileA(DEVICE, c, NULL, NULL, &(material->mpTexture), NULL)))
		{
			Koban::DebugLib::error(L"テクスチャの作成に失敗");
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
		//バーテックスバッファーを作成
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * polyNum * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertexBuffer.data();

		if (FAILED(DEVICE->CreateBuffer(&bd, &InitData, &mpVertexBuffer)))
			Koban::DebugLib::error(L"バッファの作成に失敗");
	}
}