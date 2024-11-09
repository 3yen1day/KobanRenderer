#pragma once

namespace Koban {
	class Light
	{
	public:
		Light():
			mDirection(*new D3DXVECTOR4(1,1,0,1)),
			mPosition(*new D3DXVECTOR4(2, 2, 0, 1))
		{};
		~Light() {};

		//Direction‚ðŽæ“¾
		D3DXVECTOR4 getDirection() { return mDirection; }
		D3DXVECTOR4 getPosition() { return mPosition; }

	private:
		D3DXVECTOR4 mDirection;
		D3DXVECTOR4 mPosition;
	};
}