#pragma once

#include "cShader.h"
#include <memory>
#include <unordered_map>
#include <string>

//頂点の構造体
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vUV;
};



//オリジナル　マテリアル構造体
struct MY_MATERIAL
{
public:
	CHAR szName[110];
	D3DXVECTOR4 Ka;//アンビエント
	D3DXVECTOR4 Kd;//ディフューズ
	D3DXVECTOR4 Ks;//スペキュラー
	CHAR szTextureName[110];//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;//そのマテリアルであるポリゴン数
	LPWSTR shaderPath;
	/// <summary>
	/// インデックスバッファ
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
	/// 頂点数
	/// </summary>
	DWORD m_dwNumVert;
	/// <summary>
	/// ポリゴン数
	/// </summary>
	DWORD m_dwNumFace;
	/// <summary>
	/// 頂点バッファ
	/// </summary>
	ID3D11Buffer* m_pVertexBuffer;
	/// <summary>
	/// Shaderのリスト
	/// </summary>
	std::unordered_map<std::wstring, cShader> m_Shader;
	/// <summary>
	/// 
	/// </summary>
	std::unordered_map<std::wstring, std::list<MY_MATERIAL*>*> m_Material;
	/// <summary>
	/// サンプラーステート
	/// </summary>
	ID3D11SamplerState* m_pSampleLinear;

private:
	// トランスフォーム情報
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll, m_fScale;

	/// <summary>
	/// モデルのリソースパス
	/// </summary>
	LPWSTR MODEL_PATH;
	/// <summary>
	/// デバイス
	/// </summary>
	ID3D11Device* m_pDevice;
	/// <summary>
	/// デバイスコンテキスト
	/// </summary>
	ID3D11DeviceContext* m_pDeviceContext;

	/// <summary>
	/// ファイルからMeshを読み込む
	/// </summary>
	/// <param name="FileName"></param>
	/// <returns></returns>
	HRESULT LoadResources(LPWSTR FileName);
};
