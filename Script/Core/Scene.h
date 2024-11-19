#pragma once
#include <typeindex>
#include "Component.h"//conceptを使うために必要
#include "GameObject.h"//unordered_mapでunique_ptrを使うのに完全な定義が必要

namespace Koban {
    class Scene {
	public:
        Scene() {};
        ~Scene() {};

        //GameObjectを生成
        UINT16 createGameObject(const string& name);
        //GameObjectを検索
        GameObject* const findGameObject(UINT16 key);
        //GameObjectを破棄
        void destroyGameObject(UINT16 key);

        //コンポーネントをシーンに追加。
        //GameObjectからアクセスする（friendにするとincludeで循環参照が起きる）。
        template<DerivationOfComponent T>
        T* addComponent();
        //コンポーネントを破棄。
        //GameObjectからアクセスする（friendにするとincludeで循環参照が起きる）。
        template<DerivationOfComponent T>
        void removeComponent(T* instance);
	private:

		// 型ごとのvectorを保持するコンテナ
		std::unordered_map<std::type_index , std::vector<unique_ptr<Component>>> mComponentMap;
        //GameObjectのvector
        std::unordered_map<UINT16, unique_ptr<GameObject>> mGameObjectMap;
        UINT16 _GameObjectHashMax = 0;
	};
}