#pragma once
#include "Object.h"
namespace Koban {
	/// <summary>
	/// drawが必要なレンダリング用オブジェクト
	/// </summary>
	class RenderObject : public Object
	{
	public:
		RenderObject() {};
		~RenderObject() {};
		virtual void update() {};
	};
}