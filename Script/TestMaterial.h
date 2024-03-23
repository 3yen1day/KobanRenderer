#pragma once
#include "BaseMaterial.h"

namespace Koban {
	
	
	struct TestMaterial:public BaseMaterial
	{
	public:
		
		D3DXVECTOR4 mKa;//アンビエント
		D3DXVECTOR4 mKd;//ディフューズ
		D3DXVECTOR4 mKs;//スペキュラー

		TestMaterial()
		{
		}

		~TestMaterial();
	};
}