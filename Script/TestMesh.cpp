#include "TestMesh.h"
#include "Render.h"

namespace Koban {
	TestMaterial::~TestMaterial() {
	}

	TestMesh::TestMesh() :
		mPos{ *new D3DXVECTOR3(0,0,0) },
		mYaw{ 0 },
		mPitch{ 0 },
		mRoll{ 0 },
		mScale{ 1.0f },
		mModelPath{ L"Resource/Chips.obj" }
	{
	}

	TestMesh::~TestMesh()
	{
	}

	void TestMesh::createMaterialFromFile(std::wstring FileName) {
		//マテリアルファイルを開いて内容を読み込む
		FILE* fp = NULL;
		_wfopen_s(&fp, FileName.data(), L"rt");
		wchar_t key[110] = { 0 };
		D3DXVECTOR4 v(0, 0, 0, 1);

		//本読み込み	
		fseek(fp, SEEK_SET, 0);

		auto pMaterial = new TestMaterial();

		while (!feof(fp))
		{
			//キーワード読み込み
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//マテリアル名
			if (wcscmp(key, L"newmtl") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->mName = key;
			}
			// shader
			if (wcscmp(key, L"shaderPath") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->shaderPath = key;
			}
			//Ka　アンビエント
			if (wcscmp(key, L"Ka") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mConstantBufferVal.get()->mAmbient = v;
			}
			//Kd　ディフューズ
			if (wcscmp(key, L"Kd") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mConstantBufferVal.get()->mDiffuse = v;
			}
			//Ks　スペキュラー
			if (wcscmp(key, L"Ks") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mConstantBufferVal.get()->mSpecular = v;
			}
			//map_Kd　テクスチャー
			if (wcscmp(key, L"map_Kd") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->mTextureName = key;
			}

		}
		fclose(fp);

		auto shaderPath = pMaterial->shaderPath;
		//shader自体が無ければ追加
		if (!mShaderDic.contains(shaderPath)) {
			mShaderDic[shaderPath] = *(new TestShader());
		}
		//materialを追加
		mShaderDic[shaderPath].addMaterial(pMaterial);
	}

	void TestMesh::update()
	{
		mYaw += 0.0001; //回転
		D3DXMATRIX worldMat, transMat, yawMat, pitchMat, rollMat, scaleMat;
		//ワールドトランスフォーム（絶対座標変換）
		D3DXMatrixScaling(&scaleMat, mScale, mScale, mScale);
		D3DXMatrixRotationY(&yawMat, mYaw);
		D3DXMatrixRotationX(&pitchMat, mPitch);
		D3DXMatrixRotationZ(&rollMat, mRoll);
		D3DXMatrixTranslation(&transMat, mPos.x, mPos.y, mPos.z);

		worldMat = scaleMat * yawMat * pitchMat * rollMat * transMat;

		//update
		for (auto& shader : mShaderDic) {
			auto testShader = static_cast<TestShader*>(&shader.second);
			testShader->updateBaseConstantBuffer(worldMat);
			testShader->update();
		}
	}

	void TestMesh::draw() {
		for (auto& shader : mShaderDic) {
			auto testShader = static_cast<TestShader*>(&shader.second);
			testShader->draw();
		}
	}
}