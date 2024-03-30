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
		//material–ˆ‚Éupdate‚·‚é
		for (auto& mat : mMaterialDic)
		{
			static_cast<TestMaterial*>(mat.second)->update();
		}

		//e‚Ìdraw‚ğŒÄ‚Ño‚·
		BaseShader::update();
	}

	void TestShader::draw() {
		//material–ˆ‚Édraw‚·‚é
		for (auto& mat : mMaterialDic)
		{
			static_cast<TestMaterial*>(mat.second)->draw();
		}
	}
}