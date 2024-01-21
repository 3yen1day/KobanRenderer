#include "Camera.h"

namespace Koban {

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::update() {
		// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
		D3DXVECTOR3 mPosition(0.0f, 0.0f, -0.5f); //�J�����i���_�j�ʒu
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
		D3DXMatrixLookAtLH(&mViewMat, &mPosition, &vLookatPt, &vUpVec);
		// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
		D3DXMatrixPerspectiveFovLH(&mProjMat, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);
	}
};