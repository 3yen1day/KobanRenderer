#include "BaseMaterial.h"

namespace Koban {
	BaseMaterial::~BaseMaterial() {
		SAFE_DELETE(mpIndexBuffer);
	}

	void BaseMaterial::addIndexBuffer(const int indexBuffer[], int size) const {
		//サイズ変更してぶち込む
		//vector使えないっぽいので、普通の配列で頑張る
		auto nowSize = mpIndexBuffer.size();
		mpIndexBuffer.size = nowSize + size;
	}
}