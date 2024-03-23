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
			DebugLib::error(L"メッシュ作成失敗");
		}
	}

	HRESULT BaseMesh::loadResources(std::wstring meshFileName)
	{
		//--------------OBJファイルを開いて内容を読み込む-----------------
		wchar_t key[200] = { 0 };
		FILE* fp = NULL;
		_wfopen_s(&fp, meshFileName.data(), L"rt");

		//頂点数カウント
		loadVertCount(fp);
		//マテリアルロード
		loadMaterial(fp);

		//頂点情報のロード
		unique_ptr<D3DXVECTOR3> pvCoord = unique_ptr<D3DXVECTOR3>(new D3DXVECTOR3[mVertCount.get()->vtCount]);
		unique_ptr <D3DXVECTOR3> pvNormal = unique_ptr<D3DXVECTOR3>(new D3DXVECTOR3[mVertCount.get()->normCount]);
		unique_ptr <D3DXVECTOR2> pvUV = unique_ptr <D3DXVECTOR2>(new D3DXVECTOR2[mVertCount.get()->uvCount]);
		loadVert(fp, pvCoord.get(), pvNormal.get(), pvUV.get());

		//頂点バッファ・インデックスバッファの作成
		createVtxBufAndIdxBuf(fp, pvCoord.get(), pvNormal.get(), pvUV.get());

		fclose(fp);
		//-------------------------------------------------------------

		return S_OK;
	}

	/// <summary>
	/// todo:shaderの更新は継承先でやる必要がある
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

		//	//バーテックスバッファーをセット
		//	UINT stride = sizeof(MY_VERTEX);
		//	UINT offset = 0;
		//	DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

		//	shader.update();
		//	shader.updateBaseConstantBuffer(worldMat, camera->getViewMat(), camera->getProjMat(), lightDir, camera->getEyeDir());

		//	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
		//	for (auto matItr = matList->begin(); matItr != matList->end(); ++matItr)
		//	{
		//		auto targetMat = *matItr;
		//		//使用されていないマテリアル対策
		//		if (targetMat->mFaceNum == 0)
		//		{
		//			continue;
		//		}
		//		//インデックスバッファーをセット
		//		stride = sizeof(int);
		//		offset = 0;
		//		DEVICE_CONTEXT->IASetIndexBuffer(targetMat->mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//		TestShader::SIMPLECONSTANT_BUFFER1 sg;
		//		sg.mAmbient = targetMat->mKa;//アンビエントををシェーダーに渡す
		//		sg.mDiffuse = targetMat->mKd;//ディフューズカラーをシェーダーに渡す
		//		sg.mSpecular = targetMat->mKs;//スペキュラーをシェーダーに渡す
		//		shader.updateBuffer(sg);

		//		//テクスチャーをシェーダーに渡す
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
		//		//プリミティブをレンダリング
		//		DEVICE_CONTEXT->DrawIndexed(targetMat->mFaceNum * 3, 0, 0);
		//	}
		//}
	}

#pragma region  初期化まわり
	void BaseMesh::loadVertCount(FILE* fp) {
		wchar_t key[200] = { 0 };
		fseek(fp, SEEK_SET, 0);
		while (!feof(fp))
		{
			auto size = 201 * sizeof(wchar_t);
			//キーワード読み込み
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//頂点
			if (wcscmp(key, L"v") == 0)
			{
				mVertCount.get()->vtCount++;
			}
			//法線
			if (wcscmp(key, L"vn") == 0)
			{
				mVertCount.get()->uvCount++;
			}
			//テクスチャー座標
			if (wcscmp(key, L"vt") == 0)
			{
				mVertCount.get()->normCount++;
			}
			//フェイス（ポリゴン）
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
			//キーワード読み込み
			fwscanf_s(fp, L"%s ", key, _countof(key));
			//マテリアル読み込み
			if (wcscmp(key, L"mtllib") == 0)
			{
				fwscanf_s(fp, L"%s ", key, _countof(key));
				//virtualで子の関数を呼ぶ
				createMaterialFromFile(key);
			}
		}
	}

	void BaseMesh::loadVert(FILE* fp, D3DXVECTOR3* pvCoord, D3DXVECTOR3* pvNormal, D3DXVECTOR2* pvUV) {
		//本読み込み	
		fseek(fp, SEEK_SET, 0);
		int vertCount = 0;
		int normCount = 0;
		int uvCount = 0;
		float x, y, z;
		wchar_t key[200] = { 0 };

		while (!feof(fp))
		{
			//キーワード 読み込み
			ZeroMemory(key, sizeof(key));
			fwscanf_s(fp, L"%s ", key, _countof(key));

			//頂点 読み込み
			if (wcscmp(key, L"v") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
				pvCoord[vertCount].x = -x;
				pvCoord[vertCount].y = y;
				pvCoord[vertCount].z = z;
				vertCount++;
			}

			//法線 読み込み
			if (wcscmp(key, L"vn") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &x, &y, &z);
				pvNormal[normCount].x = -x;
				pvNormal[normCount].y = y;
				pvNormal[normCount].z = z;
				normCount++;
			}

			//uv 読み込み
			if (wcscmp(key, L"vt") == 0)
			{
				fwscanf_s(fp, L"%f %f", &x, &y);
				pvUV[uvCount].x = x;
				pvUV[uvCount].y = 1 - y;//OBJファイルはY成分が逆なので合わせる
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
			//キーワード 読み込み
			ZeroMemory(key, sizeof(key));
			fwscanf_s(fp, L"%s ", key, _countof(key));

			//フェイス 読み込み→頂点インデックスに
			if (wcscmp(key, L"usemtl") == 0)
			{
				setIndexBuffer(targetMatName, materialIndexBuffer.get(), indexCount * 3);

				fwscanf_s(fp, L"%s ", key, _countof(key));
				targetMatName = key;
				materialIndexBuffer.reset(new int[mVertCount.get()->polyCount * 3]);//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
				indexCount = 0;
			}
			if (wcscmp(key, L"f") == 0 && boFlag == true)
			{
				wregex format(L"(%d/%d/%d %d/%d/%d %d/%d/%d)");//regexオブジェクトで正規表現を用いた一般化
				bool result = regex_match(key, format);//フォーマットチェックの結果を返す
				if (result) {
					//テクスチャーありサーフェイス
					fwscanf_s(fp, L"%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
				}
				else {
					//テクスチャー無しサーフェイス
					fwscanf_s(fp, L"%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
				}
				//頂点構造体に代入
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

		//indexBufferを生成
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

		//shader名はMaterialが知ってるので、全shaderを探索して探す
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
		//全shaderで同じvertexBufferを用いる
		for (auto pair : mShaderDic) {
			auto baseShader = &pair.second;
			baseShader->createVertexBuffer(vertBuf, bufferSize);
		}
	}

#pragma endregion
}