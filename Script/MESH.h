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

//頂点の構造体
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vUV;
};

struct SIMPLECONSTANT_BUFFER1
{
	D3DXVECTOR4 vAmbient;//アンビエント光
	D3DXVECTOR4 vDiffuse;//ディフューズ色
	D3DXVECTOR4 vSpecular;//鏡面反射
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
	/// Shader数
	/// </summary>
	DWORD m_dwNumShader;
	/// <summary>
	/// Shaderのリスト
	/// </summary>
	std::unordered_map<cShader*, std::list<MY_MATERIAL*>*> m_Shader;
	/// <summary>
	/// サンプラーステート
	/// </summary>
	ID3D11SamplerState* m_pSampleLinear;


private:
	// トランスフォーム情報
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll;
	float m_fScale;

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

class cShader {

public:
	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLECONSTANT_BUFFER0
	{
		D3DXMATRIX mW;//ワールド行列
		D3DXMATRIX mWVP;//ワールドから射影までの変換行列
		D3DXVECTOR4 vLightDir;//ライト方向
		D3DXVECTOR4 vEye;//カメラ位置
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
	/// Shaderパス
	/// </summary>
	LPWSTR m_ShaderPath;
	/// <summary>
	/// 頂点シェーダー
	/// </summary>
	ID3D11VertexShader* m_pVertexShader;
	/// <summary>
	/// ピクセルシェーダー
	/// </summary>
	ID3D11PixelShader* m_pPixelShader;
	/// <summary>
	/// コンスタントバッファ0
	/// </summary>
	ID3D11Buffer* m_pConstantBuffer0;
	/// <summary>
	/// コンスタントバッファ1
	/// </summary>
	ID3D11Buffer* m_pConstantBuffer1;
	/// <summary>
	/// 頂点レイアウト
	/// </summary>
	ID3D11InputLayout* m_pVertexLayout;
	// 以下はRenderクラスをシングルトンにして、アクセスできるようにする
	/// <summary>
	/// デバイス
	/// </summary>
	ID3D11Device* m_pDevice;
	/// <summary>
	/// デバイスコンテキスト
	/// </summary>
	ID3D11DeviceContext* m_pDeviceContext;
};