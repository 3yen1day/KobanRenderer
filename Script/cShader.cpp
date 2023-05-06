#include "cShader.h"

///マテリアルの各要素をエフェクト（シェーダー）に渡す
void cShader::setBuffer(SIMPLECONSTANT_BUFFER1 buffer) {
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;

	//バッファの中身を更新するために、map, unmapを使用する→lock, unlockのようなもの
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource)))
	{
		memcpy_s(mappedSubResource.pData, mappedSubResource.RowPitch, (void*)&buffer, sizeof(SIMPLECONSTANT_BUFFER1));
		m_pDeviceContext->Unmap(m_pConstantBuffer1, 0);
	}
	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
	m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
}

void cShader::render(D3DXMATRIX& mWorld, D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye) 
{
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SIMPLECONSTANT_BUFFER0 sg;
		//ワールド行列を渡す
		sg.mW = mWorld;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);
		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
		//ライトの方向を渡す
		sg.vLightDir = D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);
		//視点位置を渡す
		sg.vEye = D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
		m_pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
