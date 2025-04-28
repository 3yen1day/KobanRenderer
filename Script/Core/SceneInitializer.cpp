#include "SceneInitializer.h"
#include "Scene.h"
#include "GameObject.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Light.h"
#include "../Rendering/Mesh.h"
#include "Transform.h"

namespace Koban
{
	void SceneInitializer::initializeScene() {
		GameObject* gameObj = SCENE->createGameObject("Camera", D3DXVECTOR3(0, 0, -1));
		gameObj->createComponent<Camera>();

		gameObj = SCENE->createGameObject("Light");
		gameObj->createComponent<Light>();

		D3DXQUATERNION rot;
		D3DXVECTOR3 axiz = D3DXVECTOR3(0, 1, 0);
		D3DXQuaternionRotationAxis(&rot, &axiz, 180 * (3.14 / 180.0f));
		gameObj = SCENE->createGameObject("Mesh", D3DXVECTOR3(0, -0.75, 3), rot);
		gameObj->createComponent<Mesh>();
	}
}