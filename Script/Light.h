#pragma once
#include "Object.h"

namespace Koban {
	class Light : public Object
	{
	public:
		Light():mDirection(*new D3DXVECTOR4(0,-1,0,1)){};
		~Light() {};

		//Direction���擾
		D3DXVECTOR4 getDirection() { return mDirection; }
	private:
		D3DXVECTOR4 mDirection;
	};
}