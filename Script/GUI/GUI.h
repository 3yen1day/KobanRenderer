#pragma once
#include "../Core/Module.h"

namespace Koban {
	/// <summary>
	/// Renderƒ‚ƒWƒ…[ƒ‹
	/// </summary>
	class GUI :ModuleManager
	{
	public:
		GUI(HWND& hwnd);
		~GUI();

		void start() override;
		void update() override {};
		void draw() override;
		void destroy() override;
		
	private:
	};
}