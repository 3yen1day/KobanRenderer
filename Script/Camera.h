#pragma once
#include "Object.h"

namespace Koban {

	class Camera : public Object
	{
	public:
		Camera();
		~Camera();

		void update() override;

		/// <summary>
		/// 位置座標取得
		/// </summary>
		/// <returns></returns>
		D3DXVECTOR3 const &getPostion() {
			return mPostion;
		}

		/// <summary>
		/// positionMatrix取得
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getPositionMat(){
			return mPositionMat;
		};

		/// <summary>
		/// ViewMatrix取得
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getViewMat() {
			return mViewMat;
		};

		/// <summary>
		/// ProjectionMatrix取得
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getProjMat() {
			return mProjMat;
		};

		/// <summary>
		/// 視線ベクトル
		/// </summary>
		/// <returns></returns>
		D3DXVECTOR3 const& getEyeDir() {
			return mEyeVec;
		}

	private:
		D3DXMATRIX mPositionMat;
		D3DXMATRIX mViewMat;
		D3DXMATRIX mProjMat;
		const D3DXVECTOR3 mPostion = D3DXVECTOR3(0.0f, 0.0f, -0.5f);
		const D3DXVECTOR3 mEyeVec = D3DXVECTOR3(1, 1, -1);
	};
}