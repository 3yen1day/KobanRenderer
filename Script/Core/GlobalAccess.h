#pragma once
namespace Koban {
	class Rendering;
	class Scene;
	class GUI;
}

namespace Koban {
	class GlobalAccess {
	public:
		GlobalAccess(HWND& hwnd);

		static Rendering* const getRender() {
			return mpRender.get();
		}

		static Scene* const getScene() {
			return mpScene.get();
		}

		static GUI* const getGUI() {
			return mpGUI.get();
		}

	private:
		//�e���W���[����Manager��GA
		static std::unique_ptr<Rendering> mpRender;
		static std::unique_ptr<Scene> mpScene;
		static std::unique_ptr<GUI> mpGUI;
	};
}