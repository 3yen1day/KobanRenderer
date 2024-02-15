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
			MessageBox(0, L"メッシュ作成失敗", NULL, MB_OK);
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
		uint32_t dwVCount = 0;//読み込みカウンター
		uint32_t dwVNCount = 0;//読み込みカウンター
		uint32_t dwVTCount = 0;//読み込みカウンター
		uint32_t dwFCount = 0;//読み込みカウンター

		wchar_t key[200] = { 0 };
		//OBJファイルを開いて内容を読み込む
		FILE* fp = NULL;
		_wfopen_s(&fp, meshFileName.data(), L"rt");
		//事前に頂点数、ポリゴン数を調べる
		while (!feof(fp))
		{
			auto size = 201 * sizeof(wchar_t);
			//キーワード読み込み
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//マテリアル読み込み
			if (wcscmp(key, L"mtllib") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				loadMaterialFromFile(key);
			}
			//頂点
			if (wcscmp(key, L"v") == 0)
			{
				mVertNum++;
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
				mFaceNum++;
			}
		}

		//一時的なメモリ確保（頂点バッファとインデックスバッファ）
		MY_VERTEX* pvVertexBuffer = new MY_VERTEX[mFaceNum * 3];
		D3DXVECTOR3* pvCoord = new D3DXVECTOR3[mVertNum];
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
			fwscanf_s(fp, L"%s ", key, _countof(key));

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
		int* piFaceBuffer = new int[mFaceNum * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
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
					//キーワード 読み込み
					ZeroMemory(key, sizeof(key));
					fwscanf_s(fp, L"%s ", key, _countof(key));

					//フェイス 読み込み→頂点インデックスに
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
						if (targetMat->mpTexture != NULL)//テクスチャーありサーフェイス
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
				if (dwPartFCount == 0)//使用されていないマテリアル対策
				{
					targetMat->mpIndexBuffer = NULL;
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
				if (FAILED(DEVICE->CreateBuffer(&bd, &InitData, &targetMat->mpIndexBuffer)))
					return FALSE;
				targetMat->mFaceNum = dwPartFCount;
			}
		}

		delete[] piFaceBuffer;
		fclose(fp);

		//バーテックスバッファーを作成
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
		DEVICE->CreateSamplerState(&SamDesc, &mpSampleLinear);

		return S_OK;
	}

	HRESULT TestMesh::loadMaterialFromFile(std::wstring FileName)
	{
		//マテリアルファイルを開いて内容を読み込む
		FILE* fp = NULL;
		_wfopen_s(&fp, FileName.data(), L"rt");
		wchar_t key[110] = { 0 };
		D3DXVECTOR4 v(0, 0, 0, 1);

		//本読み込み	
		fseek(fp, SEEK_SET, 0);

		auto pMaterial = new std::vector<TestMaterial*>();
		int matCount = -1;

		while (!feof(fp))
		{
			//キーワード読み込み
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//マテリアル名
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
			//Ka　アンビエント
			if (wcscmp(key, L"Ka") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->at(matCount)->mKa = v;
			}
			//Kd　ディフューズ
			if (wcscmp(key, L"Kd") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->at(matCount)->mKd = v;
			}
			//Ks　スペキュラー
			if (wcscmp(key, L"Ks") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->at(matCount)->mKs = v;
			}
			//map_Kd　テクスチャー
			if (wcscmp(key, L"map_Kd") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				pMaterial->at(matCount)->mTextureName = key;
			}

		}
		fclose(fp);

		for (auto i = 0; i < pMaterial->size(); i++)
		{
			// shaderの追加
			auto wstrPath = pMaterial->at(i)->shaderPath;
			if (mShaderDic.size() == 0 || mShaderDic.find(wstrPath) == mShaderDic.end())
			{
				auto shader = new TestShader(wstrPath);
				//std::pair<std::wstring, cShader> shaderPair = std::pair<std::wstring, cShader>(*path, *shader);
				//m_Shader.insert(shaderPair);
				mShaderDic[wstrPath] = *shader;
			}
			// materialの追加
			auto matItr = mMaterialDic.find(wstrPath);
			auto mat = pMaterial->at(i);

			char c[110] = { 0 };
			wcstombs(c, mat->mTextureName.data(), mat->mTextureName.length());
			//テクスチャーを作成
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
		mYaw += 0.0001; //回転
		D3DXMATRIX worldMat, transMat, yawMat, pitchMat, rollMat, scaleMat;
		//ワールドトランスフォーム（絶対座標変換）
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

			//バーテックスバッファーをセット
			UINT stride = sizeof(MY_VERTEX);
			UINT offset = 0;
			DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

			shader.Render(worldMat, mViewMat, mProjMat, vLight, vEye);

			//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
			for (auto matItr = matList->begin(); matItr != matList->end(); ++matItr)
			{
				auto targetMat = *matItr;
				//使用されていないマテリアル対策
				if (targetMat->mFaceNum == 0)
				{
					continue;
				}
				//インデックスバッファーをセット
				stride = sizeof(int);
				offset = 0;
				DEVICE_CONTEXT->IASetIndexBuffer(targetMat->mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


				TestShader::SIMPLECONSTANT_BUFFER1 sg;
				sg.mAmbient = targetMat->mKa;//アンビエントををシェーダーに渡す
				sg.mDiffuse = targetMat->mKd;//ディフューズカラーをシェーダーに渡す
				sg.mSpecular = targetMat->mKs;//スペキュラーをシェーダーに渡す
				shader.setBuffer(sg);

				//テクスチャーをシェーダーに渡す
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
				//プリミティブをレンダリング
				DEVICE_CONTEXT->DrawIndexed(targetMat->mFaceNum * 3, 0, 0);
			}
		}
	}
}