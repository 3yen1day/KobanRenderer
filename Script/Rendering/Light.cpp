#include "Light.h"
#include "../../lib/imgui/imgui.h"

namespace Koban {
	void Light::drawUI() 
	{
		if (ImGui::CollapsingHeader("Light"))
		{
			ImGui::InputFloat3("Direction", (float*)(mDirection));
		}
	}
}