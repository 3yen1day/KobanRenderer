#include "Camera.h"
#include "../Core/Transform.h"
#include "../Input/Input.h"

namespace Koban {

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::update() {
		updateTransform();
		mPosition = getTransform()->getPosition();
		mWorldMat = getTransform()->getWorldMatrix();
		D3DXVECTOR3 vEyeDirLocal(0, 0, 1);//���������x�N�g��
		D3DXVec3TransformNormal(&vEyeDirLocal, &vEyeDirLocal, &mWorldMat);//���s�ړ����͕K�v�Ȃ��̂�normal

		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
		D3DXVec3Normalize(&mEyeVec, &vEyeDirLocal);

		//world
		D3DXMatrixTranslation(&mWorldMat, mPosition.x, mPosition.y, mPosition.z);
		D3DXVECTOR3 vLookAt = mPosition + mEyeVec;
		//view
		D3DXMatrixLookAtLH(&mViewMat, &mPosition, &vLookAt, &vUpVec);
		//proj�i�ˉe�ϊ��j
		D3DXMatrixPerspectiveFovLH(&mProjMat, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);
	}

	void Camera::updateTransform() {
		//�E�N���b�N���A�}�E�X�̈ړ��ŃJ��������]������
		Input* input = GA::getInput();
		if (input->isMouseButtonDown(1)) {
			float sensitivity = 0.005;
			mYaw += GA::getInput()->getMouseDeltaX() * sensitivity;
			mPitch += GA::getInput()->getMouseDeltaY() * sensitivity;
			D3DXQUATERNION rot;
			D3DXQuaternionRotationYawPitchRoll(&rot, mYaw, mPitch, 0);
			//clamp����
			getTransform()->setRotation(rot);
		}
	}
};