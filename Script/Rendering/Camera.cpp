#include "Camera.h"
#include "../Core/Transform.h"
#include "../Input/Input.h"
#include <algorithm>

namespace Koban
{

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::update()
	{
		updateTransform();

		
		//D3DXVECTOR3 vLookAt(0,0,0);

		//view
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu�BlookAtLH��lookAt�ƊO�ς�����Ď������߂�̂ŁAroll����]���Ȃ�����0,1,0��OK�B
		D3DXMatrixLookAtLH(&mViewMat, &mPosition, &mLookAt, &vUpVec);
		//proj�i�ˉe�ϊ��j
		float fov = D3DXToRadian(90.0f); // Degree�����W�A���ɕϊ�
		float aspectRatio = (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT;
		float nearZ = 0.1f;
		float farZ = 1000.0f;
		D3DXMatrixPerspectiveFovLH(&mProjMat, fov, aspectRatio, nearZ, farZ);
	}

	void Camera::updateTransform()
	{
		updateTransform_Orbit();
	}

	void Camera::updateTransform_Symple()
	{
		//�E�N���b�N���A��������alt�������A�}�E�X�̈ړ��ŃJ��������]������
		Input* input = GA::getInput();
		if (input->isMouseButtonDown(1) || input->isKeyPressed(DIK_LMENU) || input->isKeyPressed(DIK_RMENU))
		{
			mYaw += GA::getInput()->getMouseDeltaX() * mSensitivity;
			mPitch += GA::getInput()->getMouseDeltaY() * mSensitivity;
		}

		mPitch = std::clamp(mPitch, (float)(-D3DX_PI / 2 + 0.01f), (float)(D3DX_PI / 2 - 0.01f));//�^��E�^���̐���
		D3DXQUATERNION rot;
		D3DXQuaternionRotationYawPitchRoll(&rot, mYaw, mPitch, 0);
		getTransform()->setRotation(rot);

		mPosition = getTransform()->getPosition();
		mWorldMat = getTransform()->getWorldMatrix();

		//�����x�N�g��
		D3DXVECTOR3 vEyeDirLocal(0, 0, 1);//���������x�N�g��
		D3DXVec3TransformNormal(&vEyeDirLocal, &vEyeDirLocal, &mWorldMat);//���s�ړ����͕K�v�Ȃ��̂�normal
		D3DXVec3Normalize(&mEyeVec, &vEyeDirLocal);

		//LookAt
		mLookAt = mPosition + mEyeVec;
	}

	void Camera::updateTransform_Orbit()
	{
		static float distanceSpeed = 0.1f;
		Input* input = GA::getInput();
		static float distance = 10.0f;

		//WS�Œ����_�����Ɉړ�
		if(input->isKeyPressed(DIK_W))
			distance -= distanceSpeed;
		if(input->isKeyPressed(DIK_S))
			distance += distanceSpeed;
        distance = std::max<float>(distance, 1.0f);

		//�E�N���b�N���A��������alt�������A�}�E�X�̈ړ��ŃJ��������]������
		if (input->isMouseButtonDown(1) || input->isKeyPressed(DIK_LMENU) || input->isKeyPressed(DIK_RMENU))
		{
			mYaw += GA::getInput()->getMouseDeltaX() * mSensitivity;
			mPitch += GA::getInput()->getMouseDeltaY() * mSensitivity;
		}

		mPitch = std::clamp(mPitch, (float)(-D3DX_PI / 2 + 0.01f), (float)(D3DX_PI / 2 - 0.01f));//�^��E�^���̐���
		D3DXQUATERNION rot;
		D3DXQuaternionRotationYawPitchRoll(&rot, mYaw, mPitch, 0);
		getTransform()->setRotation(rot);

		//���_�𒆐S�Ɉʒu���v�Z
		D3DXVECTOR3 pos;
		pos.x = distance * cosf(mPitch) * sinf(mYaw);
		pos.y = distance * sinf(mPitch);
		pos.z = distance * cosf(mPitch) * cosf(mYaw);

		getTransform()->setPosition(pos);

		mPosition = getTransform()->getPosition();
		mWorldMat = getTransform()->getWorldMatrix();

		//LookAt
		mLookAt = D3DXVECTOR3(0, 0, 0);

		//�����x�N�g��
		D3DXVECTOR3 vEyeDirLocal = mLookAt - mPosition;//���������x�N�g��
		D3DXVec3Normalize(&mEyeVec, &vEyeDirLocal);
	}
};