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
				pMaterial->mKa = v;
			}
			//Kd�@�f�B�t���[�Y
			if (wcscmp(key, L"Kd") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mKd = v;
			}
			//Ks�@�X�y�L�����[
			if (wcscmp(key, L"Ks") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mKs = v;
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

	void TestMesh::Render(
		D3DXMATRIX& viewMat,
		D3DXMATRIX& projMat,
		D3DXVECTOR3& light,
		D3DXVECTOR3& eye)
	{
		//mYaw += 0.0001; //��]
		//D3DXMATRIX worldMat, transMat, yawMat, pitchMat, rollMat, scaleMat;
		////���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		//D3DXMatrixScaling(&scaleMat, mScale, mScale, mScale);
		//D3DXMatrixRotationY(&yawMat, mYaw);
		//D3DXMatrixRotationX(&pitchMat, mPitch);
		//D3DXMatrixRotationZ(&rollMat, mRoll);
		//D3DXMatrixTranslation(&transMat, mPos.x, mPos.y, mPos.z);

		//worldMat = scaleMat * yawMat * pitchMat * rollMat * transMat;

		//for (auto shaderItr = mShaderDic.begin(); shaderItr != mShaderDic.end(); ++shaderItr)
		//{
		//	auto shader = shaderItr->second;
		//	auto matList = mMaterialDic[shaderItr->first];
		//	if (matList == nullptr || matList->size() == 0) {
		//		continue;
		//	}

		//	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		//	UINT stride = sizeof(MY_VERTEX);
		//	UINT offset = 0;
		//	DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

		//	shader.update();
		//	shader.updateBaseConstantBuffer(worldMat, viewMat, projMat, light, eye);

		//	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
		//	for (auto matItr = matList->begin(); matItr != matList->end(); ++matItr)
		//	{
		//		auto targetMat = *matItr;
		//		//�g�p����Ă��Ȃ��}�e���A���΍�
		//		if (targetMat->mFaceNum == 0)
		//		{
		//			continue;
		//		}
		//		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		//		stride = sizeof(int);
		//		offset = 0;
		//		DEVICE_CONTEXT->IASetIndexBuffer(targetMat->mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//		TestShader::SIMPLECONSTANT_BUFFER1 sg;
		//		sg.mAmbient = targetMat->mKa;//�A���r�G���g�����V�F�[�_�[�ɓn��
		//		sg.mDiffuse = targetMat->mKd;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
		//		sg.mSpecular = targetMat->mKs;//�X�y�L�����[���V�F�[�_�[�ɓn��
		//		shader.updateBuffer(sg);

		//		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		//		if (targetMat->mTextureName[0] != NULL)
		//		{
		//			DEVICE_CONTEXT->PSSetSamplers(0, 1, &mpSampleLinear);
		//			DEVICE_CONTEXT->PSSetShaderResources(0, 1, &targetMat->mpTexture);
		//		}
		//		else
		//		{
		//			ID3D11ShaderResourceView* Nothing[1] = { 0 };
		//			DEVICE_CONTEXT->PSSetShaderResources(0, 1, Nothing);
		//		}
		//		//�v���~�e�B�u�������_�����O
		//		DEVICE_CONTEXT->DrawIndexed(targetMat->mFaceNum * 3, 0, 0);
		//	}
		//}
	}
}