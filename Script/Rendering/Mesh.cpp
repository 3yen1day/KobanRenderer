#include "Mesh.h"
#include "../Rendering/Rendering.h"
#include "Camera.h"
#include "Light.h"
#include "../Core/Scene.h"
#include "../Core/Transform.h"

namespace Koban {
	void Mesh::start() {
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
			L"Shader/VS_RenderToGBuffer.hlsl",
			L"Shader/PS_RenderToGBuffer.hlsl",
			layout,
			&mpVertexLayout,
			&mpVertexShader,
			&mpPixelShader
		);
		SAFE_RELEASE(compiledShader);

		//コンスタントバッファ初期化
		RenderUtil::createConstantBuffer<RenderUtil::CONSTANT_BUFFER_TRANSFORM>(DEVICE, &mpConstantBuffer_Transform);
		RenderUtil::createConstantBuffer<CONSTANT_BUFFER_MATERIAL>(DEVICE, &mpConstantBuffer_Material);

		//テクスチャ用サンプラ作成
		RenderUtil::createSamplerState(DEVICE, &mpSampleLinear);

		////テクスチャ作成
		//RenderLib::createTexture(DEVICE, L"Resource\\Chips_Cover.jpg", &mpTexture);
	}

	void Mesh::setMeshResourcePath(const string& resourcePath)
	{
		//fbxのロード
		if (!FbxLoader::Load(resourcePath, &mFbxVertexInfo))
		{
			DebugUtil::error(L"ファイル読み込み失敗");
		}
		//バーテックスバッファ初期化
		RenderUtil::createVertexBffer<FbxLoader::Vertex>(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.vertices, &mpVertexBuffer);
		//インデックスバッファ初期化
		RenderUtil::createIndexBuffer(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.indices, &mpIndexBuffer);
	}

	void Mesh::update()
	{
		D3DXQUATERNION rot;
		D3DXVECTOR3 vec3(0, 1, 0);
		static float angle = 0;
		static float speed = 0.01;
		angle += speed;
		angle = std::fmod(angle, 6.28f);
		D3DXQuaternionRotationAxis(&rot, &vec3, angle);
		getTransform()->setRotation(rot);
	}

	void Mesh::draw() {
		//使用するシェーダーの登録	
		DEVICE_CONTEXT->VSSetShader(mpVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(mpPixelShader, NULL, 0);

		//シェーダーのコンスタントバッファーに各種データを渡す	
		D3D11_MAPPED_SUBRESOURCE pData_default;
		RenderUtil::CONSTANT_BUFFER_TRANSFORM cb_default;
		if (SUCCEEDED(DEVICE_CONTEXT->Map(mpConstantBuffer_Transform, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData_default)))//pData_default.pDataにm_pConstantBufferのアドレス
		{
			//単位行列をセット
			cb_default.mW = getTransform()->getWorldMatrix();
			//MVP行列を渡す
			cb_default.mWVP = getMVPMatrix(cb_default.mW);
			if (memcpy_s(pData_default.pData, sizeof(RenderUtil::CONSTANT_BUFFER_TRANSFORM), (void*)(&cb_default), sizeof(cb_default))) {
				DebugUtil::error(L"memCopy時にエラー");
			}
			DEVICE_CONTEXT->Unmap(mpConstantBuffer_Transform, 0);
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

		//コンスタントバッファーの登録
		DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &mpConstantBuffer_Transform);
		DEVICE_CONTEXT->PSSetConstantBuffers(1, 1, &mpConstantBuffer_Transform);
		DEVICE_CONTEXT->VSSetConstantBuffers(2, 1, &mpConstantBuffer_Material);
		DEVICE_CONTEXT->PSSetConstantBuffers(2, 1, &mpConstantBuffer_Material);
		RENDER->setGlobalConstantBuffer();

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
	/// MVP行列を取得する
	/// </summary>
	/// <returns></returns>
	D3DXMATRIX Mesh::getMVPMatrix(const D3DXMATRIX& modelMat) {
		//ワールド、カメラ、射影行列を渡す
		auto camera = RENDER->getCamera();
		return modelMat * camera->getViewMat() * camera->getProjMat();
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