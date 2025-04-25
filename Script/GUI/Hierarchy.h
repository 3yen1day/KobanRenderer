#pragma once

namespace Koban {
	class Hierarchy {
	public:
		Hierarchy() {};
		~Hierarchy() {};

		void draw();

	private:
		UINT16 mSelectedGameObjectID = 0;
	};
}