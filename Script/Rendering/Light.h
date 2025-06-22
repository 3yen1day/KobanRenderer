#pragma once
#include "../Core/Component.h"

namespace Koban {
	class Light : public Component
	{
	public:
		Light():
			mDirection(*new D3DXVECTOR3(1,0,1))
		{};
		~Light() {};

		void start() override {};
		void update() override {};
		void draw() override {};
		void destroy() override {};
		void drawUI() override;

		//Direction‚ðŽæ“¾
		const D3DXVECTOR3& getDirection() 
		{
			D3DXVECTOR3 ret;
			D3DXVec3Normalize(&ret, &mDirection);
			return ret;
		}

	private:
		D3DXVECTOR3 mDirection;
	};
}