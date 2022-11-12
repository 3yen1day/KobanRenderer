#pragma once
#include "DirectX/Texture.h"

//=========================================
// GameSystem�N���X
// ���C�����[�v
//=========================================
class GameSystemManager {
public:
	//������
	void Initialize();
	
	//�����̎��s
	void Execute();

	// �C���X�^���X�쐬
	static void CreateInstance()
	{
		DeleteInstance();

		mInstance = new GameSystemManager();
	}
	// �C���X�^���X�폜
	static void DeleteInstance()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}
	// �B��̃C���X�^���X���擾
	static GameSystemManager& GetInstance()
	{
		return *mInstance;
	}

private:
	GameSystemManager(){}
	static GameSystemManager* mInstance;

	//�����o�ϐ�
public:
	Texture mTexture;
	// GameSystem�̗B��̃C���X�^���X���ȒP�Ɏ擾���邽�߂̃}�N��
#define GAMESYS GameSystemManager::GetInstance()
};
