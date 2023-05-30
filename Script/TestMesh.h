#pragma once
#include "TestShader.h"

//マテリアル構造体
struct TestMaterial
{
public:
	std::wstring mName;
	std::wstring mTextureName;//テクスチャーファイル名
	std::wstring shaderPath;
	D3DXVECTOR4 mKa;//アンビエント
	D3DXVECTOR4 mKd;//ディフューズ
	D3DXVECTOR4 mKs;//スペキュラー
	uint32_t mFaceNum;//そのマテリアルであるポリゴン数
	ID3D11ShaderResourceView* mpTexture;
	/// <summary>
	/// インデックスバッファ
	/// </summary>
	ID3D11Buffer* mpIndexBuffer;

	TestMaterial():
		mName{L""},
		mTextureName{L""},
		shaderPath{L""},
		mFaceNum{0},
		mpTexture{nullptr},
		mpIndexBuffer{nullptr}
	{
	}

	~TestMaterial()
	{
		SAFE_RELEASE(mpTexture);
		SAFE_RELEASE(mpIndexBuffer);
	}
};


class TestMesh
{
public:
	TestMesh();
	~TestMesh();
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT LoadMaterialFromFile(std::wstring FileName);
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	/// <summary>
	/// 頂点数
	/// </summary>
	uint32_t mVertNum;
	/// <summary>
	/// ポリゴン数
	/// </summary>
	uint32_t mFaceNum;
	/// <summary>
	/// 頂点バッファ
	/// </summary>
	ID3D11Buffer* mpVertexBuffer;
	/// <summary>
	/// ShaderのDic
	/// </summary>
	std::unordered_map<std::wstring, TestShader> mShaderDic;
	/// <summary>
	/// MaterialのDic
	/// 構造的にはMaterialがShaderを持つ形が理想だが、Shader毎に処理を行ったほうが効率がいい二律背反。一旦分けておく。
	/// </summary>
	std::unordered_map<std::wstring, std::list<TestMaterial*>*> mMaterialDic;
	/// <summary>
	/// サンプラーステート
	/// </summary>
	ID3D11SamplerState* mpSampleLinear;

private:
	//頂点の構造体
	struct MY_VERTEX
	{
		D3DXVECTOR3 mPos;
		D3DXVECTOR3 mNorm;
		D3DXVECTOR2 mUV;
	};

	// トランスフォーム情報
	D3DXVECTOR3 mPos;
	float mYaw, mPitch, mRoll, mScale;

	/// <summary>
	/// モデルのリソースパス
	/// </summary>
	std::wstring mModelPath;
	/// <summary>
	/// デバイス
	/// </summary>
	ID3D11Device* mpDevice;
	/// <summary>
	/// デバイスコンテキスト
	/// </summary>
	ID3D11DeviceContext* mpDeviceContext;

	/// <summary>
	/// ファイルからMeshを読み込む
	/// </summary>
	/// <param name="FileName"></param>
	/// <returns></returns>
	HRESULT LoadResources(std::wstring FileName);
};
