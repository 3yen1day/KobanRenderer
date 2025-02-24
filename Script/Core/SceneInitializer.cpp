#include "SceneInitializer.h"
#include "Scene.h"
#include "GameObject.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Light.h"
#include "../Rendering/Mesh.h"

namespace Koban
{
	void SceneInitializer::initializeScene() {
		auto gameObj = SCENE->createGameObject("Camera");
		gameObj->createComponent<Camera>();

		gameObj = SCENE->createGameObject("Light");
		gameObj->createComponent<Light>();

		gameObj = SCENE->createGameObject("Mesh");
		gameObj->createComponent<Mesh>();
	}
}