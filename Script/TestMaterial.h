#pragma once
#include "BaseMaterial.h"

namespace Koban {
	
	
	struct TestMaterial:public BaseMaterial
	{
	public:
		
		D3DXVECTOR4 mKa;//�A���r�G���g
		D3DXVECTOR4 mKd;//�f�B�t���[�Y
		D3DXVECTOR4 mKs;//�X�y�L�����[

		TestMaterial()
		{
		}

		~TestMaterial();
	};
}