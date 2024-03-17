#include "BaseMaterial.h"

namespace Koban {
	BaseMaterial::~BaseMaterial() {
		SAFE_DELETE(mpIndexBuffer);
	}

	void BaseMaterial::addIndexBuffer(const int indexBuffer[], int size) const {
		//�T�C�Y�ύX���ĂԂ�����
		//vector�g���Ȃ����ۂ��̂ŁA���ʂ̔z��Ŋ撣��
		auto nowSize = mpIndexBuffer.size();
		mpIndexBuffer.size = nowSize + size;
	}
}