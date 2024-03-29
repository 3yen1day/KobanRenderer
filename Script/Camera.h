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
		/// 座標取得
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getPosition(){
			return mPosition;
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

	private:
		D3DXMATRIX mPosition;
		D3DXMATRIX mViewMat;
		D3DXMATRIX mProjMat;
	};
}