#pragma once
#include "Object.h"
namespace Koban {
	/// <summary>
	/// draw���K�v�ȃ����_�����O�p�I�u�W�F�N�g
	/// </summary>
	class RenderObject : public Object
	{
	public:
		RenderObject() {};
		~RenderObject() {};
		virtual void update() {};
	};
}