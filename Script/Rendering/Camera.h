#pragma once
#include "../Core/Component.h"

namespace Koban {

	class Camera : public Component
	{
	public:
		Camera();
		~Camera();

		void start() override {};
		void update() override;
		void draw() override {};
		void destroy() override {};
		void drawUI() override {};

		/// <summary>
		/// �ʒu���W�擾
		/// </summary>
		/// <returns></returns>
		D3DXVECTOR3 const &getPostion() {
			return mPosition;
		}

		/// <summary>
		/// positionMatrix�擾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getWorldMat(){
			return mWorldMat;
		};

		/// <summary>
		/// ViewMatrix�擾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getViewMat() {
			return mViewMat;
		};

		/// <summary>
		/// ProjectionMatrix�擾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getProjMat() {
			return mProjMat;
		};

		/// <summary>
		/// ���������x�N�g��
		/// </summary>
		/// <returns></returns>
		D3DXVECTOR3 const& getEyeDir() {
			return mEyeVec;
		}

		/// <summary>
		/// MVP�s��̎擾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const& getVPMat()
		{
			return  mViewMat * mProjMat;
		}

	private:
		void updateTransform();
		/// <summary>
		/// ���[�J�����W��]
		/// </summary>
		void updateTransform_Symple();
		/// <summary>
		/// ���_���S
		/// </summary>
		void updateTransform_Orbit();

		D3DXMATRIX mWorldMat;
		D3DXMATRIX mViewMat;
		D3DXMATRIX mProjMat;
		D3DXVECTOR3 mPosition;
		D3DXVECTOR3 mEyeVec;
		D3DXVECTOR3 mLookAt;
		float mPitch = 0;
		float mYaw = 0;
		float mSensitivity = 0.005;
	};
}