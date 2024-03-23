#include "BaseMesh.h"
#include "Render.h"
#include "Camera.h"
#include <regex>

namespace Koban {
	BaseMesh::BaseMesh() :
		mModelPath{ L"Resource/Chips.obj" },
		mVertCount{ new VERTEX_COUNT() }
	{
	}

	BaseMesh::~BaseMesh()
	{
		SAFE_RELEASE(mpVertexBuffer);
		SAFE_RELEASE(mpSampleLinear);
		mShaderDic.clear();
	}

	void BaseMesh::start()
	{
		if (FAILED(loadResources(mModelPath)))
		{
			DebugLib::error(L"���b�V���쐬���s");
		}
	}

	HRESULT BaseMesh::loadResources(std::wstring meshFileName)
	{
		//--------------OBJ�t�@�C�����J���ē��e��ǂݍ���-----------------
		wchar_t key[200] = { 0 };
		FILE* fp = NULL;
		_wfopen_s(&fp, meshFileName.data(), L"rt");

		//���_���J�E���g
		loadVertCount(fp);
		//�}�e���A�����[�h
		loadMaterial(fp);

		//���_���̃��[�h
		unique_ptr<D3DXVECTOR3> pvCoord = unique_ptr<D3DXVECTOR3>(new D3DXVECTOR3[mVertCount.get()->vtCount]);
		unique_ptr <D3DXVECTOR3> pvNormal = unique_ptr<D3DXVECTOR3>(new D3DXVECTOR3[mVertCount.get()->normCount]);
		unique_ptr <D3DXVECTOR2> pvUV = unique_ptr <D3DXVECTOR2>(new D3DXVECTOR2[mVertCount.get()->uvCount]);
		loadVert(fp, pvCoord.get(), pvNormal.get(), pvUV.get());

		//���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�̍쐬
		createVtxBufAndIdxBuf(fp, pvCoord.get(), pvNormal.get(), pvUV.get());

		fclose(fp);
		//-------------------------------------------------------------

		return S_OK;
	}

