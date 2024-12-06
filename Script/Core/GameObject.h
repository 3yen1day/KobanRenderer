#pragma once
#include "Scene.h"

namespace Koban {
	class Scene;
}

namespace Koban {
	class GameObject {
	public:
		GameObject(UINT16 id, string name);
		~GameObject() {};
		template<DerivationOfComponent T>
		T* const createComponent() {
			//同じ型のコンポーネントはGameObjectに一つ
			for (int i = 0, length = mComponents.size(); i < length; i++)
			{
				if (dynamic_cast<T*>(mComponents[i]) != nullptr)
					return nullptr;
			}

			auto cmp_pt = SCENE->addComponent<T>();//シーンに追加する
			mComponents.push_back(cmp_pt);//内部配列に追加
			return cmp_pt;
		};

		template<DerivationOfComponent T>
		T removeComponent() {
			for (auto it = mComponents.begin(); it != mComponents.end(); it++)
			{
				if (dynamic_cast<T*>(*it) != nullptr) {
					SCENE->removeComponent<T>(*it);
					mComponents.erase(it);
					break;
				}
			}
		};

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

		UINT16 getID(){
			return mID;
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