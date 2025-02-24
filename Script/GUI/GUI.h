#pragma once
#include "../Core/Module.h"

namespace Koban {
	/// <summary>
	/// Render���W���[��
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