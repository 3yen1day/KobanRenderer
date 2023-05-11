#include "MESH.h"

//
//
//
MESH::MESH() :
	m_dwNumVert{ 0 },
	m_dwNumFace{ 0 },
	m_fYaw{ 0 },
	m_fPitch{ 0 },
	m_fRoll{ 0 },
	m_fScale{ 1.0f },
	MODEL_PATH{ L"Resource/Chips.obj" }
{
}

MESH::~MESH()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSampleLinear);
	m_Shader.clear();
	m_Material.clear();
}

HRESULT MESH::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pContext;


	if (FAILED(LoadResources(MODEL_PATH)))
	{
		MessageBox(0, L"���b�V���쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT MESH::LoadResources(std::wstring meshFileName)
{
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVTCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[

	wchar_t key[200] = { 0 };
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	_wfopen_s(&fp, meshFileName.data(), L"rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fwscanf_s(fp, L"%s ", key, sizeof(key));
		//�}�e���A���ǂݍ���
		if (wcscmp(key, L"mtllib") == 0)
		{
			fwscanf_s(fp, L"%s ", key, sizeof(key));
			LoadMaterialFromFile(key);
		}
		//���_
		if (wcscmp(key, L"v") == 0)
		{
			m_dwNumVert++;
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
			m_dwNumFace++;
		}
	}

	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	MY_VERTEX* pvVertexBuffer = new MY_VERTEX[m_dwNumFace * 3];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[m_dwNumVert];
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
		fwscanf_s(fp, L"%s ", key, sizeof(key));

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
	int* piFaceBuffer = new int[m_dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)
	dwFCount = 0;
	DWORD dwPartFCount = 0;

	for (auto matDicItr = m_Material.begin(); matDicItr != m_Material.end(); ++matDicItr)
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
				fwscanf_s(fp, L"%s ", key, sizeof(key));

				//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
				if (wcscmp(key, L"usemtl") == 0)
				{
					fwscanf_s(fp, L"%s ", key, sizeof(key));
					if (wcscmp(key, targetMat->szName.data()) == 0)
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
					if (targetMat->pTexture != NULL)//�e�N�X�`���[����T�[�t�F�C�X
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
					pvVertexBuffer[dwFCount * 3].vPos = pvCoord[v1 - 1];
					pvVertexBuffer[dwFCount * 3].vNorm = pvNormal[vn1 - 1];
					pvVertexBuffer[dwFCount * 3].vUV = pvTexture[vt1 - 1];
					pvVertexBuffer[dwFCount * 3 + 1].vPos = pvCoord[v2 - 1];
					pvVertexBuffer[dwFCount * 3 + 1].vNorm = pvNormal[vn2 - 1];
					pvVertexBuffer[dwFCount * 3 + 1].vUV = pvTexture[vt2 - 1];
					pvVertexBuffer[dwFCount * 3 + 2].vPos = pvCoord[v3 - 1];
					pvVertexBuffer[dwFCount * 3 + 2].vNorm = pvNormal[vn3 - 1];
					pvVertexBuffer[dwFCount * 3 + 2].vUV = pvTexture[vt3 - 1];

					dwPartFCount++;
					dwFCount++;

				}
			}
			if (dwPartFCount == 0)//�g�p����Ă��Ȃ��}�e���A���΍�
			{
				targetMat->m_pIndexBuffer = NULL;
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
			if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &targetMat->m_pIndexBuffer)))
				return FALSE;
			targetMat->dwNumFace = dwPartFCount;
		}
	}

	delete[] piFaceBuffer;
	fclose(fp);

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX) * m_dwNumFace * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;

	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
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
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return S_OK;
}

