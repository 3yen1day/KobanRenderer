#include "TestMesh.h"
#include "Render.h"

namespace Koban {
	TestMaterial::~TestMaterial() {
		SAFE_RELEASE(mpTexture);
	}

	TestMesh::TestMesh() :
		mVertNum{ 0 },
		mFaceNum{ 0 },
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
		SAFE_RELEASE(mpVertexBuffer);
		SAFE_RELEASE(mpSampleLinear);
		mShaderDic.clear();
		mMaterialDic.clear();
	}

	HRESULT TestMesh::init()
	{
		if (FAILED(loadResources(mModelPath)))
		{
			MessageBox(0, L"���b�V���쐬���s", NULL, MB_OK);
			return E_FAIL;
		}
		return S_OK;
	}

	HRESULT TestMesh::loadResources(std::wstring meshFileName)
	{
		float x, y, z;
		int v1 = 0, v2 = 0, v3 = 0;
		int vn1 = 0, vn2 = 0, vn3 = 0;
		int vt1 = 0, vt2 = 0, vt3 = 0;
		uint32_t dwVCount = 0;//�ǂݍ��݃J�E���^�[
		uint32_t dwVNCount = 0;//�ǂݍ��݃J�E���^�[
		uint32_t dwVTCount = 0;//�ǂݍ��݃J�E���^�[
		uint32_t dwFCount = 0;//�ǂݍ��݃J�E���^�[

		wchar_t key[200] = { 0 };
		//OBJ�t�@�C�����J���ē��e��ǂݍ���
		FILE* fp = NULL;
		_wfopen_s(&fp, meshFileName.data(), L"rt");
		//���O�ɒ��_���A�|���S�����𒲂ׂ�
		while (!feof(fp))
		{
			auto size = 201 * sizeof(wchar_t);
			//�L�[���[�h�ǂݍ���
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//�}�e���A���ǂݍ���
			if (wcscmp(key, L"mtllib") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				loadMaterialFromFile(key);
			}
			//���_
			if (wcscmp(key, L"v") == 0)
			{
				mVertNum++;
			}
			//�@��
			if (wcscmp(key, L"vn") == 0)
			{
				dwVNCount++;
			}
			//�e�N�X�`���[���W
			if (wcscmp(key, L"vt") == 0)
			{
				dwVTCount++;
			}
			//�t�F�C�X�i�|���S���j
			if (wcscmp(key, L"f") == 0)
			{
				mFaceNum++;
			}
		}

		//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
		MY_VERTEX* pvVertexBuffer = new MY_VERTEX[mFaceNum * 3];
		D3DXVECTOR3* pvCoord = new D3DXVECTOR3[mVertNum];
		D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
		D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];

		//�{�ǂݍ���	
		fseek(fp, SEEK_SET, 0);
		dwVCount = 0;
		dwVNCount = 0;
		dwVTCount = 0;
		dwFCount = 0;

		while (!feof(fp))
		{
			//�L�[���[�h �ǂݍ���
			ZeroMemory(key, sizeof(key));
			fwscanf_s(fp, L"%s ", key, _countof(key));

			//���_ �ǂݍ���
			if (wcscmp(key, L"v") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
				pvCoord[dwVCount].x = -x;
				pvCoord[dwVCount].y = y;
				pvCoord[dwVCount].z = z;
				dwVCount++;
			}

			//�@�� �ǂݍ���
			if (wcscmp(key, L"vn") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
				pvNormal[dwVNCount].x = -x;
				pvNormal[dwVNCount].y = y;
				pvNormal[dwVNCount].z = z;
				dwVNCount++;
			}

			//�e�N�X�`���[���W �ǂݍ���
			if (wcscmp(key, L"vt") == 0)
			{
				fwscanf_s(fp, L"%f %f", &x, &y);
				pvTexture[dwVTCount].x = x;
				pvTexture[dwVTCount].y = 1 - y;//OBJ�t�@�C����Y�������t�Ȃ̂ō��킹��
				dwVTCount++;
			}
		}

		//�t�F�C�X�@�ǂݍ��݁@�o���o���Ɏ��^����Ă���\��������̂ŁA�}�e���A�����𗊂�ɂȂ����킹��
		bool boFlag = false;
		int* piFaceBuffer = new int[mFaceNum * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
		dwFCount = 0;
		uint32_t dwPartFCount = 0;

		for (auto matDicItr = mMaterialDic.begin(); matDicItr != mMaterialDic.end(); ++matDicItr)
		{
			for (auto matListItr = matDicItr->second->begin(); matListItr != matDicItr->second->end(); ++matListItr)
			{
				auto targetMat = *matListItr;
				dwPartFCount = 0;
				fseek(fp, SEEK_SET, 0);

				while (!feof(fp))
				{
					//�L�[���[�h �ǂݍ���
					ZeroMemory(key, sizeof(key));
					fwscanf_s(fp, L"%s ", key, _countof(key));

					//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
					if (wcscmp(key, L"usemtl") == 0)
					{
						fwscanf_s(fp, L"%s ", key, _countof(key));
						if (wcscmp(key, targetMat->mName.data()) == 0)
						{
							boFlag = true;
						}
						else
						{
							boFlag = false;
						}
					}
					if (wcscmp(key, L"f") == 0 && boFlag == true)
					{
						if (targetMat->mpTexture != NULL)//�e�N�X�`���[����T�[�t�F�C�X
						{
							fwscanf_s(fp, L"%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
						}
						else//�e�N�X�`���[�����T�[�t�F�C�X
						{
							fwscanf_s(fp, L"%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
						}

						//�C���f�b�N�X�o�b�t�@�[
						piFaceBuffer[dwPartFCount * 3] = dwFCount * 3;
						piFaceBuffer[dwPartFCount * 3 + 1] = dwFCount * 3 + 1;
						piFaceBuffer[dwPartFCount * 3 + 2] = dwFCount * 3 + 2;
						//���_�\���̂ɑ��
						pvVertexBuffer[dwFCount * 3].mPos = pvCoord[v1 - 1];
						pvVertexBuffer[dwFCount * 3].mNorm = pvNormal[vn1 - 1];
						pvVertexBuffer[dwFCount * 3].mUV = pvTexture[vt1 - 1];
						pvVertexBuffer[dwFCount * 3 + 1].mPos = pvCoord[v2 - 1];
						pvVertexBuffer[dwFCount * 3 + 1].mNorm = pvNormal[vn2 - 1];
						pvVertexBuffer[dwFCount * 3 + 1].mUV = pvTexture[vt2 - 1];
						pvVertexBuffer[dwFCount * 3 + 2].mPos = pvCoord[v3 - 1];
						pvVertexBuffer[dwFCount * 3 + 2].mNorm = pvNormal[vn3 - 1];
						pvVertexBuffer[dwFCount * 3 + 2].mUV = pvTexture[vt3 - 1];

						dwPartFCount++;
						dwFCount++;

					}
				}
				if (dwPartFCount == 0)//�g�p����Ă��Ȃ��}�e���A���΍�
				{
					targetMat->mpIndexBuffer = NULL;
					continue;
				}

				//�C���f�b�N�X�o�b�t�@�[���쐬
				D3D11_BUFFER_DESC bd;
				bd.Usage = D3D11_USAGE_DEFAULT;
				bd.ByteWidth = sizeof(int) * dwPartFCount * 3;
				bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
				bd.CPUAccessFlags = 0;
				bd.MiscFlags = 0;
				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = piFaceBuffer;
				if (FAILED(DEVICE->CreateBuffer(&bd, &InitData, &targetMat->mpIndexBuffer)))
					return FALSE;
				targetMat->mFaceNum = dwPartFCount;
			}
		}

		delete[] piFaceBuffer;
		fclose(fp);

		//�o�[�e�b�N�X�o�b�t�@�[���쐬
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(MY_VERTEX) * mFaceNum * 3;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pvVertexBuffer;

		if (FAILED(DEVICE->CreateBuffer(&bd, &InitData, &mpVertexBuffer)))
			return FALSE;

		//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
		delete pvCoord;
		delete pvNormal;
		delete pvTexture;
		delete[] pvVertexBuffer;

		//�e�N�X�`���[�p�T���v���[�쐬
		D3D11_SAMPLER_DESC SamDesc;
		ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

		SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DEVICE->CreateSamplerState(&SamDesc, &mpSampleLinear);

		return S_OK;
	}

	HRESULT TestMesh::loadMaterialFromFile(std::wstring FileName)
	{
		//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
		FILE* fp = NULL;
		_wfopen_s(&fp, FileName.data(), L"rt");
		wchar_t key[110] = { 0 };
		D3DXVECTOR4 v(0, 0, 0, 1);

		//�{�ǂݍ���	
		fseek(fp, SEEK_SET, 0);

		auto pMaterial = new std::vector<TestMaterial*>();
		int matCount = -1;

		while (!feof(fp))
		{
			//�L�[���[�h�ǂݍ���
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//�}�e���A����
			if (wcscmp(key, L"newmtl") == 0)
			{
				matCount++;
				pMaterial->push_back(new TestMaterial());
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->at(matCount)->mName = key;
			}
			// shader
			if (wcscmp(key, L"shaderPath") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->at(matCount)->shaderPath = key;
			}
			//Ka�@�A���r�G���g
			if (wcscmp(key, L"Ka") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->at(matCount)->mKa = v;
			}
			//Kd�@�f�B�t���[�Y
			if (wcscmp(key, L"Kd") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->at(matCount)->mKd = v;
			}
			//Ks�@�X�y�L�����[
			if (wcscmp(key, L"Ks") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->at(matCount)->mKs = v;
			}
			//map_Kd�@�e�N�X�`���[
			if (wcscmp(key, L"map_Kd") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->at(matCount)->mTextureName = key;
			}

		}
		fclose(fp);

		for (auto i = 0; i < pMaterial->size(); i++)
		{
			// shader�̒ǉ�
			auto wstrPath = pMaterial->at(i)->shaderPath;
			if (mShaderDic.size() == 0 || mShaderDic.find(wstrPath) == mShaderDic.end())
			{
				auto shader = new TestShader(wstrPath);
				//std::pair<std::wstring, cShader> shaderPair = std::pair<std::wstring, cShader>(*path, *shader);
				//m_Shader.insert(shaderPair);
				mShaderDic[wstrPath] = *shader;
			}
			// material�̒ǉ�
			auto matItr = mMaterialDic.find(wstrPath);
			auto mat = pMaterial->at(i);

			char c[110] = { 0 };
			wcstombs(c, mat->mTextureName.data(), mat->mTextureName.length());
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(DEVICE, c, NULL, NULL, &mat->mpTexture, NULL)))
			{
				return E_FAIL;
			}
			if (matItr == mMaterialDic.end()) {
				mMaterialDic[wstrPath] = new std::list<TestMaterial*>();
				mMaterialDic[wstrPath]->push_back(mat);
			}
			else
			{
				matItr->second->push_back(mat);
			}
		}
		return S_OK;
	}

	void TestMesh::Render(D3DXMATRIX& mViewMat, D3DXMATRIX& mProjMat,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye)
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

		for (auto shaderItr = mShaderDic.begin(); shaderItr != mShaderDic.end(); ++shaderItr)
		{
			auto shader = shaderItr->second;
			auto matList = mMaterialDic[shaderItr->first];
			if (matList == nullptr || matList->size() == 0) {
				continue;
			}

			//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
			UINT stride = sizeof(MY_VERTEX);
			UINT offset = 0;
			DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

			shader.Render(worldMat, mViewMat, mProjMat, vLight, vEye);

			//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
			for (auto matItr = matList->begin(); matItr != matList->end(); ++matItr)
			{
				auto targetMat = *matItr;
				//�g�p����Ă��Ȃ��}�e���A���΍�
				if (targetMat->mFaceNum == 0)
				{
					continue;
				}
				//�C���f�b�N�X�o�b�t�@�[���Z�b�g
				stride = sizeof(int);
				offset = 0;
				DEVICE_CONTEXT->IASetIndexBuffer(targetMat->mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


				TestShader::SIMPLECONSTANT_BUFFER1 sg;
				sg.mAmbient = targetMat->mKa;//�A���r�G���g�����V�F�[�_�[�ɓn��
				sg.mDiffuse = targetMat->mKd;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
				sg.mSpecular = targetMat->mKs;//�X�y�L�����[���V�F�[�_�[�ɓn��
				shader.setBuffer(sg);

				//�e�N�X�`���[���V�F�[�_�[�ɓn��
				if (targetMat->mTextureName[0] != NULL)
				{
					DEVICE_CONTEXT->PSSetSamplers(0, 1, &mpSampleLinear);
					DEVICE_CONTEXT->PSSetShaderResources(0, 1, &targetMat->mpTexture);
				}
				else
				{
					ID3D11ShaderResourceView* Nothing[1] = { 0 };
					DEVICE_CONTEXT->PSSetShaderResources(0, 1, Nothing);
				}
				//�v���~�e�B�u�������_�����O
				DEVICE_CONTEXT->DrawIndexed(targetMat->mFaceNum * 3, 0, 0);
			}
		}
	}
}