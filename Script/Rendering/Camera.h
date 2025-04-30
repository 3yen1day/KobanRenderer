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

	private:
		void updateTransform();

		D3DXMATRIX mWorldMat;
		D3DXMATRIX mViewMat;
		D3DXMATRIX mProjMat;
		D3DXVECTOR3 mPosition;
		D3DXVECTOR3 mEyeVec;
		float mPitch = 0;
		float mYaw = 0;
	};
}