#include "Render3DModel.h"
#include "Render.h"
#include "Camera.h"

namespace Koban {
	Render3DModel::Render3DModel()
	{
		//shader初期化
		//頂点インプットレイアウトを定義	
		vector<D3D11_INPUT_ELEMENT_DESC> layout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};


		ID3DBlob* compiledShader = NULL;
		RenderLib::createShader(
			DEVICE,
			L"Shader//Simple.hlsl",
			layout,
			L"VS",
			L"PS",
			&m_pVertexLayout,
			&m_pVertexShader,
			&m_pPixelShader
		);
		SAFE_RELEASE(compiledShader);

		//コンスタントバッファ初期化
		RenderLib::createConstantBuffer<D3DXMATRIX>(DEVICE, &m_pConstantBuffer);

		//fbxのロード
		if (!FbxLoader::Load("Resource/saikoro.fbx", &mFbxVertexInfo)) {
			DebugLib::error(L"ファイル読み込み失敗");
		}

		//バーテックスバッファ初期化
		RenderLib::createVertexBffer<FbxLoader::Vertex>(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.vertices, &m_pVertexBuffer);

		//インデックスバッファ初期化
		RenderLib::createIndexBuffer(DEVICE, DEVICE_CONTEXT, mFbxVertexInfo.indices, &m_pIndexBuffer);

		//テクスチャ用サンプラ作成
		RenderLib::createSamplerState(DEVICE, &m_pSampleLinear);

		//テクスチャ作成
		RenderLib::createTexture(DEVICE, L"Resource\\sprite.jpg", &m_pTexture);
	}

	void Render3DModel::draw() {
		//使用するシェーダーの登録	
		DEVICE_CONTEXT->VSSetShader(m_pVertexShader, NULL, 0);
		DEVICE_CONTEXT->PSSetShader(m_pPixelShader, NULL, 0);

		//シェーダーのコンスタントバッファーに各種データ（位置と色）を渡す	
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;

		// スケール行列
		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

		// 回転行列
		D3DXQUATERNION quat;
		D3DXVECTOR3 vec3(0, 1, 0);
		static float angle = 0;
		static float speed = 0.0001;
		angle += speed;
		angle = std::fmod(angle, 6.28f);
		D3DXQuaternionRotationAxis(&quat, &vec3, angle);
		D3DXMATRIX matRotation;
		D3DXMatrixRotationQuaternion(&matRotation, &quat);

		//平行移動行列
		auto pos = D3DXVECTOR3(0.0f, 0.0f, -0.5f);
		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

		D3DXMATRIX wMat = matScale * matRotation * matTrans;

		//ワールド、カメラ、射影行列を渡す
		D3DXMATRIX m = wMat * Render::getCamera()->getViewMat() * Render::getCamera()->getProjMat();
		D3DXMatrixTranspose(&m, &m); //GPUの行列計算方法とDirectXのそれが異なるため転置する
		cb.mWVP = m;
		//カラーを渡す
		D3DXVECTOR4 vColor(1, 0, 0, 1);
		cb.vColor = vColor;

		if (SUCCEEDED(DEVICE_CONTEXT->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))//pData.pDataにm_pConstantBufferのアドレス
		{
			if (memcpy_s(pData.pData, sizeof(SIMPLESHADER_CONSTANT_BUFFER), (void*)(&cb), sizeof(cb))) {
				DebugLib::error(L"memCopy時にエラー");
			}
			DEVICE_CONTEXT->Unmap(m_pConstantBuffer, 0);
		}

		//このコンスタントバッファーを使うシェーダーの登録
		DEVICE_CONTEXT->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
		//頂点インプットレイアウトをセット
		DEVICE_CONTEXT->IASetInputLayout(m_pVertexLayout);
		//プリミティブ・トポロジーをセット
		DEVICE_CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//バーテックスバッファーをセット todo:毎フレ必要？
		UINT stride = sizeof(FbxLoader::Vertex);
		UINT offset = 0;
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		//インデックスバッファをセット todo:毎フレ必要？
		DEVICE_CONTEXT->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//テクスチャーをシェーダーに渡す
		DEVICE_CONTEXT->PSSetSamplers(0, 1, &m_pSampleLinear);
		DEVICE_CONTEXT->PSSetShaderResources(0, 1, &m_pTexture);
		//プリミティブをレンダリング
		DEVICE_CONTEXT->DrawIndexed(mFbxVertexInfo.indices.size(), 0, 0);
	}
}