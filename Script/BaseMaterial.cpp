#include "BaseMaterial.h"
#include "Render.h"

namespace Koban {
	BaseMaterial::~BaseMaterial() {
		SAFE_RELEASE(mpIndexBuffer);
		SAFE_RELEASE(mpTexture);
	}

	void BaseMaterial::addIndexBuffer(const int indexBuffer[], int size) const {
		//サイズ変更してぶち込む
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
		//インデックスバッファーを作成
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
			Koban::DebugLib::error(L"インデックスバッファの作成に失敗");
		}
		mpIndexBuffer_Tmp.reset();
	}
}