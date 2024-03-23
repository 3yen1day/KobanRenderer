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
				pMaterial->mKa = v;
			}
			//Kd　ディフューズ
			if (wcscmp(key, L"Kd") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mKd = v;
			}
			//Ks　スペキュラー
			if (wcscmp(key, L"Ks") == 0)
			{
				fwscanf_s(fp, L"%f %f %f", &v.x, &v.y, &v.z);
				pMaterial->mKs = v;
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

	void TestMesh::Render(
		D3DXMATRIX& viewMat,
		D3DXMATRIX& projMat,
		D3DXVECTOR3& light,
		D3DXVECTOR3& eye)
	{
		//mYaw += 0.0001; //回転
		//D3DXMATRIX worldMat, transMat, yawMat, pitchMat, rollMat, scaleMat;
		////ワールドトランスフォーム（絶対座標変換）
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

		//	//バーテックスバッファーをセット
		//	UINT stride = sizeof(MY_VERTEX);
		//	UINT offset = 0;
		//	DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);

		//	shader.update();
		//	shader.updateBaseConstantBuffer(worldMat, viewMat, projMat, light, eye);

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
}