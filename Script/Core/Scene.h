#pragma once
#include <typeindex>
#include "Component.h"//concept���g�����߂ɕK�v
#include "GameObject.h"//unordered_map��unique_ptr���g���̂Ɋ��S�Ȓ�`���K�v

namespace Koban {
    class Scene {
	public:
        Scene() {};
        ~Scene() {};

        //GameObject�𐶐�
        UINT16 createGameObject(const string& name);
        //GameObject������
        GameObject* const findGameObject(UINT16 key);
        //GameObject��j��
        void destroyGameObject(UINT16 key);

        //�R���|�[�l���g���V�[���ɒǉ��B
        //GameObject����A�N�Z�X����ifriend�ɂ����include�ŏz�Q�Ƃ��N����j�B
        template<DerivationOfComponent T>
        T* addComponent();
        //�R���|�[�l���g��j���B
        //GameObject����A�N�Z�X����ifriend�ɂ����include�ŏz�Q�Ƃ��N����j�B
        template<DerivationOfComponent T>
        void removeComponent(T* instance);
	private:

		// �^���Ƃ�vector��ێ�����R���e�i
		std::unordered_map<std::type_index , std::vector<unique_ptr<Component>>> mComponentMap;
        //GameObject��vector
        std::unordered_map<UINT16, unique_ptr<GameObject>> mGameObjectMap;
        UINT16 _GameObjectHashMax = 0;
	};
}