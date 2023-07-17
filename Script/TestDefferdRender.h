#pragma once
#include "RenderObject.h"
namespace Koban {
	class TestDefferdRender : public Koban::RenderObject
	{
	public:
		TestDefferdRender() {};
		~TestDefferdRender() {};

		void start() override {};
		void update() override {};
		void draw() override {};
		void destroy() override {};

	private:

	};
}
