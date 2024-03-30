#include "TestMaterial.h"
#include "TestShader.h"
#include "Render.h"

namespace Koban {
	TestMaterial::TestMaterial() : 
		mConstantBufferVal(std::make_unique<TestShader::SIMPLECONSTANT_BUFFER1>())
	{
		D3D11_BUFFER_DESC cb;
		//コンスタントバッファー作成  マテリアル渡し用
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(TestShader::SIMPLECONSTANT_BUFFER1);
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(DEVICE->CreateBuffer(&cb, NULL, &mpConstantBuffer)))
		{
			DebugLib::error(L"コンスタントバッファ作成失敗");
			return;
		}
	}

	void TestMaterial::update() {
		//コンスタントバッファー更新
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			TestShader::SIMPLECONSTANT_BUFFER1 sg;
			sg = *mConstantBufferVal.get();
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(TestShader::SIMPLECONSTANT_BUFFER1));
			DEVICE_CONTEXT->Unmap(mpConstantBuffer, 0);
		}
		DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &mpConstantBuffer);
		DEVICE_CONTEXT->PSSetConstantBuffers(1, 1, &mpConstantBuffer);
	}
}