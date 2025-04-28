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
		/// 位置座標取得
		/// </summary>
		/// <returns></returns>
		D3DXVECTOR3 const &getPostion() {
			return mPosition;
		}

		/// <summary>
		/// positionMatrix取得
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getWorldMat(){
			return mWorldMat;
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
		D3DXMATRIX mWorldMat;
		D3DXMATRIX mViewMat;
		D3DXMATRIX mProjMat;
		D3DXVECTOR3 mPosition;
		D3DXVECTOR3 mEyeVec;
	};
}