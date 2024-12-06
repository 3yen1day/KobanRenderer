#pragma once
#include "../Core/Component.h"

namespace Koban {
	class Light : public Component
	{
	public:
		Light():
			mDirection(*new D3DXVECTOR4(1,1,0,1)),
			mPosition(*new D3DXVECTOR4(2, 2, 0, 1))
		{};
		~Light() {};

		void start() override {};
		void update() override {};
		void draw() override {};
		void destroy() override {};

		//Direction‚ðŽæ“¾
		D3DXVECTOR4 getDirection() { return mDirection; }
		D3DXVECTOR4 getPosition() { return mPosition; }

	private:
		D3DXVECTOR4 mDirection;
		D3DXVECTOR4 mPosition;
	};
}