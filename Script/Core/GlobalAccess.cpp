#include "GlobalAccess.h"
#include "../Rendering/Rendering.h"
#include "../Core/Scene.h"

std::unique_ptr<Koban::Rendering> Koban::GlobalAccess::mpRender;
std::unique_ptr<Koban::Scene> Koban::GlobalAccess::mpScene;

namespace Koban {
	GlobalAccess::GlobalAccess(HWND* pHWnd) {
		mpRender.reset(new Rendering(pHWnd));
		mpScene.reset(new Scene());
	}
}