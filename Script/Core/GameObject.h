#pragma once
#include "Component.h"//conceptを使うために必要

namespace Koban {
	class Scene;
}

namespace Koban {
	class GameObject {
	public:
		GameObject(UINT16 id, string name);
		~GameObject() {};
		template<DerivationOfComponent T>
		T createComponent();

		template<DerivationOfComponent T>
		T removeComponent();

#pragma region プロパティ
		bool getIsUpdate() {
			return mIsUpdate;
		}

		bool setIsUpdate(bool isUpdate) {
			for (int i = 0, length = mComponents.size(); i < length; i++)
			{
				auto cmp_p = mComponents[i];
				if (cmp_p != nullptr)
					cmp_p->setIsUpdate(isUpdate);
			}
		}

		bool getIsDraw() {
			return mIsDraw;
		}

		bool setIsDraw(bool isDraw) {
			for (int i = 0, length = mComponents.size(); i < length; i++)
			{
				auto cmp_p = mComponents[i];
				if (cmp_p != nullptr)
					cmp_p->setIsDraw(isDraw);
			}
		}
#pragma endregion

	private:
		bool mIsUpdate = false;
		bool mIsDraw = true;
		UINT16 mID = 0;
		string mName = "GameObject";
		vector<Component*> mComponents;
	};
}