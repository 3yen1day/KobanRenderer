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
		//material毎にupdateする
		for (auto& mat : mMaterialDic)
		{
			static_cast<TestMaterial*>(mat.second)->update();
		}

		//親のdrawを呼び出す
		BaseShader::update();
	}

	void TestShader::draw() {
		//material毎にdrawする
		for (auto& mat : mMaterialDic)
		{
			static_cast<TestMaterial*>(mat.second)->draw();
		}

		//親のdrawを呼び出す
		BaseShader::draw();
	}
}