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
		MessageBox(0, L"メッシュ作成失敗", NULL, MB_OK);
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
	DWORD dwVCount = 0;//読み込みカウンター
	DWORD dwVNCount = 0;//読み込みカウンター
	DWORD dwVTCount = 0;//読み込みカウンター
	DWORD dwFCount = 0;//読み込みカウンター

	wchar_t key[200] = { 0 };
	//OBJファイルを開いて内容を読み込む
	FILE* fp = NULL;
	_wfopen_s(&fp, meshFileName.data(), L"rt");

	//事前に頂点数、ポリゴン数を調べる
	while (!feof(fp))
	{
		//キーワード読み込み
		fwscanf_s(fp, L"%s ", key, sizeof(key));
		//マテリアル読み込み
		if (wcscmp(key, L"mtllib") == 0)
		{
			fwscanf_s(fp, L"%s ", key, sizeof(key));
			LoadMaterialFromFile(key);
		}
		//頂点
		if (wcscmp(key, L"v") == 0)
		{
			m_dwNumVert++;
		}
		//法線
		if (wcscmp(key, L"vn") == 0)
		{
			dwVNCount++;
		}
		//テクスチャー座標
		if (wcscmp(key, L"vt") == 0)
		{
			dwVTCount++;
		}
		//フェイス（ポリゴン）
		if (wcscmp(key, L"f") == 0)
		{
			m_dwNumFace++;
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	MY_VERTEX* pvVertexBuffer = new MY_VERTEX[m_dwNumFace * 3];
	D3DXVECTOR3* pvCoord = new D3DXVECTOR3[m_dwNumVert];
	D3DXVECTOR3* pvNormal = new D3DXVECTOR3[dwVNCount];
	D3DXVECTOR2* pvTexture = new D3DXVECTOR2[dwVTCount];

	//本読み込み	
	fseek(fp, SEEK_SET, 0);
	dwVCount = 0;
	dwVNCount = 0;
	dwVTCount = 0;
	dwFCount = 0;

	while (!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key, sizeof(key));
		fwscanf_s(fp, L"%s ", key, sizeof(key));

		//頂点 読み込み
		if (wcscmp(key, L"v") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
			pvCoord[dwVCount].x = -x;
			pvCoord[dwVCount].y = y;
			pvCoord[dwVCount].z = z;
			dwVCount++;
		}

		//法線 読み込み
		if (wcscmp(key, L"vn") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
			pvNormal[dwVNCount].x = -x;
			pvNormal[dwVNCount].y = y;
			pvNormal[dwVNCount].z = z;
			dwVNCount++;
		}

		//テクスチャー座標 読み込み
		if (wcscmp(key, L"vt") == 0)
		{
			fwscanf_s(fp, L"%f %f", &x, &y);
			pvTexture[dwVTCount].x = x;
			pvTexture[dwVTCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
			dwVTCount++;
		}
	}

	//フェイス　読み込み　バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
	bool boFlag = false;
	int* piFaceBuffer = new int[m_dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
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
				//キーワード 読み込み
				ZeroMemory(key, sizeof(key));
				fwscanf_s(fp, L"%s ", key, sizeof(key));

				//フェイス 読み込み→頂点インデックスに
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
					if (targetMat->pTexture != NULL)//テクスチャーありサーフェイス
					{
						fwscanf_s(fp, L"%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
					}
					else//テクスチャー無しサーフェイス
					{
						fwscanf_s(fp, L"%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
					}

					//インデックスバッファー
					piFaceBuffer[dwPartFCount * 3] = dwFCount * 3;
					piFaceBuffer[dwPartFCount * 3 + 1] = dwFCount * 3 + 1;
					piFaceBuffer[dwPartFCount * 3 + 2] = dwFCount * 3 + 2;
					//頂点構造体に代入
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
			if (dwPartFCount == 0)//使用されていないマテリアル対策
			{
				targetMat->m_pIndexBuffer = NULL;
				continue;
			}

			//インデックスバッファーを作成
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

	//バーテックスバッファーを作成
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

	//一時的な入れ物は、もはや不要
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;

	//テクスチャー用サンプラー作成
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
	//マテリアルファイルを開いて内容を読み込む
	FILE* fp = NULL;
	_wfopen_s(&fp, FileName.data(), L"rt");
	wchar_t key[110] = { 0 };
	D3DXVECTOR4 v(0, 0, 0, 1);

	//本読み込み	
	fseek(fp, SEEK_SET, 0);

	auto pMaterial = new std::vector<MY_MATERIAL*>();
	int matCount = -1;

	while (!feof(fp))
	{
		//キーワード読み込み
		fwscanf_s(fp, L"%s ", key, sizeof(key));
		//マテリアル名
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
		//Ka　アンビエント
		if (wcscmp(key, L"Ka") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->at(matCount)->Ka = v;
		}
		//Kd　ディフューズ
		if (wcscmp(key, L"Kd") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->at(matCount)->Kd = v;
		}
		//Ks　スペキュラー
		if (wcscmp(key, L"Ks") == 0)
		{
			fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
			pMaterial->at(matCount)->Ks = v;
		}
		//map_Kd　テクスチャー
		if (wcscmp(key, L"map_Kd") == 0)
		{
			fwscanf_s(fp, L"%s", &pMaterial->at(matCount)->szTextureName, sizeof(pMaterial->at(matCount)->szTextureName));
		}

	}
	fclose(fp);

	for (auto i = 0; i < pMaterial->size(); i++)
	{
		// shaderの追加
		auto wstrPath = pMaterial->at(i)->shaderPath;
		if (m_Shader.size() == 0 || m_Shader.find(wstrPath) == m_Shader.end())
		{
			auto shader = new cShader(wstrPath, m_pDevice, m_pDeviceContext);
			//std::pair<std::wstring, cShader> shaderPair = std::pair<std::wstring, cShader>(*path, *shader);
			//m_Shader.insert(shaderPair);
			m_Shader[wstrPath] = *shader;
		}
		// materialの追加
		auto matItr = m_Material.find(wstrPath);
		auto mat = pMaterial->at(i);

		char* c;
		wcstombs(c, mat->szTextureName.data(), sizeof(mat->szTextureName.data()));
		//テクスチャーを作成
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
	m_fYaw += 0.0001; //回転
	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;
	//ワールドトランスフォーム（絶対座標変換）
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

		//バーテックスバッファーをセット
		UINT stride = sizeof(MY_VERTEX);
		UINT offset = 0;
		m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		shader.render(mWorld, mView, mProj, vLight, vEye);

		//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
		for (auto matItr = matList->begin(); matItr != matList->end(); ++matItr)
		{
			auto targetMat = *matItr;
			//使用されていないマテリアル対策
			if (targetMat->dwNumFace == 0)
			{
				continue;
			}
			//インデックスバッファーをセット
			stride = sizeof(int);
			offset = 0;
			m_pDeviceContext->IASetIndexBuffer(targetMat->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


			cShader::SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient = targetMat->Ka;//アンビエントををシェーダーに渡す
			sg.vDiffuse = targetMat->Kd;//ディフューズカラーをシェーダーに渡す
			sg.vSpecular = targetMat->Ks;//スペキュラーをシェーダーに渡す
			shader.setBuffer(sg);

			//テクスチャーをシェーダーに渡す
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
			//プリミティブをレンダリング
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
//		MessageBox(0, L"メッシュ用シェーダー作成失敗", NULL, MB_OK);
//	}
//}