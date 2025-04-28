#include "Transform.h"
#include "../../lib/imgui/imgui.h"

namespace Koban {
	void Transform::drawUI() {
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::InputFloat3("Position", (float*)(mPosition));
			ImGui::InputFloat4("Rotation", (float*)(mRotation));
			ImGui::InputFloat3("Scale", (float*)(mScale));
		}
	}
}