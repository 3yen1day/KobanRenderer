#pragma once
#include "Object.h"

namespace Koban {
	class RenderObject : public Object
	{
	public:
		RenderObject() {};
		~RenderObject() {};
		virtual void draw() {};
	};
}