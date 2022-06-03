#pragma once

//=========================================
// GameSystemクラス
// メインループ
//=========================================
class GameSystemManager {
public:
	//初期化
	void Initialize();
	
	//処理の実行
	void Execute();

private:
	static GameSystemManager* mInstance;
	GameSystemManager(){}
public:
	// インスタンス作成
	static void CreateInstance()
	{
		DeleteInstance();

		mInstance = new GameSystemManager();
	}
	// インスタンス削除
	static void DeleteInstance()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}
	// 唯一のインスタンスを取得
	static GameSystemManager& GetInstance()
	{
		return *mInstance;
	}
	// GameSystemの唯一のインスタンスを簡単に取得するためのマクロ
#define GAMESYS GameSystemManager::GetInstance()
};
