#pragma once
namespace Koban {
	/// <summary>
	/// 規定オブジェクト
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