#pragma once
#include <typeindex>
#include "Component.h"//concept���g�����߂ɕK�v
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

#pragma region Component�p�֐��Ăяo��
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
        //GameObject�𐶐�
        GameObject* const createGameObject(const string& name);

        //GameObject��j��
        void destroyGameObject(GameObject& go);
#pragma endregion

#pragma region Component
        //�R���|�[�l���g���V�[���ɒǉ��B
        //GameObject����A�N�Z�X����ifriend�ɂ����include�ŏz�Q�Ƃ��N����j�B
        template<DerivationOfComponent T>
        T* const addComponent() {
            std::type_index key = typeid(T);
            auto vec = mComponentMap.find(key);

            if (vec == mComponentMap.end()) {
                //�R���|�[�l���g���V�[���ɑ��݂��Ȃ��̂ŁA�V�����ǉ�
                mComponentMap[key] = std::vector<unique_ptr<Component>>();
            }

            unique_ptr<T> cmp = std::make_unique<T>();
            T* result = cmp.get();
            mComponentMap[key].push_back(std::move(cmp));
            return result;
        };

        //�R���|�[�l���g��j���B
        //GameObject����A�N�Z�X����ifriend�ɂ����include�ŏz�Q�Ƃ��N����j�B
        template<DerivationOfComponent T>
        void removeComponent(T* instance) {
            std::type_index key = typeid(T);
            auto vec = mComponentMap.find(key);
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

#pragma region UI�p
        std::vector<GameObject*> getAllGameObjects();
#pragma endregion
	private:
		// �^���Ƃ�vector��ێ�����R���e�i
		std::unordered_map<std::type_index , std::vector<unique_ptr<Component>>> mComponentMap;
        //GameObject��vector
        std::unordered_map<UINT16, GameObject*> mGameObjectMap;
        UINT16 _GameObjectHashMax = 0;
	};
}