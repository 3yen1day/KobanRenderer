#pragma once
namespace Koban {
	class Object
	{
	public:
		Object() {};
		~Object() {};
		virtual void start() {};
		virtual void update() {};
		virtual void destroy() {};
	};
}