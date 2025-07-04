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
		GameObject* gameObj = SCENE->createGameObject("Camera", D3DXVECTOR3(0, 0, -9));
		gameObj->createComponent<Camera>();

		gameObj = SCENE->createGameObject("Light", D3DXVECTOR3(0, 1, 0));
		gameObj->createComponent<Light>();

		/*D3DXQUATERNION rot;
		D3DXVECTOR3 axiz = D3DXVECTOR3(0, 1, 0);
		D3DXQuaternionRotationAxis(&rot, &axiz, 180 * (3.14 / 180.0f));
		gameObj = SCENE->createGameObject("Mesh", D3DXVECTOR3(0, -0.75, 3), rot);
		gameObj->createComponent<Mesh>();*/

		gameObj = SCENE->createGameObject("Sphere", D3DXVECTOR3(-1.5, 0, 0));
		Mesh* cmp = gameObj->createComponent<Mesh>();
		cmp->setMeshResourcePath("Resource/sphere.fbx");

		gameObj = SCENE->createGameObject("Cube", D3DXVECTOR3(1.5, 0, 0));
		cmp = gameObj->createComponent<Mesh>();
		cmp->setMeshResourcePath("Resource/saikoro.fbx");
	}
}