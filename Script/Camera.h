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
		/// À•Wæ“¾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getPosition(){
			return mPosition;
		};

		/// <summary>
		/// ViewMatrixæ“¾
		/// </summary>
		/// <returns></returns>
		D3DXMATRIX const &getViewMat() {
			return mViewMat;
		};

		/// <summary>
		/// ProjectionMatrixæ“¾
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