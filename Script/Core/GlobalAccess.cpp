#include "GlobalAccess.h"
#include "../Rendering/Rendering.h"
#include "../Core/Scene.h"
#include "../GUI/GUI.h"
#include "../Input/Input.h"

std::unique_ptr<Koban::Rendering> Koban::GlobalAccess::mpRender;
std::unique_ptr<Koban::Scene> Koban::GlobalAccess::mpScene;
std::unique_ptr<Koban::GUI> Koban::GlobalAccess::mpGUI;
std::unique_ptr<Koban::Input> Koban::GlobalAccess::mpInput;

namespace Koban {
	GlobalAccess::GlobalAccess(HWND& hwnd, HINSTANCE& hInstance) {
		mpRender = std::make_unique<Rendering>(hwnd);
		mpScene = std::make_unique <Scene>();
		mpGUI = std::make_unique<GUI>(hwnd);
		mpInput = std::make_unique<Input>(hInstance, hwnd);
	}
}