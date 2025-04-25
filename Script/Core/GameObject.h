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
		bool& getIsUpdate() {
			return mIsUpdate;
		}

		void setIsUpdate(bool isUpdate) {
			for (int i = 0, length = mComponents.size(); i < length; i++)
			{
				auto cmp_p = mComponents[i];
				if (cmp_p != nullptr)
					cmp_p->setIsUpdate(isUpdate);
			}
			mIsUpdate = isUpdate;
		}

		bool& getIsDraw() {
			return mIsDraw;
		}

		void setIsDraw(bool isDraw) {
			for (int i = 0, length = mComponents.size(); i < length; i++)
			{
				auto cmp_p = mComponents[i];
				if (cmp_p != nullptr)
					cmp_p->setIsDraw(isDraw);
			}
			mIsDraw = isDraw;
		}

		UINT16 getID(){
			return mID;
		}

		string getName() {
			return mName;
		}

		template<DerivationOfComponent T>
		T* const getComponent() {
			for (int i = 0, length = mComponents.size(); i < length; i++)
			{
				T* cmp_p = dynamic_cast<T*>(mComponents[i]);
				if (cmp_p != nullptr)
					return cmp_p;
			}
			return nullptr;
		}
#pragma endregion

	private:
		bool mIsUpdate = true;
		bool mIsDraw = true;
		UINT16 mID = 0;
		string mName = "GameObject";
		vector<Component*> mComponents;
	};
}