HRESULT MESH::LoadMaterialFromFile(std::wstring FileName)
{
	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	_wfopen_s(&fp, FileName.data(), L"rt");
	wchar_t key[110] = { 0 };
	D3DXVECTOR4 v(0, 0, 0, 1);

	//�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);

	auto pMaterial = new std::vector<MY_MATERIAL*>();
	int matCount = -1;

	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fwscanf_s(fp, L"%s ", key, sizeof(key));
		//�}�e���A����
		if (wcscmp(key, L"newmtl") == 0)
		{
			matCount++;
			pMaterial->push_back(new MY_MATERIAL());
			fwscanf_s(fp, L"%s ", pMaterial->at(matCount)->szName, sizeof(pMaterial->at(matCount)->szName));
		}
		// shader
		if (wcscmp(key, L"shaderPath") == 0)
		{
			fwscanf_s(fp, L"%s ", pMaterial->at(matCount)->shaderPath, sizeof(pMaterial->at(matCount)->shaderPath));
		}
		//Ka�@�A���r�G���g
		if (wcscmp(key, L"Ka") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->at(matCount)->Ka = v;
		}
		//Kd�@�f�B�t���[�Y
		if (wcscmp(key, L"Kd") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->at(matCount)->Kd = v;
		}
		//Ks�@�X�y�L�����[
		if (wcscmp(key, L"Ks") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->at(matCount)->Ks = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (wcscmp(key, L"map_Kd") == 0)
		{
			fwscanf_s(fp, L"%s", &pMaterial->at(matCount)->szTextureName, sizeof(pMaterial->at(matCount)->szTextureName));
		}

	}
	fclose(fp);

	for (auto i = 0; i < pMaterial->size(); i++)
	{
		// shader�̒ǉ�
		auto wstrPath = pMaterial->at(i)->shaderPath;
		if (m_Shader.size() == 0 || m_Shader.find(wstrPath) == m_Shader.end())
		{
			auto shader = new cShader(wstrPath, m_pDevice, m_pDeviceContext);
			//std::pair<std::wstring, cShader> shaderPair = std::pair<std::wstring, cShader>(*path, *shader);
			//m_Shader.insert(shaderPair);
			m_Shader[wstrPath] = *shader;
		}
		// material�̒ǉ�
		auto matItr = m_Material.find(wstrPath);
		auto mat = pMaterial->at(i);

		char* c;
		wcstombs(c, mat->szTextureName.data(), sizeof(mat->szTextureName.data()));
		//�e�N�X�`���[���쐬
		if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, c, NULL, NULL, &mat->pTexture, NULL)))
		{
			return E_FAIL;
		}
		if (matItr == m_Material.end()) {
			m_Material[wstrPath] = new std::list<MY_MATERIAL*>();
			m_Material[wstrPath]->push_back(mat);
		}
		else
		{
			matItr->second->push_back(mat);
		}
	}
	return S_OK;
}

void MESH::Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye)
{
	m_fYaw += 0.0001; //��]
	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	D3DXMatrixScaling(&mScale, m_fScale, m_fScale, m_fScale);
	D3DXMatrixRotationY(&mYaw, m_fYaw);
	D3DXMatrixRotationX(&mPitch, m_fPitch);
	D3DXMatrixRotationZ(&mRoll, m_fRoll);
	D3DXMatrixTranslation(&mTran, m_vPos.x, m_vPos.y, m_vPos.z);

	mWorld = mScale * mYaw * mPitch * mRoll * mTran;

	for (auto shaderItr = m_Shader.begin(); shaderItr != m_Shader.end(); ++shaderItr)
	{
		auto shader = shaderItr->second;
		auto matList = m_Material[shaderItr->first];
		if (matList == nullptr || matList->size() == 0) {
			continue;
		}

		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		shader.render(mWorld, mView, mProj, vLight, vEye);

		//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
		for (auto matItr = matList->begin(); matItr != matList->end(); ++matItr)
		{
			auto targetMat = *matItr;
			//�g�p����Ă��Ȃ��}�e���A���΍�
			if (targetMat->dwNumFace == 0)
			{
				continue;
			}
			//�C���f�b�N�X�o�b�t�@�[���Z�b�g
			stride = sizeof(int);
			offset = 0;
			m_pDeviceContext->IASetIndexBuffer(targetMat->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


			cShader::SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient = targetMat->Ka;//�A���r�G���g�����V�F�[�_�[�ɓn��
			sg.vDiffuse = targetMat->Kd;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			sg.vSpecular = targetMat->Ks;//�X�y�L�����[���V�F�[�_�[�ɓn��
			shader.setBuffer(sg);

			//�e�N�X�`���[���V�F�[�_�[�ɓn��
			if (targetMat->szTextureName[0] != NULL)
			{
				m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
				m_pDeviceContext->PSSetShaderResources(0, 1, &targetMat->pTexture);
			}
			else
			{
				ID3D11ShaderResourceView* Nothing[1] = { 0 };
				m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);
			}
			//�v���~�e�B�u�������_�����O
			m_pDeviceContext->DrawIndexed(targetMat->dwNumFace * 3, 0, 0);
		}
	}

}

//cShader::cShader(LPWSTR shaderPath, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
//	ZeroMemory(this, sizeof(cShader));
//	m_ShaderPath = shaderPath;
//	m_pDevice = device;
//	m_pDeviceContext = deviceContext;
//	if (FAILED(initShader()))
//	{
//		MessageBox(0, L"���b�V���p�V�F�[�_�[�쐬���s", NULL, MB_OK);
//	}
//}