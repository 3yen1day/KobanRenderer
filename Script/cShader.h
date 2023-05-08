#pragma once
#include "RenderInclude.h"

class cShader {
public:
	cShader() {};
	cShader(LPWSTR path, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~cShader() {};

	struct SIMPLECONSTANT_BUFFER0
	{
		D3DXMATRIX mW;//���[���h�s��
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
		D3DXVECTOR4 vLightDir;//���C�g����
		D3DXVECTOR4 vEye;//�J�����ʒu
	};

	struct SIMPLECONSTANT_BUFFER1
	{
		D3DXVECTOR4 vAmbient;//�A���r�G���g��
		D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
		D3DXVECTOR4 vSpecular;//���ʔ���
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
	/// �f�o�C�X
	/// </summary>
	ID3D11Device* m_pDevice;
	/// <summary>
	/// �f�o�C�X�R���e�L�X�g
	/// </summary>
	ID3D11DeviceContext* m_pDeviceContext;

	LPWSTR m_ShaderPath;
};
