#pragma once

namespace Koban {
	class Light
	{
	public:
		Light():mDirection(*new D3DXVECTOR4(0,-1,0,1)){};
		~Light() {};

		//Direction‚ðŽæ“¾
		D3DXVECTOR4 getDirection() { return mDirection; }
	private:
		D3DXVECTOR4 mDirection;
	};
}