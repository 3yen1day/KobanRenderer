#pragma once
#include <type_traits> // std::is_base_of

namespace Koban {
	class Component
	{
	public:
		Component() {};
		~Component() {};
		virtual void start() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void destroy() = 0;

		bool& getIsUpdate() {
			return mIsUpdate;
		}

		void setIsUpdate(bool isUpdate) {
			mIsUpdate = isUpdate;
		}

		bool& getIsDraw() {
			return mIsDraw;
		}

		void setIsDraw(bool isDraw) {
			mIsDraw = isDraw;
		}

	protected:
		bool mIsUpdate = true;
		bool mIsDraw = true;
		UINT16 mGameObjectID = 0;
	};

	template<typename T>
	concept DerivationOfComponent = std::is_base_of_v<Component, T> && !std::is_same_v<Component, T>;
}