#pragma once
#include <typeindex>
#include "Component.h"//conceptを使うために必要
#include "../Core/Module.h"

namespace Koban {
    class GameObject;
}

namespace Koban {
    class Scene : ModuleManager {
	public:
        Scene() {};
        ~Scene() {};

        void start() override;
        void update() override {};
        void draw() override {};
        void destroy() override;

#pragma region Component用関数呼び出し
        template<DerivationOfComponent T>
        void doStart() {
            std::vector<T*> cmps = findComponents<T>();
            if (cmps.empty())
                return;

            for (T* const cmp : cmps) {
                cmp->start();
            }
        };

        template<DerivationOfComponent T>
        void doUpdate() {
            std::vector<T*> cmps = findComponents<T>();
            if (cmps.empty())
                return;

            for (T* const cmp : cmps) {
                if (cmp->getIsUpdate()) {
                    cmp->update();
                }
            }
        };

        template<DerivationOfComponent T>
        void doDraw() {
            std::vector<T*> cmps = findComponents<T>();
            if (cmps.empty())
                return;

            for (T* const cmp : cmps) {
                if (cmp->getIsDraw()) {
                    cmp->draw();
                }
            }
        };

        template<DerivationOfComponent T>
        void doDestroy() {
            std::vector<T*> cmps = findComponents<T>();
            if (cmps.empty())
                return;

            for (T* const cmp : cmps) {
                cmp->destroy();
            }
        };
#pragma endregion

#pragma region GameObject
        //GameObjectを生成
        GameObject* const createGameObject(
            const string& name,
            D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, 0),
            D3DXQUATERNION rot = D3DXQUATERNION(0, 0, 0, 1),
            D3DXVECTOR3 scale = D3DXVECTOR3(1, 1, 1));

        //GameObjectを破棄
        void destroyGameObject(GameObject& go);

        //GameObjectの取得
        GameObject* const getGameObject(const UINT16 id) {
            auto it = mGameObjectMap.find(id);
            if (it != mGameObjectMap.end()) {
                return it->second;
            }
            return nullptr;
        }

        //シーンにあるすべてのGameObjectを取得
        const std::vector<GameObject*> getAllGameObjects();
#pragma endregion

#pragma region Component
        //コンポーネントをシーンに追加。
        //GameObjectからアクセスする（friendにするとincludeで循環参照が起きる）。
        template<DerivationOfComponent T>
        T* const addComponent() {
            std::type_index key = typeid(T);
            auto vec = mComponentMap.find(key);

            if (vec == mComponentMap.end()) {
                //コンポーネントがシーンに存在しないので、新しく追加
                mComponentMap[key] = std::vector<unique_ptr<Component>>();
            }

            unique_ptr<T> cmp = std::make_unique<T>();
            T* result = cmp.get();
            mComponentMap[key].push_back(std::move(cmp));
            return result;
        };

        //コンポーネントを破棄。
        //GameObjectからアクセスする（friendにするとincludeで循環参照が起きる）。
        template<DerivationOfComponent T>
        void removeComponent(T* instance) {
            std::type_index key = typeid(T);
            auto vec = mComponentMap.find(key);
            if (vec != mComponentMap.end()) {
                for (auto it = vec.begin(); it != vec.end(); it++) {
                    //iteraterはポインタのように利用できる
                    //*演算子で要素にアクセス可能
                    if ((*it).get() == instance) {
                        vec.erase(it);
                        return;
                    }
                }
            }
        };

        template<DerivationOfComponent T>
        const std::vector<T*> findComponents() {
            std::type_index key = typeid(T);
           auto vec = mComponentMap.find(key);
            if (vec == mComponentMap.end())
                return {};

            std::vector<T*> result;
            for (auto& component : vec->second) {
                result.push_back(static_cast<T*>(component.get()));
            }
            return result;
        };
#pragma endregion

	private:
		// 型ごとのvectorを保持するコンテナ
		std::unordered_map<std::type_index , std::vector<unique_ptr<Component>>> mComponentMap;
        //GameObjectのvector
        std::unordered_map<UINT16, GameObject*> mGameObjectMap;
        UINT16 _GameObjectHashMax = 0;
	};
}