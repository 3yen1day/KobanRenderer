#include "Mesh.h"
#include "../Rendering/Render.h"
#include "Camera.h"
#include "Light.h"

namespace Koban {
	Mesh::Mesh()
	{
		//shader初期化
		//頂点インプットレイアウトを定義	
		vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};


		ID3DBlob* compiledShader = NULL;
		RenderUtil::createShader(
			DEVICE,
			L"Shader/Deferred.hlsl",
			layout,
			L"VS_To_Tex",
			L"PS_To_Tex",
			&mpVertexLayout,
			&mpVertexShader,
			&mpPixelShader
		);
		SAFE_RELEASE(compiledShader);

		//コンスタントバッファ初期化
		RenderUtil::createConstantBuffer<CONSTANT_BUFFER_DEFAULT>(DEVICE, &mpConstantBuffer_Default);
		RenderUtil::createConstantBuffer<CONSTANT_BUFFER_MATERIAL>(DEVICE, &mpConstantBuffer_Material);

		//fbxのロード
		if (!FbxLoader::Load("Resource/Chips.obj", &mFbxVertexInfo)) {
			DebugUtil::error(L"ファイル読み込み失敗");
		}

		//バーテックスバッファ初期化
		RenderUtil::createVertexBffer<FbxLoader::Vertex>(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.vertices, &mpVertexBuffer);

		//インデックスバッファ初期化
		RenderUtil::createIndexBuffer(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.indices, &mpIndexBuffer);

		//テクスチャ用サンプラ作成
		RenderUtil::createSamplerState(DEVICE, &mpSampleLinear);

		////テクスチャ作成
		//RenderLib::createTexture(DEVICE, L"Resource\\Chips_Cover.jpg", &mpTexture);
	}

	void Mesh::draw() {
		//使用するシェーダーの登録	
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);

		//シェーダーのコンスタントバッファーに各種データを渡す	
		D3D11_MAPPED_SUBRESOURCE pData_default;
		CONSTANT_BUFFER_DEFAULT cb_default;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Default, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_default)))//pData_default.pDataにm_pConstantBufferのアドレス
		{
			cb_default.mW = getModelMatrix();
			//MVP行列を渡す
			cb_default.mWVP = getMVPMatrix(cb_default.mW);
			//ライトの位置を渡す
			cb_default.vLightDir = RENDER->getLight()->getDirection();
			//視点位置を渡す
			cb_default.vEye = D3DXVECTOR4(RENDER->getCamera()->getEyeDir(), 0);
			if (memcpy_s(pData_default.pData, sizeof(CONSTANT_BUFFER_DEFAULT), (void*)(&cb_default), sizeof(cb_default))) {
				DebugUtil::error(L"memCopy時にエラー");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Default, 0);
		}

		D3D11_MAPPED_SUBRESOURCE pData_material;
		CONSTANT_BUFFER_MATERIAL cb_material;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Material, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_material)))//pData_material.pDataにm_pConstantBufferのアドレス
		{
			cb_material = getMaterialVal();
			if (memcpy_s(pData_material.pData, sizeof(CONSTANT_BUFFER_MATERIAL), (void*)(&cb_material), sizeof(cb_material))) {
				DebugUtil::error(L"memCopy時にエラー");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Material, 0);
		}

		//このコンスタントバッファーを使うシェーダーの登録
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &mpConstantBuffer_Default);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &mpConstantBuffer_Default);
		DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &mpConstantBuffer_Material);
		DEVICE_CONTEXT->PSSetConstantBuffers(1, 1, &mpConstantBuffer_Material);

		//頂点インプットレイアウトをセット
		DEVICE_CONTEXT->IASetInputLayout(mpVertexLayout);
		//プリミティブ・トポロジーをセット
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//バーテックスバッファーをセット todo:毎フレ必要？
		UINT stride = sizeof(FbxLoader::Vertex);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
		//インデックスバッファをセット todo:毎フレ必要？
		DEVICE_CONTEXT->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//テクスチャーをシェーダーに渡す
		//DEVICE_CONTEXT->PSSetSamplers(0, 1, &mpSampleLinear);
		//DEVICE_CONTEXT->PSSetShaderResources(0, 1, &mpTexture);
		//プリミティブをレンダリング
		DEVICE_CONTEXT->DrawIndexed(mFbxVertexInfo.indices.size(), 0, 0);
	}

	/// <summary>
	/// Model行列を取得する
	/// </summary>
	/// <returns></returns>
	D3DXMATRIX Mesh::getModelMatrix() {
		// スケール行列
		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

		// 回転行列
		D3DXQUATERNION quat;
		D3DXVECTOR3 vec3(0, 1, 0);
		static float angle = 0;
		static float speed = 0.00005;
		angle += speed;
		angle = std::fmod(angle, 6.28f);
		D3DXQuaternionRotationAxis(&quat, &vec3, angle);
		D3DXMATRIX matRotation;
		D3DXMatrixRotationQuaternion(&matRotation, &quat);

		//平行移動行列
		auto pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

		return matScale * matRotation * matTrans;
	}

	/// <summary>
	/// MVP行列を取得する
	/// </summary>
	/// <returns></returns>
	D3DXMATRIX Mesh::getMVPMatrix(const D3DXMATRIX& modelMat) {
		//ワールド、カメラ、射影行列を渡す
		auto camera = RENDER->getCamera();
		D3DXMATRIX m = modelMat * camera->getViewMat() * camera->getProjMat();
		D3DXMatrixTranspose(&m, &m); //GPUの行列計算方法とDirectXのそれが異なるため転置する

		return m;
	}

	/// <summary>
	/// Materialの値を取得する（todo:テキストファイルから読むようにする）
	/// </summary>
	/// <returns></returns>
	Mesh::CONSTANT_BUFFER_MATERIAL Mesh::getMaterialVal() {
		auto cBuffer = CONSTANT_BUFFER_MATERIAL();
		cBuffer.vAmbient = D3DXVECTOR4(0.5f, 0, 0, 0);
		cBuffer.vDiffuse = D3DXVECTOR4(1, 1, 1, 0);
		cBuffer.vSpecular = D3DXVECTOR4(2, 2, 2, 0);
		return cBuffer;
	}
}