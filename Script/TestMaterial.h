#pragma once
#include "BaseMaterial.h"
#include "TestShader.h"

namespace Koban {
	struct TestMaterial:public BaseMaterial
	{
	public:
		TestMaterial();
		~TestMaterial();
		void update() override;

		unique_ptr<TestShader::SIMPLECONSTANT_BUFFER1> mConstantBufferVal;

	private:
		//�R���X�^���g�o�b�t�@
		ID3D11Buffer* mpConstantBuffer;
	};
}