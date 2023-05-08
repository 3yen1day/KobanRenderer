#pragma once

#include "cShader.h"
#include <memory>
#include <unordered_map>
#include <string>

//���_�̍\����
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vUV;
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

	MY_MATERIAL(const MY_MATERIAL& srcMat) {
		ZeroMemory(this, sizeof(MY_MATERIAL));
		Ka = srcMat.Ka;
		Kd = srcMat.Kd;
		Ks = srcMat.Ks;
		strcpy_s(szName, srcMat.szName);
		strcpy_s(szTextureName, srcMat.szTextureName);
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
	/// Shader�̃��X�g
	/// </summary>
	std::unordered_map<std::wstring, cShader> m_Shader;
	/// <summary>
	/// 
	/// </summary>
	std::unordered_map<std::wstring, std::list<MY_MATERIAL*>*> m_Material;
	/// <summary>
	/// �T���v���[�X�e�[�g
	/// </summary>
	ID3D11SamplerState* m_pSampleLinear;

private:
	// �g�����X�t�H�[�����
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll, m_fScale;

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
