#include "TestShader.h"
#include "TestMaterial.h"
#include "Render.h"

namespace Koban {
	TestShader::TestShader(std::wstring path)
	{
		initShader();
	}

	TestShader::~TestShader() {
	}

	void TestShader::initShader() {
		BaseShader::initShader();
	}

	void TestShader::update() {
		//material����update����
		for (auto& mat : mMaterialDic)
		{
			static_cast<TestMaterial*>(mat.second)->update();
		}

		//�e��draw���Ăяo��
		BaseShader::update();
	}

	void TestShader::draw() {
		//material����draw����
		for (auto& mat : mMaterialDic)
		{
			static_cast<TestMaterial*>(mat.second)->draw();
		}

		//�e��draw���Ăяo��
		BaseShader::draw();
	}
}