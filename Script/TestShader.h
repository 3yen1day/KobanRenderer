#pragma once
#include "BaseShader.h"

namespace Koban {
	class TestShader: public BaseShader {
	public:
		TestShader() {};
		TestShader(std::wstring path);
		~TestShader();

		struct SIMPLECONSTANT_BUFFER1
		{
			D3DXVECTOR4 mAmbient;//アンビエント光
			D3DXVECTOR4 mDiffuse;//ディフューズ色
			D3DXVECTOR4 mSpecular;//鏡面反射
		};

		void initShader() override;
		void update() override;
		void draw() override;

	private:
		
	};
}
