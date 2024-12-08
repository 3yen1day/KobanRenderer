#pragma once
namespace Koban {
	class Rendering;
	class Scene;
}

namespace Koban {
	class GlobalAccess {
	public:
		GlobalAccess(HWND* pHWnd);

		static Rendering* const getRender() {
			return mpRender.get();
		}

		static Scene* const getScene() {
			return mpScene.get();
		}

	private:
		//各モジュールのManagerはGA
		static std::unique_ptr<Rendering> mpRender;
		static std::unique_ptr<Scene> mpScene;
	};
}