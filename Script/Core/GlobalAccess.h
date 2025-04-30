#pragma once
namespace Koban {
	class Rendering;
	class Scene;
	class GUI;
	class Input;
}

namespace Koban {
	class GlobalAccess {
	public:
		GlobalAccess(HWND& hwnd, HINSTANCE& hInstance);

		static Rendering* const getRender() {
			return mpRender.get();
		}

		static Scene* const getScene() {
			return mpScene.get();
		}

		static GUI* const getGUI() {
			return mpGUI.get();
		}

		static Input* const getInput() {
			return mpInput.get();
		}

	private:
		//äeÉÇÉWÉÖÅ[ÉãÇÃManagerÇÕGA
		static std::unique_ptr<Rendering> mpRender;
		static std::unique_ptr<Scene> mpScene;
		static std::unique_ptr<GUI> mpGUI;
		static std::unique_ptr<Input> mpInput;
	};
}