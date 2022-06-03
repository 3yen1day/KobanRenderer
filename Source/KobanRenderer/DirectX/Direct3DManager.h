#pragma once
//=========================================
// Direct3Dクラスの初期化
//=========================================
// Direct3Dのライブラリを使用できるようにする
// 以下をプロジェクトのリンカーに追加する
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Direct3Dの型・クラス・関数を呼べるようにする
#include <d3d11.h>
#include <d3dcompiler.h>
//DirectXMath(数学ライブラリ)を使用できるようにする
#include <DirectXMath.h>
//ComPtr（スマートポインタ）を使用できるようにする
#include <winrt\wrl\client.h>
using Microsoft::WRL::ComPtr;

//=========================================
// Direct3DManagerクラス
// DirectXまわりの設定
//=========================================
class Direct3DManager {
	//============================================
	// メソッド
	//============================================
public:
	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static void CreateInstance()
	{
		DeleteInstance();

		mInstance = new Direct3DManager();
	}

	/// <summary>
	/// インスタンス削除
	/// </summary>
	static void DeleteInstance()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Direct3DManager& GetInstance()
	{
		return *mInstance;
	}

	//============================================
	// フィールド
	//============================================
public:
	//Direct3Dデバイス
	ComPtr<ID3D11Device>	mDevice;
	//Direct3Dデバイスコンテキスト
	ComPtr<ID3D11DeviceContext>	mDeviceContext;
	//スワップチェイン
	ComPtr<IDXGISwapChain>	mSwapChain;
	//バックバッファのＲＴビュー
	ComPtr<ID3D11RenderTargetView>	mBackBufferView;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="width">画面の幅</param>
	/// <param name="height">画面の高さ</param>
	/// <returns></returns>
	bool Initialize(HWND hWnd, int width, int height);

	// 2D描画用シェーダ―
	ComPtr<ID3D11VertexShader> mSpriteVS = nullptr; // 頂点シェーダー
	ComPtr<ID3D11PixelShader> mSpritePS = nullptr; // ピクセルシェーダー
	ComPtr<ID3D11InputLayout> mSpriteInputLayout = nullptr; // 入力レイアウト

private:
	static Direct3DManager* mInstance;
	Direct3DManager() {
	};


	// Direct3Dの唯一のインスタンスを簡単に取得するためのマクロ
#define D3D Direct3DManager::GetInstance()
};