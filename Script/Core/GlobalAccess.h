#pragma once
namespace Koban {
	class Render;
	class Scene;
}

namespace Koban {
	class GlobalAccess {
	public:
		GlobalAccess(HWND* pHWnd);

		static Render* const getRender() {
			return mpRender.get();
		}

		static Scene* const getScene() {
			return mpScene.get();
		}

	private:
		static std::unique_ptr<Render> mpRender;
		static std::unique_ptr<Scene> mpScene;
	};
}