#pragma once

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <memory>
#include <unordered_map>
#include <string>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}

//���_�̍\����
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vUV;
};

struct SIMPLECONSTANT_BUFFER1
{
	D3DXVECTOR4 vAmbient;//�A���r�G���g��
	D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
	D3DXVECTOR4 vSpecular;//���ʔ���
};

//�I���W�i���@�}�e���A���\����
struct MY_MATERIAL
{
public:
	CHAR szName[110];
	D3DXVECTOR4 Ka;//�A���r�G���g
	D3DXVECTOR4 Kd;//�f�B�t���[�Y
	D3DXVECTOR4 Ks;//�X�y�L�����[
	CHAR szTextureName[110];//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;//���̃}�e���A���ł���|���S����
	LPWSTR shaderPath;
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@
	/// </summary>
	ID3D11Buffer* m_pIndexBuffer;

	MY_MATERIAL()
	{
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
		SAFE_RELEASE(m_pIndexBuffer);
	}
};

//
//
//
class MESH
{
public:
	MESH();
	~MESH();
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT InitShader();
	HRESULT LoadMaterialFromFile(LPSTR FileName);
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	/// <summary>
	/// ���_��
	/// </summary>
	DWORD m_dwNumVert;
	/// <summary>
	/// �|���S����
	/// </summary>
	DWORD m_dwNumFace;
	/// <summary>
	/// ���_�o�b�t�@
	/// </summary>
	ID3D11Buffer* m_pVertexBuffer;
	/// <summary>
	/// Shader��
	/// </summary>
	DWORD m_dwNumShader;
	/// <summary>
	/// Shader�̃��X�g
	/// </summary>
	std::unordered_map<cShader*, std::list<MY_MATERIAL*>*> m_Shader;
	/// <summary>
	/// �T���v���[�X�e�[�g
	/// </summary>
	ID3D11SamplerState* m_pSampleLinear;


private:
	// �g�����X�t�H�[�����
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll;
	float m_fScale;

	/// <summary>
	/// ���f���̃��\�[�X�p�X
	/// </summary>
	LPWSTR MODEL_PATH;
	/// <summary>
	/// �f�o�C�X
	/// </summary>
	ID3D11Device* m_pDevice;
	/// <summary>
	/// �f�o�C�X�R���e�L�X�g
	/// </summary>
	ID3D11DeviceContext* m_pDeviceContext;

	/// <summary>
	/// �t�@�C������Mesh��ǂݍ���
	/// </summary>
	/// <param name="FileName"></param>
	/// <returns></returns>
	HRESULT LoadResources(LPWSTR FileName);
};

class cShader {

public:
	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLECONSTANT_BUFFER0
	{
		D3DXMATRIX mW;//���[���h�s��
		D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
		D3DXVECTOR4 vLightDir;//���C�g����
		D3DXVECTOR4 vEye;//�J�����ʒu
	};


	cShader() {};
	cShader(LPWSTR shaderPath, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~cShader()
	{
		SAFE_RELEASE(m_pVertexShader);
		SAFE_RELEASE(m_pPixelShader);
		SAFE_RELEASE(m_pConstantBuffer0);
		SAFE_RELEASE(m_pConstantBuffer1);
		SAFE_RELEASE(m_pVertexLayout);
	}
	HRESULT initShader();
	HRESULT render(D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& proj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	/// <summary>
	/// Shader�p�X
	/// </summary>
	LPWSTR m_ShaderPath;
	/// <summary>
	/// ���_�V�F�[�_�[
	/// </summary>
	ID3D11VertexShader* m_pVertexShader;
	/// <summary>
	/// �s�N�Z���V�F�[�_�[
	/// </summary>
	ID3D11PixelShader* m_pPixelShader;
	/// <summary>
	/// �R���X�^���g�o�b�t�@0
	/// </summary>
	ID3D11Buffer* m_pConstantBuffer0;
	/// <summary>
	/// �R���X�^���g�o�b�t�@1
	/// </summary>
	ID3D11Buffer* m_pConstantBuffer1;
	/// <summary>
	/// ���_���C�A�E�g
	/// </summary>
	ID3D11InputLayout* m_pVertexLayout;
	// �ȉ���Render�N���X���V���O���g���ɂ��āA�A�N�Z�X�ł���悤�ɂ���
	/// <summary>
	/// �f�o�C�X
	/// </summary>
	ID3D11Device* m_pDevice;
	/// <summary>
	/// �f�o�C�X�R���e�L�X�g
	/// </summary>
	ID3D11DeviceContext* m_pDeviceContext;
};