#include "GlobalAccess.h"
#include "../Rendering/Render.h"
#include "../Core/Scene.h"

std::unique_ptr<Koban::Render> Koban::GlobalAccess::mpRender;
std::unique_ptr<Koban::Scene> Koban::GlobalAccess::mpScene;

namespace Koban {
	GlobalAccess::GlobalAccess(HWND* pHWnd) {
		mpRender.reset(new Render(pHWnd));
		mpScene.reset(new Scene());
	}
}