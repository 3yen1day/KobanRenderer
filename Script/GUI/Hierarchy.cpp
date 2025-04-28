#include "Hierarchy.h"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_win32.h"
#include "../../lib/imgui/imgui_impl_dx11.h"
#include "../Core/Scene.h"
#include "../Core/GameObject.h"
#include "../Core/Transform.h"

namespace Koban {
	void Hierarchy::draw() {
		//�V�[����GameObject�̕\��
		ImGui::Begin("Hierarchy");
		std::vector<GameObject*> gameObjs = SCENE->getAllGameObjects();
		GameObject* selectedGamaObject = nullptr;
		for (GameObject* obj : gameObjs) {
			UINT16 id = obj->getID(); // �I�����ꂽ�I�u�W�F�N�g��ID
			bool isSelected = id == mSelectedGameObjectID;
			if (isSelected) 
			{
				selectedGamaObject = obj;
			}
			if (ImGui::Selectable(obj->getName().c_str(), isSelected)) 
			{
				mSelectedGameObjectID = id;
			}
		}
		ImGui::End();

		ImGui::Begin("Inspector");
		if (selectedGamaObject != nullptr) {
			vector<Component*>  cmps = selectedGamaObject->getComponents();
			for (Component* cmp : cmps) {
				cmp->drawUI();
			}
		}
		ImGui::End();
	}
}