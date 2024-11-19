#include "GameObject.h"
#include "Scene.h"

namespace Koban {
	GameObject::GameObject(UINT16 id, string name) {
		mID = id;
		mName = name;
	}

	//Component�̐���
	template<DerivationOfComponent T>
	T GameObject::createComponent() {
		//�����^�̃R���|�[�l���g��GameObject�Ɉ��
		for (int i = 0, length = mComponents.size(); i < length; i++)
		{
			if (dynamic_cast<T*>(mComponents[i]) != nullptr)
				return nullptr;
		}

		//�V�[���ɒǉ�����
		auto cmp_pt = new T();
		SCENE->addComponent(cmp_pt);

		mComponents.push_back(cmp_pt);//�����z��ɒǉ�
		return cmp_pt;
	}

	//Component�̔j��
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