#pragma once
#include "RenderInclude.h"

class cShader {
public:
	cShader() {};
	cShader(LPWSTR path, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~cShader() {};

	struct SIMPLECONSTANT_BUFFER0
	{
		D3DXMATRIX mW;//ワールド行列
		D3DXMATRIX mWVP;//ワールドから射影までの変換行列
		D3DXVECTOR4 vLightDir;//ライト方向
		D3DXVECTOR4 vEye;//カメラ位置
	};

	struct SIMPLECONSTANT_BUFFER1
	{
		D3DXVECTOR4 vAmbient;//アンビエント光
		D3DXVECTOR4 vDiffuse;//ディフューズ色
		D3DXVECTOR4 vSpecular;//鏡面反射
	};

	HRESULT initShader();
	void setBuffer(SIMPLECONSTANT_BUFFER1 buffer);
	void render(D3DXMATRIX& mWorld, D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

private:
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	/// <summary>
	/// デバイス
	/// </summary>
	ID3D11Device* m_pDevice;
	/// <summary>
	/// デバイスコンテキスト
	/// </summary>
	ID3D11DeviceContext* m_pDeviceContext;

	LPWSTR m_ShaderPath;
};
