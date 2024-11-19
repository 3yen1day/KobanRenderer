#include "Scene.h"

namespace Koban {
#pragma region GameObject
	UINT16 Scene::createGameObject(const string& name) {
		mGameObjectMap.emplace(_GameObjectHashMax, std::make_unique<GameObject>(_GameObjectHashMax, name));
		UINT16 retVal = _GameObjectHashMax;
		_GameObjectHashMax++;
		return retVal;
	}

	GameObject* const Scene::findGameObject(UINT16 key) {
		auto val = mGameObjectMap.find(key);
		if (val == mGameObjectMap.end())
			return nullptr;
		return val->second.get();
	}

	void Scene::destroyGameObject(UINT16 key) {
		mGameObjectMap.erase(key);
	}
#pragma endregion

#pragma region Component
	template<DerivationOfComponent T>
	T* Scene::addComponent() {
		auto key = typeid(T);
		std::vector<unique_ptr<Component>> vec = mComponentMap.find(key);
		if (vec == mComponentMap.end()) {
			//�R���|�[�l���g���V�[���ɑ��݂��Ȃ��̂ŁA�V�����ǉ�
			mComponentMap[key] = std::vector<unique_ptr<Component>>();
		}
		mComponentMap[key].push_back(std::make_unique<T>)();
	}

	template<DerivationOfComponent T>
	void Scene::removeComponent(T* instance) {
		auto key = typeid(T);
		std::vector<unique_ptr<Component>> vec = mComponentMap.find(key);
		if (vec != mComponentMap.end()) {
			for (auto it = vec.begin(); it != vec.end(); it++) {
				//iterater�̓|�C���^�̂悤�ɗ��p�ł���
				//*���Z�q�ŗv�f�ɃA�N�Z�X�\
				if ((*it).get() == instance) {
					vec.erase(it);
					return;
				}
			}
		}
	}
#pragma endregion
}