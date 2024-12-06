#include "GameObject.h"
#include "Scene.h"

namespace Koban {
	GameObject::GameObject(UINT16 id, string name) {
		mID = id;
		mName = name;
	}
};