#pragma once
namespace Koban {
	/// <summary>
	/// �K��I�u�W�F�N�g
	/// </summary>
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