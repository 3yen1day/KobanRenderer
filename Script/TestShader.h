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
			D3DXVECTOR4 mAmbient;//�A���r�G���g��
			D3DXVECTOR4 mDiffuse;//�f�B�t���[�Y�F
			D3DXVECTOR4 mSpecular;//���ʔ���
		};

		void initShader() override;
		void update() override;
		void draw() override;

	private:
		
	};
}
