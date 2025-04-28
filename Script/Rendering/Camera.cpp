#include "Camera.h"
#include "../Core/Transform.h"

namespace Koban {

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::update() {
		mPosition = getTransform()->getPosition();
		//mPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVECTOR3 vEyeDir(0.0f, 0.0f, 0.0f);//注視位置
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
		auto pos2look = vEyeDir - mPosition;
		D3DXVec3Normalize(&mEyeVec, &pos2look);

		//world
		D3DXMatrixTranslation(&mWorldMat, mPosition.x, mPosition.y, mPosition.z);
		//view
		D3DXMatrixLookAtLH(&mViewMat, &mPosition, &vEyeDir, &vUpVec);
		//proj（射影変換）
		D3DXMatrixPerspectiveFovLH(&mProjMat, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);
	}
};