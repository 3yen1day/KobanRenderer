#include "GameObject.h"
#include "Scene.h"

namespace Koban {
	GameObject::GameObject(UINT16 id, string name) {
		mID = id;
		mName = name;
	}

	void GameObject::setIsUpdate(bool isUpdate) {
		for (int i = 0, length = mComponents.size(); i < length; i++)
		{
			auto cmp_p = mComponents[i];
			if (cmp_p != nullptr)
				cmp_p->setIsUpdate(isUpdate);
		}
		mIsUpdate = isUpdate;
	}

	void GameObject::setIsDraw(bool isDraw) {
		for (int i = 0, length = mComponents.size(); i < length; i++)
		{
			auto cmp_p = mComponents[i];
			if (cmp_p != nullptr)
				cmp_p->setIsDraw(isDraw);
		}
		mIsDraw = isDraw;
	}
};