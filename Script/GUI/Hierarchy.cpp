#include "Hierarchy.h"
#include <format>
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_win32.h"
#include "../../lib/imgui/imgui_impl_dx11.h"
#include "../Core/Scene.h"
#include "../Core/GameObject.h"
#include "../Core/Transform.h"
#include "../Rendering/Rendering.h"
#include "../Rendering/RenderMode.h"
#include "../Rendering/GBufferToBackBuffer.h"

namespace Koban {
	void Hierarchy::draw() {
		//シーンのGameObjectの表示
		ImGui::Begin("Hierarchy");
		std::vector<GameObject*> gameObjs = SCENE->getAllGameObjects();
		GameObject* selectedGamaObject = nullptr;
		for (int i = 0, length = gameObjs.size(); i<length; i++)
		{
			GameObject* obj = gameObjs[i];
			UINT16 id = obj->getID(); // 選択されたオブジェクトのID
			bool isSelected = id == mSelectedGameObjectID;
			if (isSelected) 
				selectedGamaObject = obj;

			bool tmp = obj->getIsUpdate();
			if(ImGui::Checkbox(std::format("Update##{}", std::to_string(i)).c_str(), &tmp))
				obj->setIsUpdate(tmp);

			ImGui::SameLine(100);
			tmp = obj->getIsDraw();
			if(ImGui::Checkbox(std::format("Draw##{}", std::to_string(i)).c_str(), &tmp))
				obj->setIsDraw(tmp);

			ImGui::SameLine(200);
			if (ImGui::Selectable(std::format("{}##{}", obj->getName(), std::to_string(i)).c_str(), isSelected))
				mSelectedGameObjectID = id;
		}
		ImGui::End();

		//選択されたGameObjectのコンポーネントの表示
		ImGui::Begin("Inspector");
		if (selectedGamaObject != nullptr) {
			vector<Component*>  cmps = selectedGamaObject->getComponents();
			for (Component* cmp : cmps) {
				cmp->drawUI();
			}
		}
		ImGui::End();

		//RenderingModeの選択
		ImGui::Begin("RenderingMode");
		static const char* modeNames[] = 
		{
			"Standerd", "BaseColor", "Normal", "Depth", "UV", "Position"
		};

		int current = static_cast<int>(RENDER->getGBufferToBackBuffer()->getRenderMode());
		if (ImGui::Combo("Render Mode", &current, modeNames, IM_ARRAYSIZE(modeNames)))
		{
			RENDER->getGBufferToBackBuffer()->setRenderMode(static_cast<RenderMode::MODE>(current));
		}
		ImGui::End();
	}
}