#include "Camera.h"

namespace Koban {

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::update() {
		mPosition = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
		auto pos2look = vLookatPt - mPosition;
		D3DXVec3Normalize(&mEyeVec, &pos2look);

		//world
		D3DXMatrixTranslation(&mWorldMat, mPosition.x, mPosition.y, mPosition.z);
		//view
		D3DXMatrixLookAtLH(&mViewMat, &mPosition, &vLookatPt, &vUpVec);
		//proj�i�ˉe�ϊ��j
		D3DXMatrixPerspectiveFovLH(&mProjMat, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);
	}
};