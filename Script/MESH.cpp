#include "MESH.h"

//
//
//
MESH::MESH()
{
	ZeroMemory(this, sizeof(MESH));
	m_fScale = 1.0f;
	MODEL_PATH = L"Resource/Chips.obj";
}

MESH::~MESH()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSampleLinear);
	m_Shader.clear();
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

HRESULT MESH::LoadResources(LPWSTR meshFileName)
{
	float x, y, z;
	int v1 = 0, v2 = 0, v3 = 0;
	int vn1 = 0, vn2 = 0, vn3 = 0;
	int vt1 = 0, vt2 = 0, vt3 = 0;
	DWORD dwVCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVNCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwVTCount = 0;//�ǂݍ��݃J�E���^�[
	DWORD dwFCount = 0;//�ǂݍ��݃J�E���^�[

	char key[200] = { 0 };
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	_wfopen_s(&fp, meshFileName, L"rt");

	//���O�ɒ��_���A�|���S�����𒲂ׂ�
	while (!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A���ǂݍ���
		if (strcmp(key, "mtllib") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			LoadMaterialFromFile(key);
		}
		//���_
		if (strcmp(key, "v") == 0)
		{
			m_dwNumVert++;
		}
		//�@��
		if (strcmp(key, "vn") == 0)
		{
			dwVNCount++;
		}
		//�e�N�X�`���[���W
		if (strcmp(key, "vt") == 0)
		{
			dwVTCount++;
		}
		//�t�F�C�X�i�|���S���j
		if (strcmp(key, "f") == 0)
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
		fscanf_s(fp, "%s ", key, sizeof(key));

		//���_ �ǂݍ���
		if (strcmp(key, "v") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = -x;
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;
			dwVCount++;
		}

		//�@�� �ǂݍ���
		if (strcmp(key, "vn") == 0)
		{
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = -x;
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(key, "vt") == 0)
		{
			fscanf_s(fp, "%f %f", &x, &y);
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

	for (auto shaderItr = m_Shader.begin(); shaderItr != m_Shader.end(); ++shaderItr)
	{
		for (auto matItr = shaderItr->second->begin(); matItr != shaderItr->second->end(); ++matItr)
		{
			auto targetMat = *matItr;
			dwPartFCount = 0;
			fseek(fp, SEEK_SET, 0);

			while (!feof(fp))
			{
				//�L�[���[�h �ǂݍ���
				ZeroMemory(key, sizeof(key));
				fscanf_s(fp, "%s ", key, sizeof(key));

				//�t�F�C�X �ǂݍ��݁����_�C���f�b�N�X��
				if (strcmp(key, "usemtl") == 0)
				{
					fscanf_s(fp, "%s ", key, sizeof(key));
					if (strcmp(key, targetMat->szName) == 0)
					{
						boFlag = true;
					}
					else
					{
						boFlag = false;
					}
				}
				if (strcmp(key, "f") == 0 && boFlag == true)
				{
					if (targetMat->pTexture != NULL)//�e�N�X�`���[����T�[�t�F�C�X
					{
						fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
					}
					else//�e�N�X�`���[�����T�[�t�F�C�X
					{
						fscanf_s(fp, "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
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

HRESULT MESH::LoadMaterialFromFile(LPSTR FileName)
{
	//�}�e���A���t�@�C�����J���ē��e��ǂݍ���
	FILE* fp = NULL;
	fopen_s(&fp, FileName, "rt");
	char key[110] = { 0 };
	D3DXVECTOR4 v(0, 0, 0, 1);

	//�{�ǂݍ���	
	fseek(fp, SEEK_SET, 0);
	INT iMCount = -1;

	while (!feof(fp))
	{
		MY_MATERIAL* pMaterial = new MY_MATERIAL();
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%s ", key, sizeof(key));
		//�}�e���A����
		if (strcmp(key, "newmtl") == 0)
		{
			iMCount++;
			fscanf_s(fp, "%s ", key, sizeof(key));
			strcpy_s(pMaterial->szName, key);
		}
		//Ka�@�A���r�G���g
		if (strcmp(key, "Ka") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->Ka = v;
		}
		//Kd�@�f�B�t���[�Y
		if (strcmp(key, "Kd") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->Kd = v;
		}
		//Ks�@�X�y�L�����[
		if (strcmp(key, "Ks") == 0)
		{
			fscanf_s(fp, "%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->Ks = v;
		}
		//map_Kd�@�e�N�X�`���[
		if (strcmp(key, "map_Kd") == 0)
		{
			fscanf_s(fp, "%s", &pMaterial->szTextureName, sizeof(pMaterial->szTextureName));
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, pMaterial->szTextureName, NULL, NULL, &pMaterial->pTexture, NULL)))
			{
				return E_FAIL;
			}
		}

		// shader
		if (strcmp(key, "shaderPath") == 0)
		{
			fscanf_s(fp, "%s ", key, sizeof(key));
			wchar_t wtext[110];
			mbstowcs(wtext, key, strlen(key) + 1);//Plus null
			pMaterial->shaderPath = wtext;
			bool isExistShader = false;
			// shader�����݂��Ă����炻���ɒǉ�
			if (m_Shader.empty() == false) {
				for (auto itr = m_Shader.begin(); itr != m_Shader.end(); ++itr)
				{
					if (&itr->first.m_ShaderPath == wtext) {
						isExistShader = true;
						itr->second->push_back(pMaterial);
						break;
					}
				}
			}
			// ���݂��ĂȂ���ΐV�����쐬
			if (!isExistShader) {
				auto shader = new cShader(wtext, m_pDevice, m_pDeviceContext);
				auto list = new std::list<MY_MATERIAL*>;
				list->push_back(pMaterial);
				m_Shader[shader] = list;
			}
		}
	}
	fclose(fp);
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
		auto shader = (shaderItr->first);

		//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		shader->render(mWorld, mView, mProj, vLight, vEye);

		//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
		for (auto matItr = shaderItr->second->begin(); matItr != shaderItr->second->end(); ++matItr)
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
			//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
			D3D11_MAPPED_SUBRESOURCE mappedSubResource;
			//�o�b�t�@�̒��g���X�V���邽�߂ɁAmap, unmap���g�p���遨lock, unlock�̂悤�Ȃ���
			if (SUCCEEDED(m_pDeviceContext->Map(shader->m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource)))
			{
				SIMPLECONSTANT_BUFFER1 sg;
				sg.vAmbient = targetMat->Ka;//�A���r�G���g�����V�F�[�_�[�ɓn��
				sg.vDiffuse = targetMat->Kd;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
				sg.vSpecular = targetMat->Ks;//�X�y�L�����[���V�F�[�_�[�ɓn��
				memcpy_s(mappedSubResource.pData, mappedSubResource.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER1));
				m_pDeviceContext->Unmap(shader->m_pConstantBuffer1, 0);
			}
			m_pDeviceContext->VSSetConstantBuffers(1, 1, &shader->m_pConstantBuffer1);
			m_pDeviceContext->PSSetConstantBuffers(1, 1, &shader->m_pConstantBuffer1);
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

cShader::cShader(LPWSTR shaderPath, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	ZeroMemory(this, sizeof(cShader));
	m_ShaderPath = shaderPath;
	m_pDevice = device;
	m_pDeviceContext = deviceContext;
	if (FAILED(initShader()))
	{
		MessageBox(0, L"���b�V���p�V�F�[�_�[�쐬���s", NULL, MB_OK);
	}
}

HRESULT cShader::initShader() {
	if (m_ShaderPath == nullptr || m_ShaderPath == L"") {
		MessageBox(0, L"shaderPath���s��", NULL, MB_OK);
		return E_FAIL;
	}
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3D10Blob* pCompiledShader = NULL;
	ID3D10Blob* pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(m_ShaderPath, NULL, NULL, "VS", "vs_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(m_ShaderPath, NULL, NULL, "PS", "ps_4_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer0)))
	{
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�[�쐬  �}�e���A���n���p
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLECONSTANT_BUFFER1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer1)))
	{
		return E_FAIL;
	}

	return S_OK;
}