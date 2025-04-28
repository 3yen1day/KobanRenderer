#include "Component.h"  
#include "../Core/Transform.h"  
#include "../Core/Scene.h"  
#include "../Core/GameObject.h"  

namespace Koban {
	GameObject* const Component::getGameObject() {
		return GA::getScene()->getGameObject(mGameObjectID);
	}

	Transform* const Component::getTransform() {
		return getGameObject()->getComponent<Transform>();
	}
}