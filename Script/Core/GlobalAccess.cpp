#include "GlobalAccess.h"
#include "../Rendering/Rendering.h"
#include "../Core/Scene.h"
#include "../GUI/GUI.h"

std::unique_ptr<Koban::Rendering> Koban::GlobalAccess::mpRender;
std::unique_ptr<Koban::Scene> Koban::GlobalAccess::mpScene;
std::unique_ptr<Koban::GUI> Koban::GlobalAccess::mpGUI;

namespace Koban {
	GlobalAccess::GlobalAccess(HWND* pHWnd) {
		mpRender = std::make_unique<Rendering>(pHWnd);
		mpScene = std::make_unique <Scene>();
		mpGUI = std::make_unique<GUI>(pHWnd);
	}
}