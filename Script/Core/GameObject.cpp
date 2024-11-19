#include "GameObject.h"
#include "Scene.h"

namespace Koban {
	GameObject::GameObject(UINT16 id, string name) {
		mID = id;
		mName = name;
	}

	//Componentの生成
	template<DerivationOfComponent T>
	T GameObject::createComponent() {
		//同じ型のコンポーネントはGameObjectに一つ
		for (int i = 0, length = mComponents.size(); i < length; i++)
		{
			if (dynamic_cast<T*>(mComponents[i]) != nullptr)
				return nullptr;
		}

		//シーンに追加する
		auto cmp_pt = new T();
		SCENE->addComponent(cmp_pt);

		mComponents.push_back(cmp_pt);//内部配列に追加
		return cmp_pt;
	}

	//Componentの破棄
	template<DerivationOfComponent T>
	T GameObject::removeComponent() {
		for (auto it = mComponents.begin(); it != mComponents.end(); it++)
		{
			if (dynamic_cast<T*>(*it) != nullptr) {
				SCENE->removeComponent<T>(*it);
				mComponents.erase(it);
				break;
			}
		}
	}
};