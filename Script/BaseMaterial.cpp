#include "BaseMaterial.h"

namespace Koban {
	BaseMaterial::~BaseMaterial() {
		SAFE_RELEASE(mpIndexBuffer);
	}
}