	/// <summary>
	/// todo:shader�̍X�V�͌p����ł��K�v������
	/// </summary>
	void BaseMesh::draw()
	{
		//auto camera = Render::getCamera();

		//D3DXMATRIX worldMat, transMat, yawMat, pitchMat, rollMat, scaleMat;
		//worldMat = scaleMat * yawMat * pitchMat * rollMat * transMat;
		//auto lightDir = D3DXVECTOR3(1, -1, 1);

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
		//	shader.updateBaseConstantBuffer(worldMat, camera->getViewMat(), camera->getProjMat(), lightDir, camera->getEyeDir());

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

#pragma region  �������܂��
	void BaseMesh::loadVertCount(FILE* fp) {
		wchar_t key[200] = { 0 };
		fseek(fp, SEEK_SET, 0);
		while (!feof(fp))
		{
			auto size = 201 * sizeof(wchar_t);
			//�L�[���[�h�ǂݍ���
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//���_
			if (wcscmp(key, L"v") == 0)
			{
				mVertCount.get()->vtCount++;
			}
			//�@��
			if (wcscmp(key, L"vn") == 0)
			{
				mVertCount.get()->uvCount++;
			}
			//�e�N�X�`���[���W
			if (wcscmp(key, L"vt") == 0)
			{
				mVertCount.get()->normCount++;
			}
			//�t�F�C�X�i�|���S���j
			if (wcscmp(key, L"f") == 0)
			{
				mVertCount.get()->polyCount++;
			}
		}
	}

	void BaseMesh::loadMaterial(FILE* fp) {
		wchar_t key[200] = { 0 };
		fseek(fp, SEEK_SET, 0);
		while (!feof(fp))
		{
			auto size = 201 * sizeof(wchar_t);
			//�L�[���[�h�ǂݍ���
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//�}�e���A���ǂݍ���
			if (wcscmp(key, L"mtllib") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				//virtual�Ŏq�̊֐����Ă�
				createMaterialFromFile(key);
			}
		}
	}

	void BaseMesh::loadVert(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV) {
		//�{�ǂݍ���	
		fseek(fp, SEEK_SET, 0);
		int vertCount = 0;
		int normCount = 0;
		int uvCount = 0;
		float x, y, z;
		wchar_t key[200] = { 0 };

		while (!feof(fp))
		{
			//�L�[���[�h �ǂݍ���
			ZeroMemory(key, sizeof(key));
			fwscanf_s(fp, L"%s ", key, _countof(key));

			//���_ �ǂݍ���
			if (wcscmp(key, L"v") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
				pvCoord[vertCount].x = -x;
				pvCoord[vertCount].y = y;
				pvCoord[vertCount].z = z;
				vertCount++;
			}

			//�@�� �ǂݍ���
			if (wcscmp(key, L"vn") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
				pvNormal[normCount].x = -x;
				pvNormal[normCount].y = y;
				pvNormal[normCount].z = z;
				normCount++;
			}

			//uv �ǂݍ���
			if (wcscmp(key, L"vt") == 0)
			{
				fwscanf_s(fp, L"%f %f", &x, &y);
				pvUV[uvCount].x = x;
				pvUV[uvCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
				uvCount++;
			}
		}
	}

	void BaseMesh::createVtxBufAndIdxBuf(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV) {
		BaseShader::MY_VERTEX* vertexBuffer = new BaseShader::MY_VERTEX[mVertCount.get()->polyCount * 3];
		unique_ptr<int[]> materialIndexBuffer;

		wchar_t key[200] = { 0 };
		bool boFlag = false;
		int polygonCount = 0;
		int indexCount = 0;
		int v1 = 0, v2 = 0, v3 = 0;
		int vn1 = 0, vn2 = 0, vn3 = 0;
		int vt1 = 0, vt2 = 0, vt3 = 0;

		auto targetMatName = L"";

		fseek(fp, SEEK_SET, 0);
		while (!feof(fp))
		{
			//�L�[���[�h �ǂݍ���
			ZeroMemory(key, sizeof(key));
			fwscanf_s(fp, L"%s ", key, _countof(key));

			//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
			if (wcscmp(key, L"usemtl") == 0)
			{
				setIndexBuffer(targetMatName, materialIndexBuffer.get(), indexCount * 3);

				fwscanf_s(fp, L"%s ", key, _countof(key));
				targetMatName = key;
				materialIndexBuffer.reset(new int[mVertCount.get()->polyCount * 3]);//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
				indexCount = 0;
			}
			if (wcscmp(key, L"f") == 0 && boFlag == true)
			{
				wregex format(L"(%d/%d/%d %d/%d/%d %d/%d/%d)");//regex�I�u�W�F�N�g�Ő��K�\����p������ʉ�
				bool result = regex_match(key, format);//�t�H�[�}�b�g�`�F�b�N�̌��ʂ�Ԃ�
				if (result) {
					//�e�N�X�`���[����T�[�t�F�C�X
					fwscanf_s(fp, L"%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else {
					//�e�N�X�`���[�����T�[�t�F�C�X
					fwscanf_s(fp, L"%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
				}
				//���_�\���̂ɑ��
				vertexBuffer[polygonCount * 3].mPos = pvCoord[v1 - 1];
				vertexBuffer[polygonCount * 3].mNorm = pvNormal[vn1 - 1];
				vertexBuffer[polygonCount * 3].mUV = pvUV[vt1 - 1];
				vertexBuffer[polygonCount * 3 + 1].mPos = pvCoord[v2 - 1];
				vertexBuffer[polygonCount * 3 + 1].mNorm = pvNormal[vn2 - 1];
				vertexBuffer[polygonCount * 3 + 1].mUV = pvUV[vt2 - 1];
				vertexBuffer[polygonCount * 3 + 2].mPos = pvCoord[v3 - 1];
				vertexBuffer[polygonCount * 3 + 2].mNorm = pvNormal[vn3 - 1];
				vertexBuffer[polygonCount * 3 + 2].mUV = pvUV[vt3 - 1];

				materialIndexBuffer.get()[indexCount * 3] = polygonCount * 3;
				materialIndexBuffer.get()[indexCount * 3 + 1] = polygonCount * 3 + 1;
				materialIndexBuffer.get()[indexCount * 3 + 2] = polygonCount * 3 + 2;

				polygonCount++;
				indexCount++;
			}
		}

		setVertexBuffer(vertexBuffer, polygonCount + 3);
		setIndexBuffer(targetMatName, materialIndexBuffer.get(), indexCount);
		materialIndexBuffer.release();

		//indexBuffer�𐶐�
		for (auto item : mShaderDic) {
			auto matDic = item.second.getMaterials();
			for (auto mat : matDic) {
				mat->createIndexBuffer();
			}
		}
	}

	void BaseMesh::setIndexBuffer(std::wstring materialName, const int indexBuffer[], int bufferSize) {
		if (materialName == L"" || bufferSize == 0)
			return;

		//shader����Material���m���Ă�̂ŁA�Sshader��T�����ĒT��
		for (auto pair : mShaderDic) {
			auto baseShader = &pair.second;
			auto baseMat = baseShader->getMaterial(materialName);
			if (baseMat != nullptr) {
				baseMat->addIndexBuffer(indexBuffer, bufferSize);
				return;
			}
		}
	}

	void BaseMesh::setVertexBuffer(const BaseShader::MY_VERTEX* const vertBuf, int bufferSize) {
		//�Sshader�œ���vertexBuffer��p����
		for (auto pair : mShaderDic) {
			auto baseShader = &pair.second;
			baseShader->createVertexBuffer(vertBuf, bufferSize);
		}
	}

#pragma endregion
}