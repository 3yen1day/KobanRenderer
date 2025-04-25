#include "Scene.h"
#include "GameObject.h"
#include "SceneInitializer.h"
#include "Transform.h"

namespace Koban {
#pragma region ModuleMethod
	void Scene::start() {
		SceneInitializer::initializeScene();
	}

	void Scene::destroy() {
		for (auto i = mGameObjectMap.begin(); i != mGameObjectMap.end(); i++) {
			delete i->second;
		}
	}
#pragma endregion

#pragma region GameObject
	GameObject* const Scene::createGameObject(const string& name) {
		GameObject* pGo = new GameObject(_GameObjectHashMax, name);
		mGameObjectMap.emplace(_GameObjectHashMax, pGo);
		UINT16 retVal = _GameObjectHashMax;
		_GameObjectHashMax++;

		pGo->createComponent<Transform>();
		return pGo;
	}

	void Scene::destroyGameObject(GameObject& go) {
		mGameObjectMap.erase(go.getID());
	}

	const std::vector<GameObject*> Scene::getAllGameObjects() {
		std::vector<GameObject*> result;
		for (auto it = mGameObjectMap.begin(); it != mGameObjectMap.end(); it++)
		{
			result.push_back(it->second);
		}
		return result;
	}
#pragma endregion
}