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
		//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
		FILE* fp = NULL;
		_wfopen_s(&fp, FileName.data(), L"rt");
		wchar_t key[110] = { 0 };
		D3DXVECTOR4 v(0, 0, 0, 1);

		//�{�ǂݍ���	
		fseek(fp, SEEK_SET, 0);

		auto pMaterial = new TestMaterial();

		while (!feof(fp))
		{
			//�L�[���[�h�ǂݍ���
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//�}�e���A����
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
			//Ka�@�A���r�G���g
			if (wcscmp(key, L"Ka") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mConstantBufferVal.get()->mAmbient = v;
			}
			//Kd�@�f�B�t���[�Y
			if (wcscmp(key, L"Kd") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mConstantBufferVal.get()->mDiffuse = v;
			}
			//Ks�@�X�y�L�����[
			if (wcscmp(key, L"Ks") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mConstantBufferVal.get()->mSpecular = v;
			}
			//map_Kd�@�e�N�X�`���[
			if (wcscmp(key, L"map_Kd") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->mTextureName = key;
			}

		}
		fclose(fp);

		auto shaderPath = pMaterial->shaderPath;
		//shader���̂�������Βǉ�
		if (!mShaderDic.contains(shaderPath)) {
			mShaderDic[shaderPath] = *(new TestShader());
		}
		//material��ǉ�
		mShaderDic[shaderPath].addMaterial(pMaterial);
	}

	void TestMesh::update()
	{
		mYaw += 0.0001; //��]
		D3DXMATRIX worldMat, transMat, yawMat, pitchMat, rollMat, scaleMat;
		//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
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