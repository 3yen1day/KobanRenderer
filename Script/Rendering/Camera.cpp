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
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置。lookAtLHでlookAtと外積を取って軸を決めるので、rollが回転しない限り0,1,0でOK。
		D3DXMatrixLookAtLH(&mViewMat, &mPosition, &mLookAt, &vUpVec);
		//proj（射影変換）
		float fov = D3DXToRadian(90.0f); // Degreeをラジアンに変換
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
		//右クリック中、もしくはalt押し中、マウスの移動でカメラを回転させる
		Input* input = GA::getInput();
		if (input->isMouseButtonDown(1) || input->isKeyPressed(DIK_LMENU) || input->isKeyPressed(DIK_RMENU))
		{
			mYaw += GA::getInput()->getMouseDeltaX() * mSensitivity;
			mPitch += GA::getInput()->getMouseDeltaY() * mSensitivity;
		}

		mPitch = std::clamp(mPitch, (float)(-D3DX_PI / 2 + 0.01f), (float)(D3DX_PI / 2 - 0.01f));//真上・真下の制限
		D3DXQUATERNION rot;
		D3DXQuaternionRotationYawPitchRoll(&rot, mYaw, mPitch, 0);
		getTransform()->setRotation(rot);

		mPosition = getTransform()->getPosition();
		mWorldMat = getTransform()->getWorldMatrix();

		//視線ベクトル
		D3DXVECTOR3 vEyeDirLocal(0, 0, 1);//注視方向ベクトル
		D3DXVec3TransformNormal(&vEyeDirLocal, &vEyeDirLocal, &mWorldMat);//平行移動情報は必要ないのでnormal
		D3DXVec3Normalize(&mEyeVec, &vEyeDirLocal);

		//LookAt
		mLookAt = mPosition + mEyeVec;
	}

	void Camera::updateTransform_Orbit()
	{
		static float distanceSpeed = 0.1f;
		Input* input = GA::getInput();
		static float distance = 10.0f;

		//WSで注視点方向に移動
		if(input->isKeyPressed(DIK_W))
			distance -= distanceSpeed;
		if(input->isKeyPressed(DIK_S))
			distance += distanceSpeed;
        distance = std::max<float>(distance, 1.0f);

		//右クリック中、もしくはalt押し中、マウスの移動でカメラを回転させる
		if (input->isMouseButtonDown(1) || input->isKeyPressed(DIK_LMENU) || input->isKeyPressed(DIK_RMENU))
		{
			mYaw += GA::getInput()->getMouseDeltaX() * mSensitivity;
			mPitch += GA::getInput()->getMouseDeltaY() * mSensitivity;
		}

		mPitch = std::clamp(mPitch, (float)(-D3DX_PI / 2 + 0.01f), (float)(D3DX_PI / 2 - 0.01f));//真上・真下の制限
		D3DXQUATERNION rot;
		D3DXQuaternionRotationYawPitchRoll(&rot, mYaw, mPitch, 0);
		getTransform()->setRotation(rot);

		//原点を中心に位置を計算
		D3DXVECTOR3 pos;
		pos.x = distance * cosf(mPitch) * sinf(mYaw);
		pos.y = distance * sinf(mPitch);
		pos.z = distance * cosf(mPitch) * cosf(mYaw);

		getTransform()->setPosition(pos);

		mPosition = getTransform()->getPosition();
		mWorldMat = getTransform()->getWorldMatrix();

		//LookAt
		mLookAt = D3DXVECTOR3(0, 0, 0);

		//視線ベクトル
		D3DXVECTOR3 vEyeDirLocal = mLookAt - mPosition;//注視方向ベクトル
		D3DXVec3Normalize(&mEyeVec, &vEyeDirLocal);
	}
};